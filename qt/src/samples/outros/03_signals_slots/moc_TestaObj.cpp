/****************************************************************************
** Meta object code from reading C++ file 'TestaObj.h'
**
** Created: Sat Nov 3 17:05:33 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "TestaObj.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TestaObj.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TestaObj[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      24,   17,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TestaObj[] = {
    "TestaObj\0\0exit()\0sender\0onDestroyed(QObject*)\0"
};

void TestaObj::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TestaObj *_t = static_cast<TestaObj *>(_o);
        switch (_id) {
        case 0: _t->exit(); break;
        case 1: _t->onDestroyed((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TestaObj::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TestaObj::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TestaObj,
      qt_meta_data_TestaObj, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TestaObj::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TestaObj::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TestaObj::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TestaObj))
        return static_cast<void*>(const_cast< TestaObj*>(this));
    return QObject::qt_metacast(_clname);
}

int TestaObj::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void TestaObj::exit()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE