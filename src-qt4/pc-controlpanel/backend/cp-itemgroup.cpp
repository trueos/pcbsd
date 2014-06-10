/**************************************************************************
*   Copyright (C) 2014 by Yuri Momotyuk                                   *
*   yurkis@pcbsd.org                                                      *
*                                                                         *
*   Permission is hereby granted, free of charge, to any person obtaining *
*   a copy of this software and associated documentation files (the       *
*   "Software"), to deal in the Software without restriction, including   *
*   without limitation the rights to use, copy, modify, merge, publish,   *
*   distribute, sublicense, and/or sell copies of the Software, and to    *
*   permit persons to whom the Software is furnished to do so, subject to *
*   the following conditions:                                             *
*                                                                         *
*   The above copyright notice and this permission notice shall be        *
*   included in all copies or substantial portions of the Software.       *
*                                                                         *
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
*   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
*   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*
*   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
*   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
*   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
*   OTHER DEALINGS IN THE SOFTWARE.                                       *
***************************************************************************/

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
    mSkipRootAccess= false;
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

        if (!item.read(dir.absoluteFilePath(dir_list[i]),mSkipRootAccess))
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
void CItemGroup::setSkipRootRequiredItems(bool isSkip)
{
    mSkipRootAccess= isSkip;
}

///////////////////////////////////////////////////////////////////////////////
QVector<CControlPanelItem> CItemGroup::items(QStringList enabled_de, QString filter)
{
    Q_UNUSED(filter);
    QVector<CControlPanelItem> retVal;

    CP_CRITICAL_SECTION(mGlobalMutex);

    for (int i=0; i<mItems.size(); i++)
    {
        if (!checkItemDE(mItems[i], enabled_de))
           continue;        

        //Cache icon...
        mItems[i].displayIcon();

        retVal.push_back(mItems[i]);
    }

    return retVal;
}

///////////////////////////////////////////////////////////////////////////////
bool CItemGroup::checkItemDE(CControlPanelItem &item, const QStringList &enabled_de)
{
    QStringList item_allowed_de=item.showIn();
    QStringList item_disallowed_de=item.notShowIn();
    QString current_de= pcbsd::Utils::currentDesktop().Name.toLower().trimmed();

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
    return true;
}

///////////////////////////////////////////////////////////////////////////////
void CItemGroup::run()
{
    read();
    emit itemsReadyAssync();
    exec();
}
