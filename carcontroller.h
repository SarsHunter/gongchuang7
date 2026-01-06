#ifndef CARCONTROLLER_H
#define CARCONTROLLER_H

#include "serialcontrol.h"
#include <QObject>

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

    // 停止
    void stop();

    bool isConnected() const;

signals:
    void statusChanged(const QString &status);

private slots:
    void handleSerialData(const QByteArray &data);

private:
    SerialControl *serial;
    QString currentStatus;
};

#endif // CARCONTROLLER_H
