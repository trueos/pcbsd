/****************************************************************************
** Meta object code from reading C++ file 'dialogFSSize.h'
**
** Created: Wed Mar 20 02:46:16 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dialogFSSize.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dialogFSSize.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_dialogFSSize[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      25,   13,   13,   13, 0x08,
      37,   13,   13,   13, 0x08,
      55,   48,   13,   13, 0x08,
      83,   48,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_dialogFSSize[] = {
    "dialogFSSize\0\0saved(int)\0slotClose()\0"
    "slotSave()\0newVal\0slotSliderChangedValue(int)\0"
    "slotSpinBoxChanged(int)\0"
};

void dialogFSSize::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        dialogFSSize *_t = static_cast<dialogFSSize *>(_o);
        switch (_id) {
        case 0: _t->saved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->slotClose(); break;
        case 2: _t->slotSave(); break;
        case 3: _t->slotSliderChangedValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->slotSpinBoxChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData dialogFSSize::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject dialogFSSize::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_dialogFSSize,
      qt_meta_data_dialogFSSize, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &dialogFSSize::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *dialogFSSize::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *dialogFSSize::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_dialogFSSize))
        return static_cast<void*>(const_cast< dialogFSSize*>(this));
    return QDialog::qt_metacast(_clname);
}

int dialogFSSize::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void dialogFSSize::saved(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
