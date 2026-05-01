#include "carcontroller.h"
#include "sharedserial.h"

#include <QDebug>
#include <QCoreApplication>
#include <QTimer>
#include <cmath>

CarController::CarController(QObject *parent) : QObject(parent)
{
    serial = SharedSerial::instance();

    connect(serial, &SerialControl::dataReceived,
            this, &CarController::handleSerialData);
    connect(serial, &SerialControl::connectionError,
            this, [this](const QString &error) {
                currentStatus = "连接错误: " + error;
                emit statusChanged(currentStatus);
            });

    // 底盘对准定时器（不自动启动）
    alignTimer = new QTimer(this);
    connect(alignTimer, &QTimer::timeout,
            this, &CarController::chassisTracking);
}

CarController::~CarController()
{
    stop();
}

void CarController::carMoveDistance(int direction, float v, float d)
{
    if (!serial) {
        qDebug() << "[CarCtrl] serial is null!";
        emit taskFinished();
        return;
    }

    if (!serial->isOpen()) {
        emit statusChanged("串口未连接");
        return;
    }

    int16_t distanceInt = static_cast<int16_t>(d * 10);
    int16_t velocityInt = static_cast<int16_t>(v * 10);

    quint8 cmd = 0x01;

    quint8 distLow  = static_cast<quint8>(distanceInt & 0xFF);
    quint8 distHigh = static_cast<quint8>((distanceInt >> 8) & 0xFF);

    quint8 velLow   = static_cast<quint8>(velocityInt & 0xFF);
    quint8 velHigh  = static_cast<quint8>((velocityInt >> 8) & 0xFF);

    serial->sendPacket(
        0xAB,
        cmd,
        static_cast<quint8>(direction),
        distLow,
        distHigh,
        velLow,
        velHigh
    );

    currentStatus = QString("指令发送: 方向=%1, 速度=%2, 距离=%3")
                        .arg(direction)
                        .arg(velocityInt / 10.0)
                        .arg(distanceInt / 10.0);
    emit statusChanged(currentStatus);
}

void CarController::carMoveSpeed(float speed)
{
    if (!serial->isOpen()) {
        emit statusChanged("串口未连接");
        return;
    }
}

void CarController::carTurn(int angle, int8_t dir)
{
    if (!serial || !serial->isOpen()) {
        emit statusChanged("串口未连接");
        emit taskFinished();
        return;
    }

    quint8 cmd = 0x03;

    serial->sendPacket(
        0xAB,
        cmd,
        static_cast<quint8>(angle),
        static_cast<quint8>(dir),
        0x14
    );

    currentStatus = QString("指令发送: 方向=%1, 角度=%2")
                        .arg(dir)
                        .arg(angle);
}

void CarController::stop()
{
    if (serial->isOpen()) {
        serial->sendData("STOP\n");
        currentStatus = "已停止";
        emit statusChanged(currentStatus);
    }
}

bool CarController::isConnected() const
{
    return serial->isOpen();
}

// ════════════════════════════════════════════════════════════
//  底盘视觉对准
// ════════════════════════════════════════════════════════════

void CarController::carAlignVelocity(int8_t vx, int8_t vy)
{
    if (!serial || !serial->isOpen()) return;

    // 协议：0xAB | len | 0x05 | vx(int8) | vy(int8) | checksum
    // sendPacket 自动填充 len 和 checksum
    serial->sendPacket(
        0xAB,
        static_cast<quint8>(0x05),
        static_cast<quint8>(vx),
        static_cast<quint8>(vy)
    );
}

void CarController::updateAlignError(int dx, int dy, uint8_t xdir, uint8_t ydir)
{
    qDebug() << "[CarCtrl] updateAlignError raw:" << dx << dy << "xdir=" << xdir << "ydir=" << ydir;
    m_alignUpdateCnt++;  // 无论收到什么信号都计数，否则丢目标时永远无法进入 chassisTracking

    // 特殊码：摄像头连续丢帧超过阈值，目标已丢失
    if (xdir == 2 && ydir == 2) {
        m_alignTargetLost = true;
        return;
    }
    m_alignTargetLost = false;  // 收到正常误差，目标找回
    alignErrorX = dx;
    alignErrorY = dy;
    alignDirX   = xdir;
    alignDirY   = ydir;
}

void CarController::startChassisTracking()
{
    m_alignGeneration++;
    int myGen = m_alignGeneration;

    // 初始化误差为0，重置更新计数和目标丢失标志
    alignErrorX = 0;
    alignErrorY = 0;
    m_alignUpdateCnt = 0;
    m_alignTargetLost = false;

    alignTimer->start(50);  // 20Hz

    // 15秒超时保底，检测不到就结束任务推进下一项
    QTimer::singleShot(15000, this, [this, myGen]() {
        if (myGen != m_alignGeneration) return;
        if (alignTimer->isActive()) {
            qDebug() << "[CarCtrl] chassis align timeout, force stop";
            alignTimer->stop();
            carAlignVelocity(0, 0);  // 发停止速度
            emit alignFinished();
        }
    });
}

void CarController::stopChassisTracking()
{
    m_alignGeneration++;  // 使当前超时回调失效
    alignTimer->stop();
    carAlignVelocity(0, 0);
}

void CarController::chassisTracking()
{
    // 前10帧不执行，等摄像头稳定后再判断
    if (m_alignUpdateCnt < 10) return;

    // 目标丢失 → 停车等待，不结束任务，等5秒超时再推进
    if (m_alignTargetLost) {
        carAlignVelocity(0, 0);  // 发零速停车，防止乱跑
        return;                   // 继续等5秒超时
    }

    // 两轴均进入死区 → 停车，通知完成
    if (std::abs(alignErrorX) < ALIGN_DEAD_ZONE &&
        std::abs(alignErrorY) < ALIGN_DEAD_ZONE)
    {
        qDebug() << "[CarCtrl] chassis align finished, dx=" << alignErrorX
                 << "dy=" << alignErrorY;

        alignTimer->stop();
        m_alignGeneration++;  // 使超时回调失效

        carAlignVelocity(0, 0);  // 发零速停车
        emit alignFinished();
        return;
    }

    // ── 比例控制：像素误差 → 速度 ──────────────────────────
    // dx > 0 圆在右侧 → 底盘右移（vy_chassis 正值）
    // dy > 0 圆在下方 → 底盘前进（vx_chassis 正值）
    //
    // 注意：摄像头朝前时 dx→横向 dy→纵向
    //       若摄像头朝下或角度不同，可在此交换/取反

    float raw_vy = -alignErrorX * ALIGN_KP;
    float raw_vx = -alignErrorY * ALIGN_KP;

    // 限幅
    int8_t vx = static_cast<int8_t>(
        std::max(-ALIGN_MAX_SPEED, std::min(ALIGN_MAX_SPEED, (int)raw_vx)));
    int8_t vy = static_cast<int8_t>(
        std::max(-ALIGN_MAX_SPEED, std::min(ALIGN_MAX_SPEED, (int)raw_vy)));

    // 死区内的轴置零，避免抖动
    if (std::abs(alignErrorX) < ALIGN_DEAD_ZONE) vy = 0;
    if (std::abs(alignErrorY) < ALIGN_DEAD_ZONE) vx = 0;

    carAlignVelocity(vx, vy);

    qDebug() << "[CarCtrl] align send vx=" << vx << "vy=" << vy
             << "  errX=" << alignErrorX << "errY=" << alignErrorY;
}

// ════════════════════════════════════════════════════════════
//  串口接收（原有逻辑不变）
// ════════════════════════════════════════════════════════════

void CarController::handleSerialData(const QByteArray &data)
{
    qDebug() << "[Car RX raw]:" << data.toHex(' ');
    recvBuffer.append(data);

    while (recvBuffer.size() >= 4) {
        if (quint8(recvBuffer[0]) == 0xAB &&
            quint8(recvBuffer[1]) == 0x04 &&
            quint8(recvBuffer[2]) == 0x01 &&
            quint8(recvBuffer[3]) == 0xB0)
        {
            qDebug() << "Found sequence: AB 04 01 B0";
            emit statusChanged("on status");
            emit taskFinished();
            recvBuffer.remove(0, 4);
        } else {
            recvBuffer.remove(0, 1);
        }
    }
}
