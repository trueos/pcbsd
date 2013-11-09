#ifndef UPDATECONTROLLER_H
#define UPDATECONTROLLER_H

#include <QObject>
#include <QString>

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

    EUpdateControllerState curentState();
    SProgress              currentProgress();
    QString                updateMessage();

protected:
    void setCurrentState(EUpdateControllerState new_state);
    void reportProgress(SProgress curr_progress);
    void reportUpdatesAvail(QString message);

    virtual void onCheckUpdates()=0;
    virtual void onUpdateAll()=0;

private:
    EUpdateControllerState mCurrentState;
    SProgress              mCurrentProgress;
    QString                mUpdateMasage;

signals:
    void stateChanged(EUpdateControllerState new_state);
    void progress(SProgress progress);
    void updatesAvail(QString update_message);

public slots:
    void check();//=0;
    void updateAll();//=0;

};

#endif // UPDATECONTROLLER_H
