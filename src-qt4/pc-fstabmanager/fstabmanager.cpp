#include "fstabmanager.h"
#include "ui_fstabmanager.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

FstabManager::FstabManager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FstabManager)
{
    ui->setupUi(this);
}

FstabManager::~FstabManager()
{
    delete ui;
}

void FstabManager::on_pushButton_3_clicked()
{
    QFile file("/etc/fstab");
    if(!file.open(QIODevice::ReadOnly))
        QMessageBox::information(0,"info",file.errorString());

    QTextStream in(&file);

    ui->textBrowser->setText(in.readAll());
}
