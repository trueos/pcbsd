#include "cp-itemgroup.h"

#include <QDir>
#include <QDebug>

CItemGroup::CItemGroup(QString path, QString name):QObject(NULL)
{
    mPath= path;
    mName= name;
}

bool CItemGroup::read()
{
    QVector<CControlPanelItem> items;

    QDir dir(mPath);
    if (!dir.exists(mPath))
        return false;

    QStringList dir_list =dir.entryList(QStringList("*.desktop"));
    for(int i=0; i<dir_list.size(); i++)
    {
        CControlPanelItem item;

        if (!item.read(dir.absoluteFilePath(dir_list[i])))
        {
            continue;
        }

        items.push_back(item);
    }
    return true;
}
