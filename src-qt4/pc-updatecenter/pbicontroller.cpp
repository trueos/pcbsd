#include "pbicontroller.h"

#include <QDebug>

static const char* const AVAIL= "Available:";
static const char* const DOWNLOAD_INDICATOR = "DOWNLOADED:";
static const char* const FETCHDONE="FETCHDONE";

CPBIController::CPBIController()
{
}

void CPBIController::updateSelected(QStringList generic_names_list)
{
    mAppsToUpdate= generic_names_list;
    mCurrentUpdate=0;
    misWasFETCHDONE= false;
    launchUpdate();
}

void CPBIController::onCheckUpdates()
{
    mvUpdates.clear();
}

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

void CPBIController::onReadUpdateLine(QString line)
{
    SProgress progress;
    line= line.trimmed();
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
        // ^0    ^1     ^2          ^3   ^4     ^5

        QStringList dl_list = line.split(" ");
        progress.mProgressMax= dl_list[1].toInt();
        progress.mProgressCurr= dl_list[3].toInt();
        QString speed= dl_list[5];
        progress.mMessage= tr("[%1/%2] Downloading update for %3 at %4").arg(QString::number(progress.mItemNo),
                                                                             QString::number(progress.mItemsCount),
                                                                             current_app, speed);
        reportProgress(progress);
        return;
    }
    else
    {
        if (line == FETCHDONE)
        {
            //indicates beginning of instalation
            misWasFETCHDONE= true;
        }

        if (misWasFETCHDONE || (line == FETCHDONE))
        {
            //installing...
            progress.mMessage=tr("[%1/%2] Installing update for %3").arg(QString::number(progress.mItemNo),
                                                                         QString::number(progress.mItemsCount),
                                                                         current_app);
            progress.mSubstate= eInstall;
        }
        else
        {
            progress.mMessage=tr("[%1/%2] Preparing to install update for %3").arg(QString::number(progress.mItemNo),
                                                                         QString::number(progress.mItemsCount),
                                                                         current_app);
        }
    }
}

void CPBIController::onCheckProcessfinished(int exitCode)
{
    if (!mvUpdates.size())
    {
        setCurrentState(eFULLY_UPDATED);
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

void CPBIController::onUpdateProcessfinished(int exitCode)
{

}

void CPBIController::updateShellCommand(QString &cmd, QStringList &args)
{

}

