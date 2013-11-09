#include "indicatorwidget.h"
#include "ui_indicatorwidget.h"

#include <QPicture>

IndicatorWidget::IndicatorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IndicatorWidget)
{
    ui->setupUi(this);
}

IndicatorWidget::~IndicatorWidget()
{
    delete ui;
}

bool IndicatorWidget::init(QString check_img, QString ok_img,
                           QString avail_img, QString process_img,
                           CAbstractUpdateController *upd_controller)
{
    if (!upd_controller)
        return false;

    mCheckImage= check_img;
    mOkImage= ok_img;
    mAvailImage= avail_img;
    mProcessImage= process_img;
    mpUC = upd_controller;
    stateChanged(mpUC->curentState());

    connect(mpUC, SIGNAL(stateChanged(AbstractUpdateController::EUpdateControllerState)),
            this, SLOT(AbstractUpdateController::EUpdateControllerState));

    return true;
}

void IndicatorWidget::stateChanged(CAbstractUpdateController::EUpdateControllerState new_state)
{
    switch(new_state)
    {
        case CAbstractUpdateController::eCHECKING:
            onCheck();
            break;

        case CAbstractUpdateController::eFULLY_UPDATED:
            onFullyUpdated();
            break;

        case CAbstractUpdateController::eUPDATES_AVAIL:
            onUpdateAvail();
            break;

        case CAbstractUpdateController::eUPDATING:
            onUpdateProgress();
            break;

        default:
            //FIXME: Error maybe?
            onFullyUpdated();
            break;
    }
}

void IndicatorWidget::progress(CAbstractUpdateController::SProgress progress)
{
    Q_UNUSED(progress);
}

void IndicatorWidget::onCheck()
{
    ui->progress->setVisible(false);
    ui->checkButton->setVisible(false);
    ui->detailsButton->setVisible(false);

    ui->stateImage->setPixmap(QPixmap(mCheckImage));

    ui->msgLabel->setText(tr("Checking for updates..."));
}

void IndicatorWidget::onUpdateAvail()
{
    ui->progress->setVisible(false);
    ui->checkButton->setVisible(true);
    ui->detailsButton->setVisible(true);

    ui->stateImage->setPixmap(QPixmap(mAvailImage));

    if (mpUC)
        ui->msgLabel->setText(mpUC->updateMessage());

}

void IndicatorWidget::onFullyUpdated()
{
    ui->progress->setVisible(false);
    ui->checkButton->setVisible(true);
    ui->detailsButton->setVisible(false);

    ui->stateImage->setPixmap(QPixmap(mOkImage));
    ui->msgLabel->setText(tr("Is up to date!"));
}

void IndicatorWidget::onUpdateProgress()
{

}
