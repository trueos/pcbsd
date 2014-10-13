/***************************************************************************
 *   Copyright (C) 2007, 2012 by Tim McCormick   *
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
#include "simpleaddcode.h"
        
#include <qlineedit.h>
#include <qtooltip.h>
#include <qpushbutton.h>
#include <QMessageBox>
#include <QDebug>
        
void SimpleAddCode::programInit(UserManagerBackend *back)
{
    this->back = back;
    white = QColor(255, 255, 255);
    red = QColor(255, 78, 78);
    
    connect(usernameBox, SIGNAL(textChanged(const QString&)), this, SLOT(usernameChanged()));
    connect(passBox, SIGNAL(textChanged(const QString&)), this, SLOT(passwordChanged()));
    connect(confirmPassBox, SIGNAL(textChanged(const QString&)), this, SLOT(passwordChanged()));
    connect(fullnameBox, SIGNAL(textChanged(const QString&)), this, SLOT(fullnameChanged()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(submit()));
}

void SimpleAddCode::usernameChanged()
{
    QString username = usernameBox->text().toLower();
    usernameBox->setText(username);
    
    //Validate
    int usernameError = back->validateUsername(username);
    
    QColor usernameBoxColour = white;
    usernameBox->setToolTip( tr("Good Input"));
    
    switch (usernameError) 
    {
    case 1:
	usernameBox->setToolTip( tr("The field is empty"));
	break;
    case 2:
	usernameBoxColour = red;
	usernameBox->setToolTip( tr("This field can only accept letters and numbers"));
	break;
    case 3:
	usernameBoxColour = red;
	usernameBox->setToolTip(tr("This username is already in use, please choose another."));
    }
    
    QPalette bgPal(usernameBoxColour);
    bgPal.setColor(QPalette::Window,usernameBoxColour);
    usernameBox->setPalette(bgPal);

}

void SimpleAddCode::passwordChanged()
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
    bgPal.setColor(QPalette::Window,passBoxColour);
    passBox->setPalette(bgPal);

}

void SimpleAddCode::fullnameChanged()
{
    int fullnameError = back->validateFullname(fullnameBox->text());
    
    QColor fullnameBoxColour = white;
    fullnameBox->setToolTip(tr("Good Input"));
    
    switch(fullnameError)
    {
    case 1:
	fullnameBox->setToolTip(tr("The field is empty"));
	break;
    case 2:
	fullnameBoxColour = red;
	fullnameBox->setToolTip(tr("This field may not contain ! @ or :"));
	break;
    }
    
    QPalette bgPal(fullnameBoxColour);
    bgPal.setColor(QPalette::Window,fullnameBoxColour);
    fullnameBox->setPalette(bgPal);
}

void SimpleAddCode::submit()
{
    qDebug() << "starting submit()";

    //Validate all data fields, prompt with errors or create the new user as requested
    bool inputError = false;
    bool warnError = false;
    QString inputText = tr("The following errors occurred while trying to add a new user:\n");
    QString warnText = tr("The following non-critical errors occurred while trying to add a new user, do you wish to continue?\n");
    
    //Username validation
    qDebug() << "before username validation.........";
    QString username = usernameBox->text();
    int usernameError = back->validateUsername(username);
    
    switch (usernameError)
    {
    case 1:
	inputText += tr("\n- The 'Username' field has not been filled in.");
	break;
    case 2:
	inputError = true;
	inputText += tr("\n- The 'Username' field may only contain letters and numbers (not spaces).");
	break;
    case 3:
	inputError = true;
	inputText += tr("\n- The username you have chosen is already in use, please choose another.");
	break;
    }
    

    //Full name validation
    qDebug() << "before full name validation.........";
    QString fullname = fullnameBox->text();
    int fullnameError = back->validateFullname(fullname);
    switch (fullnameError)
    {
    case 1:
	inputError = true;
	inputText += tr("\n- The 'Full Name' field has not been filled in.");
	break;
    case 2:
	inputError = true;
	inputText += tr("\n- The 'Full Name' field may not contain ! @ or :");
	break;
    }
    
    //Password validation
    qDebug() << "before password validation.........";
    QString password = passBox->text();
    QString confirmPassword = confirmPassBox->text();
    
    int passError = back->validatePassword(password, confirmPassword);
    qDebug() << "after password validation.........";
    
    switch (passError)
    {
    case 0:
        qDebug() << "Case 0 was found!!!........." + username; //+ " pass: " + password;
        //Do NOT show the password on comitted versions - this can get saved to a log file on the system (~/.xsession-errors)
    
	//back->changePassword(username, password);
	close();
	break;
    case 1:
	inputError = true;
	inputText += tr("\n- Both password fields must be filled in.");
	break;
    case 2:
	inputError = true;
	inputText += tr("\n- Passwords don't match, please try again.");
	break;
    case 3:
	inputError = true;
	inputText += tr("\n- Invalid password");
	break;
    }
    
   //Throw up error or continue

   if (inputError)
   {
       //Throw up the error box!
       QMessageBox::critical(this, tr("Error!"), inputText);
   }
   else
   {
       //If we need to issue a warning, do it, only continue if the user wishes it to be so.
       int abortAdd = 0;
       if (warnError)
       {
	   abortAdd = QMessageBox::warning(this, tr("Warning!"), warnText, QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
       }
       //Check we didn't abort the operation
       if (abortAdd == 0)
       {
           qDebug() << "Before back->addUser";
	   back->addUser(username, fullname, "/usr/home/" + username, "/bin/csh", QString::null, password);
	   back->setEnc( username, false );
	   back->changePassword(username, password);
           qDebug() << "After back->addUser";
       }
   }
}

