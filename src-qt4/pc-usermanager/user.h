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

#include <qapplication.h>
#include <QFile>

#ifndef INC_USER_H
#define INC_USER_H

class User {
public:
    User();
    User(QString username, int uid, int gid, QString home, QString shell, QString fullname);
    User(QString username, QString home, QString shell, QString fullname, QString password = "", int gid = -1, bool isNew = true, int uid = -1);
    virtual ~User();
    virtual QString getUsername();
    virtual int getUid();
    virtual int getGid();
    virtual bool getEnc();
    virtual void setEnc(bool Enc);
    virtual QString getHome();
    virtual QString getShell();
    virtual QString getFullname();
    virtual bool getChanges();
    virtual void setChanges();
    virtual void setGid(int gid);
    virtual void setHome(QString home);
    virtual void setShell(QString shell);
    virtual void setFullname(QString fullname);
    virtual bool getNew();
    virtual void setNew();
    virtual QString getClearPassword();
    virtual QString getPassword();
    virtual void setPassword(QString password);
    virtual bool getDeleted();
    virtual void setDeleted();
    virtual uint getStatus();
    virtual void setStatus(uint status);
    virtual void setDeleteHome(bool deleteHome);
    virtual bool getDeleteHome();
    
private:
    QString username;
    int uid;
    int gid;
    QString home;
    QString shell;
    QString fullname;
    uint status; // 0 = No change, 1 = Changes, 2 = New, 3 = Deleted
    QString password;
    QString clearPass;
    bool deleteHome;
    bool enc;
};

#endif // INC_USER_H
