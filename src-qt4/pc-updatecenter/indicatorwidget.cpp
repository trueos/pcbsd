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
