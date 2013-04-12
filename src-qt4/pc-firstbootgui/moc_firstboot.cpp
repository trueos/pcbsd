/****************************************************************************
** Meta object code from reading C++ file 'firstboot.h'
**
** Created: Fri Apr 5 17:27:22 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "firstboot.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'firstboot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Installer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      22,   10,   10,   10, 0x08,
      33,   10,   10,   10, 0x08,
      44,   10,   10,   10, 0x08,
      59,   10,   10,   10, 0x08,
      80,   10,   10,   10, 0x08,
     103,   10,   10,   10, 0x08,
     135,  123,   10,   10, 0x08,
     181,   10,   10,   10, 0x08,
     199,   10,   10,   10, 0x08,
     215,   10,   10,   10, 0x08,
     237,   10,   10,   10, 0x08,
     255,   10,   10,   10, 0x08,
     281,  272,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Installer[] = {
    "Installer\0\0slotBack()\0slotHelp()\0"
    "slotNext()\0slotFinished()\0"
    "slotChangeLanguage()\0slotPushVirtKeyboard()\0"
    "slotPushKeyLayout()\0mod,lay,var\0"
    "slotKeyLayoutUpdated(QString,QString,QString)\0"
    "slotCheckRootPW()\0slotCheckUser()\0"
    "slotSuggestUsername()\0slotScanNetwork()\0"
    "slotAddNewWifi()\0key,SSID\0"
    "slotQuickConnect(QString,QString)\0"
};

void Installer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Installer *_t = static_cast<Installer *>(_o);
        switch (_id) {
        case 0: _t->slotBack(); break;
        case 1: _t->slotHelp(); break;
        case 2: _t->slotNext(); break;
        case 3: _t->slotFinished(); break;
        case 4: _t->slotChangeLanguage(); break;
        case 5: _t->slotPushVirtKeyboard(); break;
        case 6: _t->slotPushKeyLayout(); break;
        case 7: _t->slotKeyLayoutUpdated((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 8: _t->slotCheckRootPW(); break;
        case 9: _t->slotCheckUser(); break;
        case 10: _t->slotSuggestUsername(); break;
        case 11: _t->slotScanNetwork(); break;
        case 12: _t->slotAddNewWifi(); break;
        case 13: _t->slotQuickConnect((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Installer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Installer::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Installer,
      qt_meta_data_Installer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Installer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Installer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Installer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Installer))
        return static_cast<void*>(const_cast< Installer*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Installer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
