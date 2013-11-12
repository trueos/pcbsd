#include "pkgcontroller.h"

#include <QDebug>

static const char* const UPDATES_AVAIL_STRING = "Upgrades have been requested for the following";
static const char* const UPDATES_AVAIL_SIZE_STRING = "The upgrade will require ";
static const char* const UPDATES_AVAIL_SIZE_FREE_STR= "The upgrade will free ";
static const char* const UPDATES_AVAIL_DL_SIZE_STRING = " to be downloaded";
static const char* const UPDATES_AVAIL_END_STRING = "To start the upgrade run";

static const char* const INSTALLING  = "Installing";
static const char* const UPGRADING   = "Upgrading";
static const char* const REINSTALLING = "Reinstalling";

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

void CPkgController::onReadUpdateLine(QString line)
{
    Q_UNUSED(line);
}
