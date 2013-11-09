#ifndef SYSCONTROLLER_H
#define SYSCONTROLLER_H

#include "updatecontroller.h"

class CSysController : public CAbstractUpdateController
{
    Q_OBJECT
public:
    CSysController();
    
protected:
    virtual void onCheckUpdates();
    virtual void onUpdateAll();
    
};

#endif // SYSCONTROLLER_H
