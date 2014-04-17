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
#include "pcbsd-sysFlags.h"
#include <QDebug>
#include <QRegExp>



__string_constant PC_UPDATE_COMMAND = "pc-updatemanager";
//_STRING_CONSTANT FBSD_UPDATE_COMMAND = "cat";
__string_constant FBSD_UPDATE_COMMAND = "pc-fbsdupdatecheck";
static const QStringList PC_UPDATE_ARGS(QStringList()<<"check");
static const QStringList FBSD_UPDATE_ARGS (QStringList()<<"update");
//static const QStringList FBSD_UPDATE_ARGS (QStringList()<<"/home/yurkis/_sysbasesys_check.txt");

__string_constant NAME_TAG = "NAME:";
__string_constant TYPE_TAG = "TYPE:";
__string_constant TAG_TAG = "TAG:";
__string_constant VERSION_TAG = "VERSION:";
__string_constant DEATILS_TAG = "DETAILS:";
__string_constant DATE_TAG = "DATE:";
__string_constant SIZE_TAG = "SIZE:";
__string_constant CU_END_MARKER = "To install:";
__string_constant PATCH_TYPE = "PATCH";
__string_constant SYSUPDATE_TYPE = "SYSUPDATE";
__string_constant STANDALONE_TAG = "STANDALONE:";
__string_constant REQUIRESREBOOT_TAG = "REQUIRESREBOOT:";

__string_constant FILES_MODIFYED_LOCALLY = "been downloaded because the files have been modified locally:";
__string_constant FILES_TO_DELETE = "The following files will be removed as part of updating to";
__string_constant FILES_TO_UPDATE = "The following files will be updated as part of updating to";

__string_constant NETWORKING_PROBLEM= "No mirrors remaining, giving up.";

__string_constant SYS_PATCH_DOWNLOADING_WORD= "DOWNLOADING:";
__string_constant SYS_PATCH_FETCH= "FETCH";
__string_constant SYS_PATCH_DL_FINISHED= "DOWNLOADFINISHED:";
__string_constant SYS_PATCH_TOTAL_STEPS= "TOTALSTEPS:";
__string_constant SYS_PATCH_SETSTEPS= "SETSTEPS:";
__string_constant SYS_PATCH_MSG= "MSG:";
__string_constant SYS_PATCH_FINISHED= "INSTALLFINISHED:";

__string_constant UPDATE_DESCRIPTIONS_FETCH_COMMAND = "fetch -q -o-";
__string_constant UPDATE_DESCRIPTIONS_URL = "http://fbsd-update.pcbsd.org/updates.desc";
__string_constant UPDATE_DESCRIPTIONS_FIELDS_SEPARATOR = ":::";

__string_constant FILES_REQUIRED_REBOOT []= { "/boot/*", "/usr/lib/libc*" };

const int FILES_REQUIRED_REBOOT_SIZE = sizeof(FILES_REQUIRED_REBOOT) / sizeof(char*);

///////////////////////////////////////////////////////////////////////////////
CSysController::CSysController()
{
    misFREEBSDCheck= false;

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PCFETCHGUI", "YES");
    process().setProcessEnvironment(env);
    misFBSDRebootRequired= false;
    misRebootRequired = false;
    mCurrCheckState = eUndefined;
}

///////////////////////////////////////////////////////////////////////////////
bool CSysController::rebootRequired()
{
    return misRebootRequired && (currentState()!= eUPDATING);
}

///////////////////////////////////////////////////////////////////////////////
QVector<CSysController::SFbsdUpdatesDescription> CSysController::updateDescriptions(QString RelName, bool isForse)
{
    QVector<CSysController::SFbsdUpdatesDescription> out;

    if (!mvFbsdUpdateDescriptions.size() || isForse)
    {
        QStringList fetch_out = pcbsd::Utils::runShellCommand(QString(UPDATE_DESCRIPTIONS_FETCH_COMMAND) + " " + UPDATE_DESCRIPTIONS_URL);
        mvFbsdUpdateDescriptions.clear();
        for (int i=0; i<fetch_out.size(); i++)
        {
            SFbsdUpdatesDescription entry;
            QStringList line_split = fetch_out[i].split(UPDATE_DESCRIPTIONS_FIELDS_SEPARATOR);
            if (line_split.size() < 3)
                continue;
            entry.mRelease = line_split[0];
            entry.mDescription = line_split[2];
            entry.mUpdateNo = line_split[1].toInt();
            mvFbsdUpdateDescriptions.push_back(entry);
        }
    }

    //filter by RelName
    for (int i=0; i<mvFbsdUpdateDescriptions.size(); i++)
    {
        if ((!RelName.length()) || RelName.trimmed().toUpper() == mvFbsdUpdateDescriptions[i].mRelease.trimmed().toUpper() )
        {
            out.push_back(mvFbsdUpdateDescriptions[i]);
        }
    }
    return out;
}

///////////////////////////////////////////////////////////////////////////////
void CSysController::updateSelected(QVector<CSysController::SSystemUpdate> selectedUpdates)
{
    mvUpdatesToApply= selectedUpdates;
    mCurrentUpdate= 0;
    launchUpdate();
}

///////////////////////////////////////////////////////////////////////////////
void CSysController::onCheckUpdates()
{
    misFREEBSDCheck= false;
    mCurrCheckState = eUndefined;
    mvUpdates.clear();
    mFilesLocallyModifyed.clear();
    mFilesToRemove.clear();
    mFilesToUpdate.clear();
    mCurrentFbsdDescription = tr("Base system update");
    updateDescriptions();
}

///////////////////////////////////////////////////////////////////////////////
void CSysController::onUpdateAll()
{
    mvUpdatesToApply = mvUpdates;
    mCurrentUpdate= 0;
    launchUpdate();
}

///////////////////////////////////////////////////////////////////////////////
void CSysController::checkShellCommand(QString &cmd, QStringList &args)
{
    if (misFREEBSDCheck)
    {
        cmd= FBSD_UPDATE_COMMAND;
        args= FBSD_UPDATE_ARGS;
        mCurrCheckState = eUndefined;
    }
    else
    {
        cmd= PC_UPDATE_COMMAND;
        args= PC_UPDATE_ARGS;
    }
    //TODO: add sudo if regular user
    //if ()
}

///////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////
void CSysController::onReadCheckLine(QString line)
{    
    if (misFREEBSDCheck)
        parseCheckFREEBSDLine(line);
    else
        parseCheckPCBSDLine(line);

}

///////////////////////////////////////////////////////////////////////////////
void CSysController::onReadUpdateLine(QString line)
{
    line= line.trimmed();
    if (mCurrentUpdate>=mvUpdatesToApply.size())
        return;
    ESysUpdate currUpdateType= mvUpdatesToApply[mCurrentUpdate].mType;
    misRebootRequired |= mvUpdatesToApply[mCurrentUpdate].misRequiresReboot;

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

///////////////////////////////////////////////////////////////////////////////
void CSysController::onReadProcessChar(char character)
{
    qDebug()<<character;
}

///////////////////////////////////////////////////////////////////////////////
void CSysController::onCheckProcessfinished(int exitCode)
{
    if (!misFREEBSDCheck)
    {        
        misFREEBSDCheck= true;
        misFBSDRebootRequired= false;
        launchCheck();
    }
    else
    {        
        if (mFilesToRemove.size() || mFilesToUpdate.size())
        {
            SSystemUpdate entry;
            entry.mName = mCurrentFbsdDescription;
            entry.mType= eFBSDUPDATE;
            entry.misRequiresReboot= misFBSDRebootRequired;
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

///////////////////////////////////////////////////////////////////////////////
void CSysController::onUpdateProcessfinished(int exitCode)
{
    Q_UNUSED(exitCode);

    if (currentState() != eUPDATING)
        return;
    mCurrentUpdate++;
    if (mCurrentUpdate == mvUpdatesToApply.size())
    {
        if (misRebootRequired)
        {
            system("touch /tmp/.fbsdup-reboot");
        }
        check();
    }
    else
    {
        launchUpdate();
    }
}

///////////////////////////////////////////////////////////////////////////////
void CSysController::onCancel()
{
    process().terminate();
    process().waitForFinished();
    check();
}

///////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////
void CSysController::parseCheckFREEBSDLine(QString line)
{
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
        mCurrCheckState= eFilesModifyedLocally;
        return;
    }
    else if (line.contains(FILES_TO_DELETE))
    {
        mCurrCheckState= eFilesToDelete;
        mCurrentFbsdDescription= fbsdUpdateDescription(line);
        return;
    }
    else if (line.contains(FILES_TO_UPDATE))
    {        
        mCurrCheckState= eFilesToUpdate;
        mCurrentFbsdDescription= fbsdUpdateDescription(line);

        return;
    }


    if (eFilesModifyedLocally == mCurrCheckState)
    {
        mFilesLocallyModifyed<<line;
    }
    else
    if (eFilesToDelete == mCurrCheckState)
    {
        mFilesToRemove<<line;
    }
    else
    if(eFilesToUpdate == mCurrCheckState)
    {
        mFilesToUpdate<<line;
        // Check if reboot requuired.
        // FILES_REQUIRED_REBOOT const array contains wildcards for files.
        // Modification of that files requires system reboot
        QRegExp rx;
        rx.setPatternSyntax(QRegExp::WildcardUnix);
        for (int i=0; i<FILES_REQUIRED_REBOOT_SIZE; i++)
        {
            rx.setPattern(FILES_REQUIRED_REBOOT[i]);
            if (rx.exactMatch(line))
            {
                misFBSDRebootRequired= true;
            }//if match
        }//for all wildcard
    }

}

///////////////////////////////////////////////////////////////////////////////
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
        progress.misCanCancel= true;
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

///////////////////////////////////////////////////////////////////////////////
void CSysController::parseUpgradeLine(QString line)
{
    SProgress progress;
    progress.mMessage = tr("Installing system upgrade");
    reportProgress(progress);
    reportLogLine(line);
}

///////////////////////////////////////////////////////////////////////////////
void CSysController::parseFreeBSDUpdateLine(QString line)
{
    SProgress progress;
    progress.mMessage = tr("Installing system update");
    reportProgress(progress);
    reportLogLine(line);
}

///////////////////////////////////////////////////////////////////////////////
QString CSysController::fbsdUpdateDescription(QString line)
{
    // Example line: "The following files will be updated as part of updating to 10.0-RELEASE-p3:"
    line = line.trimmed();
    QString release = line.right(line.length() - line.lastIndexOf(" "));
    release = release.left(release.lastIndexOf("-")).trimmed();
    QString patch = line.right(line.length() - line.lastIndexOf("-")).replace("-p","").replace(':',"");
    int patch_num = patch.toInt();
    QVector<SFbsdUpdatesDescription> descrs = updateDescriptions(release);

    QString ret = tr("Base system update");

    for(int i=0; i<descrs.size(); i++)
    {
        if (descrs[i].mUpdateNo == patch_num)
        {
            ret = descrs[i].mDescription;
            break;
        }
    }
    return ret;
}
