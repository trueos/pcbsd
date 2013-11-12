#ifndef SYSCONTROLLER_H
#define SYSCONTROLLER_H

#include "updatecontroller.h"
#include <QProcess>
#include <QString>

class CSysController : public CAbstractUpdateController
{
    Q_OBJECT

    USES_CHECK_SHELL_COMMAND("pc-updatemanager", QString("check"))
    USES_UPDATE_SHELL_COMMAND("pc-updatemanager", QString("check"))

public:
    CSysController();
    
protected:
    void onReadCheckLine(QString line);
    void onReadUpdateLine(QString line);

private:
    
};

#endif // SYSCONTROLLER_H
