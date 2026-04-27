#ifndef CARCONTROLLER_H
#define CARCONTROLLER_H

#include "serialcontrol.h"
#include <QObject>
#include <QTimer>

class CarController : public QObject
{
    Q_OBJECT
public:
    explicit CarController(QObject *parent = nullptr);
    ~CarController();

    // 速度控制 (单位: cm/s)
    void carMoveSpeed(float speed);

    // 距离控制 (单位: cm)
    void carMoveDistance(int direction , float v ,float d);

    // 转向
    void carTurn(int angle, int8_t dir);

    // 停止
    void stop();

    bool isConnected() const;

    // ── 底盘视觉对准 ──────────────────────────────────────────
    // 启动/停止 20Hz 对准定时器
    void startChassisTracking();
    void stopChassisTracking();

    // 由 MainWindow 每帧调用，更新霍夫圆误差
    void updateAlignError(int dx, int dy, uint8_t xdir, uint8_t ydir);

    // 向 STM32 发送连续速度命令（非阻塞，不等回复）
    // vx: 横向速度 -100~100（负=左 正=右）
    // vy: 纵向速度 -100~100（负=后 正=前）
    void carAlignVelocity(int8_t vx, int8_t vy);
    // ─────────────────────────────────────────────────────────

signals:
    void statusChanged(const QString &status);
    void reachedTarget();
    void errorOccurred(QString err);
    void taskFinished();

    // 底盘对准完成（进入死区且停稳）
    void alignFinished();

private slots:
    void handleSerialData(const QByteArray &data);

    // 20Hz 对准定时器槽
    void chassisTracking();

private:
    enum class CarState {
        Idle,
        Moving,
        Error
    };

    CarState state = CarState::Idle;

    void handleFrame(const QByteArray &frame);
    void onReached();

    SerialControl *serial;
    QByteArray     recvBuffer;
    QString        currentStatus;

    // ── 底盘对准内部状态 ──────────────────────────────────────
    QTimer  *alignTimer       = nullptr;
    int      alignErrorX      = 0;
    int      alignErrorY      = 0;
    uint8_t  alignDirX        = 0;  // 0=左/负  1=右/正
    uint8_t  alignDirY        = 0;  // 0=后/负  1=前/正
    int      m_alignGeneration = 0; // 每次新对准动作 +1，使旧超时回调失效

    // 死区阈值（像素），小于此值认为已对准
    static constexpr int ALIGN_DEAD_ZONE = 15;

    // 速度增益：每像素对应的速度值（-100~100 范围内）
    // 建议先用 0.3f，若震荡再降低
    static constexpr float ALIGN_KP = 0.3f;

    // 输出速度上限（防止过快）
    static constexpr int ALIGN_MAX_SPEED = 40;
    // ─────────────────────────────────────────────────────────
};

#endif // CARCONTROLLER_H
