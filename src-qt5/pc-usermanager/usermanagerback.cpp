/***************************************************************************
 *   Copyright (C) 2005, 2012 by Tim McCormick   *
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

#include "usermanagerback.h"
#include <qtextstream.h>
#include <QDebug>
#include <QUrl>
#include <QFileInfo>
#include <QTemporaryFile>

UserManagerBackend::UserManagerBackend(QString dir) {
    chroot = dir;
    programInit();
}

void UserManagerBackend::programInit() {
    fullnameRegExp.setPattern("([:!@])+");
    homeLocRegExp.setPattern("^(/usr)?/home/?");
    usernameRegExp.setPattern("([a-z]*[A-Z]*[0-9]*[_]*)+");
    passwordRegExp.setPattern("([a-z]*[A-Z]*[0-9]*[!\"$%^&*()_+=#'`@~:?<>|{}\\x5b;\\-.,\\x5d]*)+");
    groupnameRegExp.setPattern("([a-z]*[A-Z]*[0-9]*[_]*)+");
}

QStringList UserManagerBackend::getAllUsers(int minId, int maxId) {
    if (userList.empty()) { refreshUsers(); }
    
    qDebug() << "Generating new user list for UI...";

    QStringList result;
    QMap<QString, User>::Iterator it;

    for ( it = userList.begin(); it != userList.end(); ++it ) {
	if ( ( (! it->getDeleted()) && ((it->getUid() >= minId) || (minId == -1)) && ((it->getUid() <= maxId) || (maxId == -1)) ) || (it->getUid() < 0) ) {
	    result.append(it->getUsername());
	}
    }
    
    qDebug() << "Done Generating new user list for UI...";
    return result;
}

void UserManagerBackend::refreshUsers() {
    qDebug() << "Refreshing user list...";
    userList.clear();
    QFile userFile(chroot + "/etc/passwd");
    if ( userFile.open(QIODevice::ReadOnly) ) {
	QTextStream stream(&userFile);
	stream.setCodec("UTF-8");
	QString line;
	
	while ( !stream.atEnd() ) {
	    line = stream.readLine();
	    
	    if ((line.indexOf("#") != 0) && (! line.isEmpty())) { //Make sure it isn't a comment or blank
		QString username = line.section(":",0,0);
		int uid = line.section(":",2,2).toInt();
		int gid = line.section(":",3,3).toInt();
		QString home = line.section(":",5,5);
		QString shell = line.section(":",6,6);
		QString fullname = line.section(":",4,4);
		
		userList[username] = User(username, uid, gid, home, shell, fullname);
	    }
	}
    }
    else {
	//Unable to open file error
	qWarning("Error! Unable to open /etc/passwd");
    }
}

User* UserManagerBackend::getUser(const QString &user) {
    return &userList.find(user).value();
}

QStringList UserManagerBackend::getShells() {
    QStringList result;
    QFile shellFile(chroot + "/etc/shells");
    if ( shellFile.open(QIODevice::ReadOnly) ) {
	QTextStream stream(&shellFile);
	stream.setCodec("UTF-8");

	QString line;
	
	while ( !stream.atEnd() ) {
	    line = stream.readLine();
	    
	    if ( !line.startsWith("#") && !line.isEmpty() ) { //Make sure it isn't a comment or blank
		result.append(line);
	    }
	}
    }
    else {
	//Unable to open file error
	qWarning("Error! Unable to open /etc/shells");
    }
    
    // Add /sbin/nologin as well
    result.append("/sbin/nologin");

    return result;
}

QStringList UserManagerBackend::getAllGroups(int minId, int maxId) {
    if (groupList.empty()) { refreshGroups(); }
    
    QStringList result;
    QMap<int, Group>::Iterator it;
    
    qDebug() << "Generating new group list for UI...";

    for ( it = groupList.begin(); it != groupList.end(); ++it ) {
	if ((! it->getDeleted()) && (((it->getGid() >= minId) || (minId == -1) ) && ((it->getGid() <= maxId) || (maxId == -1 )))) {
	    result.append(it->getGroupname());
	}
    }

    qDebug() << "Done Generating new group list for UI...";
    return result;
}

void UserManagerBackend::refreshGroups() {
    qDebug() << "Refreshing group list...";
    groupList.clear();
    QFile groupFile(chroot + "/etc/group");
    if ( groupFile.open(QIODevice::ReadOnly) ) {
	QTextStream stream(&groupFile);
	stream.setCodec("UTF-8");

	QString line;
	
	while ( !stream.atEnd() ) {
	    line = stream.readLine();
	    
	    if ((line.indexOf("#") != 0) && (! line.isEmpty())) { //Make sure it isn't a comment or blank
		QString groupname = line.section(":",0,0);
		int gid = line.section(":",2,2).toInt();
		QString memberString = line.section(":",3,3);
		QStringList members = memberString.split(",");
		
		groupList[gid] = Group(groupname, gid, members);
	    }
	}
    }
    else {
	//Unable to open file error
	qWarning("Error! Unable to open /etc/group");
    }
}

Group* UserManagerBackend::getGroup(int gid) {
    return &groupList.find(gid).value();
}

Group* UserManagerBackend::getGroup(QString groupname) {
    Group *result = new Group();
    QMap<int, Group>::Iterator it;
    
    for ( it = groupList.begin(); it != groupList.end(); ++it ) {
	if (it->getGroupname() == groupname) {
	    result = &it.value();
	}
    }
    
    return result;
}

int UserManagerBackend::validatePassword(QString password, QString confirmPassword) {
    int result = 0;
    if (password != confirmPassword) {
	result = 2;
    }
    if ((password == "") || (confirmPassword == "")) {
	result = 1;
    }
    if ((! passwordRegExp.exactMatch(password)) && (password != "")) {
	result = 3;
    }
    return result;
}

void UserManagerBackend::changePassword(QString username, QString password)
{
    qDebug() << "running changePassword()";
    userList[username].setPassword(password);
    emit usersChanged();
}

void UserManagerBackend::setEnc(QString username, bool nEnc)
{
    userList[username].setEnc(nEnc);
}

int UserManagerBackend::validateFullname(QString fullname) {
    int result = 0;
    
    if ((fullname.contains(fullnameRegExp) != 0) && (fullname != "")) {
	result = 2;
    }
    if (fullname == "") {
	result = 1;
    }
    
    return result;
}

int UserManagerBackend::validateHome(QString home) {
    int result = 0;
    QFileInfo homeDir(home);
    QUrl *homePath = new QUrl(home);
    if (home == "") {
	result = 1;
    }
    if ((! homeDir.isDir()) && (home != "")) {
	result = 3;
    }
    if ((homeLocRegExp.indexIn(home) == -1) && (home != "")) {
	result = 4;
    }    
    if ((homeDir.isFile()) && (home != "")) {
	result = 5;
    }
    if ((! homePath->isValid()) && (home != "")) {
	result = 2;
    }
    
    return result;
}

void UserManagerBackend::deleteUser(QString username)
{
    userList[username].setDeleted();
    emit usersChanged();
}

int UserManagerBackend::validateUsername(QString username) {
    //0 = OK, 1 = Empty, 2 = Invalid input, 3 = Username already in use
    int result = 0;
    
    if ((userList.contains(username) == true) && (username != "")) {
	result = 3;
    }
    if ((! usernameRegExp.exactMatch(username)) && (username != "")) {
	result = 2;
    }
    if (username == "") {
	result = 1;
    }
    
    return result;
}

void UserManagerBackend::addUser(QString username, QString fullname, QString home, QString shell, QString group, QString password)
{
    qDebug() << "Starting addUser " << username;
    int gid = -1;
    QMap<int, Group>::Iterator it;
    
    for ( it = groupList.begin(); it != groupList.end(); ++it )
    {
	if (it->getGroupname() == group)
        {
            gid = it->getGid();
            break;
        }
    }
    userList[username] = User(username, home, shell, fullname, password, gid);
    
    emit usersChanged();
    qDebug() << "Finished addUser: " << username;
}

void UserManagerBackend::deleteGroup(QString groupname)
{
    getGroup(groupname)->setDeleted();
    emit groupsChanged();
}

void UserManagerBackend::addGroup(QString groupname)
{
    //Find free, invalid id
    int gid = -1;
    while (groupList[gid].getStatus() != -1) { gid--; }
    groupList[gid] = Group(groupname, -1, QStringList(), true);
    emit groupsChanged();
}

int UserManagerBackend::validateGroupname(QString groupname) {
    //0 = OK, 1 = Empty, 2 = Invalid input, 3 = Groupname already in use
    int result = 0;
    QMap<int, Group>::Iterator it;
    
    for ( it = groupList.begin(); it != groupList.end(); ++it ) {
	if (it->getGroupname() == groupname) {
	    result = 3;
	}
    }
    if ((! groupnameRegExp.exactMatch(groupname)) && (groupname != "")) {
	result = 2;
    }
    if (groupname == "") {
	result = 1;
    }
    
    return result;
}

void UserManagerBackend::addUserToGroup(QString user, QString groupname)
{
    Group *group = getGroup(groupname);
    group->addMember(user);
    group->setChanges();
    emit groupsChanged();
}

void UserManagerBackend::removeUserFromGroup(QString user, QString groupname)
{
    Group *group = getGroup(groupname);
    group->removeMember(user);
    group->setChanges();
    emit groupsChanged();
}

bool UserManagerBackend::commit()
{
    
    //Process users
    QMap<QString, User>::Iterator userIt;

    QStringList args;
    
    for ( userIt = userList.begin(); userIt != userList.end(); ++userIt )
    {
	args.clear();
        switch(userIt->getStatus())
        {
            case 1:
                //Modify User
                qDebug() << "Modifying user " << userIt->getUsername();
		if ( ! chroot.isEmpty() )
		   args << chroot << "pw";
                args << "usermod";
                args << userIt->getUsername();
		// Only change home-dir on non-encrypted users
                if ( ! userIt->getEnc() ) {
                  args << "-d";
                  args << userIt->getHome();
		}
                args << "-s";
                args << userIt->getShell();
                args << "-c";
                args << userIt->getFullname();
		if ( ! chroot.isEmpty() )
		   QProcess::execute("chroot", args);
		else
		   QProcess::execute("pw", args);
                
                if (userIt->getPassword() != "")
                {
		    // Refuse to continue if we are trying to change PW
		    // On an encrypted users homedir
                    if ( userIt->getEnc() ) {
                      qDebug() << "Cannot change encrypted password: " << userIt->getUsername();
                      break;
 		    }
                    qDebug() << "Changing password: " << userIt->getUsername();

		    // Set the new PW
    		    QTemporaryFile rfile("/tmp/.XXXXXXXX");
  		    if ( rfile.open() ) {
       		      QTextStream stream( &rfile );
      		      stream << userIt->getClearPassword();
    		      rfile.close();
  		    }
		    if ( ! chroot.isEmpty() )
  		      system("cat " + rfile.fileName().toLatin1() + " | chroot " + chroot.toLatin1() + " pw usermod " + userIt->getUsername().toLatin1() + " -h 0 ");
		    else
  		      system("cat " + rfile.fileName().toLatin1() + " | pw usermod " + userIt->getUsername().toLatin1() + " -h 0 ");

  		    rfile.remove();
                }
                break;
            case 2:
	    {
                //Add User
                qDebug() << "Adding user " << userIt->getUsername();
		// Create the new home-directory
		if ( chroot.isEmpty() ) {
		   system("/usr/local/share/pcbsd/scripts/mkzfsdir.sh " + userIt->getHome().toLatin1() );
		   system("pw groupadd " + userIt->getUsername().toLatin1() );
		} else {
		   system("mkdir -p " + chroot.toLatin1() + "/" + userIt->getHome().toLatin1() + " 2>/dev/null" );
		   system("chroot " + chroot.toLatin1() + " ln -s /usr/home /home 2>/dev/null" );
		   system("chroot " + chroot.toLatin1() + " pw groupadd " + userIt->getUsername().toLatin1() );
		}

		if ( ! chroot.isEmpty() )
		   args << chroot << "pw";
                args << "useradd";
                args << userIt->getUsername();
                args << "-c";
                args << userIt->getFullname();
		args << "-m";
                args << "-d";
                args << userIt->getHome();
                args << "-s";
                args << userIt->getShell();
                if (userIt->getGid() != -1)
                {
                    args << "-g";
                    args << QString::number(userIt->getGid());
		} else {
                    args << "-g";
                    args << userIt->getUsername();
                }
                args << "-G";
                args << "operator";
		if ( ! chroot.isEmpty() )
		   QProcess::execute("chroot", args);
		else
		   QProcess::execute("pw", args);

    		QTemporaryFile nfile("/tmp/.XXXXXXXX");
  		if ( nfile.open() ) {
       		  QTextStream stream( &nfile );
      		  stream << userIt->getClearPassword();
    		  nfile.close();
  		}
		if ( ! chroot.isEmpty() )
  		  system("cat " + nfile.fileName().toLatin1() + " | chroot " + chroot.toLatin1() + " pw usermod " + userIt->getUsername().toLatin1() + " -h 0 ");
		else
  		  system("cat " + nfile.fileName().toLatin1() + " | pw usermod " + userIt->getUsername().toLatin1() + " -h 0 ");
  	        nfile.remove();

		if ( chroot.isEmpty() ) {
		   qDebug() << "Enabling Flash Plugin for " << userIt->getUsername();
		   QString flashCmd = "su " + userIt->getUsername() + " -c \"flashpluginctl on\"";
		   system(flashCmd.toLatin1());
		}

		// Set permissions
		if ( chroot.isEmpty() )
		   system("chown -R " + userIt->getUsername().toLatin1() +":" + userIt->getUsername().toLatin1() + " "  + userIt->getHome().toLatin1() );
		else
		   system("chroot " + chroot.toLatin1() + " chown -R " + userIt->getUsername().toLatin1() +":" + userIt->getUsername().toLatin1() + " "  + userIt->getHome().toLatin1() );

                break;
	    }
            case 3:
                //Delete User
                qDebug() << "Deleting user " << userIt->getUsername();

                if(userIt->getEnc()) {
		  // Unmount PEFS
	  	  system("umount " + userIt->getHome().toLatin1() );
		}
		if ( ! chroot.isEmpty() )
		   args << chroot << "pw";
                args << "userdel";
                args << userIt->getUsername();
                if(userIt->getDeleteHome()) {
			args << "-r";
			system("/usr/local/share/pcbsd/scripts/rmzfsdir.sh " + userIt->getHome().toLatin1() );
		}
		if ( ! chroot.isEmpty() )
		   QProcess::execute("chroot", args);
		else
		   QProcess::execute("pw", args);
                break;
        }
    }
    refreshUsers();
    
    //Process groups
    QMap<int, Group>::Iterator groupIt;
 
    for ( groupIt = groupList.begin(); groupIt != groupList.end(); ++groupIt )
    {
	args.clear();
        switch(groupIt->getStatus())
        {
            case 1:
                //Modify Group
                qDebug() << "Modifying group " << groupIt->getGroupname();
		if ( ! chroot.isEmpty() )
		   args << chroot << "pw";
		args << "groupmod";
		args << groupIt->getGroupname();
		args << "-M";
		args << groupIt->getMembers().join(",");
		if ( ! chroot.isEmpty() )
		   QProcess::execute("chroot", args);
		else
		   QProcess::execute("pw", args);
                break;
            case 2:
                //Add Group
                qDebug() << "Adding group " << groupIt->getGroupname();
		if ( ! chroot.isEmpty() )
		   args << chroot << "pw";
                args << "groupadd";
                args << groupIt->getGroupname();
                args << "-M";
                args << groupIt->getMembers().join(",");
		if ( ! chroot.isEmpty() )
		   QProcess::execute("chroot", args);
		else
		   QProcess::execute("pw", args);
                break;
            case 3:
                //Delete Group
                qDebug() << "Deleting group " << groupIt->getGroupname();
		if ( ! chroot.isEmpty() )
		   args << chroot << "pw";
                args << "groupdel";
                args << groupIt->getGroupname();
		if ( ! chroot.isEmpty() )
		   QProcess::execute("chroot", args);
		else
		   QProcess::execute("pw", args);
                break;
	}
    }
    refreshGroups();
    emit groupsChanged();
    emit usersChanged();
    
    return true;
}

