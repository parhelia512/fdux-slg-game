/****************************************************************************
** Meta object code from reading C++ file 'CameraControl.h'
**
** Created: Sun Aug 21 13:36:13 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../scr/CameraControl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CameraControl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CameraControl[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,
      34,   14,   14,   14, 0x0a,
      56,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CameraControl[] = {
    "CameraControl\0\0PM_POINTS_action()\0"
    "PM_WIREFRAME_action()\0PM_SOLID_action()\0"
};

const QMetaObject CameraControl::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CameraControl,
      qt_meta_data_CameraControl, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CameraControl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CameraControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CameraControl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CameraControl))
        return static_cast<void*>(const_cast< CameraControl*>(this));
    return QObject::qt_metacast(_clname);
}

int CameraControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: PM_POINTS_action(); break;
        case 1: PM_WIREFRAME_action(); break;
        case 2: PM_SOLID_action(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE