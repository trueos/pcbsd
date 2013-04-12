/****************************************************************************
** Meta object code from reading C++ file 'wizardDisk.h'
**
** Created: Tue Apr 2 17:57:41 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "wizardDisk.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wizardDisk.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_wizardDisk[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      30,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      51,   11,   11,   11, 0x08,
      60,   11,   11,   11, 0x08,
      72,   11,   11,   11, 0x08,
      92,   11,   11,   11, 0x08,
     110,   11,   11,   11, 0x08,
     132,   11,   11,   11, 0x08,
     147,   11,   11,   11, 0x08,
     162,   11,   11,   11, 0x08,
     174,   11,   11,   11, 0x08,
     196,   11,   11,   11, 0x08,
     223,   11,   11,   11, 0x08,
     238,   11,   11,   11, 0x08,
     250,   11,   11,   11, 0x08,
     263,   11,   11,   11, 0x08,
     279,   11,   11,   11, 0x08,
     292,   11,   11,   11, 0x08,
     304,   11,   11,   11, 0x08,
     320,   11,   11,   11, 0x08,
     334,   11,   11,   11, 0x08,
     349,   11,   11,   11, 0x08,
     364,   11,   11,   11, 0x08,
     378,   11,   11,   11, 0x08,
     391,   11,   11,   11, 0x08,
     405,   11,   11,   11, 0x08,
     417,   11,   11,   11, 0x08,
     430,   11,   11,   11, 0x08,
     442,   11,   11,   11, 0x08,
     455,   11,   11,   11, 0x08,
     466,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_wizardDisk[] = {
    "wizardDisk\0\0,,\0saved(QList<QStringList>,bool,bool)\0"
    "accept()\0slotClose()\0slotCheckComplete()\0"
    "slotChangedDisk()\0slotTreeDiskChanged()\0"
    "slotResizeFS()\0slotRemoveFS()\0slotAddFS()\0"
    "slotSaveFSResize(int)\0slotTreeMountsRightClick()\0"
    "slotTerminal()\0slotZCMON()\0slotZCMOFF()\0"
    "slotZCMNOAUTO()\0slotZDDOFF()\0slotZDDON()\0"
    "slotZDDVERIFY()\0slotZCmpOFF()\0"
    "slotZCmpLZJB()\0slotZCmpGZIP()\0"
    "slotZCmpZLE()\0slotZChkON()\0slotZChkOFF()\0"
    "slotZATON()\0slotZATOFF()\0slotZEXON()\0"
    "slotZEXOFF()\0slotUEnc()\0slotZSwapSize()\0"
};

void wizardDisk::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        wizardDisk *_t = static_cast<wizardDisk *>(_o);
        switch (_id) {
        case 0: _t->saved((*reinterpret_cast< QList<QStringList>(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 1: _t->accept(); break;
        case 2: _t->slotClose(); break;
        case 3: _t->slotCheckComplete(); break;
        case 4: _t->slotChangedDisk(); break;
        case 5: _t->slotTreeDiskChanged(); break;
        case 6: _t->slotResizeFS(); break;
        case 7: _t->slotRemoveFS(); break;
        case 8: _t->slotAddFS(); break;
        case 9: _t->slotSaveFSResize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->slotTreeMountsRightClick(); break;
        case 11: _t->slotTerminal(); break;
        case 12: _t->slotZCMON(); break;
        case 13: _t->slotZCMOFF(); break;
        case 14: _t->slotZCMNOAUTO(); break;
        case 15: _t->slotZDDOFF(); break;
        case 16: _t->slotZDDON(); break;
        case 17: _t->slotZDDVERIFY(); break;
        case 18: _t->slotZCmpOFF(); break;
        case 19: _t->slotZCmpLZJB(); break;
        case 20: _t->slotZCmpGZIP(); break;
        case 21: _t->slotZCmpZLE(); break;
        case 22: _t->slotZChkON(); break;
        case 23: _t->slotZChkOFF(); break;
        case 24: _t->slotZATON(); break;
        case 25: _t->slotZATOFF(); break;
        case 26: _t->slotZEXON(); break;
        case 27: _t->slotZEXOFF(); break;
        case 28: _t->slotUEnc(); break;
        case 29: _t->slotZSwapSize(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData wizardDisk::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject wizardDisk::staticMetaObject = {
    { &QWizard::staticMetaObject, qt_meta_stringdata_wizardDisk,
      qt_meta_data_wizardDisk, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &wizardDisk::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *wizardDisk::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *wizardDisk::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_wizardDisk))
        return static_cast<void*>(const_cast< wizardDisk*>(this));
    return QWizard::qt_metacast(_clname);
}

int wizardDisk::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizard::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 30)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 30;
    }
    return _id;
}

// SIGNAL 0
void wizardDisk::saved(QList<QStringList> _t1, bool _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
