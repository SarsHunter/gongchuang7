#include "carcontroller.h"

#include <QDebug>
#include <QCoreApplication>

CarController::CarController(QObject *parent) : QObject(parent)
{
    // 根据RK3588平台，串口可能是 /dev/ttyS1 或 /dev/ttyTHS1
    serial = new SerialControl("/dev/ttyS4", 115200, this);

    connect(serial, &SerialControl::dataReceived,
            this, &CarController::handleSerialData);
    connect(serial, &SerialControl::connectionError,
            this, [this](const QString &error) {
                currentStatus = "连接错误: " + error;
                emit statusChanged(currentStatus);
            });
}

CarController::~CarController()
{
    stop();
}

void CarController::carMoveDistance(int direction, float v, float d)
{
    if (!serial->isOpen()) {
        emit statusChanged("串口未连接");
        return;
    }

    // 1. 将浮点数转换为整数 (假设单位是 cm 和 cm/s，如果需要小数精度，请先乘以倍数)
    int16_t distanceInt = static_cast<int16_t>(d * 10);
    int16_t velocityInt = static_cast<int16_t>(v * 10);

    QByteArray cmd;

    // 2. 组包
    cmd.append(0xAB);                 // 帧头
    cmd.append(0x09);                 // 包长
    cmd.append(0x01);                 // 模式
    cmd.append(static_cast<char>(direction & 0xFF)); // 方向

    // 距离 (低位在前)
    cmd.append(static_cast<char>(distanceInt & 0xFF));
    cmd.append(static_cast<char>((distanceInt >> 8) & 0xFF));

    // 速度 (低位在前)
    cmd.append(static_cast<char>(velocityInt & 0xFF));
    cmd.append(static_cast<char>((velocityInt >> 8) & 0xFF));

    // 3. 计算校验和 (修正了 data -> cmd)
    quint8 checksum = 0;
    for (int i = 0; i < cmd.size(); ++i) {
        checksum += static_cast<quint8>(cmd[i]);
    }
    cmd.append(checksum);

    // 4. 发送
    serial->sendData(cmd);

    // 5. 更新状态 (修正了变量名)
    currentStatus = QString("指令发送: 方向=%1, 速度=%2, 距离=%3")
                       .arg(direction).arg(velocityInt / 10).arg(distanceInt / 10);
    emit statusChanged(currentStatus);
}


void CarController::carMoveSpeed(float speed)
{
    if (!serial->isOpen()) {
        emit statusChanged("串口未连接");
        return;
    }

//    QByteArray cmd;
////    cmd.append(QString("DIST:%1,%2\n").arg(distance).arg(speed).toUtf8());
//    serial->sendData(cmd);

//    currentStatus = QString("移动距离: %1cm, 速度=%2cm/s").arg(distance).arg(speed);
//    emit statusChanged(currentStatus);
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

void CarController::handleSerialData(const QByteArray &data)
{
    qDebug() << "接收到底盘数据:" << data;
    // 这里可以根据实际协议解析反馈数据
}
