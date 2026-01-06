#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <atomic>
#include <QThread>
#include <QMutex>
#include <QString>
#include <opencv2/opencv.hpp>
#include <zbar.h>


class videothread : public QThread
{
    Q_OBJECT

public:
    explicit videothread(int camera,QMutex *lock);
    ~videothread();
    void stop();


    void startVideo();
    void stopVideo();

    cv::Mat getFrame();

    void setDetecting(bool enable) {
        detecting = enable;
    }

    void setQrDetecting(bool enable){
        qrdetecing = enable;
    }

    //设置HSV阈值
    void setHSVThreshold(int hMin, int hMax, int sMin, int sMax, int vMin, int vMax);


protected:
    void run() override;

signals:
    void frameCaptured(cv::Mat *data);
    void previewReady(cv::Mat *mask); // 发送掩膜图

    void qrCodeResult(const QString &text);

private:
    bool running;
    int cameraID;
    QMutex *data_lock;
    cv::Mat frame;


    // HSV阈值成员变量
    int h_min = 35, h_max = 85;
    int s_min = 43, s_max = 255;
    int v_min = 46, v_max = 255;

    std::atomic<bool> detecting = false;
    void circleDetect(cv::Mat &frame, const cv::Mat &mask);

    cv::QRCodeDetector qrDetector;
    std::atomic<bool> qrdetecing = true;
    void qrCode(cv::Mat &frame);

};

#endif // VIDEOTHREAD_H
