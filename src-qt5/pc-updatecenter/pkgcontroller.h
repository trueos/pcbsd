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

#ifndef PKGCONTROLLER_H
#define PKGCONTROLLER_H

#include "updatecontroller.h"
#include <QProcess>
#include <QVector>

class CPkgController : public CAbstractUpdateController
{
    Q_OBJECT

    USES_CHECK_SHELL_COMMAND("pc-updatemanager", QString("pkgcheck"))
    USES_UPDATE_SHELL_COMMAND("pc-updatemanager", QString("pkgupdate"))
    USES_DL_TYPE("PKG")

public:

    typedef enum{
        eUnknown,
        eInstalling,
        eUpgrading,
        eReinstalling
    }EPkgAction;

    typedef struct _SPackageUpdate
    {
        QString mPackageName;
        QString mOldVersion;
        QString mNewVersion;
        QString mReinstallReason;
        EPkgAction mAction;

        _SPackageUpdate()
        {
            mAction = eUnknown;
        };

    }SPackageUpdate;

    typedef struct _SUpdateDescription
    {
        unsigned int mCommonPkgsCount;
        unsigned int mUpgradeCount;
        unsigned int mInstallCount;
        unsigned int mReinstallCount;
        signed long  mDownloadSize;
        signed long  mDiskSpace;
        QVector<SPackageUpdate> mvPkgUpdates;
        _SUpdateDescription():mCommonPkgsCount(0), mUpgradeCount(0), mInstallCount(0), mReinstallCount(0), mDownloadSize(0), mDiskSpace(0){;};
    }SUpdate;

public:
    CPkgController();

    SUpdate updateData();
    bool    logoffRequired();

protected:
    virtual void onReadCheckLine(QString line);
    virtual void onUpdateAll();
    virtual void onReadUpdateLine(QString line);
    virtual void onDownloadUpdatePercent(QString percent, QString size, QString other);
    virtual void onUpdateProcessfinished(int exitCode);
    virtual void onCancel();
    virtual void onCheckProcessfinished(int exitCode);

signals:
    void packageConflict(QString conflictList);

public slots:
    void autoResolveConflict(bool isAutoResolve);

private:
    SUpdate mUpdData;
    bool    misDownloadComplete;
    bool    misWasInstalation;
    int     mCurrentPkgNo;
    int     misInFetch;
    QString mCurrentPkgName;
    QString mLastLine;
    QString mConflictList;
    QString mConflictsReply;
    int     mCurrentDEIndex;
    bool    misLogoffRequired;
};

#endif // PKGCONTROLLER_H
