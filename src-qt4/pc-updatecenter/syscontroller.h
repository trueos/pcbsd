#ifndef SYSCONTROLLER_H
#define SYSCONTROLLER_H

#include "updatecontroller.h"
#include "utils.h"

#include <QProcess>
#include <QString>
#include <QVector>
#include <QDate>

class CSysController : public CAbstractUpdateController
{
    Q_OBJECT

    //USES_CHECK_SHELL_COMMAND("pc-updatemanager", QString("check"))
    USES_UPDATE_SHELL_COMMAND("pc-updatemanager", QString("check"))

public:
    CSysController();

    typedef enum{
        eSYSUPDATE,
        ePATCH,
        eFBSDUPDATE
    }ESysUpdate;

    typedef struct _SSystemUpdate
    {
        ESysUpdate mType;
        QString mName;
        QString mTag;
        QString mDetails; // For patch only
        QString mSize;    // For patch only
        QString mVersion; // For sysupdates only
        QDate   mDate;    // For patch only
        QStringList mFilesToUpdate; //For FreeBSD updates only

    }SSystemUpdate;
    
    QVector<SSystemUpdate> updates() {return mvUpdates;}

protected:
    virtual void onCheckUpdates();
    virtual void checkShellCommand(QString& cmd, QStringList& args);

    void onReadCheckLine(QString line);
    void onReadUpdateLine(QString line);

    virtual void onCheckProcessfinished(int exitCode);
    virtual void onCancel();

private:
    bool misFREEBSDCheck;
    QVector<SSystemUpdate> mvUpdates;

    void parseCheckPCBSDLine(QString line);
    void parseCheckFREEBSDLine(QString line);
};

#endif // SYSCONTROLLER_H
