#include "pbicontroller.h"

#include <QDebug>

static const char* const AVAIL= "Available:";

CPBIController::CPBIController()
{
}

void CPBIController::onCheckUpdates()
{
    mvUpdates.clear();
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

