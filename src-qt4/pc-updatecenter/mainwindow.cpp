#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTreeWidgetItem>
#include "pcbsd-utils.h"

const int MAIN_INDICATORS_IDX= 1;
const int MAIN_FULLY_UPDATED_IDX= 0;

const int TOOLBOX_MAIN_INDEX = 0;
const int TOOLBOX_SYS_INDEX=   1;
const int TOOLBOX_PKG_INDEX=   2;
const int TOOLBOX_PBI_INDEX=   3;

const int PKG_AVAIL_STACK_IDX = 0;
const int PKG_UPDATING_STACK_IDX = 0;

const QString SYS_CHECK_IMG =  ":images/syscheck.png";
const QString SYS_OK_IMG =     ":/images/sysok.png";
const QString SYS_AVAIL_IMG =  ":/images/sysupdates.png";
const QString SYS_PROGRESS_IMG=":/images/sysupdates.png";

const QString PKG_CHECK_IMG =  ":images/pkgcheck.png";
const QString PKG_OK_IMG =     ":/images/pkgok.png";
const QString PKG_AVAIL_IMG =  ":/images/pkgupdates.png";
const QString PKG_PROGRESS_IMG=":/images/pkgupdates.png";

const QString PBI_CHECK_IMG =  ":images/pbicheck.png";
const QString PBI_OK_IMG =     ":/images/pbiok.png";
const QString PBI_AVAIL_IMG =  ":/images/pbiupdates.png";
const QString PBI_PROGRESS_IMG=":/images/pbiupdates.png";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    for (int i=0; i<ui->mainTab->count(); i++)
    {
        if (i!=TOOLBOX_MAIN_INDEX)
            ui->mainTab->setTabEnabled(i, false);
    }

    //ui->mainToolbox->

    mSysController.check();
    mPkgController.check();
    mPBIController.check();

    ui->sysIndicator->init(SYS_CHECK_IMG, SYS_OK_IMG, SYS_AVAIL_IMG, SYS_PROGRESS_IMG,
                           &mSysController);

    ui->pkgIndicator->init(PKG_CHECK_IMG, PKG_OK_IMG, PKG_AVAIL_IMG, PKG_PROGRESS_IMG,
                           &mPkgController);
    ui->pkgDetailsIndicator->init(PKG_CHECK_IMG, PKG_OK_IMG, PKG_AVAIL_IMG, PKG_PROGRESS_IMG,
                                  &mPkgController);

    ui->pbiIndicator->init(PBI_CHECK_IMG, PBI_OK_IMG, PBI_AVAIL_IMG, PBI_PROGRESS_IMG,
                           &mPBIController);

    connect(&mPkgController, SIGNAL(stateChanged(CAbstractUpdateController::EUpdateControllerState)),
            this, SLOT(pkgStateChanged(CAbstractUpdateController::EUpdateControllerState)));


    ui->mainStatesStack->setCurrentIndex(MAIN_INDICATORS_IDX);

}

void MainWindow::pkgStateChanged(CAbstractUpdateController::EUpdateControllerState new_state)
{

    switch (new_state)
    {
        case CAbstractUpdateController::eUPDATES_AVAIL:
            ui->mainTab->setTabEnabled(TOOLBOX_PKG_INDEX, true);
            ui->pkgDetailsStack->setCurrentIndex(PKG_AVAIL_STACK_IDX);
            break;
        case CAbstractUpdateController::eUPDATING:
            ui->mainTab->setTabEnabled(TOOLBOX_PKG_INDEX, true);
            ui->pkgDetailsStack->setCurrentIndex(PKG_AVAIL_STACK_IDX);
            break;
        case CAbstractUpdateController::eCHECKING:
            ui->mainTab->setTabEnabled(TOOLBOX_PKG_INDEX, false);
            break;
    }

    if (new_state == CAbstractUpdateController::eUPDATES_AVAIL)
    {
        CPkgController::SUpdate upd = mPkgController.updateData();

        ui->pkgListsTab->setTabText(0, tr("Packages to upgrade (%1)").arg(QString::number(upd.mUpgradeCount)));
        ui->pkgListsTab->setTabText(1, tr("Packages to install (%1)").arg(QString::number(upd.mInstallCount)));
        ui->pkgListsTab->setTabText(2, tr("Packages to reinstall (%1)").arg(QString::number(upd.mReinstallCount)));

        if (upd.mDiskSpace>0)
        {
            ui->pkgSpaceReqLabel_2->setText(tr("Upgrade will require %1 more space").arg(pcbsd::Utils::bytesToHumanReadable(upd.mDiskSpace)));
        }
        else
        {
            ui->pkgSpaceReqLabel_2->setText(tr("Upgrade will free %1").arg(pcbsd::Utils::bytesToHumanReadable(- upd.mDiskSpace)));
        }

        ui->pkgDlSizeLabel_2->setText(tr("%1 to be downloaded. ").arg(pcbsd::Utils::bytesToHumanReadable(upd.mDownloadSize)));

        ui->pkgUpgradeList->clear();
        ui->pkgInstallList->clear();
        ui->pkgReinstallList->clear();
        for(int i=0; i<upd.mvPkgUpdates.size(); i++)
        {
            CPkgController::SPackageUpdate curr= upd.mvPkgUpdates[i];
            if (curr.mAction == CPkgController::eUpgrading)
            {
                QTreeWidgetItem* item = new QTreeWidgetItem(QStringList()<<curr.mPackageName<<curr.mOldVersion<<curr.mNewVersion);
                ui->pkgUpgradeList->addTopLevelItem(item);
            }
            else if (curr.mAction == CPkgController::eInstalling)
            {
                QTreeWidgetItem* item = new QTreeWidgetItem(QStringList()<<curr.mPackageName<<curr.mNewVersion);
                ui->pkgInstallList->addTopLevelItem(item);
            }
            else if (curr.mAction == CPkgController::eReinstalling)
            {
                QTreeWidgetItem* item = new QTreeWidgetItem(QStringList()<<curr.mPackageName<<curr.mOldVersion<<curr.mReinstallReason);
                ui->pkgReinstallList->addTopLevelItem(item);
            }
        }

    }



}
