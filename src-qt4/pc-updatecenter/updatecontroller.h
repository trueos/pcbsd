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



signals:
    void stateChanged(EUpdateControllerState new_state);
    void progress(SProgress progress);

public slots:
    virtual void check(){}//=0;
    virtual void updateAll(){}//=0;

};

#endif // UPDATECONTROLLER_H
