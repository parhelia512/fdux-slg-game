/****************************************************************************
** Meta object code from reading C++ file 'terraincontrol.h'
**
** Created: Thu Aug 25 18:22:59 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../scr/terraincontrol.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'terraincontrol.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TerrainControl[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      31,   15,   15,   15, 0x0a,
      51,   45,   15,   15, 0x0a,
      86,   69,   15,   15, 0x0a,
     146,  140,   15,   15, 0x0a,
     170,  140,   15,   15, 0x0a,
     195,  140,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TerrainControl[] = {
    "TerrainControl\0\0Load_Terrain()\0"
    "New_Terrain()\0state\0stateChanged(int)\0"
    "current,previous\0"
    "currentItemChanged(QListWidgetItem*,QListWidgetItem*)\0"
    "event\0mouseMove(QMouseEvent*)\0"
    "mousePress(QMouseEvent*)\0"
    "mouseRelease(QMouseEvent*)\0"
};

const QMetaObject TerrainControl::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TerrainControl,
      qt_meta_data_TerrainControl, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TerrainControl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TerrainControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TerrainControl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TerrainControl))
        return static_cast<void*>(const_cast< TerrainControl*>(this));
    return QObject::qt_metacast(_clname);
}

int TerrainControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: Load_Terrain(); break;
        case 1: New_Terrain(); break;
        case 2: stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: currentItemChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QListWidgetItem*(*)>(_a[2]))); break;
        case 4: mouseMove((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 5: mousePress((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 6: mouseRelease((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE