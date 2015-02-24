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
#include <QFileDialog>
#include <QDebug>

#include <qcombobox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <QTemporaryFile>

#include <pcbsd-utils.h>
        
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
    connect(tool_PCKey_import, SIGNAL(clicked()), this, SLOT(importPCKey()) );
    connect(tool_PCKey_export, SIGNAL(clicked()), this, SLOT(exportPCKey()) );
    connect(tool_PCKey_disable, SIGNAL(clicked()), this, SLOT(disablePCKey()) );
    connect(tool_PCKey_disablecopy, SIGNAL(clicked()), this, SLOT(disableAndCopyPCKey()) );
    connect(tool_PCKey_init, SIGNAL(clicked()), this, SLOT(initPCDevice()));
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
    //Do not allow some options for the currently logged in user
    QString tmp = getenv("SUDO_USER");
    if(tmp.isEmpty()){ tmp = getenv("USER"); }
    bool currentUser = (tmp == username);
    deleteButton->setEnabled(!currentUser);

    if ( user->getEnc() )
      passwordButton->setEnabled(false);
    else
      passwordButton->setEnabled(true);
    
    QString uid = QString::number(user->getUid());
    bool isnewuser = user->getNew();
    if (isnewuser && uid == "-1") { uid = tr("New User"); }
    else if( isnewuser ){ uid = QString(tr("New User (%1)")).arg(uid); }
    uidBox->setText(uid);
    fullnameBox->setText(user->getFullname());
    homeBox->setText(user->getHome());
    
    if( !isnewuser && (homeBox->text().contains("/usr/home/") || homeBox->text().contains("/home/")) && QFile::exists(homeBox->text()) ){
      //This is a user that can log in (has a valid home directory and not root)
      //Now activate/de-activate options as necessary
      bool haskey = QFile::exists("/var/db/personacrypt/"+username+".key");
      tool_PCKey_import->setVisible( !haskey && !currentUser );
      tool_PCKey_init->setVisible(!haskey && !currentUser);
      tool_PCKey_export->setVisible(haskey);
      tool_PCKey_disable->setVisible(haskey && !currentUser);
      tool_PCKey_disablecopy->setVisible(haskey && !currentUser);
	    
      group_personacrypt->setVisible(haskey || !currentUser);
    }else{
      group_personacrypt->setVisible(false);
    }
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

void mainDlgCode::importPCKey(){
  QString username = userList->currentItem()->text();
  //Double check that the key does not exist (button should have been hidden earlier if invalid)
  if( QFile::exists("/var/db/personacrypt/"+username+".key") ){ return; }
  //Now prompt for the key file to import
  QString filename = QFileDialog::getOpenFileName(this, tr("Import PersonaCrypt Key"), "/home/", tr("Key Files (*.key);; All Files (*)"));
  if(filename.isEmpty()){ return; } //cancelled
  //Now run the import command
  this->setEnabled(false);
  QApplication::processEvents();
  if( 0 == QProcess::execute("personacrypt import \""+filename+"\"") ){
    //Success
    QMessageBox::information(this, tr("Success"), tr("The key file was imported successfully."));
  }else{
    //Failure
    QMessageBox::warning(this, tr("Failure"), tr("The key file could not be imported. Please ensure you are using a valid file."));
  }
  this->setEnabled(true); //re-enable the UI
  QApplication::processEvents();
  getUserDetails(username); //refresh the UI
}

void mainDlgCode::exportPCKey(){
  QString username = userList->currentItem()->text();
  //Double check that the key exists (button should have been hidden earlier if invalid)
  if( !QFile::exists("/var/db/personacrypt/"+username+".key") ){ return; }
  //Now prompt for where to save the key
  QString filename = QFileDialog::getSaveFileName(this, tr("Save PersonaCrypt Key"), "/home/", tr("Key Files (*.key);; All Files (*)"));
  if(filename.isEmpty()){ return; } //cancelled
  if( !filename.endsWith(".key") ){ filename.append(".key"); }
  //Now get/save the key file
  QString key = pcbsd::Utils::runShellCommand("personacrypt export \""+username+"\"").join("\n");  
  QFile file(filename);
  if( !file.open(QIODevice::WriteOnly | QIODevice::Truncate) ){
    //Could not open output file
    QMessageBox::warning(this, tr("File Error"), tr("Output file could not be opened:")+"\n\n"+filename);
    return;
  }
  QTextStream out(&file);
  out << key;
  file.close();
  QMessageBox::information(this, tr("Success"),tr("The PersonaCrypt key has been saved successfully:")+"\n\n"+filename);
}

void mainDlgCode::disablePCKey(){
  QString username = userList->currentItem()->text();
//Double check that the key exists (button should have been hidden earlier if invalid)
  if( !QFile::exists("/var/db/personacrypt/"+username+".key") ){ return; }
  if(QMessageBox::Yes != QMessageBox::question(this, tr("Disable PersonaCrypt?"), tr("Are you sure you want to disable your PersonaCrypt device for this user? This will only de-activate your key on this system (the PersonaCrypt device will still be usable on other systems)."), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) ){
    return; //cancelled
  }
  if( QFile::remove("/var/db/personacrypt/"+username+".key") ){
    //Success
    QMessageBox::information(this, tr("Success"), tr("The PersonaCrypt user key has been disabled.") );
  }else{
    //Failure (should almost never happen, since this utility runs as root and just needs to delete a file)
    QMessageBox::warning(this, tr("Failure"), tr("The PersonaCrypt user key could not be removed. Do you have the proper permissions?") );
  }
  getUserDetails(username); //refresh the UI
}

void mainDlgCode::disableAndCopyPCKey(){
  QString username = userList->currentItem()->text();	
  QStringList cusers = pcbsd::Utils::runShellCommand("personacrypt list");
  bool available = false;
  for(int i=0; i<cusers.length(); i++){
    if(cusers[i].section(" on ",0,0) == username){ available = true; break; } //disk is connected to the system
  }
  if(!available){
    //Warn the user that they need to plug in their USB stick first
    QMessageBox::warning(this, tr("PersonaCrypt Device Not Found"), tr("Please ensure that your PersonaCrypt device is connected to the system and try again."));
    return;
  }else{
    //Verify that they want to disable the key and merge the data onto the system
    if(QMessageBox::Yes != QMessageBox::question(this, tr("Disable PersonaCrypt?"), tr("Are you sure you want to disable your PersonaCrypt device for this user? This will merge the data from the device onto your system and de-activate your key on this system (the PersonaCrypt device will still be usable on other systems)."), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) ){
      return; //cancelled
    }
  }
  //Now prompt for the password for the USB stick
  bool ok = false;
  QString pass = QInputDialog::getText(this, tr("PersonaCrypt Access"), tr("Password:"), QLineEdit::Password, "", &ok);
  if(!ok || pass.isEmpty()){ return; } //cancelled
  //Save the password to a temporary file
  QTemporaryFile tmpfile("/tmp/.XXXXXXXXXXXXXXXXXXXX");
  if( !tmpfile.open() ){ return; } //could not create a temporary file (extremely rare)
  QTextStream out(&tmpfile);
  out << pass;
  tmpfile.close();
  //Now run the PersonaCrypt command
  this->setEnabled(false); //disable the UI temporarily
  QApplication::processEvents();
  if(0 == QProcess::execute("personacrypt remove \""+username+"\" \""+tmpfile.fileName()+"\"") ){
    //Success
    QMessageBox::information(this, tr("Success"),tr("The data for this user has been merged onto the system and the system key has been disabled"));
  }else{
    //Failure
    QMessageBox::warning(this, tr("Failure"), tr("The PersonaCrypt user data could not be merged onto the system. Invalid Password?") );
  }
  this->setEnabled(true); //re-enable the UI
  QApplication::processEvents();
  getUserDetails(username); //refresh the UI
}

void mainDlgCode::initPCDevice(){
  //Routine to initialize a PersonaCrypt user/device
  QString username = userList->currentItem()->text();
  //Double check that the key does not exist (button should have been hidden earlier if invalid)
  if( QFile::exists("/var/db/personacrypt/"+username+".key") ){ return; }
  //Prompt for the user to select a device
  QStringList devlist = pcbsd::Utils::runShellCommand("personacrypt list -r");
  for(int i=0; i<devlist.length(); i++){
    //qDebug() << "Devlist:" << devlist[i];
    if(devlist[i].isEmpty() || devlist[i].startsWith("gpart:")){
      devlist.removeAt(i);
      i--;
    }
  }
  if(devlist.isEmpty() || devlist.join("").simplified().isEmpty()){
    QMessageBox::warning(this, tr("No Devices Found"), tr("Please connect a removable device and try again"));
    return;	  
  }
  User *user = back->getUser(username);
    QString home = user->getHome();
  bool ok = false;
  QString space = pcbsd::Utils::runShellCommand("df -h "+home).filter(home).join("");
  space.replace("\t"," ");
  space = space.section(" ",2,2,QString::SectionSkipEmpty);
  QString dev = QInputDialog::getItem(this, tr("Select a device"), tr("Warning 1: All the contents of the device will be wiped during PersonaCrypt initialization.")+"\n"+tr("Warning 2: Device size needs to be larger than the current home dir.")+"\n\n"+QString(tr("Home Dir Size: %1")).arg(space), devlist, 0, false, &ok);
  if(!ok || dev.isEmpty()){ return; }
  //Now have the user supply a password
  QString pass, passr;
  bool first = true;
  while(pass.isEmpty() || passr.isEmpty() || pass!=passr){
    pass = QInputDialog::getText(this, first ? tr("Create Device Password"):tr("Invalid Password or Match - try again"), tr("Password:"),QLineEdit::Password,"",&ok, 0, Qt::ImhSensitiveData | Qt::ImhNoAutoUppercase);
    if(pass.isEmpty() || !ok){ return; } //cancelled
    passr = QInputDialog::getText(this, tr("Repeat Password"), tr("Password:"),QLineEdit::Password,"",&ok, 0, Qt::ImhSensitiveData | Qt::ImhNoAutoUppercase);
    if(passr.isEmpty() || !ok){ return; } //cancelled
    first = false;
  }
  
  QString msg = tr("Are you ready to start the device initialization? (This may take a few minutes)")+"\n\n"+tr("Device: %1")+"\n"+tr("User: %2")+"\n"+tr("Current User Data: %3");
  msg = msg.arg(dev, username, space);
  if(QMessageBox::Yes != QMessageBox::question(this, tr("PersonaCrypt Ready"), msg, QMessageBox::Yes | QMessageBox::Cancel | QMessageBox::Yes) ){
    return; //cancelled
  }
  dev = dev.section(":",0,0); //only need the raw device
  //Save the password to a temporary file (for input to personacrypt)
  QTemporaryFile tmpfile("/tmp/.XXXXXXXXXXXXXXX");
  if(!tmpfile.open()){ 
    //Error: could not open a temporary file
    QMessageBox::warning(this, tr("Error"), tr("Could not create a temporary file for personacrypt"));
    return;
  }
  QTextStream out(&tmpfile);
  out << pass;
  tmpfile.close();
  //Now start the process of setting up the device
  this->setEnabled(false); //disable the UI temporarily
  QApplication::processEvents();
  if(0 == QProcess::execute("personacrypt init \""+username+"\" \""+tmpfile.fileName()+"\" "+dev) ){
    //Success
    QMessageBox::information(this, tr("Success"), tr("The PersonaCrypt device was successfully initialized"));
  }else{
    //Failure
    QMessageBox::warning(this, tr("Failure"), tr("The PersonaCrypt device could not be initialized"));
  }
  this->setEnabled(true); //re-enable the UI
  QApplication::processEvents();
  getUserDetails(username); //refresh the UI
}
