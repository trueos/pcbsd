#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialogs/kblayoutselectdialog.h"

#include <QDebug>
#include <QProcess>

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

void MainWindow::updateSettings()
{
    ks.clearLayouts();
    ks.clearOptions();
    int idx = ui->kbModelCB->currentIndex();
    QVariant udata = ui->kbModelCB->itemData(idx);
    QString kb_model = udata.value<QString>();
    ks.setKeyboardModel(kb_model);
    ui->layoutsWidget->mergeSettings(ks);
    if (ui->optionsStack->currentIndex() == 0)
    {
        ui->simpleOptsWidget->mergeSettings(ks);
    }
    else
    {
        ui->optsWidget->mergeSettings(ks);
    }
}


void MainWindow::on_advancedViewBtn_clicked()
{
   ui->simpleOptsWidget->mergeSettings(ks);
   ui->optsWidget->setSettings(ks);
   ui->optionsStack->setCurrentIndex(1);
}

void MainWindow::on_simpleViewBtn_clicked()
{
    ui->optsWidget->mergeSettings(ks);
    ui->simpleOptsWidget->setSettings(ks);
    ui->optionsStack->setCurrentIndex(0);
}

void MainWindow::on_applyBtn_clicked()
{
    updateSettings();
    QProcess::execute(QString("setxkbmap ") + ks.xkbString());
    qDebug()<<ks.xkbString();
}
