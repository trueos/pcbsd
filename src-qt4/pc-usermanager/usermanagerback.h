/***************************************************************************
 *   Copyright (C) 2005 - 2007 by Tim McCormick   *
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

#include <qapplication.h>
#include <qfile.h>
#include <user.h>
#include <qmap.h>
#include <group.h>
#include <qregexp.h>
#include <QProcess>
#include <qmessagebox.h>

#ifndef INC_USERMANAGERBACKEND_H
#define INC_USERMANAGERBACKEND_H

class UserManagerBackend : public QObject {
    Q_OBJECT
public:
    UserManagerBackend(QString);
    void programInit();
    QStringList getAllUsers(int minId = 1000, int maxId = 65532); //Set variables to -1 to ignore
    User* getUser(const QString &user);
    QStringList getShells();
    QStringList getAllGroups(int minId = 1000, int maxId = 65532); //Set variables to -1 to ignore
    Group* getGroup(int gid);
    Group* getGroup(QString groupname);
    void deleteUser(QString username);
    void addUser(QString username, QString fullname, QString home, QString shell, QString group, QString password);
    int validatePassword(QString password, QString confirmPassword); //0 = OK, 1 = Empty, 2 = No match, 3 = Invalid input
    int validateFullname(QString fullname); //0 = OK, 1 = Empty, 2 = Invalid input
    int validateHome(QString home); //0 = OK, 1 = Empty, 2 = Invalid Path, 3 = Not a directory, 4 = Not a recommended path, 5 = File selected (not directory)
    int validateUsername(QString username); //0 = OK, 1 = Empty, 2 = Invalid input, 3 = Username already in use
    int validateGroupname(QString groupname); //0 = OK, 1 = Empty, 2 = Invalid input, 3 Groupname already in use
    void changePassword(QString username, QString password);
    void setEnc(QString username, bool nEnc);
    void deleteGroup(QString groupname);
    void addGroup(QString groupname);
    void addUserToGroup(QString user, QString group);
    void removeUserFromGroup(QString user, QString group);
    bool commit();
    
private:
    QMap<QString, User> userList;
    void refreshUsers();
    QMap<int, Group> groupList;
    void refreshGroups();
    QRegExp fullnameRegExp;
    QRegExp homeLocRegExp;
    QRegExp usernameRegExp;
    QRegExp passwordRegExp;
    QRegExp groupnameRegExp;
    QProcess *pw;
    QString chroot;
    
signals:
    void usersChanged();
    void groupsChanged();
};

#endif // INC_USERMANAGER_H
