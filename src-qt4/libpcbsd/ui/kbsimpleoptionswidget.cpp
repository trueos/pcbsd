/**************************************************************************
*   Copyright (C) 2014 by Yuri Momotyuk                                   *
*   yurkis@pcbsd.org                                                      *
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

#include "kbsimpleoptionswidget.h"
#include "ui_kbsimpleoptionswidget.h"

using namespace pcbsd::keyboard;

///////////////////////////////////////////////////////////////////////////////
KbSimpleOptionsWidget::KbSimpleOptionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KbSimpleOptionsWidget)
{
    ui->setupUi(this);
    setupUI(currentSettings());
}

///////////////////////////////////////////////////////////////////////////////
KbSimpleOptionsWidget::~KbSimpleOptionsWidget()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////
void KbSimpleOptionsWidget::setSettings(KeyboardSettings ks)
{
    setupUI(ks);
}

///////////////////////////////////////////////////////////////////////////////
void KbSimpleOptionsWidget::mergeSettings(KeyboardSettings &ks)
{
    ks.removeOption(OPTION_SWITCH_ALT_SHIFT);
    ks.removeOption(OPTION_SWITCH_CTRL_SHIFT);
    ks.removeOption(OPTION_SWITCH_ALT_SPACE);
    ks.removeOption(OPTION_ALLOW_TERMINATE_X);

    if ( ui->klsAltShift->isChecked())
    {
        ks.addOption(OPTION_SWITCH_ALT_SHIFT);
    }
    else if(ui->klsCtrlShift->isChecked())
    {
        ks.addOption(OPTION_SWITCH_CTRL_SHIFT);
    }
    else if(ui->klsAltSpace->isChecked())
    {
        ks.addOption(OPTION_SWITCH_ALT_SPACE);
    }

    if(ui->ktermXorg->isChecked())
    {
        ks.addOption(OPTION_ALLOW_TERMINATE_X);
    }
}

///////////////////////////////////////////////////////////////////////////////
void KbSimpleOptionsWidget::setupUI(KeyboardSettings ks)
{    
    bool isAltShift = ks.hasOption(OPTION_SWITCH_ALT_SHIFT);
    bool isCtrlShift =ks.hasOption(OPTION_SWITCH_CTRL_SHIFT);
    bool isAltSpace = ks.hasOption(OPTION_SWITCH_ALT_SPACE);
    bool isTerminate =ks.hasOption(OPTION_ALLOW_TERMINATE_X);

    if (isAltShift || isCtrlShift || isAltSpace)
    {
        ui->klsAltShift->setChecked(isAltShift);
        ui->klsCtrlShift->setChecked(isCtrlShift);
        ui->klsAltSpace->setChecked(isAltSpace);
    }
    ui->ktermXorg->setChecked(isTerminate);
}
