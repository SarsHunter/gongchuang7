#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMutex>

#include "taskmanager.h"
#include "carcontroller.h"
#include "armcontroller.h"
#include "videothread.h"

#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // ── 初始化 ──
    void initUI();
    void initCamera();

    // ── JSON 加载 ──
    QList<Task> loadTasksFromJson(const QString &filePath);

    // ── 任务表格 ──
    void updateTaskTable      (const QList<Task> &queue);
    void highlightCurrentTask (int index);

    // ── UI 锁定 ──
    void disableManualUI();
    void enableManualUI();

    // ── QR 解析 ──
    QList<int> parseQRCode(const QString &text, int side);

private slots:
    // 摄像头
    void openCamera();
    void updateFrame  (QImage frame);
    void updatePreview(QImage mask);

    // 圆形检测
    void on_detectBtn_clicked();

    // 手动移动
    void on_frontBtn_clicked();
    void on_backBtn_clicked();
    void on_leftBtn_clicked();
    void on_rightBtn_clicked();

    // 视觉追踪
    void on_trackBtn_clicked();

    // HSV 滑条 / SpinBox 联动
    void on_hMaxSlider_valueChanged(int value);
    void on_hMinSlider_valueChanged(int value);
    void on_sMaxSlider_valueChanged(int value);
    void on_sMinSlider_valueChanged(int value);
    void on_vMaxSlider_valueChanged(int value);
    void on_vMinSlider_valueChanged(int value);

    void on_hMaxSpin_valueChanged(int value);
    void on_hMinSpin_valueChanged(int value);
    void on_sMaxSpin_valueChanged(int value);
    void on_sMinSpin_valueChanged(int value);
    void on_vMaxSpin_valueChanged(int value);
    void on_vMinSpin_valueChanged(int value);

    void on_colorComboBox_currentIndexChanged(int index);
    void updateHSVThreshold();

    // 任务管理
    void on_RunBtn_clicked();       // 选择 JSON → 加载 → 按顺序执行
    void on_loadJsonBtn_clicked();  // 仅加载，不执行
    void on_doBtn_clicked();        // 执行已加载的队列
    void on_addTaskBtn_clicked();   // 手动添加一个 ArmTrack 任务

    // 测试
    void on_testBtn_clicked();
    void on_testBtn2_clicked();

signals:
    void setHSV(int hMin, int hMax, int sMin, int sMax, int vMin, int vMax);

private:
    Ui::MainWindow *ui;

    // 摄像头
    videothread    *capturer   = nullptr;
    QMutex         *data_lock  = nullptr;

    // 显示场景
    QGraphicsScene *imageScene   = nullptr;
    QGraphicsScene *previewScene = nullptr;

    // 控制器
    CarController  *m_carCtrl    = nullptr;
    ArmController  *m_armCtrl    = nullptr;

    // 任务管理器
    TaskManager    *m_taskManager = nullptr;

    // 状态标志
    bool isDetecting = false;
    bool isTracking  = false;

    // 最近一次 QR 扫码结果
    QString m_qrResult;
};

#endif // MAINWINDOW_H
