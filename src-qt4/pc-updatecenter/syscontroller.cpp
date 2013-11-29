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

#include "syscontroller.h"
#include "utils.h"
#include "pcbsd-utils.h"
#include <QDebug>

_STRING_CONSTANT PC_UPDATE_COMMAND = "pc-updatemanager";
//_STRING_CONSTANT FBSD_UPDATE_COMMAND = "cat";
_STRING_CONSTANT FBSD_UPDATE_COMMAND = "pc-fbsdupdatecheck";
static const QStringList PC_UPDATE_ARGS(QStringList()<<"check");
static const QStringList FBSD_UPDATE_ARGS (QStringList()<<"update");
//static const QStringList FBSD_UPDATE_ARGS (QStringList()<<"/home/yurkis/_sysbasesys_check.txt");

_STRING_CONSTANT NAME_TAG = "NAME:";
_STRING_CONSTANT TYPE_TAG = "TYPE:";
_STRING_CONSTANT TAG_TAG = "TAG:";
_STRING_CONSTANT VERSION_TAG = "VERSION:";
_STRING_CONSTANT DEATILS_TAG = "DETAILS:";
_STRING_CONSTANT DATE_TAG = "DATE:";
_STRING_CONSTANT SIZE_TAG = "SIZE:";
_STRING_CONSTANT CU_END_MARKER = "To install:";
_STRING_CONSTANT PATCH_TYPE = "PATCH";
_STRING_CONSTANT SYSUPDATE_TYPE = "SYSUPDATE";
_STRING_CONSTANT STANDALONE_TAG = "STANDALONE:";
_STRING_CONSTANT REQUIRESREBOOT_TAG = "REQUIRESREBOOT:";

_STRING_CONSTANT FILES_MODIFYED_LOCALLY = "been downloaded because the files have been modified locally:";
_STRING_CONSTANT FILES_TO_DELETE = "The following files will be removed as part of updating to";
_STRING_CONSTANT FILES_TO_UPDATE = "The following files will be updated as part of updating to";

_STRING_CONSTANT NETWORKING_PROBLEM= "No mirrors remaining, giving up.";

_STRING_CONSTANT SYS_PATCH_DOWNLOADING_WORD= "DOWNLOADING:";
_STRING_CONSTANT SYS_PATCH_FETCH= "FETCH";
_STRING_CONSTANT SYS_PATCH_DL_FINISHED= "DOWNLOADFINISHED:";
_STRING_CONSTANT SYS_PATCH_TOTAL_STEPS= "TOTALSTEPS:";
_STRING_CONSTANT SYS_PATCH_SETSTEPS= "SETSTEPS:";
_STRING_CONSTANT SYS_PATCH_MSG= "MSG:";
_STRING_CONSTANT SYS_PATCH_FINISHED= "INSTALLFINISHED:";

CSysController::CSysController()
{
    misFREEBSDCheck= false;

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PCFETCHGUI", "YES");
    process().setProcessEnvironment(env);

}

void CSysController::updateSelected(QVector<CSysController::SSystemUpdate> selectedUpdates)
{
    mvUpdatesToApply= selectedUpdates;
    mCurrentUpdate= 0;
    launchUpdate();
}

void CSysController::onCheckUpdates()
{
    misFREEBSDCheck= false;
    mvUpdates.clear();
    mFilesLocallyModifyed.clear();
    mFilesToRemove.clear();
    mFilesToUpdate.clear();
}

void CSysController::checkShellCommand(QString &cmd, QStringList &args)
{
    if (misFREEBSDCheck)
    {
        cmd= FBSD_UPDATE_COMMAND;
        args= FBSD_UPDATE_ARGS;
    }
    else
    {
        cmd= PC_UPDATE_COMMAND;
        args= PC_UPDATE_ARGS;
    }
}

void CSysController::updateShellCommand(QString &cmd, QStringList &args)
{
    if (mCurrentUpdate>=mvUpdatesToApply.size())
        return;

    ESysUpdate currUpdateType= mvUpdatesToApply[mCurrentUpdate].mType;
    if ((currUpdateType == ePATCH) || (currUpdateType == eSYSUPDATE))
    {
        cmd= PC_UPDATE_COMMAND;
        args= QStringList()<<"install"<<mvUpdatesToApply[mCurrentUpdate].mTag;
    }
    else
    {
        cmd="freebsd-update";
        args= QStringList()<<"install";
    }

}

void CSysController::onReadCheckLine(QString line)
{    
    if (misFREEBSDCheck)
        parseCheckFREEBSDLine(line);
    else
        parseCheckPCBSDLine(line);

}

void CSysController::onReadUpdateLine(QString line)
{
    line= line.trimmed();
    if (mCurrentUpdate>=mvUpdatesToApply.size())
        return;
    ESysUpdate currUpdateType= mvUpdatesToApply[mCurrentUpdate].mType;
    switch(currUpdateType)
    {
        case ePATCH:
            parsePatchUpdateLine(line);
            break;
        case eSYSUPDATE:
            parseUpgradeLine(line);
            break;
        case eFBSDUPDATE:
            parseFreeBSDUpdateLine(line);
            break;
    }
}

void CSysController::onReadProcessChar(char character)
{
    qDebug()<<character;
}

void CSysController::onCheckProcessfinished(int exitCode)
{
    if (!misFREEBSDCheck)
    {        
        misFREEBSDCheck= true;
        launchCheck();
    }
    else
    {        
        if (mFilesToRemove.size() || mFilesToUpdate.size())
        {
            SSystemUpdate entry;
            entry.mName= tr("Base system update");
            entry.mType= eFBSDUPDATE;
            mvUpdates.push_back(entry);
        }

        int n= mvUpdates.size();

        if (n)
        {

            if (n>1)
                reportUpdatesAvail(tr("%1 system updates avilable").arg(QString::number(n)));
            else
                reportUpdatesAvail(tr("System update is available"));
            return;
        }
        else
        {
            if (!exitCode)
            {
                setCurrentState(eFULLY_UPDATED);
            }
            else
            {
                reportError(tr("Error while system update check process"));
            }
            return;
        }
    }
}

void CSysController::onCancel()
{

}

void CSysController::parseCheckPCBSDLine(QString line)
{
    /*if (line == QString("Your system is up to date!"))
    {
        setCurrentState(eFULLY_UPDATED);
    }*/
    static SSystemUpdate upd;
    line= line.trimmed();

#define _GET_TAG(tag, field)  if(line.indexOf(tag) == 0) {upd.field = line.replace(tag,""); return;}
    _GET_TAG(NAME_TAG, mName);
    _GET_TAG(TAG_TAG, mTag);
    _GET_TAG(VERSION_TAG, mVersion);
    _GET_TAG(DEATILS_TAG, mDetails);
    _GET_TAG(SIZE_TAG, mSize);
#undef _GET_TAG

    if (line.indexOf(TYPE_TAG) == 0)
    {
        line= line.replace(TYPE_TAG, "");
        if (line.contains(SYSUPDATE_TYPE))
        {
            upd.mType = eSYSUPDATE;
        }
        else
        {
            upd.mType = ePATCH;
        }
    }

    if (line.indexOf(DATE_TAG) == 0)
    {
        //Example:
        // DATE: 02-10-2013
        // x     ^0 ^1 ^2   (m-d-y)?
        QString y,m,d;
        line= line.replace(DATE_TAG, "");
        QStringList line_list = line.split("-");
        upd.mDate= QDate(line_list[2].toInt(),
                         line_list[0].toInt(),
                         line_list[1].toInt());
        qDebug()<<upd.mDate.toString("d MMM yyyy");
        return;
    }

    if (line.indexOf(STANDALONE_TAG) == 0)
    {
        line= line.replace(STANDALONE_TAG, "");
        if (line.trimmed().toLower() == "yes")
            upd.misStandalone= true;
    }

    if (line.indexOf(REQUIRESREBOOT_TAG) == 0)
    {
        line= line.replace(REQUIRESREBOOT_TAG, "");
        if (line.trimmed().toLower() == "yes")
            upd.misRequiresReboot= true;
    }

    if (line.indexOf(CU_END_MARKER) == 0)
    {
        mvUpdates.push_back(upd);
        upd = SSystemUpdate();
        return;
    }



}

void CSysController::parseCheckFREEBSDLine(QString line)
{
    qDebug()<<line;

    typedef enum{
        eUndefined,
        eFilesModifyedLocally,
        eFilesToDelete,
        eFilesToUpdate
    }ECheckState;

    static ECheckState currCheckState = eUndefined;

    line=line.trimmed();
    if (!line.length())
    {
        return;
    }

    if (line.contains(NETWORKING_PROBLEM) && (!mvUpdates.size()))
    {
        reportError(tr("Error during update check. Check network connection"));
    }

    if (line.contains(FILES_MODIFYED_LOCALLY))
    {
        currCheckState= eFilesModifyedLocally;
        return;
    }
    else if (line.contains(FILES_TO_DELETE))
    {
        currCheckState= eFilesToDelete;
        return;
    }
    else if (line.contains(FILES_TO_UPDATE))
    {
        currCheckState= eFilesToUpdate;
        return;
    }


    if (eFilesModifyedLocally == currCheckState)
    {
        mFilesLocallyModifyed<<line;
    }
    else
    if (eFilesToDelete == currCheckState)
    {
        mFilesToRemove<<line;
    }
    else
    if(eFilesToUpdate == currCheckState)
    {
        mFilesToUpdate<<line;
    }

}

void CSysController::parsePatchUpdateLine(QString line)
{
    SProgress progress;
    static int total_steps = 0;
    QString dl_speed;
    long dl_size, dl_complete;

    line= line.trimmed();

    if (line.indexOf(SYS_PATCH_DOWNLOADING_WORD) == 0)
    {
        reportLogLine(QString("---------------------"));
        reportLogLine(QString("Installing update ") + mvUpdatesToApply[mCurrentUpdate].mName);
        reportLogLine(line.replace(SYS_PATCH_DOWNLOADING_WORD, "Downloading"));
        progress.mMessage= tr("Preparing to download ") + mvUpdatesToApply[mCurrentUpdate].mName;
        reportProgress(progress);
        return;
    }
    else
    if (line.indexOf(SYS_PATCH_FETCH) == 0)
    {
        return;
    }
    else
    if (parseFetchOutput(line, dl_size, dl_complete, dl_speed))
    {
        progress.mMessage=tr("Downloading update (%1/%2 at %3)").arg(pcbsd::Utils::bytesToHumanReadable(dl_size),
                                                                   pcbsd::Utils::bytesToHumanReadable(dl_complete),
                                                                   dl_speed);
        progress.mProgressMax= dl_size;
        progress.mProgressCurr= dl_complete;
        progress.mSubstate= eDownload;
        reportProgress(progress);
        return;
    }
    else
    if (line.indexOf(SYS_PATCH_TOTAL_STEPS) == 0)
    {
        line.replace(SYS_PATCH_TOTAL_STEPS, "");
        total_steps= line.toInt();
        return;
    }
    else
    if (line.indexOf(SYS_PATCH_SETSTEPS) == 0)
    {
        line.replace(SYS_PATCH_SETSTEPS, "");
        progress.mMessage= tr("Installing update ") + mvUpdatesToApply[mCurrentUpdate].mName;
        progress.mProgressMax= total_steps;
        progress.mProgressCurr= line.toInt();
        progress.mSubstate= eInstall;
        reportProgress(progress);
        return;
    }
    else
    if (line.indexOf(SYS_PATCH_MSG) == 0)
    {
        line.replace(SYS_PATCH_MSG, "");
        reportLogLine(line);
        return;
    }
    else
    if (line.indexOf(SYS_PATCH_FINISHED) == 0)
    {
        reportLogLine(QString("Finished install: ") + mvUpdatesToApply[mCurrentUpdate].mName);
        return;
    }

}

void CSysController::parseUpgradeLine(QString line)
{

}

void CSysController::parseFreeBSDUpdateLine(QString line)
{

}
