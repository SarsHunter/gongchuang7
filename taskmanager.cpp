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

    // Car 底盘对准完成 → onDeviceTaskFinished
    connect(m_car, &CarController::alignFinished,
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
    m_frontIdx     = 0;        // 重置前半段索引
    m_backIdx      = 0;        // 重置后半段索引

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
    m_frontIdx     = 0;        // 重置前半段索引
    m_backIdx      = 0;        // 重置后半段索引
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

        // 检测是否开始了一个新的同 side ArmTrack 组
        // （前一个任务不是同 side 的 ArmTrack → 重置索引，从头取颜色）
        if (m_currentIndex > 0) {
            const Task &prev = m_taskQueue[m_currentIndex - 1];
            int currSide = m_currentTask.params.value("side", 0).toInt();
            if (prev.type != TaskType::ArmTrack ||
                prev.params.value("side", 0).toInt() != currSide) {
                if (currSide == 0) m_frontIdx = 0;
                else               m_backIdx  = 0;
            }
        } else {
            // 队列第一个任务就是 ArmTrack，直接重置
            int currSide = m_currentTask.params.value("side", 0).toInt();
            if (currSide == 0) m_frontIdx = 0;
            else               m_backIdx  = 0;
        }

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

    // ── 小车底盘视觉对准 ──
    case TaskType::CarAlign:
        // 延时 500ms 等小车停稳，再开启视觉检测和对准
        {
            int color = m_currentTask.params.value("color", 1).toInt();
            QTimer::singleShot(500, this, [this, color]() {
                if (!m_running) return;
                emit chassisAlignStarted(color);
                m_car->startChassisTracking();
            });
        }
        break;

    // ── 颜色分拣检测 ──
    case TaskType::ColorSort: {
        m_colorDetected = false;
        m_colorSortArmSent = false;  // 重置：还没发0xC5，不接受旧ArmDone

        // 按前半段顺序取当前目标颜色
        int targetColor = 1; // 默认红色
        if (m_colorSortIdx < m_colorOrder.size())
            targetColor = m_colorOrder[m_colorSortIdx];
        m_currentTask.params["targetColor"] = targetColor;

        qDebug() << "[ColorSort] targetColor=" << targetColor
                 << "(idx=" << m_colorSortIdx << "/" << m_colorOrder.size() << ")";
        emit colorSortStarted(targetColor);
        break;
    }
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

    const int op = m_currentTask.params.value("op", 1).toInt();

    switch (m_armStep) {
    case 0: {
        // Step 0: 只在这里取一次颜色，并存入 params 供后续 Step 使用
        const int side = m_currentTask.params.value("side", 0).toInt();
        int color = 1; // 默认值
        if (side == 0) {
            if (m_frontIdx < m_colorOrder.size())
                color = m_colorOrder[m_frontIdx++];
        } else {
            if (m_backIdx < m_colorOrderBack.size())
                color = m_colorOrderBack[m_backIdx++];
        }
        m_currentTask.params["param"] = color;   // 存起来，Step1/2 复用

        qDebug() << "[ArmStep 0] armMoveTo color=" << color << "op=" << op;
        m_arm->armMoveTo(color, op);
        break;
    }

    case 1: {
        qDebug() << "[ArmStep 1] CarAlign done, direct armPlace (skip tracking)";
        // 底盘 CarAlign 已完成视觉对准，不再做机械臂视觉伺服
        // 直接发送 0xC2 触发下位机抓取/放置
        m_arm->armPlace();
        break;
    }

    case 2:
        // Step1 的 armPlace 完成后，下位机回复 ArmDone，m_armStep 变成 2
        // 这里直接标记任务完成
    default:
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

    // ColorSort 任务：收到下位机 ArmDone → 任务完成
    if (currentType == TaskType::ColorSort) {
        if (!m_colorSortArmSent) {
            qDebug() << "[ColorSort] ignore stale actionFinished (not sent yet)";
            return;  // 忽略旧的/延迟到达的 ArmDone
        }
        qDebug() << "[ColorSort] arm action finished";
        emit taskFinished();
        advanceToNextTask();
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
void TaskManager::onColorBlockDetected(int color)
{
    if (!m_running) return;
    if (m_currentIndex < 0 || m_currentIndex >= m_taskQueue.size()) return;
    if (m_taskQueue[m_currentIndex].type != TaskType::ColorSort) return;
    if (m_colorDetected) return;

    int targetColor = m_currentTask.params.value("targetColor", 1).toInt();
    if (color != targetColor) {
        qDebug() << "[ColorSort] detected color=" << color
                 << " but target=" << targetColor << ", ignoring";
        return; // 非目标颜色，忽略，继续检测
    }

    m_colorDetected = true;
    qDebug() << "[ColorSort] detected target color=" << color
             << ", sending 0xC5 to STM32";

    // 成功分拣后推进索引
    m_colorSortIdx++;

    emit colorSortDone();
    m_colorSortArmSent = true;   // 标记：已发送0xC5，现在可以收ArmDone了
    m_arm->armColorSort(color);
}

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

   
    // 解析二维码：格式 "123+456"
    QStringList parts = text.split('+');
    QString front = parts.isEmpty() ? text : parts[0];

    // 前半段
    m_colorOrder.clear();
    for (const QChar &c : front) {
        int n = c.digitValue();
        if (n >= 1 && n <= 3)
            m_colorOrder.append(n);
    }

    // 后半段
    m_colorOrderBack.clear();
    if (parts.size() >= 2) {
        QString back = parts[1];
        for (const QChar &c : back) {
            int n = c.digitValue();
            if (n >= 1 && n <= 3)
                m_colorOrderBack.append(n);
        }
    }

    qDebug() << "[TaskManager] QR scanned, front:" << m_colorOrder
             << "back:" << m_colorOrderBack;

    // 每次新扫码后重置索引，让后续 ArmTrack / ColorSort 从新码的头开始取
    m_frontIdx = 0;
    m_backIdx  = 0;
    m_colorSortIdx = 0;

    // 不再预先填充任务队列，改为执行 ArmTrack 时根据 side 参数动态选择
    emit queueUpdated(m_taskQueue); // 刷新任务列表显示

    m_arm->armScanDown();  // 发0xC4，等臂降下后再推进
}
