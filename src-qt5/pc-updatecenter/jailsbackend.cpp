/**************************************************************************
*   Copyright (C) 2013- by Yuri Momotyuk                                   *
*   yurkis@gmail.com                                                      *
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

#include "jailsbackend.h"
#include "utils.h"
#include "pcbsd-utils.h"
#include <QStringList>
#include <QDebug>

__string_constant WARDEN_LIST_SEPARATOR = "---------------------------------------------------------------------------";

__string_constant JAIL_DEFAULT_PREFIX = "/usr/jails/";

///////////////////////////////////////////////////////////////////////////////
CJailsBackend::CJailsBackend()
{
    misEnabled= false;
}

///////////////////////////////////////////////////////////////////////////////
QVector<CJailsBackend::SJailDescription> CJailsBackend::jailsList()
{
    QVector<CJailsBackend::SJailDescription> retVal;
    QStringList warden_list;
    warden_list = pcbsd::Utils::runShellCommand("warden list");
    bool list_started = false;
    QString val;
    for (int i=0; i<warden_list.size(); i++)
    {
        val = warden_list[i];
        val= val.trimmed();

        if (val.indexOf(WARDEN_LIST_SEPARATOR) == 0)
        {
            list_started= true;
            continue;
        }
        if (list_started)
        {
            //Example:
            // owc                     Disabled    Stopped     standard
            // ^0                      ^1          ^2          ^3

            SJailDescription item;
            QStringList item_list= val.split(" ", QString::SkipEmptyParts);
            item.mName = item_list[0];
            item.mType = item_list[3];
            item.misStarted = (item_list[2].toLower() == "running");
            item.misEnabled = (item_list[1].toLower() == "enabled");
            retVal.push_back(item);
        }
    }
    return retVal;
}

///////////////////////////////////////////////////////////////////////////////
bool CJailsBackend::setJail(QString name)
{
    mName= name;
    return true;
}

///////////////////////////////////////////////////////////////////////////////
void CJailsBackend::setJailEnabled(bool isEnabled)
{
    misEnabled = isEnabled;
}

///////////////////////////////////////////////////////////////////////////////
bool CJailsBackend::jailEnabled()
{
    return misEnabled;
}

///////////////////////////////////////////////////////////////////////////////
QString CJailsBackend::jailName()
{
    return mName;
}

///////////////////////////////////////////////////////////////////////////////
QString CJailsBackend::jailPrefix()
{
    QString retVal;
    if (misEnabled && mName.length())
    {
        retVal = QString(JAIL_DEFAULT_PREFIX)+mName;
    }
    return retVal;
}


