#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialogconflict.h"

#include <QTreeWidgetItem>
#include <QFile>
#include <QPalette>
#include <QMessageBox>

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

#ifdef CONTROLLER_EMULATION_ENABLED
   // mPkgController.setEmulateCheckFile("/home/yurkis/_pkgcheck.txt");
//    mPkgController.setEmulateUpdateFile("/home/yurkis/_pkgupd.txt");
    //mPkgController.setEmulateDelay(1);
#endif


    mSysController.check();
    mPkgController.check();
    mPBIController.check();    

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


    ui->mainStatesStack->setCurrentIndex(MAIN_INDICATORS_IDX);

}

void MainWindow::globalStateChanged(CAbstractUpdateController::EUpdateControllerState new_state)
{
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

void MainWindow::on_updateAllButton_clicked()
{
    if (mSysController.currentState() == CAbstractUpdateController::eUPDATES_AVAIL)
        mSysController.updateAll();
    if (mPkgController.currentState() == CAbstractUpdateController::eUPDATES_AVAIL)
        mPkgController.updateAll();
    if (mPBIController.currentState() == CAbstractUpdateController::eUPDATES_AVAIL)
        mPBIController.updateAll();
}

void MainWindow::on_pushButton_clicked()
{
    QMessageBox::information(this, "Early beta","<b>This is early beta only for testing</b><br><br>Missed functionality:<br> <b>1.System updates installation</b><br><2.Warden support<br>2.Some error handling<br>3.Last update log view<br><br>Please report bugs at http://trac.pcbsd.org<br> or on testing@mile list");
}


void MainWindow::on_checkAllButton_clicked()
{
    mSysController.check();
    mPkgController.check();
    mPBIController.check();
}
