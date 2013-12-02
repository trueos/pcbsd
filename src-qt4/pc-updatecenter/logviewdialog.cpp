#include <QFileDialog>
#include <QStringList>
#include <QFile>
#include <QTextStream>

#include "logviewdialog.h"
#include "ui_logviewdialog.h"

LogViewDialog::LogViewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogViewDialog)
{
    ui->setupUi(this);
}

LogViewDialog::~LogViewDialog()
{
    delete ui;
}

void LogViewDialog::showLog(CAbstractUpdateController *controller)
{
    if (!controller)
        return;

    if (controller->hasLog())
    {
        ui->logView->clear();
        QStringList log = controller->updateLog();
        for (int i=0 ; i<log.size() ; i++)
            ui->logView->append(log[i]);
        ui->logView->selectAll();
        show();
    }
}

void LogViewDialog::on_cclippboardBtn_clicked()
{
    ui->logView->copy();
}

void LogViewDialog::on_logView_copyAvailable(bool b)
{
    ui->cclippboardBtn->setEnabled(b);
}

void LogViewDialog::on_sfileBtn_clicked()
{
    QString file_name= QFileDialog::getSaveFileName(this, tr("Save log file"), "update_log.txt", tr("Text file (*.txt"));
    if (!file_name.length())
        return;
    QFile file(file_name);
    if (!file.isOpen())
    {
        return;
    }
    QTextStream ts(&file);
    ts<<ui->logView->toPlainText();

    file.close();

}
