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

#include "patchsetdialog.h"
#include "ui_patchsetdialog.h"

#include "pcbsd-utils.h"

#include <QDebug>
#include <QFile>
#include <QMessageBox>

using namespace pcbsd;

const char* const PATCHSET_CONFIG_FILE = "/usr/local/share/pcbsd/pc-updatemanager/conf/sysupdate.conf";
const char* const PATCHSET_FIELD_NAME = "PATCHSET:";
const char* const PATCHSET_GENERAL = "updates";
const char* const PATCHSET_TEST = "test-updates";

///////////////////////////////////////////////////////////////////////////////
PatchsetDialog::PatchsetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PatchsetDialog)
{
    ui->setupUi(this);
}

///////////////////////////////////////////////////////////////////////////////
PatchsetDialog::~PatchsetDialog()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////
bool PatchsetDialog::execDilog(QString jailPrefix)
{
    bool ret_val = false;
    mFileName = PATCHSET_CONFIG_FILE;

    // If jail is enabled - add jail prefix
    mFileName= jailPrefix + mFileName;

    if (!QFile::exists(mFileName))
    {
        QMessageBox::critical(this, "File not found", tr("File %1 was not found").arg(mFileName));
        return false;
    }

    // Parse config file
    mCurrentPatchset= Utils::getConfFileValue(mFileName, PATCHSET_FIELD_NAME).trimmed();

    if (mCurrentPatchset == QString(PATCHSET_TEST))
    {
        ui->testPatchsetRB->setChecked(true);
    }
    else
    {
        ui->generalPatchsetRB->setChecked(true);
    }

    if (exec() == QDialog::Accepted)
    {
        QString new_patchset= (ui->testPatchsetRB->isChecked())?PATCHSET_TEST:PATCHSET_GENERAL;

        if (new_patchset != mCurrentPatchset)
        {
            Utils::setConfFileValue(mFileName, PATCHSET_FIELD_NAME, QString(PATCHSET_FIELD_NAME) + QString(" ") + new_patchset);
            ret_val = true;
        }
    }
    return ret_val;
}
