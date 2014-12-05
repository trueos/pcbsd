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

#include "group.h"

Group::Group() {
    groupname = "Not Set";
    gid = -1;
    members = QStringList();
    status = -1;
}

Group::Group(QString groupname, int gid, QStringList members, bool isNew) {
    this->groupname = groupname;
    this->gid = gid;
    this->members = members;
    if (isNew) { status = 2; }
    else { status = 0; }
}

Group::~Group() {
    
}

QString Group::getGroupname() {
    return groupname;
}

int Group::getGid() {
    return gid;
}

QStringList Group::getMembers() {
    return members;
}

bool Group::isMember(QString username) {
    if (members.contains(username) > 0) { return true; }
    return false;
}

bool Group::getChanges() {
    return status == 1;
}

void Group::setChanges() {
    if (status == 0) { status = 1; }
}

void Group::addMember(QString username) {
    members += username;
}

void Group::removeMember(QString username) {
   members.removeAll(username);
}

void Group::setDeleted()
{
    status = 3;
}

int Group::getStatus()
{
    return status;
}

bool Group::getDeleted()
{
    return status == 3;
}

void Group::setNew()
{
    status = 2;
}

bool Group::getNew()
{
    return status == 2;
}
