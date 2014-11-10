#include "kbsimpleoptionswidget.h"
#include "ui_kbsimpleoptionswidget.h"

KbSimpleOptionsWidget::KbSimpleOptionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KbSimpleOptionsWidget)
{
    ui->setupUi(this);
}

KbSimpleOptionsWidget::~KbSimpleOptionsWidget()
{
    delete ui;
}
