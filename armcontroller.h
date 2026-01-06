#ifndef ARMCONTROLLER_H
#define ARMCONTROLLER_H

#include "serialcontrol.h"
#include <QObject>

class ArmController:public QObject
{
    Q_OBJECT
public:
    explicit ArmController(QObject *parent = nullptr);
    ~ArmController();

    void armTrack(int x_error,int y_error,int x,int y);
    void armStatus();

    void stop();
signals:
    void statusChanged(const QString &status);

private slots:
//    void handleSerialData(const QByteArray &data);

private:
    SerialControl *serial;
    QString currentStatus;

};

#endif // ARMCONTROLLER_H
