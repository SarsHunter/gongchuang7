#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

// ─────────────────────────────────────────────────────────────
// 构造 / 析构
// ─────────────────────────────────────────────────────────────
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initUI();
}

MainWindow::~MainWindow()
{
    if (capturer) {
        capturer->stop();
        delete capturer;
    }
    delete data_lock;
    delete ui;
}

// ─────────────────────────────────────────────────────────────
// 初始化
// ─────────────────────────────────────────────────────────────
void MainWindow::initUI()
{
    // ── 显示场景 ──
    data_lock    = new QMutex();
    imageScene   = new QGraphicsScene(this);
    previewScene = new QGraphicsScene(this);

    ui->graphicsView->setScene(imageScene);
    ui->preView->setScene(previewScene);

    // ── 控制器 ──
    m_carCtrl = new CarController(this);
    m_armCtrl = new ArmController(this);

    connect(m_carCtrl, &CarController::statusChanged,
            this, [this](const QString &s){ ui->statusbar->showMessage(s, 0); });

    // ── 任务管理器 ──
    m_taskManager = new TaskManager(m_carCtrl, m_armCtrl, this);

    // 摄像头开启按钮
    connect(ui->opencamBtn, &QPushButton::clicked, this, &MainWindow::openCamera);

    // QRScan 任务等待扫码时：
    //   1. 如果摄像头未打开则自动打开
    //   2. 开启 QR 检测
    connect(m_taskManager, &TaskManager::waitingForQR, this, [this]() {
        if (!capturer) openCamera();   // 摄像头未打开时自动启动
        if (capturer)  capturer->enableQRDetect(true);
    });
    
    connect(m_taskManager, &TaskManager::armTrackColorChanged, this, [this](int color) {
    if (capturer) capturer->setColorType(color);
});

    // QR 扫码完成后关闭 QR 检测，防止信号重复触发跳过后续任务
    connect(m_taskManager, &TaskManager::qrScanCompleted, this, [this]() {
        if (capturer) capturer->enableQRDetect(false);
    });

    // CarAlign 任务开始：设置颜色并开启圆形检测，给小车底盘喂误差
    connect(m_taskManager, &TaskManager::chassisAlignStarted, this, [this](int color) {
        if (!capturer) openCamera();
        if (capturer) {
            capturer->setColorType(color);
            capturer->enableCircleDetect(true);
        }
    });

    // CarAlign 任务完成：关闭圆形检测
    connect(m_taskManager, &TaskManager::chassisAlignDone, this, [this]() {
        if (capturer) capturer->enableCircleDetect(false);
    });

    // ArmTrack Step1 开始：确保摄像头已打开，并开启圆形检测
    // 圆形检测产生的 circleError 信号驱动机械臂视觉伺服
    connect(m_taskManager, &TaskManager::armTrackingStarted, this, [this]() {
        if (!capturer) openCamera();
        if (capturer)  capturer->enableCircleDetect(true);
    });

    // ArmTrack Step1 完成（进入 Step2 放置）：关闭圆形检测
    connect(m_taskManager, &TaskManager::armTrackingDone, this, [this]() {
        if (capturer) capturer->enableCircleDetect(false);
    });

    // ColorSort 任务开始：开启色块检测（遍历红/绿/蓝）
    connect(m_taskManager, &TaskManager::colorSortStarted, this, [this]() {
        if (!capturer) openCamera();
        if (capturer) capturer->enableColorSort(true);
    });

    // ColorSort 任务完成：关闭色块检测
    connect(m_taskManager, &TaskManager::colorSortDone, this, [this]() {
        if (capturer) capturer->enableColorSort(false);
    });

    // 色块检测结果 → TaskManager
    connect(capturer, &videothread::colorBlockDetected,
            m_taskManager, &TaskManager::onColorBlockDetected);

    // ── 任务列表信号 ──
    connect(m_taskManager, &TaskManager::queueUpdated,
            this, &MainWindow::updateTaskTable);
    connect(m_taskManager, &TaskManager::currentTaskChanged,
            this, &MainWindow::highlightCurrentTask);

    // ── 运行中禁用手动控制 ──
    connect(m_taskManager, &TaskManager::taskStarted,
            this, &MainWindow::disableManualUI);
    connect(m_taskManager, &TaskManager::taskStopped,
            this, &MainWindow::enableManualUI);
    connect(m_taskManager, &TaskManager::allTasksFinished,
            this, &MainWindow::enableManualUI);

    connect(m_taskManager, &TaskManager::allTasksFinished, this, [this](){
        ui->statusbar->showMessage("所有任务执行完毕", 5000);
    });

    // ── 任务表格初始化 ──
    ui->taskTable->setColumnCount(4);
    ui->taskTable->setHorizontalHeaderLabels({"No.", "Type", "Params", "Status"});
    auto *hdr = ui->taskTable->horizontalHeader();
    hdr->setSectionResizeMode(QHeaderView::Interactive);
    hdr->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->taskTable->setColumnWidth(0, 50);
    ui->taskTable->setColumnWidth(1, 100);
    ui->taskTable->setColumnWidth(3, 80);
    ui->taskTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->taskTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    // ── HSV 控件初始值（不触发槽） ──
    auto setBlocked = [this](bool b) {
        for (auto *w : {(QWidget*)ui->hMinSpin, (QWidget*)ui->hMaxSpin,
                        (QWidget*)ui->sMinSpin, (QWidget*)ui->sMaxSpin,
                        (QWidget*)ui->vMinSpin, (QWidget*)ui->vMaxSpin,
                        (QWidget*)ui->hMinSlider, (QWidget*)ui->hMaxSlider,
                        (QWidget*)ui->sMinSlider, (QWidget*)ui->sMaxSlider,
                        (QWidget*)ui->vMinSlider, (QWidget*)ui->vMaxSlider})
            w->blockSignals(b);
    };
    setBlocked(true);
    ui->colorComboBox->setCurrentIndex(0);
    on_colorComboBox_currentIndexChanged(0);
    setBlocked(false);
}

// ─────────────────────────────────────────────────────────────
// JSON 加载
// ─────────────────────────────────────────────────────────────
QList<Task> MainWindow::loadTasksFromJson(const QString &filePath)
{
    QList<Task> tasks;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "[JSON] Cannot open:" << filePath;
        ui->statusbar->showMessage("无法打开 JSON 文件: " + filePath, 3000);
        return tasks;
    }

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &err);
    file.close();

    if (err.error != QJsonParseError::NoError) {
        qWarning() << "[JSON] Parse error:" << err.errorString();
        ui->statusbar->showMessage("JSON 解析错误: " + err.errorString(), 3000);
        return tasks;
    }
    if (!doc.isArray()) {
        qWarning() << "[JSON] Root must be an array";
        return tasks;
    }

    static const QMap<QString, TaskType> typeMap {
        {"CarMove",   TaskType::CarMove},
        {"CarTurn",   TaskType::CarTurn},
        {"ArmTrack",  TaskType::ArmTrack},
        {"QRScan",    TaskType::QRScan},
        {"CarAlign",  TaskType::CarAlign},
        {"ColorSort", TaskType::ColorSort},
    };

    for (const QJsonValue &val : doc.array()) {
        if (!val.isObject()) continue;

        QJsonObject obj = val.toObject();
        QString typeStr = obj.value("type").toString();

        if (!typeMap.contains(typeStr)) {
            qWarning() << "[JSON] Unknown type:" << typeStr;
            continue;
        }

        Task task;
        task.type = typeMap[typeStr];

        if (obj.contains("params") && obj["params"].isObject()) {
            // 必须先存为局部变量，否则两次 toObject() 产生不同临时对象
            // 导致 begin()/end() 迭代器失效，进而段错误崩溃
            QJsonObject paramObj = obj["params"].toObject();
            for (auto it = paramObj.constBegin(); it != paramObj.constEnd(); ++it) {
                task.params[it.key()] = it.value().toVariant();
            }
        }

        tasks.append(task);
        qDebug() << "[JSON] Loaded:" << typeStr << task.params;
    }

    qDebug() << "[JSON] Total tasks:" << tasks.size();
    return tasks;
}

// ─────────────────────────────────────────────────────────────
// 摄像头
// ─────────────────────────────────────────────────────────────
void MainWindow::openCamera()
{
    if (capturer) return; // 避免重复打开

    capturer = new videothread(0, data_lock);

    connect(this,     &MainWindow::setHSV,
            capturer, &videothread::setHSVThreshold);

    connect(capturer, &videothread::frameCaptured,
            this,     &MainWindow::updateFrame);

    connect(capturer, &videothread::maskCaptured,
            this,     &MainWindow::updatePreview);

    connect(capturer, &videothread::qrCodeResult, this, [this](const QString &text) {
        if (text == m_qrResult) return;
        m_qrResult = text;
        ui->qrLabel->setText(text);
    });

    connect(capturer, &videothread::qrCodeResult,
            m_taskManager, &TaskManager::onQRCodeScanned);

    connect(capturer, &videothread::circleError,
            m_armCtrl, &ArmController::updateError, Qt::QueuedConnection);

    connect(capturer, &videothread::circleError,
            m_carCtrl, &CarController::updateAlignError, Qt::QueuedConnection);

    capturer->start();

    // 同步当前颜色选择
    ui->colorComboBox->setCurrentIndex(0);
    on_colorComboBox_currentIndexChanged(0);
}

void MainWindow::updateFrame(QImage frame)
{
    imageScene->clear();
    imageScene->addPixmap(QPixmap::fromImage(frame));
    imageScene->update();
    ui->graphicsView->setSceneRect(frame.rect());
}

void MainWindow::updatePreview(QImage mask)
{
    previewScene->clear();
    previewScene->addPixmap(QPixmap::fromImage(mask));
    previewScene->update();
    ui->preView->setSceneRect(mask.rect());
}

// ─────────────────────────────────────────────────────────────
// RUN 按钮：选择 JSON 文件 → 加载 → 顺序执行
// ─────────────────────────────────────────────────────────────
void MainWindow::on_RunBtn_clicked()
{
    if (m_taskManager->isRunning()) {
        m_taskManager->stopTasks();
        ui->RunBtn->setText("RUN it");
        return;
    }

    QString path = "/home/pi/GC/gongchuang7/tasks.json";

    QList<Task> tasks = loadTasksFromJson(path);
    if (tasks.isEmpty()) {
        ui->statusbar->showMessage("tasks.json 为空或不存在: " + path, 3000);
        return;
    }

    m_taskManager->clearTasks();
    m_taskManager->addTasks(tasks);
    m_taskManager->startTasks();

    ui->RunBtn->setText("STOP");
}

// ─────────────────────────────────────────────────────────────
// 仅加载 JSON（不执行）
// ─────────────────────────────────────────────────────────────
void MainWindow::on_loadJsonBtn_clicked()
{
    QString path = "/home/pi/GC/gongchuang6/tasks.json";

    QList<Task> tasks = loadTasksFromJson(path);
    if (tasks.isEmpty()) {
        ui->statusbar->showMessage("tasks.json 为空或不存在: " + path, 3000);
        return;
    }

    m_taskManager->clearTasks();
    m_taskManager->addTasks(tasks);

    ui->statusbar->showMessage(QString("已加载 %1 个任务，按 RUN it 执行").arg(tasks.size()), 3000);
}

// ─────────────────────────────────────────────────────────────
// 执行已加载的队列
// ─────────────────────────────────────────────────────────────
void MainWindow::on_doBtn_clicked()
{
    m_taskManager->startTasks();
}

// ─────────────────────────────────────────────────────────────
// 手动添加一个 ArmTrack 任务到队列
// ─────────────────────────────────────────────────────────────
void MainWindow::on_addTaskBtn_clicked()
{
    m_taskManager->addTask(Task{TaskType::ArmTrack, {}});
}

// ─────────────────────────────────────────────────────────────
// 手动移动按钮
// ─────────────────────────────────────────────────────────────
void MainWindow::on_frontBtn_clicked()
{
    QList<Task> tasks {{ TaskType::CarMove, {{"dir",1},{"speed",30.0f},{"dist",50.0f}} }};
    m_taskManager->addTasks(tasks);
    m_taskManager->startTasks();
}

void MainWindow::on_backBtn_clicked()
{
    m_carCtrl->carMoveDistance(2, 30.0f, 50.0f);
}

void MainWindow::on_leftBtn_clicked()
{
    m_carCtrl->carMoveDistance(4, 30.0f, 50.0f);
}

void MainWindow::on_rightBtn_clicked()
{
    m_carCtrl->carMoveDistance(3, 30.0f, 50.0f);
}

// ─────────────────────────────────────────────────────────────
// 圆形检测开关
// ─────────────────────────────────────────────────────────────
void MainWindow::on_detectBtn_clicked()
{
    if (!capturer) return;
    isDetecting = !isDetecting;
    capturer->enableCircleDetect(isDetecting);
    ui->detectBtn->setText(isDetecting ? "停止检测" : "开启检测");
    ui->detectBtn->setStyleSheet(isDetecting ? "background-color:#ffcccc;" : "");
}

// ─────────────────────────────────────────────────────────────
// 视觉追踪开关
// ─────────────────────────────────────────────────────────────
void MainWindow::on_trackBtn_clicked()
{
    isTracking = !isTracking;
    ui->trackBtn->setText(isTracking ? "停止追踪" : "开启追踪");
    ui->trackBtn->setStyleSheet(isTracking ? "background-color:#ccffcc; color:black;" : "");
    ui->statusbar->showMessage(isTracking ? "视觉追踪已开启" : "视觉追踪已停止", 2000);
}

// ─────────────────────────────────────────────────────────────
// 测试按钮
// ─────────────────────────────────────────────────────────────
void MainWindow::on_testBtn_clicked()  { m_armCtrl->testt(1); }
void MainWindow::on_testBtn2_clicked() { m_armCtrl->test2(); }

// ─────────────────────────────────────────────────────────────
// UI 锁定 / 解锁
// ─────────────────────────────────────────────────────────────
void MainWindow::disableManualUI()
{
    ui->frontBtn->setEnabled(false);
    ui->backBtn->setEnabled(false);
    ui->leftBtn->setEnabled(false);
    ui->rightBtn->setEnabled(false);
    ui->trackBtn->setEnabled(false);
}

void MainWindow::enableManualUI()
{
    ui->frontBtn->setEnabled(true);
    ui->backBtn->setEnabled(true);
    ui->leftBtn->setEnabled(true);
    ui->rightBtn->setEnabled(true);
    ui->trackBtn->setEnabled(true);
    ui->RunBtn->setText("RUN it");
}

// ─────────────────────────────────────────────────────────────
// 任务表格更新
// ─────────────────────────────────────────────────────────────
void MainWindow::updateTaskTable(const QList<Task> &queue)
{
    static const QMap<TaskType, QString> typeNames {
        {TaskType::CarMove,   "CarMove"},
        {TaskType::CarTurn,   "CarTurn"},
        {TaskType::ArmTrack,  "ArmTrack"},
        {TaskType::QRScan,    "QRScan"},
        {TaskType::CarAlign,  "CarAlign"},
        {TaskType::ColorSort, "ColorSort"},
    };

    ui->taskTable->setRowCount(queue.size());

    for (int i = 0; i < queue.size(); ++i) {
        const Task &t = queue[i];

        ui->taskTable->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        ui->taskTable->setItem(i, 1, new QTableWidgetItem(typeNames.value(t.type, "?")));

        QString params;
        for (auto it = t.params.cbegin(); it != t.params.cend(); ++it)
            params += it.key() + "=" + it.value().toString() + " ";
        ui->taskTable->setItem(i, 2, new QTableWidgetItem(params.trimmed()));

        if (!ui->taskTable->item(i, 3))
            ui->taskTable->setItem(i, 3, new QTableWidgetItem("Wait"));
    }
}

// ─────────────────────────────────────────────────────────────
// 高亮当前任务行
// ─────────────────────────────────────────────────────────────
void MainWindow::highlightCurrentTask(int index)
{
    const int rows = ui->taskTable->rowCount();
    if (rows == 0) return;

    for (int i = 0; i < rows; ++i) {
        QTableWidgetItem *item = ui->taskTable->item(i, 3);
        if (!item) {
            item = new QTableWidgetItem();
            ui->taskTable->setItem(i, 3, item);
        }

        if (i < index) {
            item->setText("Done");
            item->setBackground(QColor(144, 238, 144)); // 绿
        } else if (i == index) {
            item->setText("Doing");
            item->setBackground(QColor(255, 255, 0));   // 黄
            QFont f = item->font(); f.setBold(true); item->setFont(f);
        } else {
            item->setText("Wait");
            item->setBackground(Qt::white);
            QFont f = item->font(); f.setBold(false); item->setFont(f);
        }
    }
}

// ─────────────────────────────────────────────────────────────
// HSV 阈值同步
// ─────────────────────────────────────────────────────────────
void MainWindow::updateHSVThreshold()
{
    if (!capturer) return;

    int hMin = ui->hMinSpin->value(), hMax = ui->hMaxSpin->value();
    int sMin = ui->sMinSpin->value(), sMax = ui->sMaxSpin->value();
    int vMin = ui->vMinSpin->value(), vMax = ui->vMaxSpin->value();

    if (hMin > hMax) hMax = hMin;
    if (sMin > sMax) sMax = sMin;
    if (vMin > vMax) vMax = vMin;

    emit setHSV(hMin, hMax, sMin, sMax, vMin, vMax);
}

void MainWindow::on_colorComboBox_currentIndexChanged(int index)
{
    if (!capturer) return;

    int type = index + 1;
    capturer->setColorType(type);
    HSVThreshold th = capturer->getThreshold(type);

    // 同步更新所有控件（阻断信号防止循环触发）
    for (QWidget *w : {(QWidget*)ui->hMinSpin, (QWidget*)ui->hMaxSpin,
                       (QWidget*)ui->sMinSpin, (QWidget*)ui->sMaxSpin,
                       (QWidget*)ui->vMinSpin, (QWidget*)ui->vMaxSpin,
                       (QWidget*)ui->hMinSlider, (QWidget*)ui->hMaxSlider,
                       (QWidget*)ui->sMinSlider, (QWidget*)ui->sMaxSlider,
                       (QWidget*)ui->vMinSlider, (QWidget*)ui->vMaxSlider})
        w->blockSignals(true);

    ui->hMinSpin->setValue(th.h_min);  ui->hMinSlider->setValue(th.h_min);
    ui->hMaxSpin->setValue(th.h_max);  ui->hMaxSlider->setValue(th.h_max);
    ui->sMinSpin->setValue(th.s_min);  ui->sMinSlider->setValue(th.s_min);
    ui->sMaxSpin->setValue(th.s_max);  ui->sMaxSlider->setValue(th.s_max);
    ui->vMinSpin->setValue(th.v_min);  ui->vMinSlider->setValue(th.v_min);
    ui->vMaxSpin->setValue(th.v_max);  ui->vMaxSlider->setValue(th.v_max);

    for (QWidget *w : {(QWidget*)ui->hMinSpin, (QWidget*)ui->hMaxSpin,
                       (QWidget*)ui->sMinSpin, (QWidget*)ui->sMaxSpin,
                       (QWidget*)ui->vMinSpin, (QWidget*)ui->vMaxSpin,
                       (QWidget*)ui->hMinSlider, (QWidget*)ui->hMaxSlider,
                       (QWidget*)ui->sMinSlider, (QWidget*)ui->sMaxSlider,
                       (QWidget*)ui->vMinSlider, (QWidget*)ui->vMaxSlider})
        w->blockSignals(false);
}

// Slider → SpinBox → 更新
void MainWindow::on_hMaxSlider_valueChanged(int v) { ui->hMaxSpin->setValue(v); updateHSVThreshold(); }
void MainWindow::on_hMinSlider_valueChanged(int v) { ui->hMinSpin->setValue(v); updateHSVThreshold(); }
void MainWindow::on_sMaxSlider_valueChanged(int v) { ui->sMaxSpin->setValue(v); updateHSVThreshold(); }
void MainWindow::on_sMinSlider_valueChanged(int v) { ui->sMinSpin->setValue(v); updateHSVThreshold(); }
void MainWindow::on_vMaxSlider_valueChanged(int v) { ui->vMaxSpin->setValue(v); updateHSVThreshold(); }
void MainWindow::on_vMinSlider_valueChanged(int v) { ui->vMinSpin->setValue(v); updateHSVThreshold(); }

// SpinBox → Slider → 更新
void MainWindow::on_hMaxSpin_valueChanged(int v) { ui->hMaxSlider->setValue(v); updateHSVThreshold(); }
void MainWindow::on_hMinSpin_valueChanged(int v) { ui->hMinSlider->setValue(v); updateHSVThreshold(); }
void MainWindow::on_sMaxSpin_valueChanged(int v) { ui->sMaxSlider->setValue(v); updateHSVThreshold(); }
void MainWindow::on_sMinSpin_valueChanged(int v) { ui->sMinSlider->setValue(v); updateHSVThreshold(); }
void MainWindow::on_vMaxSpin_valueChanged(int v) { ui->vMaxSlider->setValue(v); updateHSVThreshold(); }
void MainWindow::on_vMinSpin_valueChanged(int v) { ui->vMinSlider->setValue(v); updateHSVThreshold(); }

// ─────────────────────────────────────────────────────────────
// QR 解析工具（供外部调用）
// ─────────────────────────────────────────────────────────────
QList<int> MainWindow::parseQRCode(const QString &text, int side)
{
    QList<int> result;
    QStringList parts = text.split('+');
    if (parts.size() < 2) return result;

    const QString &target = (side == 0) ? parts[0] : parts[1];
    for (const QChar &c : target) {
        int n = c.digitValue();
        if (n >= 1 && n <= 3) result.append(n);
    }
    return result;
}
