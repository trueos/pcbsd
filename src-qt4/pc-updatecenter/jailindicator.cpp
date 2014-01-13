#include "jailindicator.h"
#include "ui_jailindicator.h"

JailIndicator::JailIndicator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JailIndicator)
{
    ui->setupUi(this);
}

JailIndicator::~JailIndicator()
{
    delete ui;
}

void JailIndicator::setJailName(QString name)
{
    ui->jailNameLabel->setText(name);
}

void JailIndicator::on_returnToHostBtn_clicked()
{
    emit (returnToHost());
}
