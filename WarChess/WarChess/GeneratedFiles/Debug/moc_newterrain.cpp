/****************************************************************************
** Meta object code from reading C++ file 'newterrain.h'
**
** Created: Sun Aug 21 13:36:08 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../scr/newterrain.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'newterrain.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_NewTerrain[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      23,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_NewTerrain[] = {
    "NewTerrain\0\0okButton()\0cancelButton()\0"
};

const QMetaObject NewTerrain::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_NewTerrain,
      qt_meta_data_NewTerrain, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &NewTerrain::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *NewTerrain::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *NewTerrain::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NewTerrain))
        return static_cast<void*>(const_cast< NewTerrain*>(this));
    return QDialog::qt_metacast(_clname);
}

int NewTerrain::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: okButton(); break;
        case 1: cancelButton(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE