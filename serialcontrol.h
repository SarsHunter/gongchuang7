#ifndef SERIALCONTROL_H
#define SERIALCONTROL_H

#include <QObject>
#include <QSerialPort>
#include <QByteArray>

class SerialControl : public QObject
{
    Q_OBJECT
public:
    explicit SerialControl(const QString &portName, qint32 baudRate, QObject *parent = nullptr);
    ~SerialControl();

    void sendData(const QByteArray &data);
    bool isOpen() const;

signals:
    void dataReceived(const QByteArray &data);
    void connectionError(const QString &error);

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort serial;
};

#endif // SERIALCONTROL_H
