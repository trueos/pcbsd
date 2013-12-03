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

#include "pbicontroller.h"

#include "pcbsd-utils.h"
#include "utils.h"
#include <QDebug>

_STRING_CONSTANT PBU_UPDATE_CMD= "pbi_update";
_STRING_CONSTANT AVAIL= "Available:";
_STRING_CONSTANT DOWNLOAD_INDICATOR = "DOWNLOADED:";
_STRING_CONSTANT FETCHDONE= "FETCHDONE";
_STRING_CONSTANT FETCH_WORLD= "FETCH";
_STRING_CONSTANT DOWNLOADING_ERROR= "ERROR: ";

///////////////////////////////////////////////////////////////////////////////
CPBIController::CPBIController()
{
}

///////////////////////////////////////////////////////////////////////////////
void CPBIController::updateSelected(QStringList generic_names_list)
{
    mAppsToUpdate= generic_names_list;
    mCurrentUpdate=0;
    misWasFETCHDONE= false;

    launchUpdate();
}

///////////////////////////////////////////////////////////////////////////////
void CPBIController::onCheckUpdates()
{
    mvUpdates.clear();
}

///////////////////////////////////////////////////////////////////////////////
void CPBIController::onUpdateAll()
{
    QStringList upd_list;
    for(int i=0; i<mvUpdates.size(); i++)
    {
        mAppsToUpdate<<mvUpdates[i].mGenericName;
    }
    mCurrentUpdate=0;
    misWasFETCHDONE= false;
}

///////////////////////////////////////////////////////////////////////////////
void CPBIController::onCancel()
{
    process().terminate();
    process().waitForFinished();
    check();
}

///////////////////////////////////////////////////////////////////////////////
void CPBIController::onReadCheckLine(QString line)
{
    //Example:
    //        vlc-2.0.8a_1-amd64 - Available: 2.1.0_5
    //        ^0                 ^1^2         ^3
    if (line.indexOf(AVAIL)>0)
    {
        QStringList upd_list = line.split(" ");
        SPBIUpdate entry;

        entry.mGenericName= upd_list[0];
        entry.mNewVer= upd_list[3];
        // remove arch
        entry.mName= entry.mGenericName.left(entry.mGenericName.lastIndexOf("-"));
        // get installed ver
        entry.mOldVer= entry.mName.right(entry.mName.length() - entry.mName.lastIndexOf("-") - 1);
        // remove old version
        entry.mName= entry.mName.left(entry.mName.lastIndexOf("-"));
        mvUpdates.push_back(entry);
    }
}

///////////////////////////////////////////////////////////////////////////////
void CPBIController::onReadUpdateLine(QString line)
{
    SProgress progress;
    line= line.trimmed();
    QString log_message= line;
    qDebug()<<line;
    progress.mLogMessages = QStringList()<<line;
    progress.mItemNo= mCurrentUpdate;
    progress.mItemsCount= mAppsToUpdate.size();

    QString current_app = mAppsToUpdate[mCurrentUpdate];
    current_app= current_app.left(current_app.lastIndexOf("-")); // remove arch
    current_app= current_app.left(current_app.lastIndexOf("-")); // remove ver

    if (line.contains(DOWNLOAD_INDICATOR))
    {
        // parse download
        progress.misCanCancel=true;

        //Example:
        // SIZE: 215710 DOWNLOADED: 3973 SPEED: 233 KB/s
        // ^0    ^1     ^2          ^3   ^4     ^5  ^6

        QStringList dl_list = line.split(" ");
        progress.mProgressMax= dl_list[1].toInt();
        progress.mProgressCurr= dl_list[3].toInt();
        QString speed= dl_list[5] + QString(" ") + dl_list[6];
        long size= dl_list[3].toInt() * 1024;
        long downloaded= dl_list[1].toInt() * 1024;
        progress.mMessage= tr("[%1/%2] Downloading update for %3 (%4/%5 at %6)").arg(QString::number(progress.mItemNo+1),
                                                                             QString::number(progress.mItemsCount),
                                                                             current_app,
                                                                             pcbsd::Utils::bytesToHumanReadable(size),
                                                                             pcbsd::Utils::bytesToHumanReadable(downloaded),
                                                                             speed);
        log_message.clear();
        reportProgress(progress);
        return;
    }
    else
    {
        if (line == FETCHDONE)
        {
            //indicates beginning of instalation
            log_message.clear();
            misWasFETCHDONE= true;
        }

        if (line.indexOf(FETCH_WORLD) == 0)
        {
            log_message = log_message.replace(FETCH_WORLD, "Downloading");
        }

        if (misWasFETCHDONE || (line == FETCHDONE))
        {
            //installing...
            progress.mMessage=tr("[%1/%2] Installing update for %3").arg(QString::number(progress.mItemNo+1),
                                                                         QString::number(progress.mItemsCount),
                                                                         current_app);
            progress.mSubstate= eInstall;
        }
        else
        {
            progress.mMessage=tr("[%1/%2] Preparing to install update for %3").arg(QString::number(progress.mItemNo+1),
                                                                         QString::number(progress.mItemsCount),
                                                                         current_app);
        }
        if (log_message.length())
            reportLogLine(log_message);
        reportProgress(progress);
    }
}

///////////////////////////////////////////////////////////////////////////////
void CPBIController::onCheckProcessfinished(int exitCode)
{
    if (!mvUpdates.size())
    {
        if (!exitCode)
            setCurrentState(eFULLY_UPDATED);
        else
            reportError(tr("Error while software update check"));
        return;
    }
    else
    {
        if (mvUpdates.size() == 1)
        {
            reportUpdatesAvail(tr("Available update for: %1").arg(mvUpdates[0].mName));
        }
        reportUpdatesAvail(tr("Updates available for %1 applications").arg(mvUpdates.size()));
    }
}

///////////////////////////////////////////////////////////////////////////////
void CPBIController::onUpdateProcessfinished(int exitCode)
{
    SProgress progress;

    Q_UNUSED(exitCode); // TODO: more accurate error check

    QString current_app = mAppsToUpdate[mCurrentUpdate];
    current_app= current_app.left(current_app.lastIndexOf("-")); // remove arch
    current_app= current_app.left(current_app.lastIndexOf("-")); // remove ver
    progress.mItemNo= mCurrentUpdate;
    progress.mItemsCount= mAppsToUpdate.size();

    progress.mMessage=tr("[%1/%2] Preparing to install update for %3").arg(QString::number(progress.mItemNo+1),
                                                                 QString::number(progress.mItemsCount),
                                                                 current_app);


    if (mCurrentUpdate < (mAppsToUpdate.size() - 1))
    {
        SProgress progress;
        progress.mLogMessages= QStringList()<<"===============================";
    }

    mCurrentUpdate++;

    if (mCurrentUpdate >= mAppsToUpdate.size())
    {
        mCurrentUpdate=0;
        check();
    }
    else
    {
        reportProgress(progress);
        launchUpdate();
    }
}

///////////////////////////////////////////////////////////////////////////////
void CPBIController::updateShellCommand(QString &cmd, QStringList &args)
{
    cmd= PBU_UPDATE_CMD;
    args= QStringList()<<mAppsToUpdate[mCurrentUpdate];

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PBI_FETCH_PARSING","YES"); //For readable download notifications
    process().setProcessEnvironment(env);
}

