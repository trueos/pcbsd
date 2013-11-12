#include "syscontroller.h"

#include <QDebug>

static const QString UPDATE_MANAGER = "pc-updatemanager";
#define UM_CHECK_PARAMS "check"

CSysController::CSysController()
{

}

void CSysController::onReadCheckLine(QString line)
{
    if (line == QString("Your system is up to date!"))
    {
        setCurrentState(eFULLY_UPDATED);
    }

    qDebug()<<line;

}

void CSysController::onReadUpdateLine(QString line)
{

}
