#ifndef PKGCONTROLLER_H
#define PKGCONTROLLER_H

#include "updatecontroller.h"

class CPkgController : public CAbstractUpdateController
{
    Q_OBJECT
public:
    CPkgController();

protected:
    virtual void onCheckUpdates();
    virtual void onUpdateAll();
};

#endif // PKGCONTROLLER_H
