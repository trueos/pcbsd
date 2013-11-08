#include "indicatorwidget.h"
#include "ui_indicatorwidget.h"

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

}

void IndicatorWidget::stateChanged(CAbstractUpdateController::EUpdateControllerState new_state)
{

}

void IndicatorWidget::progress(CAbstractUpdateController::SProgress progress)
{

}
