#include "taskmanager.h"
#include "carcontroller.h"
#include "armcontroller.h"

#include <QDebug>

// ─────────────────────────────────────────────────────────────
// 构造 / 析构
// ─────────────────────────────────────────────────────────────
TaskManager::TaskManager(CarController *car,
                         ArmController *arm,
                         QObject       *parent)
    : QObject(parent)
    , m_car(car)
    , m_arm(arm)
{
    // Car 完成整个运动命令 → onDeviceTaskFinished
    connect(m_car, &CarController::taskFinished,
            this,  &TaskManager::onDeviceTaskFinished);

    // Arm 完成完整 ArmTrack（三步全部走完）→ onDeviceTaskFinished
    connect(m_arm, &ArmController::taskFinished,
            this,  &TaskManager::onDeviceTaskFinished);

    // Arm 完成单个子步骤（moveTo 或 tracking）→ onArmSubStepFinished
    connect(m_arm, &ArmController::actionFinished,
            this,  &TaskManager::onArmSubStepFinished);
}

// ─────────────────────────────────────────────────────────────
// 队列管理
// ─────────────────────────────────────────────────────────────
void TaskManager::addTask(const Task &task)
{
    m_taskQueue.append(task);
    emit queueUpdated(m_taskQueue);
}

void TaskManager::addTasks(const QList<Task> &tasks)
{
    if (tasks.isEmpty()) return;
    m_taskQueue.append(tasks);
    emit queueUpdated(m_taskQueue);
}

void TaskManager::clearTasks()
{
    m_taskQueue.clear();
    emit queueUpdated(m_taskQueue);
}

// ─────────────────────────────────────────────────────────────
// 运行控制
// ─────────────────────────────────────────────────────────────
void TaskManager::startTasks()
{
    if (m_running || m_taskQueue.isEmpty()) return;

    m_running      = true;
    m_currentIndex = 0;
    m_armStep      = 0;        // ← 加这行
    m_qrProcessed  = false;    // ← 加这行

    emit taskStarted();
    executeCurrentTask();
}

void TaskManager::stopTasks()
{
    if (!m_running) return;

    m_running      = false;
    m_armStep      = 0;        // ← 加这行
    m_qrProcessed  = false;    // ← 加这行
    m_currentIndex = -1;       // ← 加这行
    m_taskQueue.clear();

    emit taskStopped();
    emit queueUpdated(m_taskQueue);
}

// ─────────────────────────────────────────────────────────────
// 执行当前任务
// ─────────────────────────────────────────────────────────────
void TaskManager::executeCurrentTask()
{
    if (!m_running) return;

    if (m_currentIndex < 0 || m_currentIndex >= m_taskQueue.size()) {
        // 队列执行完毕
        m_running = false;
        emit currentTaskChanged(m_taskQueue.size()); // UI 标记全部完成
        emit allTasksFinished();
        return;
    }

    m_currentTask = m_taskQueue[m_currentIndex];
    emit currentTaskChanged(m_currentIndex);

    qDebug() << "[TaskManager] Execute index:" << m_currentIndex
             << "type:" << static_cast<int>(m_currentTask.type);

    switch (m_currentTask.type) {
    // ── 小车直线运动 ──
    case TaskType::CarMove:
        m_car->carMoveDistance(
            m_currentTask.params.value("dir",   1).toInt(),
            m_currentTask.params.value("speed", 30.0f).toFloat(),
            m_currentTask.params.value("dist",  20.0f).toFloat()
        );
        break;

    // ── 小车转向 ──
    case TaskType::CarTurn:
        m_car->carTurn(
            m_currentTask.params.value("angle", 90).toInt(),
            m_currentTask.params.value("dir",    0).toInt()
        );
        break;

    // ── 机械臂追踪+放置（多步骤） ──
    case TaskType::ArmTrack:
        m_armStep = 0;
        executeArmStep();
        break;

    // ── 扫码等待 ──
    case TaskType::QRScan:
        // 流程：
        //   Step1: 发 0xC3 → STM32 转向+抬起 → actionFinished()
        //   Step2: onArmSubStepFinished 检测到 QRScan → emit waitingForQR()
        //   Step3: 摄像头扫到码 → onQRCodeScanned() → 推进到下一任务
        m_qrProcessed = false;
        m_arm->armToScanPosition();   // 先让机械臂到扫码姿态
        break;
    }
}

// ─────────────────────────────────────────────────────────────
// ArmTrack 子步骤状态机
//   Step 0: armMoveTo  — 移到目标颜色上方
//   Step 1: startTracking — 视觉伺服对准
//   Step 2: armPlace   — 抓取 / 放置
//   Step 3+: 标记任务完成，推进到下一任务
// ─────────────────────────────────────────────────────────────
void TaskManager::executeArmStep()
{
    if (!m_running) return;

    const int color = m_currentTask.params.value("param", 1).toInt();
    const int op    = m_currentTask.params.value("op",    1).toInt();

    switch (m_armStep) {
    case 0:
        qDebug() << "[ArmStep 0] armMoveTo color=" << color << "op=" << op;
        m_arm->armMoveTo(color, op);
        break;

    case 1:
        qDebug() << "[ArmStep 1] startTracking";
        // 通知 MainWindow 开启圆形检测，摄像头才能给机械臂发误差数据
        emit armTrackColorChanged(color); 
        emit armTrackingStarted();
        m_arm->startTracking();
        break;

    case 2:
        qDebug() << "[ArmStep 2] armPlace";
        // 视觉对准完成，关闭圆形检测，进入放置阶段
        emit armTrackingDone();
        m_arm->armPlace();
        break;

    default:
        // 三步全部完成
        qDebug() << "[ArmStep] ArmTrack finished";
        emit taskFinished();
        advanceToNextTask();
        break;
    }
}

// ─────────────────────────────────────────────────────────────
// 槽：Arm 单步完成
// ─────────────────────────────────────────────────────────────
void TaskManager::onArmSubStepFinished()
{
     if (!m_running) return;
    if (m_currentIndex < 0 || m_currentIndex >= m_taskQueue.size()) return;

    TaskType currentType = m_taskQueue[m_currentIndex].type;

    if (currentType == TaskType::QRScan)
    {
        if (!m_qrProcessed)
        {
            qDebug() << "[TaskManager] QRScan arm ready, enabling camera QR detect";
            emit waitingForQR();
        }
        else
        {
            qDebug() << "[TaskManager] QRScan arm down, advancing to next task";
            advanceToNextTask();
        }
        return;
    }

    // ← 关键守卫：非 ArmTrack 任务期间收到 actionFinished 直接忽略
    // 防止超时 Timer 在 CarMove/CarTurn 期间触发导致段错误
    if (currentType != TaskType::ArmTrack) return;

    m_armStep++;
    executeArmStep();
}
// ─────────────────────────────────────────────────────────────
// 槽：Car 或 Arm 完整任务完成
// ─────────────────────────────────────────────────────────────
void TaskManager::onDeviceTaskFinished()
{
    if (!m_running) return;
    emit taskFinished();
    advanceToNextTask();
}

// ─────────────────────────────────────────────────────────────
// 推进到下一任务
// ─────────────────────────────────────────────────────────────
void TaskManager::advanceToNextTask()
{
    m_currentIndex++;
    executeCurrentTask();
}

// ─────────────────────────────────────────────────────────────
// 槽：扫码完成
// ─────────────────────────────────────────────────────────────
void TaskManager::onQRCodeScanned(const QString &text)
{
    if (!m_running) return;

    // ── 防止同一帧 QR 结果多次触发 ──
    // qrCodeResult 每帧都会发射，必须用标志位只处理第一次
    if (m_qrProcessed) return;

    // 只有当前任务确实是 QRScan 时才处理
    if (m_currentIndex < 0 || m_currentIndex >= m_taskQueue.size()) return;
    if (m_taskQueue[m_currentIndex].type != TaskType::QRScan) return;

    m_qrProcessed = true;

    // 通知 MainWindow 关闭 QR 检测，避免继续触发
    emit qrScanCompleted();

   
    // 解析二维码：格式 "123+456"，取 '+' 前半段作为颜色序列
    QStringList parts = text.split('+');
    QString target = parts.isEmpty() ? text : parts[0];

    m_colorOrder.clear();
    for (const QChar &c : target) {
        int n = c.digitValue();
        if (n >= 1 && n <= 3)
            m_colorOrder.append(n);
    }

    qDebug() << "[TaskManager] QR scanned, colorOrder:" << m_colorOrder;

    // 把后续 ArmTrack 任务的 param 替换成实际颜色
    int colorIdx = 0;
    for (int i = m_currentIndex + 1; i < m_taskQueue.size(); ++i) {
        if (m_taskQueue[i].type == TaskType::ArmTrack
            && colorIdx < m_colorOrder.size())
        {
            m_taskQueue[i].params["param"] = m_colorOrder[colorIdx++];
        }
    }

    emit queueUpdated(m_taskQueue); // 刷新任务列表显示

    m_arm->armScanDown();  // 发0xC4，等臂降下后再推进
}
