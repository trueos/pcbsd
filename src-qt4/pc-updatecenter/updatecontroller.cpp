#include "updatecontroller.h"

CAbstractUpdateController::CAbstractUpdateController()
{
    mCurrentState= eNOT_INITIALIZED;
}

CAbstractUpdateController::EUpdateControllerState CAbstractUpdateController::currentState()
{
    return mCurrentState;
}

QString CAbstractUpdateController::updateMessage()
{
    return mUpdateMasage;
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

void CAbstractUpdateController::check()
{
    //TODO: correct current state check
    setCurrentState(eCHECKING);
    onCheckUpdates();
}

void CAbstractUpdateController::updateAll()
{
    //TODO: correct current state check
    setCurrentState(eUPDATING);
    onUpdateAll();
}
