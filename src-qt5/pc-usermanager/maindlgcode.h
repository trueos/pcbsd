/***************************************************************************
 *   Copyright (C) 2007 by Tim McCormick   *
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
#ifndef MAINDLGCODE_H
#define MAINDLGCODE_H

#include "ui_mainDlg.h"
#include "usermanagerback.h"
#include "adddlgcode.h"
#include "changepasscode.h"
        
#include <qstringlist.h>
#include <qcolor.h>
#include <QListWidgetItem>

class mainDlgCode: public QDialog, private Ui::MainDlg {
Q_OBJECT
public:
    mainDlgCode() : QDialog()
    { 
      setupUi(this);
    }
    void programInit(UserManagerBackend *back, QString);
    void updateUserList();
    void updateShellList();
    void updateGroupList();
    UserManagerBackend* getBackend();
    
public slots:
    void getUserDetails(const QString &username);
    void getGroupDetails(const QString &groupname);
    void showAddDlg();
    void usersChanged();
    void removeUser();
    void makeMember();
    void makeAvailable();
    void groupsChanged();
    void removeGroup();
    void addGroup();
    void changePassword();
    void fullnameChanged();
    void homeChanged();
    void simplePressed();
    void shellChanged();
    void groupChanged();
    //PersonaCrypt Options
    void importPCKey();
    void exportPCKey();
    void disablePCKey();
    void disableAndCopyPCKey();

private:
    UserManagerBackend *back;
    QStringList *users;
    AddDlgCode *addui;
    QListWidgetItem *availablePointer;
    QListWidgetItem *memberPointer;
    ChangePassCode *changePassUi;
    QColor white;
    QColor red;
    QColor orange;
    bool systemChange;
    QString chroot;
    
    void enableEdits(bool enable);
    
signals:
    void changed(bool changed);
    void simpleView();
};

#endif
