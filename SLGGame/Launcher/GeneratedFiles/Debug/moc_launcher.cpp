/****************************************************************************
** Meta object code from reading C++ file 'launcher.h'
**
** Created: Wed Aug 3 22:17:57 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../launcher.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'launcher.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Launcher[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x0a,
      26,    9,    9,    9, 0x0a,
      45,    9,    9,    9, 0x0a,
      58,    9,    9,    9, 0x0a,
      67,    9,    9,    9, 0x0a,
      82,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Launcher[] = {
    "Launcher\0\0windowRunGame()\0fullSceneRunGame()\0"
    "gameManual()\0readMe()\0setupDirectx()\0"
    "visitWebSite()\0"
};

const QMetaObject Launcher::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Launcher,
      qt_meta_data_Launcher, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Launcher::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Launcher::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Launcher::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Launcher))
        return static_cast<void*>(const_cast< Launcher*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Launcher::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: windowRunGame(); break;
        case 1: fullSceneRunGame(); break;
        case 2: gameManual(); break;
        case 3: readMe(); break;
        case 4: setupDirectx(); break;
        case 5: visitWebSite(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
