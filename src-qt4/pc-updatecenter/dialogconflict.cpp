#include "dialogconflict.h"
#include "ui_dialogconflict.h"

DialogConflict::DialogConflict(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConflict)
{
    ui->setupUi(this);
}

DialogConflict::~DialogConflict()
{
    delete ui;
}

int DialogConflict::exec(QString conflictList, CPkgController *controller)
{
    mController= controller;
    ui->conflictList->append(conflictList);
    return QDialog::exec();
}

void DialogConflict::on_buttonBox_accepted()
{
    if (mController)
    {
        mController->autoResolveConflict(true);
    }
    setResult(QDialog::Accepted);
}


void DialogConflict::on_buttonBox_rejected()
{
    if (mController)
    {
        mController->autoResolveConflict(false);
    }
    setResult(QDialog::Rejected);
}
