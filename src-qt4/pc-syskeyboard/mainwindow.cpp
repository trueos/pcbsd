#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialogs/kblayoutselectdialog.h"

#include <QDebug>

using namespace pcbsd::keyboard;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ks = currentSettings();
    qDebug()<<pcbsd::keyboard::currentSettings().xkbString();

    fillKbModels();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fillKbModels()
{
    allKbModels = possibleModels();
    for(int i=0; i<allKbModels.size(); i++)
    {
        QVariant udata;
        udata.setValue(allKbModels[i]);
        ui->kbModelCB->addItem(modelDescription(allKbModels[i]), udata);
        if (allKbModels[i] == ks.keyboardModel())
        {
            ui->kbModelCB->setCurrentIndex(ui->kbModelCB->count()-1);
        }
    }
}
