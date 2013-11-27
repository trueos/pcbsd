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
        bool   misStandalone;
        bool   misRequiresReboot;

        _SSystemUpdate()
        {
            misStandalone= false;
            misRequiresReboot= false;
        }
    }SSystemUpdate;
    
    QVector<SSystemUpdate> updates() {return mvUpdates;}
    QStringList filesToUpdate()      {return mFilesToUpdate;}
    QStringList filesToDelete()      {return mFilesToRemove;}
    QStringList fileslocalyModifyed(){return mFilesLocallyModifyed;}

    void updateSelected(QVector<SSystemUpdate> selectedUpdates);

protected:
    virtual void onCheckUpdates();
    virtual void checkShellCommand(QString& cmd, QStringList& args);
    virtual void updateShellCommand(QString& cmd, QStringList& args);

    void onReadCheckLine(QString line);
    void onReadUpdateLine(QString line);
    virtual void onReadProcessChar(char character);

    virtual void onCheckProcessfinished(int exitCode);
    virtual void onCancel();

private:
    bool misFREEBSDCheck;
    QVector<SSystemUpdate> mvUpdates;
    QStringList mFilesLocallyModifyed;
    QStringList mFilesToRemove;
    QStringList mFilesToUpdate;

    QVector<SSystemUpdate> mvUpdatesToApply;
    int                    mCurrentUpdate;

    void parseCheckPCBSDLine(QString line);
    void parseCheckFREEBSDLine(QString line);

    void parsePatchUpdateLine(QString line);
    void parseUpgradeLine(QString line);
    void parseFreeBSDUpdateLine(QString line);



};

#endif // SYSCONTROLLER_H
