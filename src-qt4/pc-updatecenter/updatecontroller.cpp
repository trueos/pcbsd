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

#include "updatecontroller.h"

#include <QDebug>
#include <unistd.h>

const char* const EMULATION_PROC = "cat";
const QStringList EMULATION_DEF_ARGS;

///////////////////////////////////////////////////////////////////////////////
CAbstractUpdateController::CAbstractUpdateController()
{
    mCurrentState= eNOT_INITIALIZED;
    mUpdProc.setProcessChannelMode(QProcess::MergedChannels);
    connect(&mUpdProc, SIGNAL(readyReadStandardOutput()),
            this, SLOT(slotProcessRead()));
    connect(&mUpdProc, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(slotProcessFinished(int,QProcess::ExitStatus)));
}

///////////////////////////////////////////////////////////////////////////////
void CAbstractUpdateController::setJailPrefix(QString prefix)
{
    mJailPrefix= prefix;
}

///////////////////////////////////////////////////////////////////////////////
void CAbstractUpdateController::removeJailPrefix()
{
    mJailPrefix.clear();
}

///////////////////////////////////////////////////////////////////////////////
CAbstractUpdateController::EUpdateControllerState CAbstractUpdateController::currentState()
{
    return mCurrentState;
}

///////////////////////////////////////////////////////////////////////////////
QString CAbstractUpdateController::updateMessage()
{
    return mUpdateMasage;
}

///////////////////////////////////////////////////////////////////////////////
QStringList CAbstractUpdateController::updateLog()
{
    return mLogMessages;
}

///////////////////////////////////////////////////////////////////////////////
bool CAbstractUpdateController::hasLog()
{
    return (mLogMessages.size()!=0);
}

///////////////////////////////////////////////////////////////////////////////
void CAbstractUpdateController::parseProcessLine(CAbstractUpdateController::EUpdateControllerState state, QString line)
{
    switch (state)
    {
        case eCHECKING:
            onReadCheckLine(line);
            break;
        case eUPDATING:
            onReadUpdateLine(line);
            break;
        case eNOT_INITIALIZED:
            qDebug()<<"Updater internal state error";
            break;
        default:
            break;
    }
}

///////////////////////////////////////////////////////////////////////////////
void CAbstractUpdateController::setCurrentState(CAbstractUpdateController::EUpdateControllerState new_state)
{


    //Set default start values for update progress
    if((new_state == eUPDATING) && (mCurrentState!=eUPDATING))
    {
        mUpdateMasage = tr("Starting update...");
        mCurrentProgress = SProgress();
        mLogMessages.clear();
    }

    mCurrentState= new_state;

    emit stateChanged(mCurrentState);
}

///////////////////////////////////////////////////////////////////////////////
void CAbstractUpdateController::reportProgress(CAbstractUpdateController::SProgress curr_progress)
{
    mCurrentProgress= curr_progress;
    emit progress(mCurrentProgress);
}

///////////////////////////////////////////////////////////////////////////////
void CAbstractUpdateController::reportLogLine(QString line)
{
    mLogMessages.append(line);
    emit logLinePresent(line);
}

///////////////////////////////////////////////////////////////////////////////
void CAbstractUpdateController::reportUpdatesAvail(QString message)
{
    mUpdateMasage = message;
    setCurrentState(eUPDATES_AVAIL);
    emit updatesAvail(mUpdateMasage);
}

///////////////////////////////////////////////////////////////////////////////
void CAbstractUpdateController::reportError(QString error_message)
{
    mErrorMessage = error_message;
    setCurrentState(eUPDATING_ERROR);
    emit updateError(mErrorMessage);
}

///////////////////////////////////////////////////////////////////////////////
void CAbstractUpdateController::launchUpdate()
{
    QString proc;
    QStringList args;
    updateShellCommand(proc, args);

    //if jail is present
    if (mJailPrefix.length())
    {
        args.push_front(proc);
        args.push_front(mJailPrefix);
        proc = "chroot";
    }

    mUpdProc.start(proc,args);
    if (!mUpdProc.waitForStarted())
    {
        reportError(tr("Can not execute update shell command"));
        return;
    }

    if (currentState() != eUPDATING)
        setCurrentState(eUPDATING);
}

///////////////////////////////////////////////////////////////////////////////
void CAbstractUpdateController::launchCheck()
{
    QString proc;
    QStringList args;
    checkShellCommand(proc, args);

    //if jail is present
    if (mJailPrefix.length())
    {
        args.push_front(proc);
        args.push_front(mJailPrefix);
        proc = "chroot";
    }

    mUpdProc.start(proc,args);
    if (!mUpdProc.waitForStarted())
    {
        reportError(tr("Can not execute update shell command"));
        return;
    }

    setCurrentState(eCHECKING);
}

///////////////////////////////////////////////////////////////////////////////
void CAbstractUpdateController::check()
{
    onCheckUpdates();
    launchCheck();
}

///////////////////////////////////////////////////////////////////////////////
void CAbstractUpdateController::updateAll()
{
    //TODO: correct current state check

    onUpdateAll();

    launchUpdate();
}

///////////////////////////////////////////////////////////////////////////////
void CAbstractUpdateController::cancel()
{
    if (mCurrentProgress.misCanCancel)
    {
        onCancel();
    }
}

///////////////////////////////////////////////////////////////////////////////
void CAbstractUpdateController::slotProcessRead()
{
    /*qint64 size= mUpdProc.bytesAvailable();
    for(qint64 i=0; i<size; i++)
    {
        char ch;
        mUpdProc.getChar(&ch);
        mUpdProc.putChar(ch);
        onReadProcessChar(ch);
    }*/

    while (mUpdProc.canReadLine())
    {
        parseProcessLine(currentState(), mUpdProc.readLine().simplified());
    }
}

///////////////////////////////////////////////////////////////////////////////
void CAbstractUpdateController::slotProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus)

    switch (currentState())
    {
        case eCHECKING:
            onCheckProcessfinished(exitCode);
            break;
        case eUPDATING:
            onUpdateProcessfinished(exitCode);
            break;
        default:
            break;
    }
}
