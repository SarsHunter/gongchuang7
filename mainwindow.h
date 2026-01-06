#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>


#include <QMutex>


#include "carcontroller.h"
#include "armcontroller.h"

#include "videothread.h"
#include "opencv2/opencv.hpp"





QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initUI();

private slots:
    //cam
    void updateFrame(cv::Mat*);
    void openCamera();

    //更新预览图的槽函数
    void updatePreview(cv::Mat *);


    void on_detectBtn_clicked();

    //测试
    // 自动连接槽函数，命名规则：on_对象名_信号名()
       void on_frontBtn_clicked();  // 前进
       void on_backBtn_clicked();   // 后退
       void on_leftBtn_clicked();   // 左转
       void on_rightBtn_clicked();  // 右转
//       void on_stopBtn_clicked();   // 停止

       //更新hsv阈值
       void on_hMaxSlider_valueChanged(int value);
       void on_hMinSlider_valueChanged(int value);
       void on_sMaxSlider_valueChanged(int value);
       void on_sMinSlider_valueChanged(int value);
       void on_vMaxSlider_valueChanged(int value);
       void on_vMinSlider_valueChanged(int value);

       // 如果实现了 SpinBox 的回调，也要加上
           void on_hMaxSpin_valueChanged(int arg1);
       void on_hMinSpin_valueChanged(int arg1);
       void on_sMaxSpin_valueChanged(int arg1);
       void on_sMinSpin_valueChanged(int arg1);
       void on_vMaxSpin_valueChanged(int arg1);
       void on_vMinSpin_valueChanged(int arg1);

       void updateHSVThreshold(); // 统一更新阈值



private:
    Ui::MainWindow *ui;

    SerialControl *serial;



    QGraphicsScene *imageScene;
    QGraphicsView *imageView;

    // 新增：预览界面的 Scene
    QGraphicsScene *previewScene;



    cv::Mat currentFrame;
    cv::Mat currentMask;
    //for videothread
    QMutex *data_lock;
    videothread *capturer;





    bool isDetecting;


    //
    ArmController *m_armCtrl;   //机械臂控制
    CarController *m_carCtrl;    // 地盘控制器实例指针
};
#endif // MAINWINDOW_H
