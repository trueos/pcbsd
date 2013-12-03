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

#include "indicatorwidget.h"
#include "ui_indicatorwidget.h"

#include <QPicture>

///////////////////////////////////////////////////////////////////////////////
IndicatorWidget::IndicatorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IndicatorWidget)
{
    ui->setupUi(this);
}

///////////////////////////////////////////////////////////////////////////////
IndicatorWidget::~IndicatorWidget()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////
bool IndicatorWidget::init(QString check_img, QString ok_img, QString avail_img,
                           QString download_img, QString install_img, QString error_img,
                           CAbstractUpdateController *upd_controller)
{
    if (!upd_controller)
        return false;

    mStateImages[CAbstractUpdateController::eCHECKING]= check_img;
    mStateImages[CAbstractUpdateController::eFULLY_UPDATED]= ok_img;
    mStateImages[CAbstractUpdateController::eUPDATING]= download_img;
    mStateImages[CAbstractUpdateController::eUPDATING_ERROR]= error_img;
    mStateImages[CAbstractUpdateController::eUPDATES_AVAIL]= avail_img;
    mInstallImage= install_img;
    mDownloadImage= download_img;

    mpUC = upd_controller;
    stateChanged(mpUC->currentState());

    connect(mpUC, SIGNAL(stateChanged(CAbstractUpdateController::EUpdateControllerState)),
            this, SLOT(stateChanged(CAbstractUpdateController::EUpdateControllerState)));
    connect(mpUC, SIGNAL(progress(CAbstractUpdateController::SProgress)),
            this, SLOT(progress(CAbstractUpdateController::SProgress)));
    connect(mpUC, SIGNAL(updateError(QString)),
            this, SLOT(updateError(QString)));

    connect(ui->checkButton, SIGNAL(clicked()), mpUC, SLOT(check()));
    connect(ui->installButton , SIGNAL(clicked()), mpUC, SLOT(updateAll()));
    connect(ui->cancelButton , SIGNAL(clicked()), mpUC, SLOT(cancel()));


    return true;
}

///////////////////////////////////////////////////////////////////////////////
void IndicatorWidget::stateChanged(CAbstractUpdateController::EUpdateControllerState new_state)
{
    ui->stateImage->setPixmap(QPixmap(mStateImages[new_state]));
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

         case CAbstractUpdateController::eUPDATING_ERROR:
            onUpdateError();

        default:
            //FIXME: Error maybe?
            onFullyUpdated();
            break;
    }
}

///////////////////////////////////////////////////////////////////////////////
void IndicatorWidget::progress(CAbstractUpdateController::SProgress progress)
{
    ui->progress->setMinimum(progress.mProgressMin);
    ui->progress->setMaximum(progress.mProgressMax);
    ui->progress->setValue(progress.mProgressCurr);
    ui->msgLabel->setText(progress.mMessage);
    ui->cancelButton->setEnabled(progress.misCanCancel);

    QString pict_name= (progress.mSubstate == CAbstractUpdateController::eInstall)? mInstallImage : mDownloadImage;

    ui->stateImage->setPixmap(QPixmap(pict_name));
}

///////////////////////////////////////////////////////////////////////////////
void IndicatorWidget::updateError(QString message)
{
    ui->msgLabel->setText(message);
}

///////////////////////////////////////////////////////////////////////////////
void IndicatorWidget::onCheck()
{
    ui->progress->setVisible(false);
    ui->checkButton->setVisible(false);
    ui->installButton->setVisible(false);
    ui->cancelButton->setVisible(false);

    ui->msgLabel->setText(tr("Checking for updates..."));
}

///////////////////////////////////////////////////////////////////////////////
void IndicatorWidget::onUpdateAvail()
{
    ui->progress->setVisible(false);
    ui->checkButton->setVisible(true);
    ui->installButton->setVisible(true);
    ui->cancelButton->setVisible(false);

    if (mpUC)
        ui->msgLabel->setText(mpUC->updateMessage());

}

///////////////////////////////////////////////////////////////////////////////
void IndicatorWidget::onFullyUpdated()
{
    ui->progress->setVisible(false);
    ui->checkButton->setVisible(true);
    ui->installButton->setVisible(false);
    ui->cancelButton->setVisible(false);

    ui->msgLabel->setText(tr("Is up to date!"));
}

///////////////////////////////////////////////////////////////////////////////
void IndicatorWidget::onUpdateProgress()
{
    ui->progress->setVisible(true);
    ui->progress->setMaximum(0);
    ui->progress->setMinimum(0);
    ui->cancelButton->setVisible(true);
    ui->cancelButton->setEnabled(false);
    ui->checkButton->setVisible(false);
    ui->installButton->setVisible(false);

    ui->msgLabel->setText(tr("Preparing update..."));
}

///////////////////////////////////////////////////////////////////////////////
void IndicatorWidget::onUpdateError()
{
    ui->progress->setVisible(false);
    ui->cancelButton->setVisible(false);
    ui->checkButton->setVisible(true);
    ui->installButton->setVisible(false);
}
