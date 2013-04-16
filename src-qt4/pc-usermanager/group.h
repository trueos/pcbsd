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
#include <QStringList>

#ifndef INC_GROUP_H
#define INC_GROUP_H

class Group {
public:
    Group();
    Group(QString groupname, int gid, QStringList members, bool isNew = false);
    virtual ~Group();
    virtual QString getGroupname();
    virtual int getGid();
    virtual QStringList getMembers();
    virtual bool isMember(QString username);
    virtual bool getChanges();
    virtual void setChanges();
    virtual void addMember(QString username);
    virtual void removeMember(QString username);
    virtual void setDeleted();
    virtual int getStatus();
    virtual bool getDeleted();
    virtual void setNew();
    virtual bool getNew();
    
private:
    QString groupname;
    int gid;
    QStringList members;
    int status; // 0 = No change, 1 = Changes, 2 = New, 3 = Deleted
    
};

#endif // INC_GROUP_H
