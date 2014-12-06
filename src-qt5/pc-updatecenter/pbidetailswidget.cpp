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

#include <QFile>

#include "pbidetailswidget.h"
#include "ui_pbidetailswidget.h"

const QString DEFAULT_PBI_DB_DIR="/var/db/pbi";
const QString INSTALLED_IN_DB="/installed";

const QString DEFAULT_APP_ICON=":/images/application.png";

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PBIDetailsWidget::PBIDetailsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PBIDetailsWidget)
{
    ui->setupUi(this);
    mUpdateController= NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PBIDetailsWidget::~PBIDetailsWidget()
{
    delete ui;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool PBIDetailsWidget::init(QString check_img, QString ok_img, QString avail_img,
                            QString download_img, QString install_img, QString error_img,
                            CPBIController *upd_controller)
{
    if (!upd_controller)
        return false;
    ui->pbiDetailsIndicator->init(check_img, ok_img, avail_img, download_img, install_img, error_img, upd_controller);

    mUpdateController= upd_controller;

    connect(mUpdateController, SIGNAL(stateChanged(CAbstractUpdateController::EUpdateControllerState)),
            this, SLOT(slotControllerStateChanged(CAbstractUpdateController::EUpdateControllerState)));

    connect(mUpdateController, SIGNAL(logLinePresent(QString)),
                this, SLOT(slotControllerLogLine(QString)));

    QPalette palette = ui->pbiUpdateLog->palette();
    palette.setColor(QPalette::Base, Qt::black);
    palette.setColor(QPalette::Text, Qt::white);
    ui->pbiUpdateLog->setPalette(palette);

    ui->pbiUpdateList->header()->resizeSection(0, 260);

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PBIDetailsWidget::slotControllerStateChanged(CAbstractUpdateController::EUpdateControllerState new_state)
{
    if (new_state == CAbstractUpdateController::eUPDATES_AVAIL)
    {
        QVector<CPBIController::SPBIUpdate> updates = mUpdateController->pbiUpdates();
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
        ui->pbiUpdateStack->setCurrentIndex(0);
    }
    else
    if (new_state == CAbstractUpdateController::eUPDATING)
    {
        ui->pbiUpdateStack->setCurrentIndex(1);
        ui->pbiUpdateLog->clear();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PBIDetailsWidget::slotControllerLogLine(QString line)
{
    ui->pbiUpdateLog->append(line);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PBIDetailsWidget::on_updateSelectedPBIBtn_clicked()
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

    mUpdateController->updateSelected(ListToUpdate);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PBIDetailsWidget::on_pbiUpdateList_itemChanged(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(item);
    Q_UNUSED(column);
    bool is_enable=false;
    for(int i=0; i < ui->pbiUpdateList->topLevelItemCount(); i++)
    {
        QTreeWidgetItem* item= ui->pbiUpdateList->topLevelItem(i);

        if (item->checkState(0) == Qt::Checked)
        {
            is_enable= true;
            break;
        }
    }
    ui->updateSelectedPBIBtn->setEnabled(is_enable);
}
