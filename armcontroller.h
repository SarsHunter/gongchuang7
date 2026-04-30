#ifndef ARMCONTROLLER_H
#define ARMCONTROLLER_H

#include "serialcontrol.h"
#include <QObject>
#include <QTimer>

class ArmController:public QObject
{
    Q_OBJECT
public:
    explicit ArmController(QObject *parent = nullptr);
    ~ArmController();

    void testt(int p);
    void test2();

    void placeObject();
    void armTrack(int x_error,int y_error,int x,int y);
    void armStatus();
    void handleFrame(const QByteArray &frame);

    void stop();
    void armMoveTo(int colorNum = 1,int op = 1);
    
    void startTracking();

    void armPlace();

    // 颜色分拣：通知下位机根据颜色自动抓取并放置
    // 下位机新增 case 0xC5，收到后自动完成整套动作，完成后回复 ArmDone
    void armColorSort(int color);

    // 扫码前姿态：转到扫码方向 + 抬起摄像头
    // 发送 0xAA 0xC3，STM32 完成后回 0xAA BB CC DD → emit actionFinished()
    void armToScanPosition();
    void armScanDown();
signals:
    void statusChanged(const QString &status);
    void taskFinished();

    void actionFinished();
private slots:

    void handleSerialData(const QByteArray &data);

public slots:
    void armTracking();
    void updateError(int dx, int dy, uint8_t xdir, uint8_t ydir);

private:
    SerialControl *serial;
    QByteArray recvBuffer;
    QString currentStatus;
    QByteArray build8BytePacket(quint8 header, quint8 cmd,
                                quint8 p1, quint8 p2, quint8 p3, quint8 p4);


    QTimer *trackingTimer;

    int errorX = 0;
    int errorY = 0;

    uint8_t x_dir = 0;
    uint8_t y_dir = 0;
    bool m_placeDone = false;
    bool m_scanDone  = false;
    int  m_timerGeneration = 0; // 每次新动作+1，让旧Timer回调失效
};

#endif // ARMCONTROLLER_H
