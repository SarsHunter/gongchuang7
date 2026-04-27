/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.15)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.15. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[42];
    char stringdata0[715];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 6), // "setHSV"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 4), // "hMin"
QT_MOC_LITERAL(4, 24, 4), // "hMax"
QT_MOC_LITERAL(5, 29, 4), // "sMin"
QT_MOC_LITERAL(6, 34, 4), // "sMax"
QT_MOC_LITERAL(7, 39, 4), // "vMin"
QT_MOC_LITERAL(8, 44, 4), // "vMax"
QT_MOC_LITERAL(9, 49, 10), // "openCamera"
QT_MOC_LITERAL(10, 60, 11), // "updateFrame"
QT_MOC_LITERAL(11, 72, 5), // "frame"
QT_MOC_LITERAL(12, 78, 13), // "updatePreview"
QT_MOC_LITERAL(13, 92, 4), // "mask"
QT_MOC_LITERAL(14, 97, 20), // "on_detectBtn_clicked"
QT_MOC_LITERAL(15, 118, 19), // "on_frontBtn_clicked"
QT_MOC_LITERAL(16, 138, 18), // "on_backBtn_clicked"
QT_MOC_LITERAL(17, 157, 18), // "on_leftBtn_clicked"
QT_MOC_LITERAL(18, 176, 19), // "on_rightBtn_clicked"
QT_MOC_LITERAL(19, 196, 19), // "on_trackBtn_clicked"
QT_MOC_LITERAL(20, 216, 26), // "on_hMaxSlider_valueChanged"
QT_MOC_LITERAL(21, 243, 5), // "value"
QT_MOC_LITERAL(22, 249, 26), // "on_hMinSlider_valueChanged"
QT_MOC_LITERAL(23, 276, 26), // "on_sMaxSlider_valueChanged"
QT_MOC_LITERAL(24, 303, 26), // "on_sMinSlider_valueChanged"
QT_MOC_LITERAL(25, 330, 26), // "on_vMaxSlider_valueChanged"
QT_MOC_LITERAL(26, 357, 26), // "on_vMinSlider_valueChanged"
QT_MOC_LITERAL(27, 384, 24), // "on_hMaxSpin_valueChanged"
QT_MOC_LITERAL(28, 409, 24), // "on_hMinSpin_valueChanged"
QT_MOC_LITERAL(29, 434, 24), // "on_sMaxSpin_valueChanged"
QT_MOC_LITERAL(30, 459, 24), // "on_sMinSpin_valueChanged"
QT_MOC_LITERAL(31, 484, 24), // "on_vMaxSpin_valueChanged"
QT_MOC_LITERAL(32, 509, 24), // "on_vMinSpin_valueChanged"
QT_MOC_LITERAL(33, 534, 36), // "on_colorComboBox_currentIndex..."
QT_MOC_LITERAL(34, 571, 5), // "index"
QT_MOC_LITERAL(35, 577, 18), // "updateHSVThreshold"
QT_MOC_LITERAL(36, 596, 17), // "on_RunBtn_clicked"
QT_MOC_LITERAL(37, 614, 22), // "on_loadJsonBtn_clicked"
QT_MOC_LITERAL(38, 637, 16), // "on_doBtn_clicked"
QT_MOC_LITERAL(39, 654, 21), // "on_addTaskBtn_clicked"
QT_MOC_LITERAL(40, 676, 18), // "on_testBtn_clicked"
QT_MOC_LITERAL(41, 695, 19) // "on_testBtn2_clicked"

    },
    "MainWindow\0setHSV\0\0hMin\0hMax\0sMin\0"
    "sMax\0vMin\0vMax\0openCamera\0updateFrame\0"
    "frame\0updatePreview\0mask\0on_detectBtn_clicked\0"
    "on_frontBtn_clicked\0on_backBtn_clicked\0"
    "on_leftBtn_clicked\0on_rightBtn_clicked\0"
    "on_trackBtn_clicked\0on_hMaxSlider_valueChanged\0"
    "value\0on_hMinSlider_valueChanged\0"
    "on_sMaxSlider_valueChanged\0"
    "on_sMinSlider_valueChanged\0"
    "on_vMaxSlider_valueChanged\0"
    "on_vMinSlider_valueChanged\0"
    "on_hMaxSpin_valueChanged\0"
    "on_hMinSpin_valueChanged\0"
    "on_sMaxSpin_valueChanged\0"
    "on_sMinSpin_valueChanged\0"
    "on_vMaxSpin_valueChanged\0"
    "on_vMinSpin_valueChanged\0"
    "on_colorComboBox_currentIndexChanged\0"
    "index\0updateHSVThreshold\0on_RunBtn_clicked\0"
    "on_loadJsonBtn_clicked\0on_doBtn_clicked\0"
    "on_addTaskBtn_clicked\0on_testBtn_clicked\0"
    "on_testBtn2_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      30,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    6,  164,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,  177,    2, 0x08 /* Private */,
      10,    1,  178,    2, 0x08 /* Private */,
      12,    1,  181,    2, 0x08 /* Private */,
      14,    0,  184,    2, 0x08 /* Private */,
      15,    0,  185,    2, 0x08 /* Private */,
      16,    0,  186,    2, 0x08 /* Private */,
      17,    0,  187,    2, 0x08 /* Private */,
      18,    0,  188,    2, 0x08 /* Private */,
      19,    0,  189,    2, 0x08 /* Private */,
      20,    1,  190,    2, 0x08 /* Private */,
      22,    1,  193,    2, 0x08 /* Private */,
      23,    1,  196,    2, 0x08 /* Private */,
      24,    1,  199,    2, 0x08 /* Private */,
      25,    1,  202,    2, 0x08 /* Private */,
      26,    1,  205,    2, 0x08 /* Private */,
      27,    1,  208,    2, 0x08 /* Private */,
      28,    1,  211,    2, 0x08 /* Private */,
      29,    1,  214,    2, 0x08 /* Private */,
      30,    1,  217,    2, 0x08 /* Private */,
      31,    1,  220,    2, 0x08 /* Private */,
      32,    1,  223,    2, 0x08 /* Private */,
      33,    1,  226,    2, 0x08 /* Private */,
      35,    0,  229,    2, 0x08 /* Private */,
      36,    0,  230,    2, 0x08 /* Private */,
      37,    0,  231,    2, 0x08 /* Private */,
      38,    0,  232,    2, 0x08 /* Private */,
      39,    0,  233,    2, 0x08 /* Private */,
      40,    0,  234,    2, 0x08 /* Private */,
      41,    0,  235,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,    6,    7,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QImage,   11,
    QMetaType::Void, QMetaType::QImage,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Int,   34,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->setHSV((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 1: _t->openCamera(); break;
        case 2: _t->updateFrame((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 3: _t->updatePreview((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 4: _t->on_detectBtn_clicked(); break;
        case 5: _t->on_frontBtn_clicked(); break;
        case 6: _t->on_backBtn_clicked(); break;
        case 7: _t->on_leftBtn_clicked(); break;
        case 8: _t->on_rightBtn_clicked(); break;
        case 9: _t->on_trackBtn_clicked(); break;
        case 10: _t->on_hMaxSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->on_hMinSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->on_sMaxSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->on_sMinSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->on_vMaxSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->on_vMinSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->on_hMaxSpin_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->on_hMinSpin_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->on_sMaxSpin_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->on_sMinSpin_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->on_vMaxSpin_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->on_vMinSpin_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->on_colorComboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->updateHSVThreshold(); break;
        case 24: _t->on_RunBtn_clicked(); break;
        case 25: _t->on_loadJsonBtn_clicked(); break;
        case 26: _t->on_doBtn_clicked(); break;
        case 27: _t->on_addTaskBtn_clicked(); break;
        case 28: _t->on_testBtn_clicked(); break;
        case 29: _t->on_testBtn2_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)(int , int , int , int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::setHSV)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 30)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 30;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 30)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 30;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::setHSV(int _t1, int _t2, int _t3, int _t4, int _t5, int _t6)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t6))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
