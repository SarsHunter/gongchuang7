/****************************************************************************
** Meta object code from reading C++ file 'carcontroller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.15)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../carcontroller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'carcontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.15. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CarController_t {
    QByteArrayData data[12];
    char stringdata0[133];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CarController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CarController_t qt_meta_stringdata_CarController = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CarController"
QT_MOC_LITERAL(1, 14, 13), // "statusChanged"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 6), // "status"
QT_MOC_LITERAL(4, 36, 13), // "reachedTarget"
QT_MOC_LITERAL(5, 50, 13), // "errorOccurred"
QT_MOC_LITERAL(6, 64, 3), // "err"
QT_MOC_LITERAL(7, 68, 12), // "taskFinished"
QT_MOC_LITERAL(8, 81, 13), // "alignFinished"
QT_MOC_LITERAL(9, 95, 16), // "handleSerialData"
QT_MOC_LITERAL(10, 112, 4), // "data"
QT_MOC_LITERAL(11, 117, 15) // "chassisTracking"

    },
    "CarController\0statusChanged\0\0status\0"
    "reachedTarget\0errorOccurred\0err\0"
    "taskFinished\0alignFinished\0handleSerialData\0"
    "data\0chassisTracking"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CarController[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       4,    0,   52,    2, 0x06 /* Public */,
       5,    1,   53,    2, 0x06 /* Public */,
       7,    0,   56,    2, 0x06 /* Public */,
       8,    0,   57,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    1,   58,    2, 0x08 /* Private */,
      11,    0,   61,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray,   10,
    QMetaType::Void,

       0        // eod
};

void CarController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CarController *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->statusChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->reachedTarget(); break;
        case 2: _t->errorOccurred((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->taskFinished(); break;
        case 4: _t->alignFinished(); break;
        case 5: _t->handleSerialData((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 6: _t->chassisTracking(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CarController::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CarController::statusChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CarController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CarController::reachedTarget)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CarController::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CarController::errorOccurred)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CarController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CarController::taskFinished)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (CarController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CarController::alignFinished)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CarController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CarController.data,
    qt_meta_data_CarController,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CarController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CarController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CarController.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CarController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void CarController::statusChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CarController::reachedTarget()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void CarController::errorOccurred(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CarController::taskFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void CarController::alignFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
