
#ifndef __TAGGED_LISTVIEWITEM

#define __TAGGED_LISTVIEWITEM

#include <QTreeWidgetItem>
#include <QTreeWidget>

template<class _TagT>
class BasicTaggedListViewItem : public QTreeWidgetItem
{
    _TagT _tag;
    
public:

    BasicTaggedListViewItem ( QTreeWidget *list )
        : QTreeWidgetItem(list), _tag(0)
    {
    }
       
    ~BasicTaggedListViewItem ( void )
    {
    }
    
    inline _TagT tag ( void )
    {
        return _tag;
    }
    
    inline void tag ( _TagT value )
    {
        _tag = value;
    }
};

typedef BasicTaggedListViewItem<size_t> TaggedListViewItem;

#endif
