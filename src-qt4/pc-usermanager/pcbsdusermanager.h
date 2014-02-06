/***************************************************************************
 *   Copyright (C) 2007, 2012 by Tim McCormick				   *
 *   Modified 2009 by Kris Moore					   *
 *   kris@pcbsd.org				    	    	 	   *	 
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


#ifndef _PCBSDUSERMANAGER_H_
#define _PCBSDUSERMANAGER_H_

#include <QDialog>
#include <QSettings>

#include "maindlgcode.h"
#include "usermanagerback.h"
#include "simpledlgcode.h"
#include "ui_pcbsdusermanager.h"

class PCBSDUserManager: public QDialog, private Ui::PCBSDUserManager
{
    Q_OBJECT

public:
    PCBSDUserManager() : QDialog()
    {
       setupUi(this);
       //setupUMDialogs();
    }
    
    ~PCBSDUserManager();
    void setupUMDialogs();
    void setWardenMode(QString, QString);
    virtual void closeEvent(QCloseEvent*);

public slots:
    void switchToAdvanced();
    void switchToSimple();

private slots:
    void configChanged();
    void slotSave();
    void slotClose();

private:
    mainDlgCode *advancedGui;
    UserManagerBackend *back;
    SimpleDlgCode *simpleGui;
    QGridLayout *layout;
    QSettings *settings;
    bool changesMade;
    QString wDir;
    QString wIP;
};

#endif
