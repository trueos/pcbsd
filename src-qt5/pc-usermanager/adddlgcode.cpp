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
#include "adddlgcode.h"
        
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qtooltip.h>
#include <QMessageBox>
        
void AddDlgCode::programInit(UserManagerBackend *back)
{
    this->back = back;
    white = QColor(255, 255, 255);
    red = QColor(255, 78, 78);
    orange = QColor(255, 211, 65);
    
    connect(groupRadioOld, SIGNAL(clicked()), this, SLOT(changeGroupBox()));
    connect(groupRadioNew, SIGNAL(clicked()), this, SLOT(changeGroupBox()));
    connect(usernameBox, SIGNAL(textChanged(const QString&)), this, SLOT(usernameChanged()));
    connect(passBox, SIGNAL(textChanged(const QString&)), this, SLOT(passwordChanged()));
    connect(confirmPassBox, SIGNAL(textChanged(const QString&)), this, SLOT(passwordChanged()));
    connect(fullnameBox, SIGNAL(textChanged(const QString&)), this, SLOT(fullnameChanged()));
    connect(homeBox, SIGNAL(textChanged(const QString&)), this, SLOT(homeChanged()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(submit()));
    
    //Populate shell box
    QStringList shells = back->getShells();
    shells.sort();
    shellBox->clear();
    shellBox->addItems(shells);
    
    //Populate group box
    QStringList groups = back->getAllGroups(-1, -1);
    groups.sort();
    groupBox->clear();
    groupBox->addItems(groups);
    
}

void AddDlgCode::changeGroupBox()
{
    if ( groupRadioOld->isChecked() ) { groupBox->setEnabled(true); }
    else { groupBox->setEnabled(false); }
}

void AddDlgCode::usernameChanged()
{
    QString username = usernameBox->text().toLower();
    usernameBox->setText(username);
    
    QString groupText = tr("New Group");
    if (username != "")
    {
	groupText+=" (";
	groupText+=username;
	groupText+=")";
    }
    groupRadioNew->setText(groupText);
    
    QString homeDir = "/usr/home/";
    homeDir+=username;
    if (username != "") {
	homeDir+="/";
    }
    homeBox->setText(homeDir);
    
    //Validate
    int usernameError = back->validateUsername(username);
    
    QColor usernameBoxColour = white;
    usernameBox->setToolTip(tr("Good Input"));
    
    switch (usernameError) 
    {
    case 1:
	usernameBox->setToolTip( tr("The field is empty"));
	break;
    case 2:
	usernameBoxColour = red;
	usernameBox->setToolTip(tr("This field can only accept letters and numbers"));
	break;
    case 3:
	usernameBoxColour = red;
	usernameBox->setToolTip(tr("This username is already in use, please choose another."));
    }
    
    QPalette bgPal(usernameBoxColour);
    bgPal.setColor(QPalette::Window,usernameBoxColour);
    usernameBox->setPalette(bgPal);

}

void AddDlgCode::passwordChanged()
{
    int passError = back->validatePassword(passBox->text(), confirmPassBox->text());
    
    QColor passBoxColour = white;
    passBox->setToolTip( tr("Passwords match"));
    
    switch (passError)
    {
    case 1:
	passBox->setToolTip( "");
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

void AddDlgCode::fullnameChanged()
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

void AddDlgCode::homeChanged()
{
    int homeError = back->validateHome(homeBox->text());
    
    QColor homeBoxColour = white;
    homeBox->setToolTip(tr("Directory will be used"));
    
    switch (homeError)
    {
    case 1:
	homeBox->setToolTip( tr("The field is empty"));
	break;
    case 2:
	homeBoxColour = red;
	homeBox->setToolTip(tr("Invalid Path!"));
	break;
    case 3:
	homeBox->setToolTip( tr("Directory does not exist, but will be made"));
	break;
    case 4:
	homeBoxColour = orange;
	homeBox->setToolTip(tr("Placing home directory outside '/home/' can cause compatibility problems with some programs. NOT RECOMMENDED"));
	break;
    case 5:
	homeBoxColour = red;
	homeBox->setToolTip(tr("Requested path is a file (not a directory!)"));
	break;
    }
    
    QPalette bgPal(homeBoxColour);
    bgPal.setColor(QPalette::Window,homeBoxColour);
    homeBox->setPalette(bgPal);

}

void AddDlgCode::submit()
{
    //Validate all data fields, prompt with errors or create the new user as requested
    bool inputError = false;
    bool warnError = false;
    QString inputText = tr("The following errors occurred while trying to add a new user:\n");
    QString warnText = tr("The following non-critical errors occurred while trying to add a new user, do you wish to continue?\n");
    
    //Username validation
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
    
    //Home directory validation
    QString home = homeBox->text();
    int homeError = back->validateHome(home);
	    
    switch (homeError)
    {
    case 1:
	inputError = true;
	inputText += "\n" + tr("- The 'Home Directory' field has not been filled in.");
	break;
    case 2:
	inputError = true;
	inputText += "\n" + tr("- The value entered as 'Home Directory' is not a valid path.");
	break;
    case 4:
	warnError = true;
	warnText += "\n" + tr("- Placing home directory outside '/home/' can cause compatibility problems with some programs and is NOT RECOMMENDED.");
	break;
    case 5:
	inputError = true;
	inputText += "\n" + tr("- A file was selected as 'Home Directory', this value must be a directory.");
	break;
    }
    
    //Password validation
    QString password = passBox->text();
    QString confirmPassword = confirmPassBox->text();
    
    int passError = back->validatePassword(password, confirmPassword);
    
    switch (passError)
    {
    case 0:
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
       int abortAdd = QMessageBox::Yes;
       if (warnError)
	 abortAdd = QMessageBox::warning(this, tr("Warning!"), warnText, QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

       //Check we didn't abort the operation
       if (abortAdd == QMessageBox::Yes)
       {
	   QString group = groupBox->currentText();
	   if (groupRadioNew->isChecked() == true) { group = QString::null; }
	   back->addUser(username, fullname, home, shellBox->currentText(), group, password);
	   back->setEnc(username, false);
	   back->changePassword(username, password);
       }
   }
}

