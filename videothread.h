#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QThread>
#include <QMutex>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <zbar.h>

struct HSVThreshold
{
    int h_min;
    int h_max;
    int s_min;
    int s_max;
    int v_min;
    int v_max;
}; 

class videothread : public QThread
{
    Q_OBJECT

public:
    explicit videothread(int camera, QMutex *lock);
    ~videothread();

    void stop();

    void setHSVThreshold(int hMin,int hMax,
                         int sMin,int sMax,
                         int vMin,int vMax);

    void setColorType(int type);


    HSVThreshold getThreshold(int type);

    void enableBlockDetect(bool flag);
    void enableCircleDetect(bool flag);
    void enableQRDetect(bool flag);




signals:

    void frameCaptured(QImage);
    void maskCaptured(QImage);
    void qrCodeResult(QString);

    void circleError(int dx, int dy, uint8_t x_dir, uint8_t y_dir);

protected:
    void run() override;

private:

    cv::Mat createMask(cv::Mat &frame);

    void detectColorBlock(cv::Mat &frame, cv::Mat &mask);

    void detectCircle(cv::Mat &frame, cv::Mat &mask);

    void detectQRCode(cv::Mat &frame);

    QImage matToQimage(cv::Mat &frame);


    zbar::ImageScanner scanner;
    QString lastQRResult;


private:

    bool running;

    int cameraID;

    QMutex *data_lock;

    int currentColorType;

    HSVThreshold redThreshold;
    HSVThreshold greenThreshold;
    HSVThreshold blueThreshold;

    bool blockDetecting;
    bool circleDetecting;
    bool qrDetecting;



    cv::Point2f lastCenter;  //
    bool hasLastCenter = false; //
    int lostFrames = 0;       //
    const int maxLostFrames = 3; //


};

#endif
