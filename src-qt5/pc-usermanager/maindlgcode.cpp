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
#include "maindlgcode.h"
#include "user.h"
#include "group.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>

#include <qcombobox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qtooltip.h>
        
void mainDlgCode::programInit(UserManagerBackend *back, QString dir)
{
    setEnabled(false);
    
    chroot = dir;

    this->back = back;
    availablePointer = 0;
    memberPointer = 0;
    white = QColor(255, 255, 255);
    red = QColor(255, 78, 78);
    orange = QColor(255, 211, 65);
    systemChange = false;
    
    updateShellList();
    updateGroupList();
    updateUserList();
    
    setEnabled(true);
    
    //Make connections
    connect(groupList, SIGNAL(currentTextChanged( const QString & )), this, SLOT(getGroupDetails( const QString& )));
    connect(newButton, SIGNAL(clicked()), this, SLOT(showAddDlg()));
    connect(back, SIGNAL(usersChanged()), this, SLOT(usersChanged()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(removeUser()));
    connect(makeMemberBut, SIGNAL(clicked()), this, SLOT(makeMember()));
    connect(makeAvailableBut, SIGNAL(clicked()), this, SLOT(makeAvailable()));
    connect(back, SIGNAL(groupsChanged()), this, SLOT(groupsChanged()));
    connect(removeGroupBut, SIGNAL(clicked()), this, SLOT(removeGroup()));
    connect(addGroupBut, SIGNAL(clicked()), this, SLOT(addGroup()));
    connect(passwordButton, SIGNAL(clicked()), this, SLOT(changePassword()));
    connect(fullnameBox, SIGNAL(textChanged(const QString&)), this, SLOT(fullnameChanged()));
    connect(homeBox, SIGNAL(textChanged(const QString&)), this, SLOT(homeChanged()));
    connect(simpleBut, SIGNAL(clicked()), this, SLOT(simplePressed()));
    connect(shellBox, SIGNAL(activated(const QString&)), this, SLOT(shellChanged()));
    connect(groupBox, SIGNAL(activated(const QString&)), this, SLOT(groupChanged()));
}

void mainDlgCode::updateUserList()
{
    qDebug() << "MainDlg: Updating user list";
    disconnect(userList, SIGNAL(currentTextChanged( const QString & )), 0, 0);
    
    QStringList users = back->getAllUsers(-1, -1);
    users.sort();
    userList->clear();
    userList->addItems(users);
    userList->setCurrentRow(0);
    
    getUserDetails(userList->currentItem()->text());
    
    connect(userList, SIGNAL(currentTextChanged( const QString & )), this, SLOT(getUserDetails( const QString& )));
    
    //These won't always invoke first time without these lines
    fullnameChanged();
    homeChanged();
}

void mainDlgCode::updateShellList()
{
    qDebug() << "MainDlg: Updating shell list";
    QStringList shells = back->getShells();
    shells.sort();
    shellBox->clear();
    shellBox->addItems(shells);
}

void mainDlgCode::updateGroupList()
{
    qDebug() << "MainDlg: Updating group list";
    disconnect(groupList, SIGNAL(currentTextChanged( const QString & )), 0, 0);

    QStringList groups = back->getAllGroups(-1, -1);
    groups.sort();
    groupBox->clear();
    groupBox->addItems(groups);
    groupList->clear();
    groupList->addItems(groups);
    groupList->setCurrentRow(0);

    getGroupDetails(groupList->currentItem()->text());

    qDebug() << "MainDlg: Done with updateGroupList()";
    connect(groupList, SIGNAL(currentTextChanged( const QString & )), this, SLOT(getGroupDetails( const QString& )));
}

void mainDlgCode::enableEdits(bool enable)
{
    deleteButton->setEnabled(enable);
    fullnameBox->setEnabled(enable);
    homeBox->setEnabled(enable);
    groupBox->setEnabled(enable);
}

void mainDlgCode::getUserDetails(const QString &username)
{
    systemChange = true;
    
    qDebug() << "Fetching details for user: " << username;
    User *user = back->getUser(username);
    
    //Disable certain form fields if the user is root, to stop changing of important fields
    enableEdits(user->getUid() != 0);
    //Do not allow the removal of the currently logged in user
    QString tmp = getenv("SUDO_USER");
    if ( ! tmp.isEmpty() )
      deleteButton->setEnabled(tmp != username);
    else
      deleteButton->setEnabled(getenv("USER") != username);

    if ( user->getEnc() )
      passwordButton->setEnabled(false);
    else
      passwordButton->setEnabled(true);
    
    QString uid = QString::number(user->getUid());
    if (uid == "-1") { uid = tr("New User"); }
    uidBox->setText(uid);
    fullnameBox->setText(user->getFullname());
    homeBox->setText(user->getHome());
    
    //Make sure to show the current user shell 
    QString curshell = user->getShell();
    for(int i=0; i<shellBox->count(); i++){
      if(shellBox->itemText(i)==curshell){ shellBox->setCurrentIndex(i); break; }
      if(i==shellBox->count()-1){
        //Current Shell not found in list - add it to the end
	qDebug() << "Shell not Found - adding:" << curshell;
	shellBox->addItem(curshell); //will catch it on the next go-around
      }
    }
    //shellBox->setItemText(shellBox->currentIndex(), user->getShell());
    
    QString groupname;
    if ( user->getGid() != -1 ) {
    	Group *group = back->getGroup(user->getGid());
    	groupname = group->getGroupname();
    	if (groupname == "Not Set") { groupname = username; }
    } else {
    	groupname = username;
    }
    groupBox->setItemText(groupBox->currentIndex(), groupname);
    
    systemChange = false;
}

void mainDlgCode::getGroupDetails(const QString &groupname)
{
    qDebug() << "Fetching details for group: " << groupname;
    QStringList users = back->getAllUsers(-1, -1);
    users.sort();
    Group *group = back->getGroup(groupname);

    //Disable remove button if we're wheel
    removeGroupBut->setEnabled(group->getGid() != 0);
    memberPointer = memberList->currentItem();
    availablePointer = availableList->currentItem();
    memberList->clear();
    availableList->clear();
    
    //Traverse the user list, if the user is a member place in the 'members' box, otherwise place them in the 'available' box.
    for (QStringList::Iterator it = users.begin(); it != users.end(); it++)
    {
        if (group->isMember(*it) == true) { memberList->addItem(*it); }
	else { availableList->addItem(*it); }
    }
    //memberList->setCurrentItem(memberPointer);
    //availableList->setCurrentItem(availablePointer);
}

void mainDlgCode::showAddDlg()
{
    addui = new AddDlgCode();
    addui->programInit(back);
    addui->show();
}

UserManagerBackend* mainDlgCode::getBackend()
{
    return back;
}

void mainDlgCode::usersChanged()
{
    updateGroupList();
    updateUserList();
    emit changed(true);
}

void mainDlgCode::removeUser()
{
    if ( ! userList->currentItem() )
       return;

    QString username = userList->currentItem()->text();

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

void mainDlgCode::makeAvailable()
{
    back->removeUserFromGroup(memberList->currentItem()->text(), groupList->currentItem()->text());
}

void mainDlgCode::makeMember()
{
    back->addUserToGroup(availableList->currentItem()->text(), groupList->currentItem()->text());
}

void mainDlgCode::groupsChanged()
{
    updateGroupList();
    emit changed(true);
}

void mainDlgCode::removeGroup()
{
    back->deleteGroup(groupList->currentItem()->text());
}

void mainDlgCode::addGroup()
{
    int valid = -1;
    QString group = "";
    while (valid != 0)
    {
        switch(valid)
        {
            case 1:
                QMessageBox::critical(this, tr("Error adding group..."), tr("You must specify a name for your group"));
                break;
            case 2:
                QMessageBox::critical(this, tr("Error adding group..."), tr("Invalid group name specified. Group names may contain only letters and number. Spaces are NOT permitted"));
                break;
            case 3:
                QMessageBox::critical(this, tr("Error adding group..."), tr("The group '") + group + tr("' already exists, please choose a different name"));
                break;
        }
        
        group = QInputDialog::getText(this, tr("New Group"), tr("Please enter a name for your new group"));
        if (group == QString::null) { return; }
        valid = back->validateGroupname(group);
    }
    back->addGroup(group);
}

void mainDlgCode::changePassword()
{
    changePassUi = new ChangePassCode();
    changePassUi->programInit(userList->currentItem()->text(), back);
    changePassUi->show();
}

void mainDlgCode::homeChanged()
{
    User *user = back->getUser(userList->currentItem()->text());
    
    int homeError = back->validateHome(homeBox->text());
    
    QColor homeBoxColour = white;
    homeBox->setToolTip(tr("Directory will be used"));
    
    switch (homeError)
    {
    case 1:
	homeBox->setToolTip(tr("The field is empty"));
	break;
    case 2:
	homeBoxColour = red;
	homeBox->setToolTip(tr("Invalid Path!"));
	break;
    case 3:
	homeBox->setToolTip(tr("Directory does not exist, but will be made"));
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
    
    if ((! systemChange) && (homeError != 5) && (homeError != 2))
    {
        user->setHome(homeBox->text());
        user->setChanges();
        emit changed(true);
    }
    
    QPalette bgPal(homeBoxColour);
    bgPal.setColor(QPalette::Window,homeBoxColour);
    homeBox->setPalette(bgPal);
}

void mainDlgCode::fullnameChanged()
{
    User *user = back->getUser(userList->currentItem()->text());
    
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
    
    if ((! systemChange) && (fullnameError != 2))
    {
        user->setFullname(fullnameBox->text());
        user->setChanges();
        emit changed(true);
    }
    
    QPalette bgPal(fullnameBoxColour);
    bgPal.setColor(QPalette::Window,fullnameBoxColour);
    fullnameBox->setPalette(bgPal);
}

void mainDlgCode::simplePressed()
{
    emit simpleView();
}

void mainDlgCode::shellChanged()
{
    User *user = back->getUser(userList->currentItem()->text());
    
    if (! systemChange)
    {
        user->setShell(shellBox->currentText());
        user->setChanges();
        emit changed(true);
    }
}

void mainDlgCode::groupChanged()
{
    User *user = back->getUser(userList->currentItem()->text());
    
    if (! systemChange)
    {
        user->setGid(back->getGroup(groupBox->currentText())->getGid());
        user->setChanges();
        emit changed(true);
    }
}

