#include "cp-itemgroup.h"
#include "misc.h"

#include "pcbsd-utils.h"

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
        CControlPanelItem item = mItems[i];

        if (!checkItemDE(item, enabled_de))
           continue;
        if (!checkItemFilter(item, filter))
           continue;

        retVal.push_back(item);
    }

    return retVal;
}

///////////////////////////////////////////////////////////////////////////////
bool CItemGroup::checkItemDE(CControlPanelItem &item, const QStringList &enabled_de)
{
    QStringList item_allowed_de=item.showIn();
    QStringList item_disallowed_de=item.notShowIn();
    QString current_de= pcbsd::Utils::currentDesktop().Name.toLower().trimmed();

    // Check allowed desktop environments
    if (item_allowed_de.size())
    {
        for (int i=0 ;i<item_allowed_de.size(); i++)
        {
            for (int j=0; j<enabled_de.size(); j++)
            {
                if (item_allowed_de[i].toLower().trimmed() == enabled_de[j].toLower().trimmed())
                {
                    return true;
                }
            }
        }
        return false;
    }

    //Check disallowed desktop environments
    if (item_disallowed_de.size())
    {
        for (int i=0 ;i<item_disallowed_de.size(); i++)
        {
            for (int j=0; j<enabled_de.size(); j++)
            {
                if (item_disallowed_de[i].toLower().trimmed() == enabled_de[j].toLower().trimmed())
                {
                    return false;
                }
            }
        }//for all item's disallowed de
    }//if disallowed de list is not empty
    return true;
}

///////////////////////////////////////////////////////////////////////////////
bool CItemGroup::checkItemFilter(const CControlPanelItem &item, QString filter)
{
    //TODO: implement!
    return true;
}

///////////////////////////////////////////////////////////////////////////////
void CItemGroup::run()
{
    read();
    emit itemsReadyAssync();
    exec();
}
