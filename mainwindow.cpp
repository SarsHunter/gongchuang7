#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
        , capturer(nullptr)       // 必须初始化为空指针
        , data_lock(nullptr)      // 必须初始化为空指针
        , m_carCtrl(nullptr)      // 必须初始化为空指针
        , m_armCtrl(nullptr)
        , isDetecting(false)

    , imageScene(nullptr)
        , imageView(nullptr)

{
    ui->setupUi(this);
    initUI();

}

MainWindow::~MainWindow()
{
    if (capturer) {
          capturer->stop(); // 假设你的线程里有stop方法，如果没有需要加一个

          delete capturer;
          capturer = nullptr;
      }

      if (data_lock) {
          delete data_lock;
          data_lock = nullptr;
      }


    delete ui;
}



void MainWindow::updateFrame(cv::Mat *mat){
    data_lock->lock();
    currentFrame = *mat;
    data_lock->unlock();

    cv::cvtColor(currentFrame,currentFrame,cv::COLOR_BGR2RGB);

    QImage frame(
        currentFrame.data,
        currentFrame.cols,
        currentFrame.rows,
        currentFrame.step,
        QImage::Format_RGB888);
    QPixmap image = QPixmap::fromImage(frame);

    imageScene->clear();
//    imageView->resetMatrix();
    imageScene->addPixmap(image);
    imageScene->update();
    imageView->setSceneRect(image.rect());

}

void MainWindow::updatePreview(cv::Mat *mask)
{
    data_lock->lock();
    currentMask = *mask;
    data_lock->unlock();
    // 注意：mask 是单通道 8-bit (CV_8U)
    // 使用 Format_Grayscale8 格式构建 QImage，无需手动转换
    QImage preview(
        currentMask.data,
        currentMask.cols,
        currentMask.rows,
        currentMask.step,
        QImage::Format_Grayscale8);

    QImage show = preview.scaled(QSize(320,240),Qt::KeepAspectRatio,Qt::SmoothTransformation);

    QPixmap pixmap = QPixmap::fromImage(show);

    // 显示预览图
    previewScene->clear();
    previewScene->addPixmap(pixmap);
    previewScene->update();
    ui->preView->setSceneRect(pixmap.rect());
}



void MainWindow::openCamera()
{
    capturer = new videothread(0, data_lock);

    // 初始化HSV阈值
    capturer->setHSVThreshold(
        ui->hMinSpin->value(), ui->hMaxSpin->value(),
        ui->sMinSpin->value(), ui->sMaxSpin->value(),
        ui->vMinSpin->value(), ui->vMaxSpin->value()
    );
    //连接原始画面
    connect(capturer, &videothread::frameCaptured, this, &MainWindow::updateFrame);

    // 连接掩膜预览流
    connect(capturer, &videothread::previewReady, this, &MainWindow::updatePreview);

    connect(capturer, &videothread::qrCodeResult, this, [=](const QString &text){
        static QString last;
        if(text == last) return;
        last = text;
        ui->qrLabel->setText(text);
    });
    capturer->start();
}







void MainWindow::on_detectBtn_clicked()
{
    if (!capturer) return; // 确保线程已创建

        isDetecting = !isDetecting; // 切换状态

        // 设置线程里的检测标志
        capturer->setDetecting(isDetecting);

        // 更新按钮文字，提供直观反馈
        if (isDetecting) {
            ui->detectBtn->setText("停止检测");
            ui->detectBtn->setStyleSheet("background-color: #ffcccc;"); // 可选：变红一点提示正在运行
        } else {
            ui->detectBtn->setText("开启检测");
            ui->detectBtn->setStyleSheet(""); // 恢复默认样式
        }
}


//测试移动按钮
// 前进：方向设为 1，速度 30 cm/s，距离 20 cm
void MainWindow::on_frontBtn_clicked()
{
      m_armCtrl->armTrack(0,5,0,1);
//    m_carCtrl->carMoveDistance(1, 50.0f, 300.0f);
}

// 后退：方向设为 2，速度 30 cm/s，距离 20 cm
void MainWindow::on_backBtn_clicked()
{
    m_armCtrl->armTrack(0,5,0,0);
//    m_carCtrl->carMoveDistance(2, 50.0f, 300.0f);
}

// 左转：方向设为 3，速度 20 cm/s，距离 10 cm
void MainWindow::on_leftBtn_clicked()
{
    m_armCtrl->armTrack(5,0,0,0);
//    m_carCtrl->carMoveDistance(4, 50.0f, 300.0f);
}

// 右转：方向设为 4，速度 20 cm/s，距离 10 cm
void MainWindow::on_rightBtn_clicked()
{
       m_armCtrl->armTrack(5,0,1,0);
//    m_carCtrl->carMoveDistance(3, 50.0f, 300.0f);
}


void MainWindow::initUI()
{
    // ==================== 1. 初始化场景和互斥锁 ====================
    imageScene = new QGraphicsScene(this);
    imageView = ui->graphicsView;
    imageView->setScene(imageScene);

    // === 预览 Mask 显示 ===
    previewScene = new QGraphicsScene(this);
    ui->preView->setScene(previewScene); // 关联到 UI 上的 preView
    data_lock = new QMutex();

    // ==================== 2. 初始化 HSV 控件 ====================
    // 先阻塞信号，防止在设置初始值时触发槽函数导致逻辑混乱或崩溃
    ui->hMinSpin->blockSignals(true);
    ui->hMaxSpin->blockSignals(true);
    ui->sMinSpin->blockSignals(true);
    ui->sMaxSpin->blockSignals(true);
    ui->vMinSpin->blockSignals(true);
    ui->vMaxSpin->blockSignals(true);

    ui->hMinSlider->blockSignals(true);
    ui->hMaxSlider->blockSignals(true);
    ui->sMinSlider->blockSignals(true);
    ui->sMaxSlider->blockSignals(true);
    ui->vMinSlider->blockSignals(true);
    ui->vMaxSlider->blockSignals(true);

    // 设置默认值
    ui->hMinSpin->setValue(35);
    ui->hMaxSpin->setValue(85);
    ui->sMinSpin->setValue(43);
    ui->sMaxSpin->setValue(255);
    ui->vMinSpin->setValue(46);
    ui->vMaxSpin->setValue(255);

    ui->hMinSlider->setValue(35);
    ui->hMaxSlider->setValue(85);
    ui->sMinSlider->setValue(43);
    ui->sMaxSlider->setValue(255);
    ui->vMinSlider->setValue(46);
    ui->vMaxSlider->setValue(255);

    // 恢复信号连接
    ui->hMinSpin->blockSignals(false);
    ui->hMaxSpin->blockSignals(false);
    ui->sMinSpin->blockSignals(false);
    ui->sMaxSpin->blockSignals(false);
    ui->vMinSpin->blockSignals(false);
    ui->vMaxSpin->blockSignals(false);

    ui->hMinSlider->blockSignals(false);
    ui->hMaxSlider->blockSignals(false);
    ui->sMinSlider->blockSignals(false);
    ui->sMaxSlider->blockSignals(false);
    ui->vMinSlider->blockSignals(false);
    ui->vMaxSlider->blockSignals(false);

    // ==================== 3. 按钮连接 ====================
    connect(ui->opencamBtn, SIGNAL(clicked()), this, SLOT(openCamera()));

    // ==================== 4. 小车控制器初始化 ====================
    // 注意：如果这里因为串口权限等问题导致 CarController 抛出异常，
    // 程序也会崩溃。如果 RK3588 上没有串口或权限不足，建议先注释掉这段代码测试。
    m_carCtrl = new CarController(this);

    m_armCtrl = new ArmController(this);

    connect(m_carCtrl, &CarController::statusChanged,
            this, [this](const QString &status) {
                ui->statusbar->showMessage(status, 0);
            });
}


void MainWindow::updateHSVThreshold()
{
    if (!capturer) return;

    int hMin = ui->hMinSpin->value();
    int hMax = ui->hMaxSpin->value();
    int sMin = ui->sMinSpin->value();
    int sMax = ui->sMaxSpin->value();
    int vMin = ui->vMinSpin->value();
    int vMax = ui->vMaxSpin->value();

    // 确保最小值不大于最大值
    if (hMin > hMax) hMax = hMin;
    if (sMin > sMax) sMax = sMin;
    if (vMin > vMax) vMax = vMin;

    // 更新线程中的HSV阈值
    capturer->setHSVThreshold(hMin, hMax, sMin, sMax, vMin, vMax);

    // 可选：在状态栏显示当前阈值
    ui->statusbar->showMessage(QString("HSV: H[%1-%2] S[%3-%4] V[%5-%6]")
                                .arg(hMin).arg(hMax)
                                .arg(sMin).arg(sMax)
                                .arg(vMin).arg(vMax), 2000);
}



// ==================== HSV 滑条回调函数 ====================
// H 通道
void MainWindow::on_hMaxSlider_valueChanged(int value)
{
    ui->hMaxSpin->setValue(value);
    updateHSVThreshold();
}

void MainWindow::on_hMinSlider_valueChanged(int value)
{
    ui->hMinSpin->setValue(value);
    updateHSVThreshold();
}

// S 通道
void MainWindow::on_sMaxSlider_valueChanged(int value)
{
    ui->sMaxSpin->setValue(value);
    updateHSVThreshold();
}

void MainWindow::on_sMinSlider_valueChanged(int value)
{
    ui->sMinSpin->setValue(value);
    updateHSVThreshold();
}

// V 通道
void MainWindow::on_vMaxSlider_valueChanged(int value)
{
    ui->vMaxSpin->setValue(value);
    updateHSVThreshold();
}

void MainWindow::on_vMinSlider_valueChanged(int value)
{
    ui->vMinSpin->setValue(value);
    updateHSVThreshold();
}

// ==================== HSV SpinBox 回调函数 ====================
// 注意：如果 SpinBox 没有报错，可以不写下面这些，但为了双向同步最好加上

void MainWindow::on_hMaxSpin_valueChanged(int value)
{
    ui->hMaxSlider->setValue(value);
    updateHSVThreshold();
}

void MainWindow::on_hMinSpin_valueChanged(int value)
{
    ui->hMinSlider->setValue(value);
    updateHSVThreshold();
}

void MainWindow::on_sMaxSpin_valueChanged(int value)
{
    ui->sMaxSlider->setValue(value);
    updateHSVThreshold();
}

void MainWindow::on_sMinSpin_valueChanged(int value)
{
    ui->sMinSlider->setValue(value);
    updateHSVThreshold();
}

void MainWindow::on_vMaxSpin_valueChanged(int value)
{
    ui->vMaxSlider->setValue(value);
    updateHSVThreshold();
}

void MainWindow::on_vMinSpin_valueChanged(int value)
{
    ui->vMinSlider->setValue(value);
    updateHSVThreshold();
}













