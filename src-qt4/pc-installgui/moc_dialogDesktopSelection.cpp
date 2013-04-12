/****************************************************************************
** Meta object code from reading C++ file 'dialogDesktopSelection.h'
**
** Created: Wed Mar 20 02:46:15 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dialogDesktopSelection.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dialogDesktopSelection.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_desktopSelection[] = {

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
      18,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      37,   17,   17,   17, 0x08,
      49,   17,   17,   17, 0x08,
      71,   60,   17,   17, 0x08,
     111,   60,   17,   17, 0x08,
     153,   17,   17,   17, 0x08,
     174,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_desktopSelection[] = {
    "desktopSelection\0\0saved(QStringList)\0"
    "slotClose()\0slotSave()\0aItem,aCol\0"
    "slotShowLabelDesc(QTreeWidgetItem*,int)\0"
    "slotDeskPkgsChanged(QTreeWidgetItem*,int)\0"
    "slotMetaRightClick()\0slotMetaViewPkgs()\0"
};

void desktopSelection::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        desktopSelection *_t = static_cast<desktopSelection *>(_o);
        switch (_id) {
        case 0: _t->saved((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 1: _t->slotClose(); break;
        case 2: _t->slotSave(); break;
        case 3: _t->slotShowLabelDesc((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->slotDeskPkgsChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->slotMetaRightClick(); break;
        case 6: _t->slotMetaViewPkgs(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData desktopSelection::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject desktopSelection::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_desktopSelection,
      qt_meta_data_desktopSelection, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &desktopSelection::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *desktopSelection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *desktopSelection::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_desktopSelection))
        return static_cast<void*>(const_cast< desktopSelection*>(this));
    return QWidget::qt_metacast(_clname);
}

int desktopSelection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void desktopSelection::saved(QStringList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
