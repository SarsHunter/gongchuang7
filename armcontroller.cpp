#include "armcontroller.h"
#include "sharedserial.h"

#include <QDebug>
#include <QCoreApplication>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>
#include <cmath>

QMutex g_armRecvMutex;
enum class ArmState {
    Idle,
    Tracking,
    Error
};

ArmController::ArmController(QObject *parent) : QObject(parent)
{
    serial = SharedSerial::instance();
    connect(serial, &SerialControl::dataReceived,
            this, &ArmController::handleSerialData);

    trackingTimer = new QTimer(this);
    trackingTimer->setInterval(50); // 20Hz
    connect(trackingTimer, &QTimer::timeout,
            this, &ArmController::armTracking);

    m_timerGeneration = 0;
    m_placeDone = false;
    m_scanDone = false;
    errorX = 0;
    errorY = 0;
    x_dir = 0;
    y_dir = 0;
}

ArmController::~ArmController()
{
    stop();
}

// 通用8字节包构建函数
QByteArray ArmController::build8BytePacket(quint8 header, quint8 cmd,
                                          quint8 p1, quint8 p2, quint8 p3, quint8 p4)
{
    QByteArray pkt;
    pkt.append(header);    // 帧头
    pkt.append(0x08);      // 固定8字节长度
    pkt.append(cmd);       // 指令码
    pkt.append(p1);        // 参数1
    pkt.append(p2);        // 参数2
    pkt.append(p3);        // 参数3
    pkt.append(p4);        // 参数4
    
    // 计算校验和（前7字节求和）
    quint8 checksum = 0;
    for (int i = 0; i < 7; ++i) {
        checksum += static_cast<quint8>(pkt[i]);
    }
    pkt.append(checksum);  // 校验和
    
    return pkt;
}

void ArmController::placeObject()
{
    if (!serial || !serial->isOpen()) {
        emit statusChanged("串口未连接");
        return;
    }

    // 8字节放置指令包
    QByteArray pkt = build8BytePacket(0xAA, 0xC2, 0x00, 0x00, 0x00, 0x00);
    serial->sendData(pkt);
    emit statusChanged("发送放置指令");
}

void ArmController::armTrack(int x_error, int y_error, int x, int y)
{
    if (!serial || !serial->isOpen()) {
        emit statusChanged("串口未连接");
        return;
    }

    // 8字节追踪指令包
    QByteArray pkt = build8BytePacket(0xAA, 0xB1,
                                     static_cast<quint8>(x_error),
                                     static_cast<quint8>(y_error),
                                     static_cast<quint8>(x),
                                     static_cast<quint8>(y));
    qDebug() << "[TX] ArmTrack:" << pkt.toHex(' ').toUpper()
             << "x_err=" << x_error << "y_err=" << y_error
             << "x_dir=" << x << "y_dir=" << y;
    serial->sendData(pkt);
}

void ArmController::stop()
{
    trackingTimer->stop();
    if (serial && serial->isOpen()) {
        QByteArray pkt = build8BytePacket(0xAA, 0xFF, 0x00, 0x00, 0x00, 0x00);
        serial->sendData(pkt);
        emit statusChanged("机械臂已停止");
    }
}

void ArmController::handleSerialData(const QByteArray &data)
{
    QMutexLocker locker(&g_armRecvMutex);
    qDebug() << "[Arm RX raw]:" << data.toHex(' ');
    recvBuffer.append(data);

    // 检测机械臂完成回复（4字节）
    while (recvBuffer.size() >= 4) {
        if (quint8(recvBuffer[0]) == 0xAA &&
            quint8(recvBuffer[1]) == 0xBB &&
            quint8(recvBuffer[2]) == 0xCC &&
            quint8(recvBuffer[3]) == 0xDD) {

            qDebug() << "机械臂动作完成";
            m_placeDone = true;
            m_scanDone = true;
            emit actionFinished();
            recvBuffer.remove(0, 4);
        } else {
            recvBuffer.remove(0, 1); // 滑动窗口
        }
    }
}

void ArmController::armMoveTo(int colorNum, int op)
{
    uint8_t param = ((op & 0x0F) << 4) | (colorNum & 0x0F);
    qDebug() << "armMoveTo: op=" << op << ", color=" << colorNum;

    if (!serial || !serial->isOpen()) {
        emit statusChanged("串口未连接");
        return;
    }

    // 8字节动作指令包
    QByteArray pkt = build8BytePacket(0xAA, 0xC1, param, 0x00, 0x00, 0x00);
    serial->sendData(pkt);
}

void ArmController::updateError(int dx, int dy, uint8_t xdir, uint8_t ydir)
{
    errorX = dx;
    errorY = dy;
    x_dir = xdir;
    y_dir = ydir;
}

void ArmController::startTracking()
{
    m_timerGeneration++;
    int myGen = m_timerGeneration;

    errorX = 1000;
    errorY = 1000;
    trackingTimer->start();

    // 15秒超时保护
    QTimer::singleShot(10000, this, [this, myGen]() {
        if (myGen != m_timerGeneration || !trackingTimer->isActive()) return;
        qDebug() << "[ArmCtrl] 追踪超时，强制结束";
        trackingTimer->stop();
        emit actionFinished();
    });
}

void ArmController::armTracking()
{
    const float deadarea = 3.0f;
    if (std::abs(errorX) < deadarea && std::abs(errorY) < deadarea) {
        qDebug() << "追踪完成（进入死区）";
        trackingTimer->stop();
        emit actionFinished();
        return;
    }

    // 发送8字节追踪指令
    QByteArray pkt = build8BytePacket(0xAA, 0xB1,
                                     static_cast<quint8>(errorX),
                                     static_cast<quint8>(errorY),
                                     x_dir, y_dir);
    qDebug() << "[TX] ArmTrack:" << pkt.toHex(' ').toUpper()
             << "x_err=" << errorX << "y_err=" << errorY
             << "x_dir=" << x_dir << "y_dir=" << y_dir;
    serial->sendData(pkt);
}

void ArmController::armPlace()
{
    qDebug() << "执行放置动作";
    trackingTimer->stop();      // 防止 Step 1 的 trackingTimer 继续发 0xB1
    m_timerGeneration++;
    int myGen = m_timerGeneration;

    m_placeDone = false;
    placeObject(); // 发送8字节放置指令

    // 18秒超时保护（下位机夹取约12.8s，留足余量）
    QTimer::singleShot(18000, this, [this, myGen]() {
        if (myGen != m_timerGeneration || m_placeDone) return;
        qDebug() << "[ArmCtrl] 放置超时，强制结束";
        m_placeDone = true;
        emit actionFinished();
    });
}

void ArmController::armToScanPosition()
{
    qDebug() << "[ArmCtrl] 转到扫码位置";
    m_timerGeneration++;
    int myGen = m_timerGeneration;

    if (!serial || !serial->isOpen()) {
        emit statusChanged("串口未连接");
        QTimer::singleShot(100, this, [this, myGen]() {
            if (myGen != m_timerGeneration) return;
            emit actionFinished();
        });
        return;
    }

    m_scanDone = false;
    // 8字节扫码位置指令
    QByteArray pkt = build8BytePacket(0xAA, 0xC3, 0x00, 0x00, 0x00, 0x00);
    serial->sendData(pkt);

    // 18秒超时保护
    QTimer::singleShot(18000, this, [this, myGen]() {
        if (myGen != m_timerGeneration || m_scanDone) return;
        qDebug() << "[ArmCtrl] 扫码位置超时，强制结束";
        m_scanDone = true;
        emit actionFinished();
    });
}

void ArmController::armScanDown()
{
    qDebug() << "[ArmCtrl] 扫码下放";
    m_timerGeneration++;
    int myGen = m_timerGeneration;

    if (!serial || !serial->isOpen()) {
        QTimer::singleShot(100, this, [this, myGen]() {
            if (myGen != m_timerGeneration) return;
            emit actionFinished();
        });
        return;
    }

    m_scanDone = false;
    // 8字节扫码下放指令
    QByteArray pkt = build8BytePacket(0xAA, 0xC4, 0x00, 0x00, 0x00, 0x00);
    serial->sendData(pkt);

    // 18秒超时保护
    QTimer::singleShot(18000, this, [this, myGen]() {
        if (myGen != m_timerGeneration || m_scanDone) return;
        qDebug() << "[ArmCtrl] 扫码下放超时，强制结束";
        m_scanDone = true;
        emit actionFinished();
    });
}

void ArmController::testt(int p)
{
    if (!serial || !serial->isOpen()) {
        emit statusChanged("串口未连接");
        return;
    }
    QByteArray pkt = build8BytePacket(0xAA, 0xC1, static_cast<quint8>(p), 0x00, 0x00, 0x00);
    serial->sendData(pkt);
}

void ArmController::test2()
{
    if (!serial || !serial->isOpen()) {
        emit statusChanged("串口未连接");
        return;
    }
    QByteArray pkt = build8BytePacket(0xAA, 0xC2, 0x00, 0x00, 0x00, 0x00);
    serial->sendData(pkt);
}