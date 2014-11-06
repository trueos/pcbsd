#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialogs/kblayoutselectdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    KbLayoutSelectDialog* dlg = new KbLayoutSelectDialog(this);
    dlg->setDisableCurrent(true);
    dlg->setCurrent("ua");
    dlg->exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}
