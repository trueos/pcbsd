/****************************************************************************
** Meta object code from reading C++ file 'wizardFreeBSD.h'
**
** Created: Wed Mar 20 02:46:20 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "wizardFreeBSD.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wizardFreeBSD.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_wizardFreeBSD[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   15,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     107,   14,   14,   14, 0x08,
     116,   14,   14,   14, 0x08,
     128,   14,   14,   14, 0x08,
     148,   14,   14,   14, 0x08,
     170,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_wizardFreeBSD[] = {
    "wizardFreeBSD\0\0,,,,,,,,,\0"
    "saved(QString,QString,QString,QString,QString,QString,bool,bool,bool,Q"
    "StringList)\0"
    "accept()\0slotClose()\0slotCheckComplete()\0"
    "slotSuggestUsername()\0slotChangedNic()\0"
};

void wizardFreeBSD::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        wizardFreeBSD *_t = static_cast<wizardFreeBSD *>(_o);
        switch (_id) {
        case 0: _t->saved((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< bool(*)>(_a[7])),(*reinterpret_cast< bool(*)>(_a[8])),(*reinterpret_cast< bool(*)>(_a[9])),(*reinterpret_cast< QStringList(*)>(_a[10]))); break;
        case 1: _t->accept(); break;
        case 2: _t->slotClose(); break;
        case 3: _t->slotCheckComplete(); break;
        case 4: _t->slotSuggestUsername(); break;
        case 5: _t->slotChangedNic(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData wizardFreeBSD::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject wizardFreeBSD::staticMetaObject = {
    { &QWizard::staticMetaObject, qt_meta_stringdata_wizardFreeBSD,
      qt_meta_data_wizardFreeBSD, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &wizardFreeBSD::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *wizardFreeBSD::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *wizardFreeBSD::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_wizardFreeBSD))
        return static_cast<void*>(const_cast< wizardFreeBSD*>(this));
    return QWizard::qt_metacast(_clname);
}

int wizardFreeBSD::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizard::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void wizardFreeBSD::saved(QString _t1, QString _t2, QString _t3, QString _t4, QString _t5, QString _t6, bool _t7, bool _t8, bool _t9, QStringList _t10)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)), const_cast<void*>(reinterpret_cast<const void*>(&_t8)), const_cast<void*>(reinterpret_cast<const void*>(&_t9)), const_cast<void*>(reinterpret_cast<const void*>(&_t10)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
