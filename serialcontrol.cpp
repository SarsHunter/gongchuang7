#include "serialcontrol.h"
#include <QDebug>

SerialControl::SerialControl(const QString &portName, qint32 baudRate, QObject *parent)
    : QObject(parent)
{
    serial.setPortName(portName);
    serial.setBaudRate(baudRate);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    connect(&serial, &QSerialPort::readyRead, this, &SerialControl::handleReadyRead);
    connect(&serial, &QSerialPort::errorOccurred, this, &SerialControl::handleError);

    if (serial.open(QIODevice::ReadWrite)) {
        qDebug() << "串口打开成功:" << portName;
    } else {
        qDebug() << "串口打开失败:" << serial.errorString();
        emit connectionError(serial.errorString());
    }
}

SerialControl::~SerialControl()
{
    if (serial.isOpen()) {
        serial.close();
    }
}

void SerialControl::sendData(const QByteArray &data)
{
    if (serial.isOpen()) {
        serial.write(data);
        serial.flush();
    }
}

bool SerialControl::isOpen() const
{
    return serial.isOpen();
}

void SerialControl::handleReadyRead()
{
    QByteArray data = serial.readAll();
    while (serial.waitForReadyRead(10)) {
        data += serial.readAll();
    }
    emit dataReceived(data);
}

void SerialControl::handleError(QSerialPort::SerialPortError error)
{
    if (error != QSerialPort::NoError) {
        qDebug() << "串口错误:" << serial.errorString();
        emit connectionError(serial.errorString());
    }
}
