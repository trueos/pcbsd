/****************************************************************************
** Meta object code from reading C++ file 'installer.h'
**
** Created: Tue Apr 2 17:57:40 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "installer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'installer.h' doesn't include <QObject>."
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
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      23,   10,   10,   10, 0x08,
      34,   10,   10,   10, 0x08,
      45,   10,   10,   10, 0x08,
      56,   10,   10,   10, 0x08,
      71,   10,   10,   10, 0x08,
      92,   10,   10,   10, 0x08,
     112,   10,   10,   10, 0x08,
     142,  139,   10,   10, 0x08,
     208,  192,   10,   10, 0x08,
     258,   10,   10,   10, 0x08,
     284,   10,   10,   10, 0x08,
     307,   10,   10,   10, 0x08,
     327,   10,   10,   10, 0x08,
     352,   10,   10,   10, 0x08,
     378,   10,   10,   10, 0x08,
     408,   10,   10,   10, 0x08,
     451,  441,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Installer[] = {
    "Installer\0\0slotAbort()\0slotBack()\0"
    "slotHelp()\0slotNext()\0slotFinished()\0"
    "slotChangeLanguage()\0slotCheckHardware()\0"
    "slotDiskCustomizeClicked()\0,,\0"
    "slotSaveDiskChanges(QList<QStringList>,bool,bool)\0"
    "exitCode,status\0"
    "slotInstallProcFinished(int,QProcess::ExitStatus)\0"
    "slotReadInstallerOutput()\0"
    "slotPushVirtKeyboard()\0slotPushKeyLayout()\0"
    "slotDesktopLeftClicked()\0"
    "slotDesktopRightClicked()\0"
    "slotDesktopCustomizeClicked()\0"
    "slotSaveMetaChanges(QStringList)\0"
    ",,,,,,,,,\0"
    "slotSaveFBSDSettings(QString,QString,QString,QString,QString,QString,b"
    "ool,bool,bool,QStringList)\0"
};

void Installer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Installer *_t = static_cast<Installer *>(_o);
        switch (_id) {
        case 0: _t->slotAbort(); break;
        case 1: _t->slotBack(); break;
        case 2: _t->slotHelp(); break;
        case 3: _t->slotNext(); break;
        case 4: _t->slotFinished(); break;
        case 5: _t->slotChangeLanguage(); break;
        case 6: _t->slotCheckHardware(); break;
        case 7: _t->slotDiskCustomizeClicked(); break;
        case 8: _t->slotSaveDiskChanges((*reinterpret_cast< QList<QStringList>(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 9: _t->slotInstallProcFinished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 10: _t->slotReadInstallerOutput(); break;
        case 11: _t->slotPushVirtKeyboard(); break;
        case 12: _t->slotPushKeyLayout(); break;
        case 13: _t->slotDesktopLeftClicked(); break;
        case 14: _t->slotDesktopRightClicked(); break;
        case 15: _t->slotDesktopCustomizeClicked(); break;
        case 16: _t->slotSaveMetaChanges((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 17: _t->slotSaveFBSDSettings((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< bool(*)>(_a[7])),(*reinterpret_cast< bool(*)>(_a[8])),(*reinterpret_cast< bool(*)>(_a[9])),(*reinterpret_cast< QStringList(*)>(_a[10]))); break;
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
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
