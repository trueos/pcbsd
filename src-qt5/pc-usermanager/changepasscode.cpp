/***************************************************************************
 *   Copyright (C) 2007 by Tim McCormick   *
 *   tim@pcbsd.org   *
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
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. *
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qtooltip.h>
#include <QMessageBox>
        
#include "changepasscode.h"

void ChangePassCode::programInit(QString username, UserManagerBackend *back)
{
    this->username = username;
    this->back = back;
    
    white = QColor(255, 255, 255);
    red = QColor(255, 78, 78);
    
    setWindowTitle(tr("Change Password for '") + username + tr("'"));
    
    connect(okButton, SIGNAL(clicked()), this, SLOT(commit()));
    connect(passBox, SIGNAL(textChanged(const QString&)), this, SLOT(passwordChanged()));
    connect(confirmPassBox, SIGNAL(textChanged(const QString&)), this, SLOT(passwordChanged()));
}

void ChangePassCode::commit()
{
    int valid = back->validatePassword(passBox->text(), confirmPassBox->text()); //0 = OK, 1 = Empty, 2 = No match, 3 = Invalid input
    
    switch(valid)
    {
        case 0:
            back->changePassword(username, passBox->text());
            close();
            break;
        case 1:
            QMessageBox::critical(this, tr("You must specify a password"), tr("You must specify a password"));
            break;
        case 2:
            QMessageBox::critical(this, tr("The passwords didn't match. Both boxes most contain the same value."), tr("The passwords didn't match. Both boxes most contain the same value."));
            break;
        case 3:
            QMessageBox::critical(this, tr("Invalid password, please choose another."), tr("Invalid password, please choose another."));
            break;
    }
}

void ChangePassCode::passwordChanged()
{
    int passError = back->validatePassword(passBox->text(), confirmPassBox->text());
    
    QColor passBoxColour = white;
    passBox->setToolTip(tr("Passwords match"));
    
    switch (passError)
    {
    case 1:
	passBox->setToolTip("");
	break;
    case 2:
        passBoxColour = red;
	passBox->setToolTip(tr("Passwords do not match"));
	break;
    case 3:
        passBoxColour = red;
        passBox->setToolTip(tr("Invalid input"));
    }
    
    QPalette bgPal(passBoxColour);
    bgPal.setColor(QPalette::Window, passBoxColour);
    passBox->setPalette(bgPal);
}

