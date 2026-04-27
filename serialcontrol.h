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


    template<typename... Args>
    void sendPacket(quint8 header, Args... data)
    {
        QByteArray packet = buildPacket(header, data...);
        sendData(packet);
    }

signals:
    void dataReceived(const QByteArray &data);
    void connectionError(const QString &error);

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort serial;

    template<typename... Args>
    QByteArray buildPacket(quint8 header, Args... data)
    {
        QByteArray packet;

        // 1payload
        QByteArray payload;
        (payload.append(static_cast<char>(data)), ...);

        // 2length
        quint8 length = payload.size() + 3;

        // 3header + length
        packet.append(static_cast<char>(header));
        packet.append(static_cast<char>(length));

        // 4data
        packet.append(payload);

        // 5checksum
        quint8 sum = 0;
        for (auto byte : packet)
            sum += static_cast<quint8>(byte);

        packet.append(static_cast<char>(sum));

        return packet;
    }

};

#endif // SERIALCONTROL_H
