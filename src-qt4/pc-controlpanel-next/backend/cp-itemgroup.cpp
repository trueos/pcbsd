#include "cp-itemgroup.h"
#include "misc.h"

#include <QDir>
#include <QDebug>

///////////////////////////////////////////////////////////////////////////////
CItemGroup::CItemGroup(QString path, QString name):QThread(NULL)
{
    mPath= path;
    mName= name;
}

///////////////////////////////////////////////////////////////////////////////
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

        {
            items.push_back(item);
        }
    }

    {
        CP_CRITICAL_SECTION(mGlobalMutex);
        mItems = items;
    }

    emit itemsReady();

    return true;
}

///////////////////////////////////////////////////////////////////////////////
void CItemGroup::readAssync()
{
    start();
}

///////////////////////////////////////////////////////////////////////////////
QVector<CControlPanelItem> CItemGroup::items(QStringList enabled_de, QString filter)
{
    QVector<CControlPanelItem> retVal;

    CP_CRITICAL_SECTION(mGlobalMutex);

    for (int i=0; i<mItems.size(); i++)
    {

    }
    return retVal;
}

///////////////////////////////////////////////////////////////////////////////
void CItemGroup::run()
{
    read();
    emit itemsReadyAssync();
}
