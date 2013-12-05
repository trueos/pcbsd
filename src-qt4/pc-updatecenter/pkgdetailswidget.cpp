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

#include "pcbsd-utils.h"

#include "dialogs/dialogconflict.h"
#include "pkgdetailswidget.h"
#include "ui_pkgdetailswidget.h"


const int PKG_AVAIL_STACK_IDX = 0;
const int PKG_UPDATING_STACK_IDX = 1;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PkgDetailsWidget::PkgDetailsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PkgDetailsWidget)
{
    ui->setupUi(this);
    mUpdateController= NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PkgDetailsWidget::~PkgDetailsWidget()
{
    delete ui;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool PkgDetailsWidget::init(QString check_img, QString ok_img, QString avail_img,
                            QString download_img, QString install_img, QString error_img,
                            CPkgController *upd_controller)
{
    if (!upd_controller)
        return false;

    mUpdateController= upd_controller;

    ui->pkgDetailsIndicator->init(check_img, ok_img, avail_img, download_img, install_img, error_img, upd_controller);

    connect(mUpdateController, SIGNAL(stateChanged(CAbstractUpdateController::EUpdateControllerState)),
            this, SLOT(slotControllerStateChanged(CAbstractUpdateController::EUpdateControllerState)));

    connect(mUpdateController, SIGNAL(logLinePresent(QString)),
                this, SLOT(slotControllerLogLine(QString)));

    connect(mUpdateController, SIGNAL(packageConflict(QString)),
                this, SLOT(slotControllerPkgConflict(QString)));

    QPalette palette = ui->pkgUpdateLog->palette();
    palette.setColor(QPalette::Base, Qt::black);
    palette.setColor(QPalette::Text, Qt::white);
    ui->pkgUpdateLog->setPalette(palette);

    ui->pkgUpgradeList->header()->resizeSection(0, 260);
    ui->pkgInstallList->header()->resizeSection(0, 350);
    ui->pkgReinstallList->header()->resizeSection(0, 200);

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PkgDetailsWidget::slotControllerStateChanged(CAbstractUpdateController::EUpdateControllerState new_state)
{
    if (new_state == CAbstractUpdateController::eUPDATES_AVAIL)
    {
        CPkgController::SUpdate upd = mUpdateController->updateData();

        ui->pkgListsTab->setTabText(0, tr("Packages to upgrade (%1)").arg(QString::number(upd.mUpgradeCount)));
        ui->pkgListsTab->setTabText(1, tr("Packages to install (%1)").arg(QString::number(upd.mInstallCount)));
        ui->pkgListsTab->setTabText(2, tr("Packages to reinstall (%1)").arg(QString::number(upd.mReinstallCount)));

        if (upd.mDiskSpace>0)
        {
            ui->pkgSpaceReqLabel_2->setText(tr("Upgrade will require %1 more space").arg(pcbsd::Utils::bytesToHumanReadable(upd.mDiskSpace)));
        }
        else
        {
            ui->pkgSpaceReqLabel_2->setText(tr("Upgrade will free %1").arg(pcbsd::Utils::bytesToHumanReadable(0 - upd.mDiskSpace)));
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

        ui->pkgDetailsStack->setCurrentIndex(PKG_AVAIL_STACK_IDX);

    }// if updates evail.
    else
    if (new_state == CAbstractUpdateController::eUPDATING)
    {
        ui->pkgDetailsStack->setCurrentIndex(PKG_UPDATING_STACK_IDX);
        ui->pkgUpdateLog->clear();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PkgDetailsWidget::slotControllerLogLine(QString line)
{
    ui->pkgUpdateLog->append(line);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PkgDetailsWidget::slotControllerPkgConflict(QString conflictList)
{
    DialogConflict* dlg = new  DialogConflict(this);
    dlg->exec(conflictList, mUpdateController);
}
