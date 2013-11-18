#include "syscontroller.h"
#include "utils.h"
#include <QDebug>

_STRING_CONSTANT PC_UPDATE_COMMAND = "pc-updatemanager";
_STRING_CONSTANT FBSD_UPDATE_COMMAND = "pc-fbsdupdatecheck";
static const QStringList PC_UPDATE_ARGS(QStringList()<<"check");
static const QStringList FBSD_UPDATE_ARGS;

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


CSysController::CSysController()
{
    misFREEBSDCheck= false;
}

void CSysController::onCheckUpdates()
{
    misFREEBSDCheck= false;
    mvUpdates.clear();
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

void CSysController::onReadCheckLine(QString line)
{    
    if (misFREEBSDCheck)
        parseCheckFREEBSDLine(line);
    else
        parseCheckPCBSDLine(line);

}

void CSysController::onReadUpdateLine(QString line)
{

}

void CSysController::onCheckProcessfinished(int exitCode)
{
    if (!misFREEBSDCheck)
    {
        qDebug()<<"PC updates check proc finished!";
        misFREEBSDCheck= true;
        launchCheck();
    }
    else
    {
        qDebug()<<"Sys updates check done!";
        if (mvUpdates.size())
        {
            setCurrentState(eUPDATES_AVAIL);
            return;
        }
        else
        {
            setCurrentState(eFULLY_UPDATED);
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
}
