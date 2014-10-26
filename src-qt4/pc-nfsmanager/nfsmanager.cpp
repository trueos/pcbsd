#include "nfsmanager.h"
#include "ui_nfsmanager.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

NFSManager::NFSManager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NFSManager)
{
    ui->setupUi(this);
}

NFSManager::~NFSManager()
{
    delete ui;
}

void NFSManager::on_pushButton_3_clicked()
{
    QFile file("/etc/fstab");
    if(!file.open(QIODevice::ReadOnly))
        QMessageBox::information(0,"info",file.errorString());

    QTextStream in(&file);

    ui->textBrowser->setText(in.readAll());
}
