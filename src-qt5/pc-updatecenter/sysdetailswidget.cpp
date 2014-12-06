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

#include "sysdetailswidget.h"
#include "ui_sysdetailswidget.h"

#include <QDebug>

const QString SYSUPDATE_PATCH_ICON = ":/images/sysupdates-patch.png";
const QString SYSUPDATE_UPGRADE_ICON = ":/images/sysupdates-upgrade.png";
const QString SYSUPDATE_FBSD_ICON = ":/images/sysupdates-freebsd.png";

const QString SYSUPDATE_DATE_FORMAT= "d MMM yyyy";

SysDetailsWidget::SysDetailsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysDetailsWidget)
{
    ui->setupUi(this);
    mUpdateController= NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SysDetailsWidget::~SysDetailsWidget()
{
    delete ui;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SysDetailsWidget::init(QString check_img, QString ok_img, QString avail_img,
                            QString download_img, QString install_img, QString error_img,
                            CSysController *upd_controller)
{
    if (!upd_controller)
        return false;
    ui->sysDetailsIndicator->init(check_img, ok_img, avail_img, download_img, install_img, error_img, upd_controller);

    mUpdateController= upd_controller;

    connect(mUpdateController, SIGNAL(stateChanged(CAbstractUpdateController::EUpdateControllerState)),
            this, SLOT(slotControllerStateChanged(CAbstractUpdateController::EUpdateControllerState)));
    connect(mUpdateController, SIGNAL(logLinePresent(QString)),
                this, SLOT(slotControllerLogLine(QString)));

    ui->sysUpdatesList->header()->resizeSection(0, 420);

    QPalette palette = ui->sysUpdateLog->palette();
    palette.setColor(QPalette::Base, Qt::black);
    palette.setColor(QPalette::Text, Qt::white);
    ui->sysUpdateLog->setPalette(palette);

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SysDetailsWidget::slotControllerStateChanged(CAbstractUpdateController::EUpdateControllerState new_state)
{
    if (CAbstractUpdateController::eUPDATES_AVAIL == new_state)
    {
        //if we have updates
        ui->sysUpdatesStack->setCurrentIndex(0);
        ui->sysUpdatesList->clear();
        QVector<CSysController::SSystemUpdate> updates = mUpdateController->updates();
        for(int i=0; i<updates.count(); i++)
        {
            //for all updates
            QString name = updates[i].mName;
            QString type;
            QString icon_file;
            //Change icon
            switch (updates[i].mType)
            {
                case CSysController::ePATCH:
                    icon_file= SYSUPDATE_PATCH_ICON;
                    name+=QString(" (")+updates[i].mDate.toString(SYSUPDATE_DATE_FORMAT)+QString(")");
                    type= tr("Patch");
                    break;
                 case CSysController::eSYSUPDATE:
                    icon_file= SYSUPDATE_UPGRADE_ICON;
                    type= tr("Upgrade");
                    break;
                 case CSysController::eFBSDUPDATE:
                    icon_file= SYSUPDATE_FBSD_ICON;
                    type= tr("FreeBSD update");
                    break;
            }//switch
            /*if (updates[i].misStandalone)
            {
                type+=tr(",standalone");
            }
            if (updates[i].misRequiresReboot)
            {
                type+=tr(",reboot required");
            }*/

            QTreeWidgetItem* item = new QTreeWidgetItem(QStringList()<<name<<type);
            item->setText(0, name);
            item->setIcon(0, QIcon(icon_file));
            QVariant uData(i);
            item->setData(0, Qt::UserRole, uData);

            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(0, Qt::Checked);

            ui->sysUpdatesList->addTopLevelItem(item);

            //set first tree widget item as current item (hilight it)
            if(i==0)
            {
                ui->sysUpdatesList->setCurrentItem(item);

            }

        }//for all updates
    }//if updates are avail.
    else
    if (CAbstractUpdateController::eUPDATING == new_state)
    {
        ui->sysUpdatesStack->setCurrentIndex(1);
        ui->sysUpdateLog->clear();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SysDetailsWidget::slotControllerLogLine(QString line)
{
    ui->sysUpdateLog->append(line);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SysDetailsWidget::on_sysUpdatesList_itemSelectionChanged()
{
    QTreeWidgetItem* item = ui->sysUpdatesList->currentItem();
    if (!item)
        return;
    QVariant v= item->data(0, Qt::UserRole);
    int id= v.toInt();

    QVector<CSysController::SSystemUpdate> updates = mUpdateController->updates();

    if (updates[id].mType == CSysController::ePATCH)
    {
        ui->sysUpdateDetailsStack->setCurrentIndex(0);
        ui->sysPatchDate->setText(updates[id].mDate.toString(SYSUPDATE_DATE_FORMAT));
        ui->sysPatchSize->setText(updates[id].mSize);
        ui->sysPadthDescription->setText(updates[id].mDetails);
        ui->sysPatchStandalone->setVisible(updates[id].misStandalone);
        ui->sysPatchRebootRequired->setVisible(updates[id].misRequiresReboot);

        //if description is http url - show 'open in web browser' button
        ui->openDeskriptionBtn->setVisible(updates[id].mDetails.trimmed().toLower().indexOf("http://")==0);

    }
    else
    if (updates[id].mType == CSysController::eSYSUPDATE)
    {
        ui->sysUpdateDetailsStack->setCurrentIndex(1);
        ui->sysUpgradeText->setText(tr("This update will upgrade your PC-BSD to %1").arg(updates[id].mVersion));
        ui->sysUpgradeStandalone->setVisible(updates[id].misStandalone);
        ui->sysUpgradehRebootRequired->setVisible(updates[id].misRequiresReboot);
    }
    else
    if (updates[id].mType == CSysController::eFBSDUPDATE)
    {
        ui->sysFilesModifyedList->clear();
        ui->sysFilesToDeleteList->clear();
        ui->sysFilesToUpdateList->clear();

        ui->sysFilesModifyedList->addItems(mUpdateController->fileslocalyModifyed());
        ui->sysFilesToDeleteList->addItems(mUpdateController->filesToDelete());
        ui->sysFilesToUpdateList->addItems(mUpdateController->filesToUpdate());

        ui->sysUpdateFilesTab->setTabText(0, tr("Files to update (%1)").arg(QString::number(mUpdateController->filesToUpdate().size())));
        ui->sysUpdateFilesTab->setTabText(1, tr("Files to remove (%1)").arg(QString::number(mUpdateController->filesToDelete().size())));
        ui->sysUpdateFilesTab->setTabText(2, tr("Locally modified files (%1)").arg(QString::number(mUpdateController->fileslocalyModifyed().size())));

        ui->sysUpdateDetailsStack->setCurrentIndex(2);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SysDetailsWidget::on_sysInstallSelectedBtn_clicked()
{
    QVector<CSysController::SSystemUpdate> toUpdate;
    QVector<CSysController::SSystemUpdate> updates = mUpdateController->updates();

    for(int i=0; i < ui->sysUpdatesList->topLevelItemCount(); i++)
    {
        QTreeWidgetItem* item= ui->sysUpdatesList->topLevelItem(i);

        if (item->checkState(0) == Qt::Checked)
        {
            QVariant v= item->data(0, Qt::UserRole);
            int id= v.toInt();
            toUpdate.push_back(updates[id]);
        }
    }
    mUpdateController->updateSelected(toUpdate);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SysDetailsWidget::on_sysUpdatesList_itemChanged(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(item);
    Q_UNUSED(column);
    bool is_enable=false;
    for(int i=0; i < ui->sysUpdatesList->topLevelItemCount(); i++)
    {
        QTreeWidgetItem* item= ui->sysUpdatesList->topLevelItem(i);

        if (item->checkState(0) == Qt::Checked)
        {
            is_enable= true;
            break;
        }
    }
    ui->sysInstallSelectedBtn->setEnabled(is_enable);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SysDetailsWidget::on_openDeskriptionBtn_clicked()
{
    QProcess::execute("xdg-open", QStringList()<<ui->sysPadthDescription->text().trimmed());\
}
