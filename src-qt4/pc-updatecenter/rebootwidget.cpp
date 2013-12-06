#include "rebootwidget.h"
#include "ui_rebootwidget.h"

#include <QProcess>
#include <QDebug>
#include "utils.h"

__string_constant REBOOT_IMAGE = ":/images/system-reboot.png";
__string_constant LOGOFF_IMAGE = ":/images/system-log-out.png";

RebootWidget::RebootWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RebootWidget)
{
    ui->setupUi(this);
    mSysController= NULL;
    mPkgController= NULL;
    mPBIController = NULL;
    misReboot= false;
    misLogoff = false;
}

RebootWidget::~RebootWidget()
{
    delete ui;
}

void RebootWidget::init(CSysController *sys_controller, CPkgController *pkg_controller, CPBIController *pbi_controller)
{
    mSysController = sys_controller;
    mPkgController = pkg_controller;
    mPBIController = pbi_controller;

    if (mSysController)
        connect(mSysController, SIGNAL(stateChanged(CAbstractUpdateController::EUpdateControllerState)),
                this, SLOT(globalStateChanged(CAbstractUpdateController::EUpdateControllerState)));
    if (mPkgController)
        connect(mPkgController, SIGNAL(stateChanged(CAbstractUpdateController::EUpdateControllerState)),
                this, SLOT(globalStateChanged(CAbstractUpdateController::EUpdateControllerState)));
    if (mPBIController)
        connect(mPBIController, SIGNAL(stateChanged(CAbstractUpdateController::EUpdateControllerState)),
                this, SLOT(globalStateChanged(CAbstractUpdateController::EUpdateControllerState)));

}

void RebootWidget::on_restartButton_clicked()
{
    QProcess::startDetached(QString("shutdown"),QStringList()<<QString("-r")<<QString("now"));
}

void RebootWidget::globalStateChanged(CAbstractUpdateController::EUpdateControllerState new_state)
{
    Q_UNUSED(new_state);

    bool is_reboot_btn = false;
    if ((!mSysController) || (!mPkgController) || (!mPBIController))
        return;        

    is_reboot_btn = (mSysController->currentState() != CAbstractUpdateController::eUPDATING)
                 && (mPkgController->currentState() != CAbstractUpdateController::eUPDATING)
                 && (mPBIController->currentState() != CAbstractUpdateController::eUPDATING)
                 && mSysController->rebootRequired();

    ui->restartButton->setVisible(is_reboot_btn);

    bool misReboot = mSysController->rebootRequired() && (mSysController->currentState() != CAbstractUpdateController::eUPDATING);
    bool misLogoff = mPkgController->logoffRequired() && (mPkgController->currentState() != CAbstractUpdateController::eUPDATING);

    setVisible((misReboot) || (misLogoff));

    if (misLogoff && (!misReboot))
    {
        ui->IconLabel->setPixmap(QPixmap(LOGOFF_IMAGE));
        ui->messageLabel->setText(tr("Please re-enter to desktop environment to prevent unexpected behaviour"));
    }
    if (misReboot)
    {
        ui->IconLabel->setPixmap(QPixmap(REBOOT_IMAGE));
        ui->messageLabel->setText(tr("Your system should be restarted to finalize update"));
    }
}
