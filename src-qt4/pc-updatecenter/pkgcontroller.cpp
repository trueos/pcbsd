#include "pkgcontroller.h"

#include "pcbsd-utils.h"

#include <QDebug>
#include <QFile>

static const char* const UPDATES_AVAIL_STRING = "Upgrades have been requested for the following";
static const char* const UPDATES_AVAIL_SIZE_STRING = "The upgrade will require ";
static const char* const UPDATES_AVAIL_SIZE_FREE_STR= "The upgrade will free ";
static const char* const UPDATES_AVAIL_DL_SIZE_STRING = " to be downloaded";
static const char* const UPDATES_AVAIL_END_STRING = "To start the upgrade run";

static const char* const INSTALLING  = "Installing";
static const char* const UPGRADING   = "Upgrading";
static const char* const REINSTALLING = "Reinstalling";

static const char* const FETCH = "FETCH:";
static const char* const FETCH_DONE = "FETCHDONE";
static const char* const SIZE_DL_MARKER = "SIZE:";
static const char* const DOWNLOADED_DL_MARKER = "DOWNLOADED:";
static const char* const DL_FETCH_START = "FETCH:";
static const char* const DL_RESUME_DOWNLOAD= "Resuming download of:";
static const char* const PKG_CONFLICTS_LIST= "PKGCONFLICTS: ";
static const char* const PKG_CONFLICTS_REPLY= "PKGREPLY: ";

typedef enum{
    eCommonInfo,
    ePkgList,

}ECheckClState;

static long long sizeToLong(QString size_with_units)
{
    size_with_units = size_with_units.trimmed();

    long long space = size_with_units.split(" ")[0].toInt();
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

CPkgController::CPkgController()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PCFETCHGUI","YES"); //For readable download notifications
    process().setProcessEnvironment(env);
}

CPkgController::SUpdate CPkgController::updateData()
{
    return mUpdData;
}

void CPkgController::onReadCheckLine(QString line)
{
    static ECheckClState curChkrState = eCommonInfo;
    line = line.trimmed();
    static int emty_lines=0;

    if (eCommonInfo == curChkrState)
    {
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
            line= line.replace(UPDATES_AVAIL_SIZE_STRING, "");
            mUpdData.mDiskSpace= 0 - sizeToLong(line);
            return;
        }
        if (line.indexOf(UPDATES_AVAIL_DL_SIZE_STRING) > 0 )
        {
            mUpdData.mDownloadSize = sizeToLong(line);
            return;
        }
    }//if we read common update data
    else
    {
        EPkgAction action = eUnknown;
        if (!line.length())
        {
            if (++emty_lines >= 2)
            {
                curChkrState= eCommonInfo;
                emty_lines = 0;
            }
            return;
        }

        QStringList line_list = line.split(" ");
        QString name, old_ver, new_ver, reason;

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
    }// if inside packages list
}

void CPkgController::onUpdateAll()
{
    misDownloadComplete= false;
    mCurrentPkgNo= 0;
    misInFetch= 0;
}

void CPkgController::onReadUpdateLine(QString line)
{
    qDebug()<<line;
    line= line.trimmed();

    if (!line.length())
        return;

    SProgress progress;
    progress.mItemNo = mCurrentPkgNo;
    progress.mItemsCount = mUpdData.mCommonPkgsCount;
    progress.mSubstate= (misDownloadComplete)?eInstall:eDownload;
    progress.mLogMessages= QStringList()<<line;

    if (line.indexOf(DL_FETCH_START) == 0)
    {
        //starting fetch
        mCurrentPkgNo++;
        progress.mLogMessages.clear();
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
            progress.mLogMessages= QStringList()<<(QString("Downloading: ")+line.replace(FETCH, ""));

    }
    if(line == FETCH_DONE)
    {
        progress.mLogMessages.clear();
        misInFetch--;
        return;
    }
    else
    if ( line.contains(SIZE_DL_MARKER) && line.contains(DOWNLOADED_DL_MARKER))
    {
        //downloading progress parsing
        progress.mLogMessages.clear();
        progress.misCanCancel= true;

        //Example:
        // SIZE: 215710 DOWNLOADED: 3973 SPEED: 233 KB/s
        // ^0    ^1     ^2          ^3   ^4     ^5  ^6

        //TODO: as eparate function (code duplication)
        QStringList dl_list = line.split(" ");
        progress.mProgressMax= dl_list[1].toInt();
        progress.mProgressCurr= dl_list[3].toInt();
        QString speed= dl_list[5] + QString(" ") + dl_list[6];
        long size= dl_list[3].toInt() * 1024;
        long downloaded= dl_list[1].toInt() * 1024;

        progress.mMessage= tr("[%1/%2] Downloading %3 (%4/%5 at %6)").arg(QString::number(progress.mItemNo+1),
                                                                                  QString::number(progress.mItemsCount),
                                                                                  mCurrentPkgName,
                                                                                  pcbsd::Utils::bytesToHumanReadable(size),
                                                                                  pcbsd::Utils::bytesToHumanReadable(downloaded),
                                                                                  speed);
    }
    else
    if (line.indexOf(PKG_CONFLICTS_LIST) == 0)
    {
        //----------- package conflict
        mConflictList= line.replace(PKG_CONFLICTS_LIST, "");
        progress.misCanCancel= true;
        progress.mLogMessages= QStringList()<<QString("ERROR: Package upgrade conflict for packages: ")<<mConflictList;
        reportProgress(progress);

        reportError(tr("Package conflict: ") + mConflictList);
        return;
    }
    else
    if (line.indexOf(PKG_CONFLICTS_REPLY))
    {
        mConflictsReply= line.replace(PKG_CONFLICTS_REPLY, "");
        emit packageConflict(mConflictList);
        return;
    }

    mLastLine = line;

    reportProgress(progress);
}

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
