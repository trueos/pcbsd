#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialogconflict.h"

#include <QTreeWidgetItem>
#include <QFile>
#include <QPalette>

#include "pcbsd-utils.h"

const int MAIN_INDICATORS_IDX= 1;
const int MAIN_FULLY_UPDATED_IDX= 0;

const int TOOLBOX_MAIN_INDEX = 0;
const int TOOLBOX_SYS_INDEX=   1;
const int TOOLBOX_PKG_INDEX=   2;
const int TOOLBOX_PBI_INDEX=   3;

const int PKG_AVAIL_STACK_IDX = 0;
const int PKG_UPDATING_STACK_IDX = 1;

const QString SYS_CHECK_IMG =  ":images/syscheck.png";
const QString SYS_OK_IMG =     ":/images/sysok.png";
const QString SYS_AVAIL_IMG =  ":/images/sysupdates.png";
const QString SYS_DL_IMG=      ":/images/sysdownload.png";
const QString SYS_INSTALL_IMG= ":/images/sysinstall.png";
const QString SYS_ERROR_IMG=   ":/images/syserror.png";

const QString PKG_CHECK_IMG =  ":images/pkgcheck.png";
const QString PKG_OK_IMG =     ":/images/pkgok.png";
const QString PKG_AVAIL_IMG =  ":/images/pkgupdates.png";
const QString PKG_PROGRESS_IMG=":/images/pkgupdates.png";
const QString PKG_DL_IMG=      ":/images/pkgdownload.png";
const QString PKG_INSTALL_IMG= ":/images/pkginstall.png";
const QString PKG_ERROR_IMG=   ":/images/pkgerror.png";


const QString PBI_CHECK_IMG =  ":images/pbicheck.png";
const QString PBI_OK_IMG =     ":/images/pbiok.png";
const QString PBI_AVAIL_IMG =  ":/images/pbiupdates.png";
const QString PBI_PROGRESS_IMG=":/images/pbiupdates.png";
const QString PBI_DL_IMG=      ":/images/pbidownload.png";
const QString PBI_INSTALL_IMG= ":/images/pbiinstall.png";
const QString PBI_ERROR_IMG=   ":/images/pbierror.png";

const QString DEFAULT_APP_ICON=":/images/application.png";

const QString DEFAULT_PBI_DB_DIR="/var/db/pbi";
const QString INSTALLED_IN_DB="/installed";

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

#if 1
    mPkgController.setEmulateCheckFile("/home/yurkis/_pkgcheck.txt");
    mPkgController.setEmulateUpdateFile("/home/yurkis/_pkguc.txt");
#endif


    mSysController.check();
    mPkgController.check();
    mPBIController.check();    

    ui->sysIndicator->init(SYS_CHECK_IMG, SYS_OK_IMG, SYS_AVAIL_IMG,
                           SYS_DL_IMG, SYS_INSTALL_IMG, SYS_ERROR_IMG,
                           &mSysController);

    ui->pkgIndicator->init(PKG_CHECK_IMG, PKG_OK_IMG, PKG_AVAIL_IMG,
                           PKG_DL_IMG, PKG_INSTALL_IMG, PKG_ERROR_IMG,
                           &mPkgController);
    ui->pkgDetailsIndicator->init(PKG_CHECK_IMG, PKG_OK_IMG, PKG_AVAIL_IMG,
                                  PKG_DL_IMG, PKG_INSTALL_IMG, PKG_ERROR_IMG,
                                  &mPkgController);

    ui->pbiIndicator->init(PBI_CHECK_IMG, PBI_OK_IMG, PBI_AVAIL_IMG,
                           PBI_DL_IMG, PBI_INSTALL_IMG, PBI_ERROR_IMG,
                           &mPBIController);
    ui->pbiDetailsIndicator->init(PBI_CHECK_IMG, PBI_OK_IMG, PBI_AVAIL_IMG,
                                  PBI_DL_IMG, PBI_INSTALL_IMG, PBI_ERROR_IMG,
                                  &mPBIController);


    connect(&mPkgController, SIGNAL(stateChanged(CAbstractUpdateController::EUpdateControllerState)),
            this, SLOT(pkgStateChanged(CAbstractUpdateController::EUpdateControllerState)));
    connect(&mPkgController, SIGNAL(progress(CAbstractUpdateController::SProgress)),
            this, SLOT(pkgProgress(CAbstractUpdateController::SProgress)));
    connect(&mPkgController, SIGNAL(packageConflict(QString)),
            this, SLOT(pkgConflict(QString)));
    connect(&mPBIController, SIGNAL(stateChanged(CAbstractUpdateController::EUpdateControllerState)),
            this, SLOT(pbiStateChanged(CAbstractUpdateController::EUpdateControllerState)));
    connect(&mPBIController, SIGNAL(progress(CAbstractUpdateController::SProgress)),
            this, SLOT(pbiProgress(CAbstractUpdateController::SProgress)));


    ui->mainStatesStack->setCurrentIndex(MAIN_INDICATORS_IDX);

    QPalette palette = ui->pbiUpdateLog->palette();
    palette.setColor(QPalette::Base, Qt::black);
    palette.setColor(QPalette::Text, Qt::white);
    ui->pbiUpdateLog->setPalette(palette);
    ui->pkgUpdateLog->setPalette(palette);

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
            ui->pkgDetailsStack->setCurrentIndex(PKG_UPDATING_STACK_IDX);
            break;
        case CAbstractUpdateController::eCHECKING:
            ui->mainTab->setTabEnabled(TOOLBOX_PKG_INDEX, false);
            break;
        default: //supress warning
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
        }//populate lists

    }// if updates evail.
}

void MainWindow::pbiStateChanged(CAbstractUpdateController::EUpdateControllerState new_state)
{

    if (new_state == CAbstractUpdateController::eUPDATES_AVAIL)
    {
        QVector<CPBIController::SPBIUpdate> updates = mPBIController.pbiUpdates();
        ui->pbiUpdateList->clear();
        for(int i=0; i<updates.size(); i++)
        {
            QTreeWidgetItem* item = new QTreeWidgetItem(QStringList()<<updates[i].mName<<updates[i].mOldVer<<updates[i].mNewVer);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(0, Qt::Checked);

            //Parse pbi db to get application icon...
            QString icon_file = DEFAULT_PBI_DB_DIR + INSTALLED_IN_DB + QString("/") + updates[i].mGenericName + QString("/pbi_icon.png");

            if (!QFile::exists(icon_file))
            {
                icon_file= DEFAULT_APP_ICON;
            }

            item->setIcon(0, QIcon(icon_file));
            QVariant uData(updates[i].mGenericName);
            item->setData(0, Qt::UserRole, uData);

            ui->pbiUpdateList->addTopLevelItem(item);
        }
    }

    switch (new_state)
    {
        case CAbstractUpdateController::eUPDATES_AVAIL:
            ui->mainTab->setTabEnabled(TOOLBOX_PBI_INDEX, true);
            ui->pbiUpdateStack->setCurrentIndex(0);
            break;
        case CAbstractUpdateController::eUPDATING:
            ui->mainTab->setTabEnabled(TOOLBOX_PBI_INDEX, true);
            ui->pbiUpdateStack->setCurrentIndex(1);
            break;
        case CAbstractUpdateController::eCHECKING:
            ui->mainTab->setTabEnabled(TOOLBOX_PBI_INDEX, false);
        default: //supress warning
            break;
    }
}

void MainWindow::pbiProgress(CAbstractUpdateController::SProgress progress)
{
    for(int i=0; i<progress.mLogMessages.size(); i++)
    {
        ui->pbiUpdateLog->append(progress.mLogMessages[i]);
    }
}

void MainWindow::pkgProgress(CAbstractUpdateController::SProgress progress)
{
    for(int i=0; i<progress.mLogMessages.size(); i++)
    {
        ui->pkgUpdateLog->append(progress.mLogMessages[i]);
        qDebug()<<"<<<"<<progress.mLogMessages[i];
    }
}

void MainWindow::pkgConflict(QString conflictList)
{
    DialogConflict* dlg = new  DialogConflict(this);
    dlg->exec(conflictList, &mPkgController); //QDialog::Accepted | QDialog::Rejected
}

void MainWindow::on_updateSelectedPBIBtn_clicked()
{
    QStringList ListToUpdate;
    for(int i=0; i < ui->pbiUpdateList->topLevelItemCount(); i++)
    {
        if (ui->pbiUpdateList->topLevelItem(i)->checkState(0) == Qt::Checked)
        {
            QVariant v= ui->pbiUpdateList->topLevelItem(i)->data(0, Qt::UserRole);
            QString gen_name= v.toString();
            ListToUpdate<<gen_name;
        }
    }

    mPBIController.updateSelected(ListToUpdate);
}
