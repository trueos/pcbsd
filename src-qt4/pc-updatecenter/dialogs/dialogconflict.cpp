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

#include "dialogconflict.h"
#include "ui_dialogconflict.h"

///////////////////////////////////////////////////////////////////////////////
DialogConflict::DialogConflict(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConflict)
{
    ui->setupUi(this);
}

///////////////////////////////////////////////////////////////////////////////
DialogConflict::~DialogConflict()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////
int DialogConflict::exec(QString conflictList, CPkgController *controller)
{
    mController= controller;
    ui->conflictList->append(conflictList);
    return QDialog::exec();
}

///////////////////////////////////////////////////////////////////////////////
void DialogConflict::on_buttonBox_accepted()
{
    if (mController)
    {
        mController->autoResolveConflict(true);
    }
    setResult(QDialog::Accepted);
}

///////////////////////////////////////////////////////////////////////////////
void DialogConflict::on_buttonBox_rejected()
{
    if (mController)
    {
        mController->autoResolveConflict(false);
    }
    setResult(QDialog::Rejected);
}
