/****************************************************************************
** Meta object code from reading C++ file 'videothread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.15)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../videothread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'videothread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.15. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_videothread_t {
    QByteArrayData data[11];
    char stringdata0[91];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_videothread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_videothread_t qt_meta_stringdata_videothread = {
    {
QT_MOC_LITERAL(0, 0, 11), // "videothread"
QT_MOC_LITERAL(1, 12, 13), // "frameCaptured"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 12), // "maskCaptured"
QT_MOC_LITERAL(4, 40, 12), // "qrCodeResult"
QT_MOC_LITERAL(5, 53, 11), // "circleError"
QT_MOC_LITERAL(6, 65, 2), // "dx"
QT_MOC_LITERAL(7, 68, 2), // "dy"
QT_MOC_LITERAL(8, 71, 7), // "uint8_t"
QT_MOC_LITERAL(9, 79, 5), // "x_dir"
QT_MOC_LITERAL(10, 85, 5) // "y_dir"

    },
    "videothread\0frameCaptured\0\0maskCaptured\0"
    "qrCodeResult\0circleError\0dx\0dy\0uint8_t\0"
    "x_dir\0y_dir"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_videothread[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       3,    1,   37,    2, 0x06 /* Public */,
       4,    1,   40,    2, 0x06 /* Public */,
       5,    4,   43,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage,    2,
    QMetaType::Void, QMetaType::QImage,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, 0x80000000 | 8, 0x80000000 | 8,    6,    7,    9,   10,

       0        // eod
};

void videothread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<videothread *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->frameCaptured((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 1: _t->maskCaptured((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 2: _t->qrCodeResult((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->circleError((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< uint8_t(*)>(_a[3])),(*reinterpret_cast< uint8_t(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (videothread::*)(QImage );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&videothread::frameCaptured)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (videothread::*)(QImage );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&videothread::maskCaptured)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (videothread::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&videothread::qrCodeResult)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (videothread::*)(int , int , uint8_t , uint8_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&videothread::circleError)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject videothread::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_videothread.data,
    qt_meta_data_videothread,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *videothread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *videothread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_videothread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int videothread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void videothread::frameCaptured(QImage _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void videothread::maskCaptured(QImage _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void videothread::qrCodeResult(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void videothread::circleError(int _t1, int _t2, uint8_t _t3, uint8_t _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
