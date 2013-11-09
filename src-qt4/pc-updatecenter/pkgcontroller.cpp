#include "pkgcontroller.h"

CPkgController::CPkgController()
{
}

void CPkgController::onCheckUpdates()
{
    setCurrentState(eFULLY_UPDATED);
}

void CPkgController::onUpdateAll()
{

}
