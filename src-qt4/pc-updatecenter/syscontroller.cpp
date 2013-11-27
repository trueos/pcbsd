#include "syscontroller.h"
#include "utils.h"
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

}

void CSysController::parseUpgradeLine(QString line)
{

}

void CSysController::parseFreeBSDUpdateLine(QString line)
{

}
