/**************************************************************************
*   Copyright (C) 2013- by Yuri Momotyuk                                   *
*   yurkis@gmail.com                                                      *
*                                                                         *
*   Permission is hereby granted, free of charge, to any person obtaining *
*   a copy of this software and associated documentation files (the       *
*   "Software"), to deal in the Software without restriction, including   *
*   without limitation the rights to use, copy, modify, merge, publish,   *
*   distribute, sublicense, and/or sell copies of the Software, and to    *
*   permit persons to whom the Software is furnished to do so, subject to *
*   the following conditions:                                             *
*                                                                         *
*   The above copyright notice and this permission notice shall be        *
*   included in all copies or substantial portions of the Software.       *
*                                                                         *
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
*   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
*   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*
*   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
*   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
*   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
*   OTHER DEALINGS IN THE SOFTWARE.                                       *
***************************************************************************/

#ifndef UPDATECONTROLLER_H
#define UPDATECONTROLLER_H

#include <QObject>
#include <QString>
#include <QProcess>
#include <QStringList>

#include "pcbsd-DLProcess.h"

//#define CONTROLLER_EMULATION_ENABLED

#define USES_CHECK_SHELL_COMMAND(command, arguments)\
    protected:\
    virtual void checkShellCommand(QString& cmd, QStringList& args){cmd= QString(command); args.clear(); args<<arguments;};\
    private:

#define USES_UPDATE_SHELL_COMMAND(command, arguments)\
    protected:\
    virtual void updateShellCommand(QString& cmd, QStringList& args){cmd= QString(command);args.clear();args<<arguments;};\
    private:

#define USES_DL_TYPE(type)\
    protected:\
    virtual QString dlType(){return QString(type);};\
    private:

#define USES_SYS_FLAG(name)\
    protected:\
    virtual QString sysFlagName(){return QString(name);};

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
       eUPDATING_ERROR,
       eMAX
    }EUpdateControllerState;

    typedef enum{
        eDownload,
        eInstall
    }EUpdateSubstate;

    typedef struct _SProgress
    {
        int mItemNo;
        int mItemsCount;
        int mProgressMin;
        int mProgressMax;
        int mProgressCurr;
        EUpdateSubstate mSubstate;
        bool        misCanCancel;
        QString     mMessage;
        QStringList mLogMessages;
        _SProgress()
            {   mSubstate = eDownload; misCanCancel = false;
                mMessage = tr("Preparing update...");
                mItemNo=0; mItemsCount=0;
                mProgressMin=0; mProgressMax=0; mProgressCurr=0;
            }
    }SProgress;

public:
    CAbstractUpdateController();

    void setJailPrefix(QString prefix);
    void removeJailPrefix();
    bool isHostSystem();

    EUpdateControllerState currentState();
    SProgress              currentProgress();
    QString                updateMessage();
    QStringList            updateLog();
    bool                   hasLog();

    void parseProcessLine(EUpdateControllerState state, QString line);


protected:
    virtual void setCurrentState(EUpdateControllerState new_state);
    void reportProgress(SProgress curr_progress);
    void reportLogLine(QString line);
    void reportUpdatesAvail(QString message);
    void reportError(QString error_message);
    void launchUpdate();
    void launchCheck();
    DLProcess& process() {return mUpdProc;}    

    virtual void checkShellCommand(QString& cmd, QStringList& args)=0;
    virtual void updateShellCommand(QString& cmd, QStringList& args)=0;
    virtual QString dlType(){return QString("");}
    virtual QString sysFlagName(){return QString("");}

    //! May be overrided by child. Calls on update check
    virtual void onCheckUpdates(){}
    //! May be overrided by child. Calls on update install
    virtual void onUpdateAll(){}

    virtual void onCancel(){}

    //! Calls on check command process is finished
    virtual void onCheckProcessfinished(int exitCode){Q_UNUSED(exitCode)}
    //! Calls on update command process is finished
    virtual void onUpdateProcessfinished(int exitCode){Q_UNUSED(exitCode)}

    virtual void onReadCheckLine(QString line)=0;
    virtual void onReadUpdateLine(QString line)=0;
    virtual void onReadProcessChar(char character){Q_UNUSED(character);}

    virtual void onDownloadUpdatePercent(QString percent, QString size, QString other)
    {Q_UNUSED(percent); Q_UNUSED(size); Q_UNUSED(other);}

private:
    EUpdateControllerState mCurrentState;
    SProgress              mCurrentProgress;
    QString                mUpdateMasage;
    QString                mErrorMessage;
    DLProcess              mUpdProc;
    QStringList            mLogMessages;
    QString                mJailPrefix;

    void setSysFlag(QString flag, QString val);

public: signals:
    void stateChanged(CAbstractUpdateController::EUpdateControllerState new_state);
    void progress(CAbstractUpdateController::SProgress progress);
    void updatesAvail(QString update_message);
    void updateError(QString message);
    void logLinePresent(QString line);

public slots:
    void check();
    void updateAll();
    void cancel();

private slots:
    void slotProcessRead(QString str);
    void slotProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void slotDLProcUpdatePercent(QString percent, QString size, QString other);

};

#endif // UPDATECONTROLLER_H
