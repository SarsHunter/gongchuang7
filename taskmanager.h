#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QObject>
#include <QList>
#include <QVariantMap>

class CarController;
class ArmController;

// ─── 任务类型 ───────────────────────────────────────────────
enum class TaskType {
    CarMove,    // 小车直线运动
    CarTurn,    // 小车原地转向
    ArmTrack,   // 机械臂：移到目标 → 视觉追踪 → 放置
    QRScan,     // 扫码，结果写入后续 ArmTrack 的 param
    CarAlign,   // 小车底盘视觉对准（停稳后延时启动）
    ColorSort,  // 颜色分拣检测，检测到后通知下位机自动抓取放置
    DiskMove,   // 盘位移动：在滑轨固定盘位之间移动（1~3号位）
};

// ─── 任务结构体 ─────────────────────────────────────────────
struct Task {
    TaskType    type;
    QVariantMap params;
};

// ─── TaskManager ────────────────────────────────────────────
// 把 Task 队列严格串行执行：
//   当前任务完成 → 发 taskFinished / actionFinished → 推进到下一任务
// ────────────────────────────────────────────────────────────
class TaskManager : public QObject
{
    Q_OBJECT

public:
    explicit TaskManager(CarController *car,
                         ArmController *arm,
                         QObject       *parent = nullptr);

    // 队列管理
    void addTask (const Task        &task);
    void addTasks(const QList<Task> &tasks);
    void clearTasks();

    // 运行控制
    void startTasks();
    void stopTasks();

    // 状态查询
    bool isRunning()    const { return m_running; }
    int  getQueueSize() const { return m_taskQueue.size(); }

signals:
    void queueUpdated      (const QList<Task> &queue); // 队列内容变化
    void currentTaskChanged(int index);                // 当前执行索引
    void taskStarted();
    void taskFinished();
    void allTasksFinished();
    void taskStopped();
    void waitingForQR();         // 通知 MainWindow 开启 QR 检测
    void qrScanCompleted();      // QR 扫码完成，关闭 QR 检测
    void armTrackingStarted();   // ArmTrack Step1 开始，通知 MainWindow 开启圆形检测
    void armTrackingDone();      // ArmTrack Step1 完成，通知 MainWindow 关闭圆形检测
    void armTrackColorChanged(int color);
    void chassisAlignStarted(int color);  // CarAlign 开始，通知 MainWindow 开启圆形检测（带颜色）
    void chassisAlignDone();              // CarAlign 完成，通知 MainWindow 关闭圆形检测
    void colorSortStarted(int targetColor); // ColorSort 开始，通知 MainWindow 开启目标颜色色块检测
    void colorSortDone();                 // ColorSort 完成，通知 MainWindow 关闭色块检测

public slots:
    void onDeviceTaskFinished();               // Car / Arm 完整任务完成
    void onArmSubStepFinished();               // Arm 单步（moveTo / tracking）完成
    void onQRCodeScanned(const QString &text); // 扫码结果回调
    void onColorBlockDetected(int color);      // 颜色分拣检测到色块


private:
    void executeCurrentTask();   // 执行 m_currentIndex 对应的任务
    void executeArmStep();       // 执行 ArmTrack 的当前子步骤
    void advanceToNextTask();    // m_currentIndex++ 并执行下一个任务

    QList<Task>   m_taskQueue;
    Task          m_currentTask;

    CarController *m_car = nullptr;
    ArmController *m_arm = nullptr;

    bool m_running      = false;
    int  m_currentIndex = -1;
    int  m_armStep      = 0;     // ArmTrack 内部步骤（0:moveTo 1:tracking 2:place）

    QList<int> m_colorOrder;      // QRScan 解析出的前半段颜色顺序
    QList<int> m_colorOrderBack;  // QRScan 解析出的后半段颜色顺序
    int  m_frontIdx     = 0;       // 前半段当前使用索引（ArmTrack）
    int  m_backIdx      = 0;       // 后半段当前使用索引
    int  m_colorSortIdx = 0;       // ColorSort 从前半段取颜色的索引
    bool m_qrProcessed  = false;   // 防止 qrCodeResult 多次触发
    int  m_qrStep      = 0;       // QRScan 子步骤：0=arm准备中 1=等待扫码结果
    bool m_colorDetected = false; // ColorSort 防重：只处理第一次检测
    bool m_colorSortArmSent = false; // ColorSort：只有发了0xC5后才接受ArmDone

    int m_currentSlot = 2;  // 当前底盘盘位，默认2号位
};

#endif // TASKMANAGER_H
