/**************************************************************************
*   Copyright (C) 2013- by Yuri Momotyuk                                   *
*   yurkis@gmail.com                                                      *
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
*   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*
*   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
*   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
*   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
*   OTHER DEALINGS IN THE SOFTWARE.                                       *
***************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>

#include "syscontroller.h"
#include "pkgcontroller.h"
#include "pbicontroller.h"
#include "jailsbackend.h"


namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(CJailsBackend* jail=0, QWidget *parent = 0);
    ~MainWindow();

    void setJail(CJailsBackend jail);    
    
private:
    Ui::MainWindow *ui;

    void init();
    void jailRefresh();

    void refreshMenu();

    CSysController  mSysController;
    CPkgController  mPkgController;
    CPBIController  mPBIController;

    CJailsBackend   mJail;

    bool misRegularUser;

public slots:
    void slotSingleInstance();

private slots:

    void globalStateChanged(CAbstractUpdateController::EUpdateControllerState new_state);
    void slotReturnToHost();

private slots:

    void on_updateAllButton_clicked();
    void on_pushButton_clicked();
    void on_checkAllButton_clicked();
    void on_actionLast_system_update_log_triggered();
    void on_actionLast_package_update_log_triggered();
    void on_actionLast_software_update_log_triggered();
    void on_actionExit_triggered();
    void on_actionJail_triggered();
    void on_actionSystem_branches_triggered();
    void on_actionUpdate_set_triggered();   
    void on_actionBase_system_update_log_triggered();
};

#endif // MAINWINDOW_H
