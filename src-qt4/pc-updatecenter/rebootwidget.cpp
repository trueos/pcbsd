#include "rebootwidget.h"
#include "ui_rebootwidget.h"

#include <QProcess>

RebootWidget::RebootWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RebootWidget)
{
    ui->setupUi(this);
    mSysController= NULL;
    mPkgController= NULL;
    mPBIController = NULL;
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

    bool this_visible = mSysController->rebootRequired() && (mSysController->currentState() != CAbstractUpdateController::eUPDATING);
    setVisible(this_visible);
}
