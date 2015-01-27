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
#include "simpledlgcode.h"
#include "user.h"
        
#include <qstringlist.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qtooltip.h>
#include <qcolor.h>
#include <QDebug>
#include "../config.h"

void SimpleDlgCode::programInit(UserManagerBackend *back, QString dir)
{
    this->back = back;
    userImage = QPixmap(PREFIX + "/share/icons/crystalsvg/48x48/kdm/user_male.png");
    systemChange = false;
    chroot = dir;
    
    refreshUsers();
    
    connect(advancedBut, SIGNAL(clicked()), this, SLOT(advancedPressed()));
    connect(userList, SIGNAL(currentTextChanged( const QString & )), this, SLOT(getUserDetails( const QString& )));
    connect(fullnameBox, SIGNAL(textChanged(const QString&)), this, SLOT(fullnameChanged()));
    connect(canAdminCheck, SIGNAL(toggled(bool)), this, SLOT(canAdminChanged(bool)));
    connect(changePasswordBut, SIGNAL(clicked()), this, SLOT(changePassword()));
    connect(rootPasswordBut, SIGNAL(clicked()), this, SLOT(changeRootPassword()));
    connect(removeUserBut, SIGNAL(clicked()), this, SLOT(removeUser()));
    connect(back, SIGNAL(usersChanged()), this, SLOT(refreshUsers()));
    connect(addUserBut, SIGNAL(clicked()), this, SLOT(addUser()));
}

void SimpleDlgCode::refreshUsers()
{
    qDebug("Adding user to userlist:" );

    disconnect(userList, SIGNAL(currentTextChanged( const QString & )), 0, 0);

    QString tmp;
    wheel = back->getGroup(0);
    QStringList users = back->getAllUsers();
    users.sort();
    userList->clear();
    
    qDebug() << "All Users in list:" << users;

    // If no users setup yet, lets diable the details and return
    if ( users.isEmpty() ) {
      frame->setEnabled(false); 
      return;
    }
    frame->setEnabled(true); 
    for ( QStringList::Iterator it = users.begin(); it != users.end(); ++it )
    {
	new QListWidgetItem(userImage, *it + " (" + back->getUser(*it)->getFullname() + ")", userList);
    }
    userList->setCurrentRow(0);
    getUserDetails(userList->currentItem()->text());

    connect(userList, SIGNAL(currentTextChanged( const QString & )), this, SLOT(getUserDetails( const QString& )));
}

void SimpleDlgCode::advancedPressed()
{
    emit advancedView();
}

void SimpleDlgCode::getUserDetails(const QString& username)
{
    qDebug() << "Simple dialog, fetching information for: " << username;
    
    systemChange = true;
    User *user = back->getUser(username.section(" ", 0, 0));
    
    fullnameBox->setText(user->getFullname());
    
    canAdminCheck->setChecked(wheel->isMember(user->getUsername()));
    
    //Do not allow the removal of the currently logged in user
    QString tmp = getenv("SUDO_USER");
    if ( ! tmp.isEmpty() )
      removeUserBut->setEnabled(tmp != user->getUsername());
    else
      removeUserBut->setEnabled(getenv("USER") != user->getUsername());

    if ( user->getEnc() )
      changePasswordBut->setEnabled(false);
    else
      changePasswordBut->setEnabled(true);
    
    systemChange = false;
}

void SimpleDlgCode::fullnameChanged()
{
    User *user = back->getUser(userList->currentItem()->text().section(" ", 0, 0));
    
    int fullnameError = back->validateFullname(fullnameBox->text());
    
    QColor fullnameBoxColour("white");
    fullnameBox->setToolTip(tr("Good Input"));
    
    switch(fullnameError)
    {
    case 1:
	fullnameBox->setToolTip(tr("The field is empty"));
	break;
    case 2:
	fullnameBoxColour.setRed(fullnameBoxColour.red());
	fullnameBox->setToolTip(tr("This field may not contain ! @ or :"));
	break;
    }
    
    if ((! systemChange) && (fullnameError != 2))
    {
        user->setFullname(fullnameBox->text());
        user->setChanges();
        userList->currentItem()->setText(user->getUsername() + " (" + fullnameBox->text() + ")");
        emit changed();
    }
    
    QPalette bgPal(fullnameBoxColour);
    bgPal.setColor(QPalette::Window, fullnameBoxColour);
    fullnameBox->setPalette(bgPal);
}

void SimpleDlgCode::canAdminChanged(bool on)
{
    if (! systemChange)
    {
        QString username = userList->currentItem()->text().section(" ", 0, 0);
        
        if ((! wheel->isMember(username)) && on) { back->addUserToGroup(username, "wheel"); }
        else if (wheel->isMember(username) && (! on)) { back->removeUserFromGroup(username, "wheel"); }
    }
}

void SimpleDlgCode::changePassword()
{
    changePassUi = new ChangePassCode();
    changePassUi->programInit(userList->currentItem()->text().section(" ", 0, 0), back);
    changePassUi->show();
}

void SimpleDlgCode::changeRootPassword()
{
    changePassUi = new ChangePassCode();
    changePassUi->programInit("root", back);
    changePassUi->show();
}

void SimpleDlgCode::removeUser()
{
    if ( ! userList->currentItem() )
        return;

    QString username = userList->currentItem()->text().section(" ", 0, 0);
    if ( username.isEmpty() )
       return;

    User *user = back->getUser(username);
    QString home = user->getHome();
    int answer = QMessageBox::question(this,
                                       tr("Delete Home Directory?"),
                                       tr("Delete the home directory for ") + username + " (" + home + ")?" ,
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::No);
    if(answer == QMessageBox::Yes) 
      user->setDeleteHome(true);
    else
      user->setDeleteHome(false);
    back->deleteUser(username);
}

void SimpleDlgCode::addUser()
{
    addUserUi = new SimpleAddCode();
    addUserUi->programInit(back);
    addUserUi->show();
}

