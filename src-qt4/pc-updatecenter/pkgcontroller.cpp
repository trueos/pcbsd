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

#include "pkgcontroller.h"

#include "pcbsd-utils.h"
#include "utils.h"

#include <QDebug>
#include <QFile>
#include <QRegExp>

__string_constant FULLY_UPDATED_MESSAGE = "All packages are up to date!";
__string_constant UPDATES_AVAIL_STRING = "Upgrades have been requested for the following";
__string_constant UPDATES_AVAIL_SIZE_STRING = "The upgrade will require ";
__string_constant UPDATES_AVAIL_SIZE_FREE_STR= "The upgrade will free ";
__string_constant UPDATES_AVAIL_DL_SIZE_STRING = " to be downloaded";
__string_constant UPDATES_AVAIL_END_STRING = "To start the upgrade run";

__string_constant INSTALLING  = "Installing";
__string_constant UPGRADING   = "Upgrading";
__string_constant REINSTALLING = "Reinstalling";

__string_constant FETCH = "FETCH:";
__string_constant FETCH_DONE = "FETCHDONE";
__string_constant SIZE_DL_MARKER = "SIZE:";
__string_constant DOWNLOADED_DL_MARKER = "DOWNLOADED:";
__string_constant DL_FETCH_START = "FETCH:";
__string_constant DL_RESUME_DOWNLOAD= "Resuming download of:";
__string_constant PKG_CONFLICTS_LIST= "PKGCONFLICTS: ";
__string_constant PKG_CONFLICTS_REPLY= "PKGREPLY: ";
__string_constant PKG_INSTALL_START_MARKER= "Upgrades have been requested for the following";
__string_constant PKG_INSTALL_DONE = "... done";

__string_constant PKG_NETWORK_ERROR = ": No address record";
__string_constant PKG_COMMON_ERROR = "ERROR: Error";
typedef struct
{
    QString mDEName;
    QStringList mPkgSet;
}SDesktopCriticalPkgs;

//============== This is list of packages that should cause logaut from desktop environment
//TODO: Add MATE, XFCE. Check KDE and LXDE

const SDesktopCriticalPkgs DESKTOP_CRITICAL_PKG[]={
{QString("kde"), QStringList()<<"kde-runtime*"<<"kde-workspace*"<<"kdebindings*"<<"kdelibs*"<<"plasma-scriptengine*"},
{QString("lxde"), QStringList()<<"lxpanel*"<<"lxde-meta*"<<"lxsession*"<<"lxde-common*"},
};

const int DESKTOP_CRITICAL_PKG_COUNT = sizeof(DESKTOP_CRITICAL_PKG) / sizeof(SDesktopCriticalPkgs);

typedef enum{
    eCommonInfo,
    ePkgList,

}ECheckClState;

///////////////////////////////////////////////////////////////////////////////
static signed long sizeToLong(QString size_with_units)
{
    size_with_units = size_with_units.trimmed();

    signed long space = size_with_units.split(" ")[0].toInt();
    QString units = size_with_units.split(" ")[1].toLower().trimmed();
    if (units == QString("kb"))
    {
        space*= 1024;
    }
    else if (units == QString("mb"))
    {
        space*= 1024*1024;
    }
    else if (units == QString("gb"))
    {
        space*= 1024*1024*1024;
    }

    return space;
}

///////////////////////////////////////////////////////////////////////////////
CPkgController::CPkgController()
{
    /*QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PCFETCHGUI","YES"); //For readable download notifications
    process().setProcessEnvironment(env);*/
    process().setDLType("PKG");
    mCurrentDEIndex=-1;

    // Get current DE
    pcbsd::DesktopEnvironmentInfo currDE = pcbsd::Utils::currentDesktop();
    if (currDE.Name.length())
    {
        for(int i=0; i<DESKTOP_CRITICAL_PKG_COUNT; i++)
        {
            if (DESKTOP_CRITICAL_PKG[i].mDEName.toLower().trimmed() == currDE.Name.trimmed().toLower())
            {
                mCurrentDEIndex = i;
                qDebug()<<currDE.Name;
                break;
            }
        }//for all DESKTOP_CRITICAL_PKG
    }// if detected DE

    misLogoffRequired= false;    

}

///////////////////////////////////////////////////////////////////////////////
CPkgController::SUpdate CPkgController::updateData()
{
    return mUpdData;
}

///////////////////////////////////////////////////////////////////////////////
bool CPkgController::logoffRequired()
{
    return (currentState() != eUPDATING) && misLogoffRequired;
}

///////////////////////////////////////////////////////////////////////////////
void CPkgController::onReadCheckLine(QString line)
{
    static ECheckClState curChkrState = eCommonInfo;
    line = line.trimmed();

    if (line.contains(PKG_NETWORK_ERROR))
    {
        reportError(tr("Error during update check. Check network connection"));
        return;
    }
    if (line.indexOf(PKG_COMMON_ERROR) == 0)
    {
        line = line.replace(PKG_COMMON_ERROR,"");
        reportError(tr("Error checking updates. Error code: %1").arg(line.left(line.indexOf(":"))));
        return;
    }
    if ( line.contains(FULLY_UPDATED_MESSAGE))
    {
        setCurrentState(eFULLY_UPDATED);
        return;
    }
    if ( line.indexOf(UPDATES_AVAIL_STRING) == 0)
    {
        line = line.replace(UPDATES_AVAIL_STRING, "");
        mUpdData = SUpdate();
        mUpdData.mvPkgUpdates.clear();
        mUpdData.mCommonPkgsCount = line.trimmed().split(" ")[0].toInt();
        curChkrState= ePkgList;
        return;
    }

    if(line.indexOf(UPDATES_AVAIL_END_STRING) == 0)
    {
        QString ReportString = QString(tr("Updates available for %1 packages")).arg(QString::number(mUpdData.mCommonPkgsCount));
        reportUpdatesAvail(ReportString);
        return;
    }

    if (line.indexOf(UPDATES_AVAIL_SIZE_STRING) == 0)
    {
        line= line.replace(UPDATES_AVAIL_SIZE_STRING, "");
         mUpdData.mDiskSpace= sizeToLong(line);
         return;
    }
    if (line.indexOf(UPDATES_AVAIL_SIZE_FREE_STR) == 0 )
    {
        line= line.replace(UPDATES_AVAIL_SIZE_FREE_STR, "");
        mUpdData.mDiskSpace= 0 - sizeToLong(line);
        return;
    }
    if (line.indexOf(UPDATES_AVAIL_DL_SIZE_STRING) > 0 )
    {
        mUpdData.mDownloadSize = sizeToLong(line);
        return;
    }

    QStringList line_list = line.split(" ");
    QString name, old_ver, new_ver, reason;
    EPkgAction action= eUnknown;

    if ( line_list[0] == INSTALLING )
    {
            // Example:
            // Installing tinyxml: 2.6.2_1
            // ^0         ^1       ^2

            action= eInstalling;
            name= line_list[1];
            name= name.replace(":","");
            new_ver= line_list[2];
            mUpdData.mInstallCount++;

        }
        else if (line_list[0] == UPGRADING)
        {
            // Example:
            // Upgrading curl: 7.32.0 -> 7.33.0
            // ^0        ^1    ^2     ^3 ^4

            action= eUpgrading;
            name= line_list[1];
            name= name.replace(":","");
            old_ver= line_list[2];
            new_ver= line_list[4];
            mUpdData.mUpgradeCount++;
        }
        else if (line_list[0] == REINSTALLING)
        {
            //Example:
            // Reinstalling doxygen-1.8.3.1_1 (direct dependency changed)
            // ^0           ^1                ^...

            line= line.replace(REINSTALLING, "").trimmed();
            int space_pos= line.indexOf(" ");
            if (space_pos<1)
                return;
            name= line.left(space_pos);
            reason= line.right(line.length() - space_pos - 1);
            int ver_div_idx= name.lastIndexOf("-");
            old_ver= name.right(name.length() - ver_div_idx -1 );
            name= name.left(ver_div_idx);

            action= eReinstalling;
            mUpdData.mReinstallCount++;

        }// reinstalling parsing

        if (action != eUnknown)
        {
            SPackageUpdate entry;
            entry.mAction= action;
            entry.mPackageName= name;
            entry.mNewVersion= new_ver;
            entry.mOldVersion= old_ver;
            entry.mReinstallReason= reason;
            mUpdData.mvPkgUpdates.push_back(entry);
        }   
}

///////////////////////////////////////////////////////////////////////////////
void CPkgController::onUpdateAll()
{
    misDownloadComplete= false;
    mCurrentPkgNo= 0;
    misInFetch= 0;
}

///////////////////////////////////////////////////////////////////////////////
void CPkgController::onReadUpdateLine(QString line)
{
    static QString last_message;
    //qDebug()<<line;
    line= line.trimmed();

    if (!line.length())
        return;

    SProgress progress;
    QString log_line;
    long dl_size;
    long dl_complete;
    QString dl_speed;

    progress.mItemNo = mCurrentPkgNo;
    progress.mItemsCount = mUpdData.mCommonPkgsCount;
    progress.mSubstate= (misDownloadComplete)?eInstall:eDownload;
    log_line= line;

    if (line.indexOf(DL_FETCH_START) == 0)
    {
        misWasInstalation= false;
        //starting fetch
        mCurrentPkgNo++;
        log_line.clear();
        misInFetch++;
        //Example:
        // Resuming download of: /usr/local/tmp/All/pcbsd-base-1382021797.txz
        //
        mCurrentPkgName= line.right(line.size() - line.lastIndexOf("/") - 1); // get package file name (ex: pcbsd-base-1382021797.txz)
        mCurrentPkgName= mCurrentPkgName.left(mCurrentPkgName.lastIndexOf("-")); // get packagename without version
        progress.misCanCancel= true;
        progress.mMessage= tr("[%1/%2] Downloading %3").arg(QString::number(progress.mItemNo+1),
                                                            QString::number(progress.mItemsCount),
                                                            mCurrentPkgName);
        if (!mLastLine.contains(DL_RESUME_DOWNLOAD))
        {
            log_line= QString("Downloading: ")+line.replace(FETCH, "");
        }

    }
    if(line == FETCH_DONE)
    {
        log_line.clear();
        misInFetch--;
        return;
    }
    else
    if (parseFetchOutput(line, dl_size, dl_complete, dl_speed))
    {
        log_line.clear();
        progress.misCanCancel= true;
        progress.mProgressMax= dl_size;
        progress.mProgressCurr= dl_complete;
        progress.mMessage= tr("[%1/%2] Downloading %3 (%4/%5 at %6)").arg(QString::number(progress.mItemNo+1),
                                                                                  QString::number(progress.mItemsCount),
                                                                                  mCurrentPkgName,
                                                                                  pcbsd::Utils::bytesToHumanReadable(dl_complete),
                                                                                  pcbsd::Utils::bytesToHumanReadable(dl_size),
                                                                                  dl_speed);
    }
    else
    if (line.indexOf(PKG_CONFLICTS_LIST) == 0)
    {
        //----------- package conflict
        mConflictList= line.replace(PKG_CONFLICTS_LIST, "");        
        return;
    }
    else
    if (line.indexOf(PKG_CONFLICTS_REPLY) == 0)
    {
        reportLogLine(QString("ERROR: Package upgrade conflict for packages: ") + mConflictList);

        reportError(tr("Package conflict: ") + mConflictList);
         mConflictsReply= line.replace(PKG_CONFLICTS_REPLY, "");
        emit packageConflict(mConflictList);
        return;
    }

    if (line.indexOf(PKG_INSTALL_START_MARKER) == 0)
    {
        mCurrentPkgNo=0;
        misDownloadComplete= true;
    }
    else
    if (line.contains(UPDATES_AVAIL_DL_SIZE_STRING))
    {
        // Checking integrity

    }
    else
    if ((line[0] == '[') && misDownloadComplete)
    {
        //Example:
        // [2/717] Reinstalling a2ps-4.13b_5... done
        // ^0      ^1           ^2

        QStringList line_list = line.split(" ");
        QString state;
        if (line_list[1] == INSTALLING)
            state= tr("Installed");
        if (line_list[1] == UPGRADING)
            state= tr("Upgraded");
        if (line_list[1] == REINSTALLING)
            state= tr("Reinstalled");
        QString pkg_name = line_list[2].replace(PKG_INSTALL_DONE, "");
        pkg_name= pkg_name.left(pkg_name.lastIndexOf("-"));

        QString msg = line_list[0] + QString(" ") + state + QString(" ") + pkg_name;
        progress.mMessage = msg;

        // Check for lofoff required packages
        if (mCurrentDEIndex>=0)
        {
            QRegExp rx;
            rx.setPatternSyntax(QRegExp::WildcardUnix);
            for (int i=0; i<DESKTOP_CRITICAL_PKG[mCurrentDEIndex].mPkgSet.size(); i++)
            {
                rx.setPattern(DESKTOP_CRITICAL_PKG[mCurrentDEIndex].mPkgSet[i]);
                if (rx.exactMatch(pkg_name))
                {
                    misLogoffRequired= true;
                    qDebug()<<"Logoff required!";
                    break;
                }//if match
            }//for all wildcard
        }

        misWasInstalation= true;
        last_message= msg;
    }
    else
    if (misDownloadComplete && misWasInstalation)
    {
        progress.mMessage = last_message;
    }

    mLastLine = line;

    reportProgress(progress);
    if(log_line.length())
        reportLogLine(log_line);
}

///////////////////////////////////////////////////////////////////////////////
void CPkgController::onDownloadUpdatePercent(QString percent, QString size, QString other)
{
    SProgress progress;

    static QString last_name;

    //I can't catch pkg sownload finish event :( I just check file name
    if (last_name != other)
    {
        mCurrentPkgNo++;
        last_name= other;
    }

    progress.mItemNo = mCurrentPkgNo;
    progress.mItemsCount = mUpdData.mCommonPkgsCount;
    QString No = (mCurrentPkgNo<=(int)mUpdData.mCommonPkgsCount)?QString(tr("[%1 of %2] ").arg(mCurrentPkgNo).arg(progress.mItemsCount)):
                                                                 QString("[%1] ").arg(mCurrentPkgNo);

    size= size.left(size.indexOf(" "));
    progress.mMessage = No + tr("Downloading ") + other;
    progress.mProgressMax = (int)(size.toFloat()*100);
    progress.mProgressCurr = (int)((float)(progress.mProgressMax) * percent.toFloat() / 100.);
    progress.mSubstate = eDownload;
    progress.misCanCancel = true;
    reportProgress(progress);
}

///////////////////////////////////////////////////////////////////////////////
void CPkgController::onUpdateProcessfinished(int exitCode)
{
    if (!misWasInstalation || exitCode)
    {
        reportError(tr("Installation process was unxepected terminated"));
    }
    else
        check();
}

///////////////////////////////////////////////////////////////////////////////
void CPkgController::onCancel()
{
    process().terminate();
    process().waitForFinished();
    check();
}

///////////////////////////////////////////////////////////////////////////////
void CPkgController::onCheckProcessfinished(int exitCode)
{
    if (exitCode)
    {
        qDebug()<<"Unable to check pkg updates!";
        reportError(tr("Update check error"));
    }
}

///////////////////////////////////////////////////////////////////////////////
void CPkgController::autoResolveConflict(bool isAutoResolve)
{
    QString answer = (isAutoResolve)?"yes":"no";

    QFile pkgReply( mConflictsReply );
    if ( pkgReply.open( QIODevice::WriteOnly ) )
    {
       QTextStream streamReply( &pkgReply );
       streamReply << answer;
       pkgReply.close();
    }
    if (!isAutoResolve)
    {
        //process().terminate();
    }
}
