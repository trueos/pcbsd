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

#include "branchesdialog.h"
#include "ui_branchesdialog.h"

#include <QListWidgetItem>
#include <QFont>

#include "pcbsd-utils.h"
#include "utils.h"

__string_constant BRANCHES_SEPARATOR = "----------------------------------";

///////////////////////////////////////////////////////////////////////////////
BranchesDialog::BranchesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BranchesDialog)
{
    ui->setupUi(this);
    init();
}

///////////////////////////////////////////////////////////////////////////////
BranchesDialog::~BranchesDialog()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////
void BranchesDialog::init()
{
    QStringList upd_output = pcbsd::Utils::runShellCommand("pc-updatemanager branches");
    bool is_branches_separator = false;
    for (int i=0; i<upd_output.size(); i++)
    {
        QString line = upd_output[i].trimmed();
        if (line.indexOf(BRANCHES_SEPARATOR) == 0)
        {
            is_branches_separator = true;
            continue;
        }
        if (is_branches_separator)
        {
            if (!line.length())
                break;

            QListWidgetItem* item = new QListWidgetItem(ui->branchesL);

            ui->branchesL->addItem(item);

            if (line[line.length()-1] == '*')
            {
                //current branch
                QFont font = item->font();
                font.setBold(true);
                item->setFont(font);
                line = line.replace(" *", "");
                ui->branchesL->setCurrentItem(item);
            }//if current branch
            item->setText(line);
        }//if branches list
    }//for all lines
}
