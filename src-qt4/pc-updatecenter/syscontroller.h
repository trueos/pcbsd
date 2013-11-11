#ifndef SYSCONTROLLER_H
#define SYSCONTROLLER_H

#include "updatecontroller.h"
#include <QProcess>
#include <QString>

class CSysController : public CAbstractUpdateController
{
    Q_OBJECT
public:
    CSysController();
    
    virtual void parseProcessLine(EUpdateControllerState state, QString line);

protected:
    virtual void onCheckUpdates();
    virtual void onUpdateAll();

private slots:
    void slotProcessRead();

private:
    void checkReadLine(QString line);
    void updateReadLine(QString line);

private:
    QProcess mUpdProcess;

    
};

#endif // SYSCONTROLLER_H
