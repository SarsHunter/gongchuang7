#include "armcontroller.h"

#include <QDebug>
#include <QCoreApplication>

ArmController::ArmController(QObject *parent) : QObject(parent)
{
    // ttyS4 或 ttyS6
    serial = new SerialControl("/dev/ttyS6", 115200, this);

//    connect(serial, &SerialControl::dataReceived,
//            this, &CarController::handleSerialData);
//    connect(serial, &SerialControl::connectionError,
//            this, [this](const QString &error) {
//                currentStatus = "连接错误: " + error;
//                emit statusChanged(currentStatus);
//            });
}

ArmController::~ArmController()
{
    stop();
}

void ArmController::armTrack(int x_error,int y_error,int x,int y)
{
    if (!serial->isOpen()) {
        emit statusChanged("串口未连接");
        return;
    }



    QByteArray cmd;

    // 2. 组包
    cmd.append(0xAA);                 // 帧头
    cmd.append(0x08);                 // 包长
    cmd .append(0xB1);
    // 误差

    cmd.append(static_cast<char>(x_error));
    cmd.append(static_cast<char>(y_error));

    cmd.append(static_cast<char>(x));
    cmd.append(static_cast<char>(y));

    // 3. 计算校验和 (修正了 data -> cmd)
    quint8 checksum = 0;
    for (int i = 0; i < cmd.size(); ++i) {
        checksum += static_cast<quint8>(cmd[i]);
    }
    cmd.append(checksum);

    // 4. 发送
    serial->sendData(cmd);

    // 5. 更新状态 (修正了变量名)
//    currentStatus = QString("指令发送: 方向=%1, 速度=%2, 距离=%3")
//                       .arg(direction).arg(velocityInt / 10).arg(distanceInt / 10);

}


void ArmController::armStatus()
{
    if (!serial->isOpen()) {
        emit statusChanged("串口未连接");
        return;
    }

//    QByteArray cmd;
//    cmd.append(QString("DIST:%1,%2\n").arg(distance).arg(speed).toUtf8());
//    serial->sendData(cmd);

//    currentStatus = QString("移动距离: %1cm, 速度=%2cm/s").arg(distance).arg(speed);
//    emit statusChanged(currentStatus);
}

void ArmController::stop()
{
//    if (serial->isOpen()) {
//        serial->sendData("STOP\n");
//        currentStatus = "已停止";
//        emit statusChanged(currentStatus);
//    }
}

//bool ArmController::isConnected() const
//{
//    return serial->isOpen();
//}

//void ArmController::handleSerialData(const QByteArray &data)
//{
//    qDebug() << "接收到底盘数据:" << data;
//    // 这里可以根据实际协议解析反馈数据
//}
