#ifndef UPDATECONTROLLER_H
#define UPDATECONTROLLER_H

#include <QObject>
#include <QString>

class CUpdateController:public QObject
{
    Q_OBJECT
public:
    typedef enum{
       eNOT_INITIALIZED = 0,
       eCHECKING,
       eFULLY_UPDATED,
       eUPDATES_AVAIL,
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
    CUpdateController();

    EUpdateControllerState curentState();

signals:
    virtual void stateChanged(EUpdateControllerState)=0;
    virtual void progress(SProgress progress);

public slots:


};

#endif // UPDATECONTROLLER_H
