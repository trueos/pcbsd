#ifndef PBICONTROLLER_H
#define PBICONTROLLER_H

#include "updatecontroller.h"

#include <QVector>

class CPBIController : public CAbstractUpdateController
{
    Q_OBJECT
    USES_CHECK_SHELL_COMMAND("pbi_update", QString("--check-all"))
    USES_UPDATE_SHELL_COMMAND("pbi_update", QString("ppppp"))

public:
    typedef struct _SPBIUpdate
    {
        QString mName;
        QString mGenericName;
        QString mOldVer;
        QString mNewVer;
    }SPBIUpdate;

public:
    CPBIController();

    QVector<SPBIUpdate> pbiUpdates()
        { return mvUpdates;}

protected:
    virtual void onCheckUpdates();
    void onReadCheckLine(QString line);
    void onReadUpdateLine(QString line);

    virtual void onCheckProcessfinished(int exitCode);

private:
    QVector<SPBIUpdate> mvUpdates;

};

#endif // PBICONTROLLER_H
