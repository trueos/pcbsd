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

#include <unistd.h>
        
#include "user.h"
#include <QDebug>

User::User() {
    username = "Not set";
    uid = -1;
    gid = -1;
    home = "Not set";
    shell = "Not set";
    fullname = "Not set";
    enc = false;
    status = 0;
    password = "";
    deleteHome = false;
}

User::User(QString username, int uid, int gid, QString home, QString shell, QString fullname) {
    this->username = username;
    this->uid = uid;
    this->gid = gid;
    this->home = home;
    if ( QFile::exists(home + "/.pefs.db"))
      this->enc = true; 
    else
      this->enc = false; 
    this->shell = shell;
    this->fullname = fullname;
    status = 0;
    this->password = "";
}

User::User(QString username, QString home, QString shell, QString fullname, QString password, int gid, bool isNew, int uid)
{
    this->username = username;
    this->home = home;
    this->shell = shell;
    this->fullname = fullname;
    this->enc = false;
    status = 0;
    if (isNew) { status = 2; }
    this->uid = uid;
    this->gid = gid;
    this->clearPass = password;
    
    setPassword(password);
}

User::~User() {
    
}

QString User::getUsername() {
    return username;
}

int User::getUid() {
    return uid;
}

int User::getGid() {
    return gid;
}

bool User::getEnc() {
    return enc;
}

void User::setEnc(bool Enc) {
    this->enc = Enc;
}

QString User::getHome() {
    return home;
}

QString User::getShell() {
    return shell;
}

QString User::getFullname() {
    return fullname;
}

bool User::getChanges() {
    return status == 1;
}

void User::setChanges() {
    if (status == 0) { status = 1; }
}

void User::setGid(int gid) {
    this->gid = gid;
}

void User::setHome(QString home) {
    this->home = home;
}

void User::setShell(QString shell) {
    this->shell = shell;
}

void User::setFullname(QString fullname) {
    this->fullname = fullname;
}

bool User::getNew()
{
    return status == 2;
}

void User::setNew()
{
    status = 2;
}

void User::setPassword(QString password)
{
    this->clearPass = password;
    this->password = crypt(password.toAscii(), "KG");
    qDebug("Done with crypt now........");
    setChanges();
}

QString User::getPassword()
{
    return password;
}

QString User::getClearPassword()
{
    return clearPass;
}

bool User::getDeleted()
{
    return status == 3;
}

void User::setDeleted()
{
    status = 3;
}

uint User::getStatus()
{
    return status;
}

void User::setStatus(uint status)
{
    this->status = status;
}

/**
 * @param deleteHome is true if the home directory should be deleted
 */
void User::setDeleteHome(bool deleteHome)
{
    this->deleteHome = deleteHome;
}

/**
  * @return true if the home directory should be deleted
  */
bool User::getDeleteHome()
{
    return deleteHome;
}
