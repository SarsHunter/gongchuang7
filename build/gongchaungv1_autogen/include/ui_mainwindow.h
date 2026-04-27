/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.15
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *VideoTab;
    QGraphicsView *graphicsView;
    QPushButton *opencamBtn;
    QPushButton *detectBtn;
    QSlider *sMaxSlider;
    QSpinBox *sMaxSpin;
    QSpinBox *sMinSpin;
    QSpinBox *vMaxSpin;
    QSpinBox *vMinSpin;
    QSlider *sMinSlider;
    QSlider *vMaxSlider;
    QSlider *vMinSlider;
    QSlider *hMaxSlider;
    QSlider *hMinSlider;
    QSpinBox *hMaxSpin;
    QSpinBox *hMinSpin;
    QGraphicsView *preView;
    QPushButton *trackBtn;
    QComboBox *colorComboBox;
    QComboBox *comboBox;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QWidget *CarTab;
    QPushButton *frontBtn;
    QPushButton *leftBtn;
    QPushButton *rightBtn;
    QPushButton *backBtn;
    QPushButton *testBtn;
    QPushButton *testBtn2;
    QPushButton *turnleftBtn;
    QPushButton *turnrightBtn;
    QWidget *TaskTab;
    QTableWidget *taskTable;
    QPushButton *addTaskBtn;
    QPushButton *doBtn;
    QPushButton *loadJsonBtn;
    QWidget *RunTab;
    QPushButton *RunBtn;
    QLabel *qrLabel;
    QPushButton *fullBtn;
    QPushButton *norBtn;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 480);
        QIcon icon;
        icon.addFile(QString::fromUtf8("boom.webp"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 800, 458));
        VideoTab = new QWidget();
        VideoTab->setObjectName(QString::fromUtf8("VideoTab"));
        graphicsView = new QGraphicsView(VideoTab);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(0, 0, 320, 240));
        opencamBtn = new QPushButton(VideoTab);
        opencamBtn->setObjectName(QString::fromUtf8("opencamBtn"));
        opencamBtn->setGeometry(QRect(350, 10, 80, 26));
        detectBtn = new QPushButton(VideoTab);
        detectBtn->setObjectName(QString::fromUtf8("detectBtn"));
        detectBtn->setGeometry(QRect(350, 60, 80, 26));
        sMaxSlider = new QSlider(VideoTab);
        sMaxSlider->setObjectName(QString::fromUtf8("sMaxSlider"));
        sMaxSlider->setGeometry(QRect(90, 260, 160, 16));
        sMaxSlider->setMaximum(255);
        sMaxSlider->setOrientation(Qt::Horizontal);
        sMaxSlider->setTickPosition(QSlider::NoTicks);
        sMaxSpin = new QSpinBox(VideoTab);
        sMaxSpin->setObjectName(QString::fromUtf8("sMaxSpin"));
        sMaxSpin->setGeometry(QRect(30, 260, 42, 26));
        sMaxSpin->setMaximum(255);
        sMinSpin = new QSpinBox(VideoTab);
        sMinSpin->setObjectName(QString::fromUtf8("sMinSpin"));
        sMinSpin->setGeometry(QRect(30, 300, 42, 26));
        sMinSpin->setMaximum(255);
        vMaxSpin = new QSpinBox(VideoTab);
        vMaxSpin->setObjectName(QString::fromUtf8("vMaxSpin"));
        vMaxSpin->setGeometry(QRect(30, 340, 42, 26));
        vMaxSpin->setMaximum(255);
        vMinSpin = new QSpinBox(VideoTab);
        vMinSpin->setObjectName(QString::fromUtf8("vMinSpin"));
        vMinSpin->setGeometry(QRect(30, 380, 42, 26));
        vMinSpin->setMaximum(255);
        sMinSlider = new QSlider(VideoTab);
        sMinSlider->setObjectName(QString::fromUtf8("sMinSlider"));
        sMinSlider->setGeometry(QRect(90, 300, 160, 16));
        sMinSlider->setMaximum(255);
        sMinSlider->setOrientation(Qt::Horizontal);
        vMaxSlider = new QSlider(VideoTab);
        vMaxSlider->setObjectName(QString::fromUtf8("vMaxSlider"));
        vMaxSlider->setGeometry(QRect(90, 350, 160, 16));
        vMaxSlider->setMaximum(255);
        vMaxSlider->setOrientation(Qt::Horizontal);
        vMinSlider = new QSlider(VideoTab);
        vMinSlider->setObjectName(QString::fromUtf8("vMinSlider"));
        vMinSlider->setGeometry(QRect(90, 380, 160, 16));
        vMinSlider->setMaximum(255);
        vMinSlider->setOrientation(Qt::Horizontal);
        hMaxSlider = new QSlider(VideoTab);
        hMaxSlider->setObjectName(QString::fromUtf8("hMaxSlider"));
        hMaxSlider->setGeometry(QRect(280, 260, 20, 131));
        hMaxSlider->setMaximum(179);
        hMaxSlider->setOrientation(Qt::Vertical);
        hMinSlider = new QSlider(VideoTab);
        hMinSlider->setObjectName(QString::fromUtf8("hMinSlider"));
        hMinSlider->setGeometry(QRect(330, 260, 16, 131));
        hMinSlider->setMaximum(179);
        hMinSlider->setOrientation(Qt::Vertical);
        hMaxSpin = new QSpinBox(VideoTab);
        hMaxSpin->setObjectName(QString::fromUtf8("hMaxSpin"));
        hMaxSpin->setGeometry(QRect(270, 400, 42, 26));
        hMaxSpin->setMaximum(180);
        hMinSpin = new QSpinBox(VideoTab);
        hMinSpin->setObjectName(QString::fromUtf8("hMinSpin"));
        hMinSpin->setGeometry(QRect(320, 400, 42, 26));
        hMinSpin->setMaximum(179);
        preView = new QGraphicsView(VideoTab);
        preView->setObjectName(QString::fromUtf8("preView"));
        preView->setGeometry(QRect(480, 0, 320, 240));
        trackBtn = new QPushButton(VideoTab);
        trackBtn->setObjectName(QString::fromUtf8("trackBtn"));
        trackBtn->setGeometry(QRect(350, 160, 80, 26));
        colorComboBox = new QComboBox(VideoTab);
        colorComboBox->addItem(QString());
        colorComboBox->addItem(QString());
        colorComboBox->addItem(QString());
        colorComboBox->addItem(QString());
        colorComboBox->setObjectName(QString::fromUtf8("colorComboBox"));
        colorComboBox->setGeometry(QRect(640, 270, 72, 26));
        comboBox = new QComboBox(VideoTab);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(540, 270, 72, 26));
        label = new QLabel(VideoTab);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(110, 270, 61, 21));
        QFont font;
        font.setPointSize(11);
        label->setFont(font);
        label_2 = new QLabel(VideoTab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(110, 320, 61, 21));
        label_2->setFont(font);
        label_3 = new QLabel(VideoTab);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(110, 360, 61, 21));
        label_3->setFont(font);
        label_4 = new QLabel(VideoTab);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(110, 390, 61, 21));
        label_4->setFont(font);
        label_5 = new QLabel(VideoTab);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(270, 240, 61, 21));
        label_5->setFont(font);
        label_6 = new QLabel(VideoTab);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(320, 240, 61, 21));
        label_6->setFont(font);
        tabWidget->addTab(VideoTab, QString());
        CarTab = new QWidget();
        CarTab->setObjectName(QString::fromUtf8("CarTab"));
        frontBtn = new QPushButton(CarTab);
        frontBtn->setObjectName(QString::fromUtf8("frontBtn"));
        frontBtn->setGeometry(QRect(100, 150, 60, 60));
        leftBtn = new QPushButton(CarTab);
        leftBtn->setObjectName(QString::fromUtf8("leftBtn"));
        leftBtn->setGeometry(QRect(20, 240, 60, 60));
        rightBtn = new QPushButton(CarTab);
        rightBtn->setObjectName(QString::fromUtf8("rightBtn"));
        rightBtn->setGeometry(QRect(180, 240, 60, 60));
        backBtn = new QPushButton(CarTab);
        backBtn->setObjectName(QString::fromUtf8("backBtn"));
        backBtn->setGeometry(QRect(100, 240, 60, 60));
        testBtn = new QPushButton(CarTab);
        testBtn->setObjectName(QString::fromUtf8("testBtn"));
        testBtn->setGeometry(QRect(410, 170, 80, 81));
        testBtn2 = new QPushButton(CarTab);
        testBtn2->setObjectName(QString::fromUtf8("testBtn2"));
        testBtn2->setGeometry(QRect(570, 175, 80, 71));
        turnleftBtn = new QPushButton(CarTab);
        turnleftBtn->setObjectName(QString::fromUtf8("turnleftBtn"));
        turnleftBtn->setGeometry(QRect(20, 150, 60, 60));
        turnrightBtn = new QPushButton(CarTab);
        turnrightBtn->setObjectName(QString::fromUtf8("turnrightBtn"));
        turnrightBtn->setGeometry(QRect(180, 150, 60, 60));
        tabWidget->addTab(CarTab, QString());
        TaskTab = new QWidget();
        TaskTab->setObjectName(QString::fromUtf8("TaskTab"));
        taskTable = new QTableWidget(TaskTab);
        taskTable->setObjectName(QString::fromUtf8("taskTable"));
        taskTable->setGeometry(QRect(0, 30, 491, 251));
        addTaskBtn = new QPushButton(TaskTab);
        addTaskBtn->setObjectName(QString::fromUtf8("addTaskBtn"));
        addTaskBtn->setGeometry(QRect(640, 120, 80, 41));
        doBtn = new QPushButton(TaskTab);
        doBtn->setObjectName(QString::fromUtf8("doBtn"));
        doBtn->setGeometry(QRect(630, 190, 80, 71));
        QFont font1;
        font1.setPointSize(20);
        doBtn->setFont(font1);
        loadJsonBtn = new QPushButton(TaskTab);
        loadJsonBtn->setObjectName(QString::fromUtf8("loadJsonBtn"));
        loadJsonBtn->setGeometry(QRect(600, 50, 80, 26));
        tabWidget->addTab(TaskTab, QString());
        RunTab = new QWidget();
        RunTab->setObjectName(QString::fromUtf8("RunTab"));
        RunBtn = new QPushButton(RunTab);
        RunBtn->setObjectName(QString::fromUtf8("RunBtn"));
        RunBtn->setGeometry(QRect(630, 310, 121, 71));
        QFont font2;
        font2.setPointSize(16);
        RunBtn->setFont(font2);
        qrLabel = new QLabel(RunTab);
        qrLabel->setObjectName(QString::fromUtf8("qrLabel"));
        qrLabel->setGeometry(QRect(330, 50, 181, 131));
        QFont font3;
        font3.setPointSize(32);
        qrLabel->setFont(font3);
        fullBtn = new QPushButton(RunTab);
        fullBtn->setObjectName(QString::fromUtf8("fullBtn"));
        fullBtn->setGeometry(QRect(650, 50, 91, 31));
        norBtn = new QPushButton(RunTab);
        norBtn->setObjectName(QString::fromUtf8("norBtn"));
        norBtn->setGeometry(QRect(650, 110, 91, 31));
        tabWidget->addTab(RunTab, QString());
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(fullBtn, SIGNAL(clicked()), MainWindow, SLOT(showFullScreen()));
        QObject::connect(norBtn, SIGNAL(clicked()), MainWindow, SLOT(showNormal()));

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        opencamBtn->setText(QCoreApplication::translate("MainWindow", "OpenCam", nullptr));
        detectBtn->setText(QCoreApplication::translate("MainWindow", "detect", nullptr));
        trackBtn->setText(QCoreApplication::translate("MainWindow", "\350\247\206\350\247\211\350\277\275\350\270\252", nullptr));
        colorComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "red1", nullptr));
        colorComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "red2", nullptr));
        colorComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "green", nullptr));
        colorComboBox->setItemText(3, QCoreApplication::translate("MainWindow", "blue", nullptr));

        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "ring", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "block", nullptr));

        label->setText(QCoreApplication::translate("MainWindow", "S max", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "S min", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "V max", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "V max", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "H max", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "H min", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(VideoTab), QCoreApplication::translate("MainWindow", "\350\247\206\350\247\211\347\225\214\351\235\242", nullptr));
        frontBtn->setText(QCoreApplication::translate("MainWindow", "\345\211\215", nullptr));
        leftBtn->setText(QCoreApplication::translate("MainWindow", "\345\267\246", nullptr));
        rightBtn->setText(QCoreApplication::translate("MainWindow", "\345\217\263", nullptr));
        backBtn->setText(QCoreApplication::translate("MainWindow", "\345\220\216", nullptr));
        testBtn->setText(QCoreApplication::translate("MainWindow", "C1", nullptr));
        testBtn2->setText(QCoreApplication::translate("MainWindow", "c2", nullptr));
        turnleftBtn->setText(QCoreApplication::translate("MainWindow", "\351\200\206", nullptr));
        turnrightBtn->setText(QCoreApplication::translate("MainWindow", "\351\200\206", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(CarTab), QCoreApplication::translate("MainWindow", "\350\277\220\345\212\250\347\225\214\351\235\242", nullptr));
        addTaskBtn->setText(QCoreApplication::translate("MainWindow", "add", nullptr));
        doBtn->setText(QCoreApplication::translate("MainWindow", "Do it!", nullptr));
        loadJsonBtn->setText(QCoreApplication::translate("MainWindow", "load", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(TaskTab), QCoreApplication::translate("MainWindow", "\344\273\273\345\212\241\350\260\203\350\257\225\347\225\214\351\235\242", nullptr));
        RunBtn->setText(QCoreApplication::translate("MainWindow", "RUN!!!!!!", nullptr));
        qrLabel->setText(QCoreApplication::translate("MainWindow", "123+123", nullptr));
        fullBtn->setText(QCoreApplication::translate("MainWindow", "\345\205\250\345\261\217", nullptr));
        norBtn->setText(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272\345\205\250\345\261\217", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(RunTab), QCoreApplication::translate("MainWindow", "\345\220\257\345\212\250\347\225\214\351\235\242", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
