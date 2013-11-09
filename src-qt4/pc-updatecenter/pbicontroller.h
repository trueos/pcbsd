#ifndef PBICONTROLLER_H
#define PBICONTROLLER_H

#include "updatecontroller.h"

class CPBIController : public CAbstractUpdateController
{
    Q_OBJECT

public:
    CPBIController();

protected:
    virtual void onCheckUpdates();
    virtual void onUpdateAll();
};

#endif // PBICONTROLLER_H
