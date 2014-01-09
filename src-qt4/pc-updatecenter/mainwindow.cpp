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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialogs/dialogconflict.h"
#include "dialogs/logviewdialog.h"
#include "dialogs/jailsdialog.h"
#include "dialogs/branchesdialog.h"
#include "dialogs/patchsetdialog.h"
#include "dialogs/updatehistorydialog.h"

#include <unistd.h>
#include <QTreeWidgetItem>
#include <QFile>
#include <QPalette>
#include <QMessageBox>
#include <QApplication>

#include <QDebug>

#include "pcbsd-utils.h"

const int MAIN_INDICATORS_IDX= 1;
const int MAIN_FULLY_UPDATED_IDX= 0;

const int TOOLBOX_MAIN_INDEX = 0;
const int TOOLBOX_SYS_INDEX=   1;
const int TOOLBOX_PKG_INDEX=   2;
const int TOOLBOX_PBI_INDEX=   3;

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

///////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(CJailsBackend *jail, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if (jail)
    {
        mJail = *jail;
    }
    init();
}

///////////////////////////////////////////////////////////////////////////////
MainWindow::~MainWindow()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::setJail(CJailsBackend jail)
{
    mJail= jail;
    jailRefresh();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::init()
{
    misRegularUser = getuid() != 0;

    for (int i=0; i<ui->mainTab->count(); i++)
    {
        if (i!=TOOLBOX_MAIN_INDEX)
            ui->mainTab->setTabEnabled(i, false);
    }

    ui->RebootW->setVisible(false);
    ui->RebootW->init(&mSysController, &mPkgController, &mPBIController);

    ui->jailIndicatorWidget->setVisible(false);
    jailRefresh();

    if (!mJail.jailEnabled())
    {
        mSysController.check();
        if (!misRegularUser)
            mPkgController.check();
        mPBIController.check();
    }

    ui->sysIndicator->init(SYS_CHECK_IMG, SYS_OK_IMG, SYS_AVAIL_IMG,
                           SYS_DL_IMG, SYS_INSTALL_IMG, SYS_ERROR_IMG,
                           &mSysController);    
    ui->sysDetailsW->init(SYS_CHECK_IMG, SYS_OK_IMG, SYS_AVAIL_IMG,
                          SYS_DL_IMG, SYS_INSTALL_IMG, SYS_ERROR_IMG,
                          &mSysController);

    ui->pkgIndicator->init(PKG_CHECK_IMG, PKG_OK_IMG, PKG_AVAIL_IMG,
                           PKG_DL_IMG, PKG_INSTALL_IMG, PKG_ERROR_IMG,
                           &mPkgController);
    ui->pkgDetailsW->init(PKG_CHECK_IMG, PKG_OK_IMG, PKG_AVAIL_IMG,
                          PKG_DL_IMG, PKG_INSTALL_IMG, PKG_ERROR_IMG,
                          &mPkgController);

    ui->pbiIndicator->init(PBI_CHECK_IMG, PBI_OK_IMG, PBI_AVAIL_IMG,
                           PBI_DL_IMG, PBI_INSTALL_IMG, PBI_ERROR_IMG,
                           &mPBIController);
    ui->pbiDetailsW->init(PBI_CHECK_IMG, PBI_OK_IMG, PBI_AVAIL_IMG,
                           PBI_DL_IMG, PBI_INSTALL_IMG, PBI_ERROR_IMG,
                           &mPBIController);



    connect(&mSysController, SIGNAL(stateChanged(CAbstractUpdateController::EUpdateControllerState)),
            this, SLOT(globalStateChanged(CAbstractUpdateController::EUpdateControllerState)));

    connect(&mPkgController, SIGNAL(stateChanged(CAbstractUpdateController::EUpdateControllerState)),
            this, SLOT(globalStateChanged(CAbstractUpdateController::EUpdateControllerState)));

    connect(&mPBIController, SIGNAL(stateChanged(CAbstractUpdateController::EUpdateControllerState)),
            this, SLOT(globalStateChanged(CAbstractUpdateController::EUpdateControllerState)));

    connect(ui->jailIndicatorWidget, SIGNAL(returnToHost()),
            this, SLOT(slotReturnToHost()));

    ui->mainStatesStack->setCurrentIndex(MAIN_INDICATORS_IDX);

    if (misRegularUser)
    {
        ui->pkgIndicator->setVisible(false);
        ui->packagesIndicatorGB->setVisible(false);
        ui->pkgIndictorLine->setVisible(false);
        ui->actionJail->setEnabled(false);
        ui->actionSystem_branches->setEnabled(false);
        ui->actionUpdate_set->setEnabled(false);
    }

}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::jailRefresh()
{
    static bool last_enabled= false;
    static QString last_name;

    ui->jailIndicatorWidget->setVisible(mJail.jailEnabled());

    if (mJail.jailEnabled())
    {
        ui->jailIndicatorWidget->setJailName(mJail.jailName());
        QString prefix= mJail.jailPrefix();
        mSysController.setJailPrefix(prefix);
        mPkgController.setJailPrefix(prefix);
        mPBIController.setJailPrefix(prefix);
    }
    else
    {
        mSysController.removeJailPrefix();
        mPkgController.removeJailPrefix();
        mPBIController.removeJailPrefix();
    }

    if ((last_enabled!=mJail.jailEnabled()) || (last_name != mJail.jailName()))
    {
        mSysController.check();
        mPkgController.check();
        mPBIController.check();
        last_enabled = mJail.jailEnabled();
        last_name = mJail.jailName();
    }
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::refreshMenu()
{
    bool en_syslog= (mSysController.currentState() != CAbstractUpdateController::eUPDATING) && mSysController.hasLog();
    ui->actionLast_system_update_log->setEnabled(en_syslog);
    bool en_pkglog= (mPkgController.currentState() != CAbstractUpdateController::eUPDATING) && mPkgController.hasLog();
    ui->actionLast_package_update_log->setEnabled(en_pkglog);
    bool en_pbiglog= (mPBIController.currentState() != CAbstractUpdateController::eUPDATING) && mPBIController.hasLog();
    ui->actionLast_software_update_log->setEnabled(en_pbiglog);

    bool is_no_upd = (mSysController.currentState() != CAbstractUpdateController::eUPDATING)
                  && (mPkgController.currentState() != CAbstractUpdateController::eUPDATING)
                  && (mPBIController.currentState() != CAbstractUpdateController::eUPDATING);

    ui->actionJail->setEnabled(is_no_upd && (!misRegularUser));
    ui->actionSystem_branches->setEnabled(is_no_upd && (!mJail.jailEnabled()) && (!misRegularUser));
    ui->actionUpdate_set->setEnabled(is_no_upd && (!misRegularUser));
    ui->actionExit->setEnabled(is_no_upd);
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::slotSingleInstance()
{    
    this->hide();
    this->showNormal();
    //setWindowState( (windowState() & ~Qt::WindowMinimized ) | Qt::WindowActive );
    this->activateWindow();
    this->raise();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::globalStateChanged(CAbstractUpdateController::EUpdateControllerState new_state)
{
    refreshMenu();    

    bool isUpdatesAvail= (mSysController.currentState() == CAbstractUpdateController::eUPDATES_AVAIL)
                       ||(mPkgController.currentState() == CAbstractUpdateController::eUPDATES_AVAIL)
                       ||(mPBIController.currentState() == CAbstractUpdateController::eUPDATES_AVAIL);

    ui->updateAllButton->setEnabled(isUpdatesAvail);

    bool isFullyUpdate= (mSysController.currentState() == CAbstractUpdateController::eFULLY_UPDATED)
            &&(mPkgController.currentState() == CAbstractUpdateController::eFULLY_UPDATED)
            &&(mPBIController.currentState() == CAbstractUpdateController::eFULLY_UPDATED);

    ui->mainStatesStack->setCurrentIndex((isFullyUpdate)?MAIN_FULLY_UPDATED_IDX:MAIN_INDICATORS_IDX);

    switch(mSysController.currentState())
    {
        case CAbstractUpdateController::eUPDATES_AVAIL:
        case CAbstractUpdateController::eUPDATING:
            ui->mainTab->setTabEnabled(TOOLBOX_SYS_INDEX, true);
            break;
        case CAbstractUpdateController::eUPDATING_ERROR:
            break;
        default:
            if (ui->mainTab->currentIndex() == TOOLBOX_SYS_INDEX)
                ui->mainTab->setCurrentIndex(TOOLBOX_MAIN_INDEX);
            ui->mainTab->setTabEnabled(TOOLBOX_SYS_INDEX, false);
            break;
    }

    switch(mPkgController.currentState())
    {
        case CAbstractUpdateController::eUPDATES_AVAIL:
        case CAbstractUpdateController::eUPDATING:
            ui->mainTab->setTabEnabled(TOOLBOX_PKG_INDEX, true);
            break;
        case CAbstractUpdateController::eUPDATING_ERROR:
            break;
        default:
            if (ui->mainTab->currentIndex() == TOOLBOX_PKG_INDEX)
                ui->mainTab->setCurrentIndex(TOOLBOX_MAIN_INDEX);
            ui->mainTab->setTabEnabled(TOOLBOX_PKG_INDEX, false);
            break;
    }

    switch(mPBIController.currentState())
    {
        case CAbstractUpdateController::eUPDATES_AVAIL:
        case CAbstractUpdateController::eUPDATING:
            ui->mainTab->setTabEnabled(TOOLBOX_PBI_INDEX, true);
            break;
        case CAbstractUpdateController::eUPDATING_ERROR:
            break;
        default:
            if (ui->mainTab->currentIndex() == TOOLBOX_PBI_INDEX)
                ui->mainTab->setCurrentIndex(TOOLBOX_MAIN_INDEX);
            ui->mainTab->setTabEnabled(TOOLBOX_PBI_INDEX, false);
            break;
    }
    Q_UNUSED(new_state)
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::slotReturnToHost()
{
    mJail.setJailEnabled(false);
    jailRefresh();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_updateAllButton_clicked()
{
    if (mSysController.currentState() == CAbstractUpdateController::eUPDATES_AVAIL)
        mSysController.updateAll();
    if (mPkgController.currentState() == CAbstractUpdateController::eUPDATES_AVAIL)
        mPkgController.updateAll();
    if (mPBIController.currentState() == CAbstractUpdateController::eUPDATES_AVAIL)
        mPBIController.updateAll();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_pushButton_clicked()
{
    QMessageBox::information(this, "Early beta","<b>This is early beta only for testing</b><br><br>Missed functionality:<br> <b>1.System updates parsing unfinished</b><br>2.Warden support untested<br>2.Some error handling<br><br>Please report bugs at http://trac.pcbsd.org<br> or on testing@lists.pcbsd.org");
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_checkAllButton_clicked()
{
    mSysController.check();
    mPkgController.check();
    mPBIController.check();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionLast_system_update_log_triggered()
{
    LogViewDialog* dlg = new LogViewDialog(this);
    dlg->showLog(&mSysController);
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionLast_package_update_log_triggered()
{
    LogViewDialog* dlg = new LogViewDialog(this);
    dlg->showLog(&mPkgController);
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionLast_software_update_log_triggered()
{
    LogViewDialog* dlg = new LogViewDialog(this);
    dlg->showLog(&mPBIController);
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionExit_triggered()
{
    bool isUpdate= (mSysController.currentState() == CAbstractUpdateController::eUPDATING)
                 ||(mPkgController.currentState() == CAbstractUpdateController::eUPDATING)
                 ||(mPBIController.currentState() == CAbstractUpdateController::eUPDATING);
    if (isUpdate)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Update process not finished"), tr("One or more update process is not finished. Exit anyway?"),
                                        QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }
    }
    QApplication::exit();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionJail_triggered()
{
    JailsDialog* dlg = new JailsDialog(this);
    dlg->execDialog(&mJail);
    jailRefresh();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionSystem_branches_triggered()
{
    BranchesDialog* dlg = new BranchesDialog(this);
    dlg->exec();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionUpdate_set_triggered()
{
    PatchsetDialog* dlg = new PatchsetDialog(this);
    if (dlg->execDilog(mJail))
    {
        mSysController.check();
        mPkgController.check();
        mPBIController.check();
    }
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionBase_system_update_log_triggered()
{
    UpdateHistoryDialog* dlg = new UpdateHistoryDialog(this);
    dlg->execDialog(&mSysController);
}
