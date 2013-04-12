/****************************************************************************
** Meta object code from reading C++ file 'dialogKeyboard.h'
**
** Created: Wed Mar 20 02:46:17 2013
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
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x08,
      28,   15,   15,   15, 0x08,
      44,   40,   15,   15, 0x08,
      76,   15,   15,   15, 0x08,
      96,   40,   15,   15, 0x08,
     129,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_widgetKeyboard[] = {
    "widgetKeyboard\0\0slotClose()\0slotApply()\0"
    "row\0slotCurrentKbLayoutChanged(int)\0"
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
        case 0: _t->slotClose(); break;
        case 1: _t->slotApply(); break;
        case 2: _t->slotCurrentKbLayoutChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->slotUpdateKbOnSys(); break;
        case 4: _t->slotCurrentKbVariantChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->slotSelectedKbItemChanged(); break;
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
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
