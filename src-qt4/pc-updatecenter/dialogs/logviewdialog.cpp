/**************************************************************************
*   Copyright (C) 2013- by Yuri Momotyuk                                   *
*   yurkis@gmail.com                                                      *
*                                                                         *
*   Permission is hereby granted, free of charge, to any person obtaining *
*   a copy of this software and associated documentation files (the       *
*   "Software"), to deal in the Software without restriction, including   *
*   without limitation the rights to use, copy, modify, merge, publish,   *
*   distribute, sublicense, and/or sell copies of the Software, and to    *
*   permit persons to whom the Software is furnished to do so, subject to *
*   the following conditions:                                             *
*                                                                         *
*   The above copyright notice and this permission notice shall be        *
*   included in all copies or substantial portions of the Software.       *
*                                                                         *
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
*   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
*   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*
*   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
*   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
*   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
*   OTHER DEALINGS IN THE SOFTWARE.                                       *
***************************************************************************/

#include <QFileDialog>
#include <QStringList>
#include <QFile>
#include <QTextStream>

#include "logviewdialog.h"
#include "ui_logviewdialog.h"

///////////////////////////////////////////////////////////////////////////////
LogViewDialog::LogViewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogViewDialog)
{
    ui->setupUi(this);
}

///////////////////////////////////////////////////////////////////////////////
LogViewDialog::~LogViewDialog()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////
void LogViewDialog::on_cclippboardBtn_clicked()
{
    ui->logView->copy();
}

///////////////////////////////////////////////////////////////////////////////
void LogViewDialog::on_logView_copyAvailable(bool b)
{
    ui->cclippboardBtn->setEnabled(b);
}

///////////////////////////////////////////////////////////////////////////////
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
