/****************************************************************************
** Meta object code from reading C++ file 'taskmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.15)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../taskmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'taskmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.15. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TaskManager_t {
    QByteArrayData data[21];
    char stringdata0[277];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TaskManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TaskManager_t qt_meta_stringdata_TaskManager = {
    {
QT_MOC_LITERAL(0, 0, 11), // "TaskManager"
QT_MOC_LITERAL(1, 12, 12), // "queueUpdated"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 11), // "QList<Task>"
QT_MOC_LITERAL(4, 38, 5), // "queue"
QT_MOC_LITERAL(5, 44, 18), // "currentTaskChanged"
QT_MOC_LITERAL(6, 63, 5), // "index"
QT_MOC_LITERAL(7, 69, 11), // "taskStarted"
QT_MOC_LITERAL(8, 81, 12), // "taskFinished"
QT_MOC_LITERAL(9, 94, 16), // "allTasksFinished"
QT_MOC_LITERAL(10, 111, 11), // "taskStopped"
QT_MOC_LITERAL(11, 123, 12), // "waitingForQR"
QT_MOC_LITERAL(12, 136, 15), // "qrScanCompleted"
QT_MOC_LITERAL(13, 152, 18), // "armTrackingStarted"
QT_MOC_LITERAL(14, 171, 15), // "armTrackingDone"
QT_MOC_LITERAL(15, 187, 20), // "armTrackColorChanged"
QT_MOC_LITERAL(16, 208, 5), // "color"
QT_MOC_LITERAL(17, 214, 20), // "onDeviceTaskFinished"
QT_MOC_LITERAL(18, 235, 20), // "onArmSubStepFinished"
QT_MOC_LITERAL(19, 256, 15), // "onQRCodeScanned"
QT_MOC_LITERAL(20, 272, 4) // "text"

    },
    "TaskManager\0queueUpdated\0\0QList<Task>\0"
    "queue\0currentTaskChanged\0index\0"
    "taskStarted\0taskFinished\0allTasksFinished\0"
    "taskStopped\0waitingForQR\0qrScanCompleted\0"
    "armTrackingStarted\0armTrackingDone\0"
    "armTrackColorChanged\0color\0"
    "onDeviceTaskFinished\0onArmSubStepFinished\0"
    "onQRCodeScanned\0text"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TaskManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,
       5,    1,   87,    2, 0x06 /* Public */,
       7,    0,   90,    2, 0x06 /* Public */,
       8,    0,   91,    2, 0x06 /* Public */,
       9,    0,   92,    2, 0x06 /* Public */,
      10,    0,   93,    2, 0x06 /* Public */,
      11,    0,   94,    2, 0x06 /* Public */,
      12,    0,   95,    2, 0x06 /* Public */,
      13,    0,   96,    2, 0x06 /* Public */,
      14,    0,   97,    2, 0x06 /* Public */,
      15,    1,   98,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      17,    0,  101,    2, 0x0a /* Public */,
      18,    0,  102,    2, 0x0a /* Public */,
      19,    1,  103,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   16,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   20,

       0        // eod
};

void TaskManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TaskManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->queueUpdated((*reinterpret_cast< const QList<Task>(*)>(_a[1]))); break;
        case 1: _t->currentTaskChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->taskStarted(); break;
        case 3: _t->taskFinished(); break;
        case 4: _t->allTasksFinished(); break;
        case 5: _t->taskStopped(); break;
        case 6: _t->waitingForQR(); break;
        case 7: _t->qrScanCompleted(); break;
        case 8: _t->armTrackingStarted(); break;
        case 9: _t->armTrackingDone(); break;
        case 10: _t->armTrackColorChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->onDeviceTaskFinished(); break;
        case 12: _t->onArmSubStepFinished(); break;
        case 13: _t->onQRCodeScanned((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TaskManager::*)(const QList<Task> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::queueUpdated)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TaskManager::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::currentTaskChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TaskManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::taskStarted)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TaskManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::taskFinished)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (TaskManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::allTasksFinished)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (TaskManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::taskStopped)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (TaskManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::waitingForQR)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (TaskManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::qrScanCompleted)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (TaskManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::armTrackingStarted)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (TaskManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::armTrackingDone)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (TaskManager::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::armTrackColorChanged)) {
                *result = 10;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TaskManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_TaskManager.data,
    qt_meta_data_TaskManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TaskManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TaskManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TaskManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TaskManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void TaskManager::queueUpdated(const QList<Task> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TaskManager::currentTaskChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TaskManager::taskStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TaskManager::taskFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void TaskManager::allTasksFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void TaskManager::taskStopped()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void TaskManager::waitingForQR()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void TaskManager::qrScanCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void TaskManager::armTrackingStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void TaskManager::armTrackingDone()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void TaskManager::armTrackColorChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
