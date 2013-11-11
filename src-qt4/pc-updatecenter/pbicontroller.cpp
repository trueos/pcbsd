#include "pbicontroller.h"

CPBIController::CPBIController()
{
}

void CPBIController::onCheckUpdates()
{
    //reportUpdatesAvail("Available updates for 3 PBI packages");
    setCurrentState(eFULLY_UPDATED);
}

void CPBIController::onUpdateAll()
{

}
