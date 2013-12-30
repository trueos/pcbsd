/**************************************************************************
*   Copyright (C) 2011 by Yuri Momotyuk                                   *
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

#ifndef MAINWND_H
#define MAINWND_H

#include <QMainWindow>
#include <QFileSystemWatcher>
#include <QMutex>

#include "grouplist.h"

namespace Ui {
    class MainWnd;
}

class MainWnd : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWnd(QWidget *parent = 0);
    ~MainWnd();

public slots:
    void slotSingleInstance();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWnd *ui;

	QGroupList* SoftwareList;
	QGroupList* SystemList;
	QGroupList* NetworkingList;
	QGroupList* ToolsList;
	QGroupList* HardwareList;
        QGroupList* DEList;

        QFileSystemWatcher mDirWatcher;

	void setupGroups();
	void setupGroup(QGroupList** List, QWidget* Parrent);
	void fillGroups();
        void setupDEChooser();

        bool checkUserGroup(QString groupName);

        //CDEList mDEList;
        QMenu* DEChoiseMenu;

        QVector<QString> mvEnabledDE;
        CDEInfo          mCurrentDE;
        bool             misDisplayDEName;

        bool             misWheelGroup;
        bool             misOperatorGroup;

        QMutex          mRefreshMutex;

private slots:
        void on_actionUnsupported_triggered();
        void on_actionXFCE_triggered();
        void on_actionEnlightenment_triggered();
        void on_actionMate_triggered();
        void on_actionCinnamon_triggered();
        void on_actionGnome_triggered();
        void on_actionLXDE_triggered();
        void on_actionKDE_triggered();
        void on_actionAll_triggered();		
        void on_toolButton_2_clicked();
        void on_lineEdit_textChanged( QString );
        void on_itemActivated ( QListWidgetItem* item );

        void onDirectoryChanged(const QString & path);
    };

#endif // MAINWND_H
