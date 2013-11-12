#ifndef UPDATECONTROLLER_H
#define UPDATECONTROLLER_H

#include <QObject>
#include <QString>
#include <QProcess>

#define USES_CHECK_SHELL_COMMAND(command, arguments)\
    protected:\
    virtual void checkShellCommand(QString& cmd, QStringList& args){cmd= QString(command); args.clear(); args<<arguments;};\
    private:

#define USES_UPDATE_SHELL_COMMAND(command, arguments)\
    protected:\
    virtual void updateShellCommand(QString& cmd, QStringList& args){cmd= QString(command);args.clear();args<<arguments;};\
    private:

class CAbstractUpdateController:public QObject
{
    Q_OBJECT
public:
    typedef enum{
       eNOT_INITIALIZED = 0,
       eCHECKING,
       eFULLY_UPDATED,
       eUPDATES_AVAIL,
       eUPDATING,
       eUPDATING_ERROR
    }EUpdateControllerState;

    typedef struct _SProgress
    {
        int ItemNo;
        int ItemsCount;
        int ProgressMin;
        int ProgressMax;
        int ProgressCurr;
        QString Message;
    }SProgress;

public:
    CAbstractUpdateController();

    EUpdateControllerState currentState();
    SProgress              currentProgress();
    QString                updateMessage();

    void parseProcessLine(EUpdateControllerState state, QString line);

protected:
    virtual void setCurrentState(EUpdateControllerState new_state);
    void reportProgress(SProgress curr_progress);
    void reportUpdatesAvail(QString message);
    void reportError(QString error_message);

    virtual void checkShellCommand(QString& cmd, QStringList& args)=0;
    virtual void updateShellCommand(QString& cmd, QStringList& args)=0;

    //! May be overrided by child. Calls on update check
    virtual void onCheckUpdates(){}
    //! May be overrided by child. Calls on update install
    virtual void onUpdateAll(){}

    //! Calls on check command process is finished
    virtual void onCheckProcessfinished(int exitCode){Q_UNUSED(exitCode)}
    //! Calls on update command process is finished
    virtual void onUpdateProcessfinished(int exitCode){Q_UNUSED(exitCode)}

    virtual void onReadCheckLine(QString line)=0;
    virtual void onReadUpdateLine(QString line)=0;


private:
    EUpdateControllerState mCurrentState;
    SProgress              mCurrentProgress;
    QString                mUpdateMasage;
    QString                mErrorMessage;
    QProcess               mUpdProc;

public: signals:
    void stateChanged(CAbstractUpdateController::EUpdateControllerState new_state);
    void progress(CAbstractUpdateController::SProgress progress);
    void updatesAvail(QString update_message);
    void updateError(QString message);

public slots:
    void check();//=0;
    void updateAll();//=0;

private slots:
    void slotProcessRead();
    void slotProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

};

#endif // UPDATECONTROLLER_H
