/**************************************************************************
*   Copyright (C) 2011 by Yuri Momotyuk                                   *
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

#include "deinfo.h"
#include <QProcess>
#include <QStringList>

static const char* const DE_NAME = "DE name:";
static const char* const DE_ACTIVE = "Current DE:";
static const char* const DE_XDG = "XDG compatible:";
static const char* const DE_INSTALLED = "Installed:";
static const char* const DE_SUDO = "Sudo command:";

///////////////////////////////////////////////////////////////////////////////
#define TRY_GET_VALUE_STR(val_name, field)\
if (Str.contains(val_name)){\
                            Entry.field = Str.replace(val_name,"").trimmed();\
                                          continue;}\

#define TRY_GET_VALUE_BOOL(val_name, field, true_val)\
if (Str.contains(val_name)){\
                            Entry.field = Str.replace(val_name,"").trimmed().toLower() == QString(true_val).toLower();\
                                          continue;}\

int CDEList::refresh(bool isAll)
{
    mvDE.clear();

    QStringList flags;
    if (isAll)
        flags<<"-a";
    else
        flags<<"-i";

    QProcess* deinfo = new QProcess();
    deinfo->setProcessChannelMode(QProcess::MergedChannels);
    deinfo->start(QString("/usr/local/bin/de-info"), flags);
    deinfo->waitForFinished(-1);
    CDEInfo Entry;
    QString Str;

    while ( deinfo->canReadLine() )
    {
        Str = deinfo->readLine().simplified();
	//qDebug() << "de-info line:" << Str;
        if (Str.contains(DE_NAME))
        {
            if (Entry.mName.length())
            {
                mvDE.push_back(Entry);
                Entry = CDEInfo();
            }
            Entry.mName = Str.replace(DE_NAME,"").trimmed();
            continue;
        }//if found 'DE name'

        TRY_GET_VALUE_BOOL(DE_ACTIVE, misActive, "yes");
        TRY_GET_VALUE_BOOL(DE_INSTALLED, misInstalled, "yes");
        TRY_GET_VALUE_STR(DE_SUDO, mSudoCommand);
    }//while process output reading

    if (Entry.mName.length())
        mvDE.push_back(Entry);

    return mvDE.size();
}

///////////////////////////////////////////////////////////////////////////////
CDEInfo* CDEList::active()
{
    for(int i =0; i<mvDE.size(); i++)
    {
        if (mvDE[i].misActive)
            return &mvDE[i];
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
CDEInfo* CDEList::byName(QString Name)
{
    for (int i =0; i<mvDE.size(); i++)
    {
        if (!mvDE[i].mName.compare(Name.trimmed(), Qt::CaseInsensitive))
            return &mvDE[i];
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
CSingleDEList& CSingleDEList::getRef()
{
    static CSingleDEList instance;
    return instance;
}
