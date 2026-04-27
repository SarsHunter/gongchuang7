/****************************************************************************
** Meta object code from reading C++ file 'armcontroller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.15)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../armcontroller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'armcontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.15. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ArmController_t {
    QByteArrayData data[15];
    char stringdata0[134];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ArmController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ArmController_t qt_meta_stringdata_ArmController = {
    {
QT_MOC_LITERAL(0, 0, 13), // "ArmController"
QT_MOC_LITERAL(1, 14, 13), // "statusChanged"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 6), // "status"
QT_MOC_LITERAL(4, 36, 12), // "taskFinished"
QT_MOC_LITERAL(5, 49, 14), // "actionFinished"
QT_MOC_LITERAL(6, 64, 16), // "handleSerialData"
QT_MOC_LITERAL(7, 81, 4), // "data"
QT_MOC_LITERAL(8, 86, 11), // "armTracking"
QT_MOC_LITERAL(9, 98, 11), // "updateError"
QT_MOC_LITERAL(10, 110, 2), // "dx"
QT_MOC_LITERAL(11, 113, 2), // "dy"
QT_MOC_LITERAL(12, 116, 7), // "uint8_t"
QT_MOC_LITERAL(13, 124, 4), // "xdir"
QT_MOC_LITERAL(14, 129, 4) // "ydir"

    },
    "ArmController\0statusChanged\0\0status\0"
    "taskFinished\0actionFinished\0"
    "handleSerialData\0data\0armTracking\0"
    "updateError\0dx\0dy\0uint8_t\0xdir\0ydir"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ArmController[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    0,   47,    2, 0x06 /* Public */,
       5,    0,   48,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   49,    2, 0x08 /* Private */,
       8,    0,   52,    2, 0x0a /* Public */,
       9,    4,   53,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, 0x80000000 | 12, 0x80000000 | 12,   10,   11,   13,   14,

       0        // eod
};

void ArmController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ArmController *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->statusChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->taskFinished(); break;
        case 2: _t->actionFinished(); break;
        case 3: _t->handleSerialData((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 4: _t->armTracking(); break;
        case 5: _t->updateError((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< uint8_t(*)>(_a[3])),(*reinterpret_cast< uint8_t(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ArmController::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ArmController::statusChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ArmController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ArmController::taskFinished)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ArmController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ArmController::actionFinished)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ArmController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ArmController.data,
    qt_meta_data_ArmController,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ArmController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ArmController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ArmController.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ArmController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ArmController::statusChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ArmController::taskFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ArmController::actionFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
