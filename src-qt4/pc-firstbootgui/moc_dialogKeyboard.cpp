/****************************************************************************
** Meta object code from reading C++ file 'dialogKeyboard.h'
**
** Created: Fri Apr 5 17:27:23 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dialogKeyboard.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dialogKeyboard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_widgetKeyboard[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   16,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      58,   15,   15,   15, 0x08,
      70,   15,   15,   15, 0x08,
      86,   82,   15,   15, 0x08,
     118,   15,   15,   15, 0x08,
     138,   82,   15,   15, 0x08,
     171,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_widgetKeyboard[] = {
    "widgetKeyboard\0\0,,\0"
    "changedLayout(QString,QString,QString)\0"
    "slotClose()\0slotApply()\0row\0"
    "slotCurrentKbLayoutChanged(int)\0"
    "slotUpdateKbOnSys()\0"
    "slotCurrentKbVariantChanged(int)\0"
    "slotSelectedKbItemChanged()\0"
};

void widgetKeyboard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        widgetKeyboard *_t = static_cast<widgetKeyboard *>(_o);
        switch (_id) {
        case 0: _t->changedLayout((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 1: _t->slotClose(); break;
        case 2: _t->slotApply(); break;
        case 3: _t->slotCurrentKbLayoutChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->slotUpdateKbOnSys(); break;
        case 5: _t->slotCurrentKbVariantChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->slotSelectedKbItemChanged(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData widgetKeyboard::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject widgetKeyboard::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_widgetKeyboard,
      qt_meta_data_widgetKeyboard, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &widgetKeyboard::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *widgetKeyboard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *widgetKeyboard::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_widgetKeyboard))
        return static_cast<void*>(const_cast< widgetKeyboard*>(this));
    return QWidget::qt_metacast(_clname);
}

int widgetKeyboard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void widgetKeyboard::changedLayout(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
