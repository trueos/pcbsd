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

#include "pbiinfo.h"

#include <QProcess>
#include <QFile>
#include <QCoreApplication>
#include <QTextStream>

static const char* const GENERIC_NAME_FIELD = "PBI Information for:";
static const char* const NAME_FIELD         = "Name:";
static const char* const VERSION_FIELD      = "Version:";
static const char* const PREFIX_FIELD       = "Prefix:";
static const char* const AUTHOR_FIELD       = "Author:";
static const char* const WEBSITE_FIELD      = "Website:";
static const char* const ARCH_FIELD         = "Arch:";
static const char* const FREE_BSD_VER_FIELD = "FbsdVer:";
static const char* const CREATOR_VER_FIELD  = "CreatorVer:";
static const char* const REPO_FIELD         = "Associated Repo:";
static const char* const DESKTOP_ICONS_FIELD= "DesktopIcons:";
static const char* const MENU_ICONS_FIELD   = "MenuIcons:";
static const char* const SIGNATURE_FIELD    = "Signature:";
static const char* const BUILT_FIELD        = "Built:";

///////////////////////////////////////////////////////////////////////////////

//! Macro for getting string field value
#define TRY_GET_VALUE_STR(val_name, field)\
if (Str.contains(val_name)){\
                            Entry.field = Str.replace(val_name,"").trimmed();\
                                          continue;}\

//! Macro for getting boolean field value
#define TRY_GET_VALUE_BOOL(val_name, field, true_val)\
if (Str.contains(val_name)){\
                            Entry.field = Str.replace(val_name,"").trimmed().toLower() == QString(true_val).toLower();\
                                          continue;}\

///////////////////////////////////////////////////////////////////////////////
int CInstalledPBI::refresh()
{
    QString Str;
    SPBIInfo Entry;

    clear();

    QProcess* loadPBIs = new QProcess();
    loadPBIs->setProcessChannelMode(QProcess::MergedChannels);
    loadPBIs->start(QString("pbi_info"), QStringList() << "-v");
    while(loadPBIs->state() == QProcess::Starting || loadPBIs->state() == QProcess::Running) {
      loadPBIs->waitForFinished(200);
      QCoreApplication::processEvents();
    }
    QString line;
    while ( loadPBIs->canReadLine() )
    {
        Str = loadPBIs->readLine().simplified();
        if (Str.contains(GENERIC_NAME_FIELD))
        {
            if (Entry.mName.length())
                push_back(Entry);
            //if we have 'PBI Information for:' start new entry filling
            Entry = SPBIInfo(); // clear Entry structure
            Entry.mGenericName = Str.replace(GENERIC_NAME_FIELD,"").trimmed();
            continue;
        }
        TRY_GET_VALUE_STR(NAME_FIELD, mName);
        TRY_GET_VALUE_STR(VERSION_FIELD, mVersion);
        TRY_GET_VALUE_STR(PREFIX_FIELD, mPrefix);
        TRY_GET_VALUE_STR(AUTHOR_FIELD, mAuthor);
        TRY_GET_VALUE_STR(WEBSITE_FIELD, mWebsite);
        TRY_GET_VALUE_STR(ARCH_FIELD, mArch);
        TRY_GET_VALUE_STR(FREE_BSD_VER_FIELD, mFreeBSDVersion);
        TRY_GET_VALUE_STR(CREATOR_VER_FIELD, mCreatorVersion);
        TRY_GET_VALUE_STR(REPO_FIELD, mRepo);
        TRY_GET_VALUE_STR(BUILT_FIELD, mBuildTimeString);

        TRY_GET_VALUE_BOOL(DESKTOP_ICONS_FIELD, misDesktopIconsInstalled, "YES");
        TRY_GET_VALUE_BOOL(MENU_ICONS_FIELD, misMenuIconsInstalled, "YES");
        TRY_GET_VALUE_BOOL(SIGNATURE_FIELD, misSignatureVerified, "Verified");
    }
    if (Entry.mName.length())
        push_back(Entry);
    return size();
}

#undef TRY_GET_VALUE_STR
#undef TRY_GET_VALUE_BOOL

///////////////////////////////////////////////////////////////////////////////
bool CInstalledPBI::findByName(const QString What,
                               SPBIInfo& RetVal,
                               Qt::CaseSensitivity cs)
{
    for (int i=0; i<size(); i++)
    {
        if (What.compare(operator[](i).mName, cs) == 0)
        {
            RetVal = operator[](i);
            return true;
        }//if
    }//for
    return false;
}

///////////////////////////////////////////////////////////////////////////////
CSingleInstalledPBI& CSingleInstalledPBI::getRef()
{
    static CSingleInstalledPBI instance;
    return instance;
}

