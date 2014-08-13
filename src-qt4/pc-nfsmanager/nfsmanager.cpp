#include "nfsmanager.h"
#include "ui_nfsmanager.h"

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
