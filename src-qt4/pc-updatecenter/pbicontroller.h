#ifndef PBICONTROLLER_H
#define PBICONTROLLER_H

#include "updatecontroller.h"

#include <QVector>

class CPBIController : public CAbstractUpdateController
{
    Q_OBJECT
    USES_CHECK_SHELL_COMMAND("pbi_update", QString("--check-all"))

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

    void updateSelected(QStringList generic_names_list);

protected:
    virtual void onCheckUpdates();
    virtual void onUpdateAll();

    void onReadCheckLine(QString line);
    void onReadUpdateLine(QString line);

    virtual void onCheckProcessfinished(int exitCode);
    virtual void onUpdateProcessfinished(int exitCode);

     virtual void updateShellCommand(QString& cmd, QStringList& args);

private:
    QVector<SPBIUpdate> mvUpdates;

    QStringList mAppsToUpdate;
    int         mCurrentUpdate;
    bool        misWasFETCHDONE;

};

#endif // PBICONTROLLER_H
