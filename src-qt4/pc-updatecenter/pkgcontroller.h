#ifndef PKGCONTROLLER_H
#define PKGCONTROLLER_H

#include "updatecontroller.h"

#include <QProcess>
#include <QVector>

class CPkgController : public CAbstractUpdateController
{
    Q_OBJECT
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
        long long  mDownloadSize;
        long long  mDiskSpace;
        QVector<SPackageUpdate> mvPkgUpdates;
        _SUpdateDescription():mCommonPkgsCount(0), mUpgradeCount(0), mInstallCount(0), mReinstallCount(0), mDownloadSize(0), mDiskSpace(0){;};
    }SUpdate;

public:
    CPkgController();

    SUpdate updateData();

    virtual void parseProcessLine(EUpdateControllerState state, QString line);

protected:
    virtual void onCheckUpdates();
    virtual void onUpdateAll();

private slots:
    void slotProcessRead();

private:
    void checkReadLine(QString line);
    void updateReadLine(QString line);

private:
    QProcess mUpdProcess;
    SUpdate mUpdData;
};

#endif // PKGCONTROLLER_H
