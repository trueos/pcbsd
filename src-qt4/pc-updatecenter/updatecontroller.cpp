#include "updatecontroller.h"

#include <QDebug>
#include <unistd.h>

const char* const EMULATION_PROC = "cat";
const QStringList EMULATION_DEF_ARGS;

CAbstractUpdateController::CAbstractUpdateController()
{
#ifdef CONTROLLER_EMULATION_ENABLED
    mEmulationDelay= 0;
#endif
    mCurrentState= eNOT_INITIALIZED;
    mUpdProc.setProcessChannelMode(QProcess::MergedChannels);
    connect(&mUpdProc, SIGNAL(readyReadStandardOutput()),
            this, SLOT(slotProcessRead()));
    connect(&mUpdProc, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(slotProcessFinished(int,QProcess::ExitStatus)));
}

CAbstractUpdateController::EUpdateControllerState CAbstractUpdateController::currentState()
{
    return mCurrentState;
}

QString CAbstractUpdateController::updateMessage()
{
    return mUpdateMasage;
}

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
    }
}

void CAbstractUpdateController::setCurrentState(CAbstractUpdateController::EUpdateControllerState new_state)
{
    mCurrentState= new_state;

    //Set default start values for update progress
    if(new_state == eUPDATING)
    {
        mUpdateMasage = tr("Starting update...");
        mCurrentProgress = SProgress();
    }

    emit stateChanged(mCurrentState);
}

void CAbstractUpdateController::reportProgress(CAbstractUpdateController::SProgress curr_progress)
{
    mCurrentProgress= curr_progress;
    emit progress(mCurrentProgress);
}

void CAbstractUpdateController::reportUpdatesAvail(QString message)
{
    mUpdateMasage = message;
    setCurrentState(eUPDATES_AVAIL);
    emit updatesAvail(mUpdateMasage);
}

void CAbstractUpdateController::reportError(QString error_message)
{
    mErrorMessage = error_message;
    setCurrentState(eUPDATING_ERROR);
    emit updateError(mErrorMessage);
}

void CAbstractUpdateController::launchUpdate()
{
    QString proc;
    QStringList args;
    updateShellCommand(proc, args);
#ifdef CONTROLLER_EMULATION_ENABLED
    if (mEmulateUpd.length())
    {
        proc = EMULATION_PROC;
        args = EMULATION_DEF_ARGS;
        args<<mEmulateUpd;
    }
#endif
    mUpdProc.start(proc,args);
    if (!mUpdProc.waitForStarted())
    {
        reportError(tr("Can not execute update shell command"));
        return;
    }

    setCurrentState(eUPDATING);
}

void CAbstractUpdateController::launchCheck()
{
    QString proc;
    QStringList args;
    checkShellCommand(proc, args);
#ifdef CONTROLLER_EMULATION_ENABLED
    if (mEmulateCheck.length())
    {
        proc = EMULATION_PROC;
        args = EMULATION_DEF_ARGS;
        args<<mEmulateCheck;
    }
#endif
    mUpdProc.start(proc,args);
    if (!mUpdProc.waitForStarted())
    {
        reportError(tr("Can not execute update shell command"));
        return;
    }

    setCurrentState(eCHECKING);
}

void CAbstractUpdateController::check()
{
    onCheckUpdates();
    launchCheck();
}

void CAbstractUpdateController::updateAll()
{
    //TODO: correct current state check

    onUpdateAll();

    launchUpdate();
}

void CAbstractUpdateController::cancel()
{
    if (mCurrentProgress.misCanCancel)
    {
        onCancel();
    }
}

void CAbstractUpdateController::slotProcessRead()
{
    while (mUpdProc.canReadLine())
    {
#ifdef CONTROLLER_EMULATION_ENABLED
        if (mEmulationDelay)
        {
           usleep(mEmulationDelay );
        }
#endif
        parseProcessLine(currentState(), mUpdProc.readLine().simplified());
    }
}

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
    }
}
