/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Sat Nov 3 17:04:36 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      35,   11,   11,   11, 0x08,
      69,   60,   11,   11, 0x08,
     103,   98,   11,   11, 0x08,
     133,   11,   11,   11, 0x08,
     159,   98,   11,   11, 0x08,
     199,   11,   11,   11, 0x08,
     226,   98,   11,   11, 0x08,
     265,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0on_webButton_clicked()\0"
    "on_webView_loadStarted()\0progress\0"
    "on_webView_loadProgress(int)\0arg1\0"
    "on_webView_loadFinished(bool)\0"
    "on_actionExit_triggered()\0"
    "on_actionShow_Hide_broser_toggled(bool)\0"
    "on_actionSobre_triggered()\0"
    "on_actionActionFavoritos_toggled(bool)\0"
    "showTime()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->on_webButton_clicked(); break;
        case 1: _t->on_webView_loadStarted(); break;
        case 2: _t->on_webView_loadProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_webView_loadFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->on_actionExit_triggered(); break;
        case 5: _t->on_actionShow_Hide_broser_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->on_actionSobre_triggered(); break;
        case 7: _t->on_actionActionFavoritos_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->showTime(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
