//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
//  This is the main interface for the Lumina File Manager (Insight)
//===========================================
#ifndef _LUMINA_FILE_MANAGER_UI_H
#define _LUMINA_FILE_MANAGER_UI_H
// Qt includes
#include <QMainWindow>
#include <QTabBar>
#include <QLineEdit>
#include <QFileSystemModel>
#include <QStringList>
#include <QDebug>
#include <QAction>
#include <QProcess>
#include <QSettings>
#include <QInputDialog>
#include <QMessageBox>
#include <QDir>
#include <QTimer>
#include <QDateTime>
#include <QShortcut>
#include <QCompleter>

// libLumina includes
#include <LuminaXDG.h>

// Local includes
#include "FODialog.h" //file operation dialog

namespace Ui{
	class MainUI;
};

class MainUI : public QMainWindow{
	Q_OBJECT
public:
	MainUI();
	~MainUI();

	void OpenDirs(QStringList); //called from the main.cpp after initialization

private:
	Ui::MainUI *ui;
	//Internal non-ui widgets
	QTabBar *tabBar;
	QLineEdit *currentDir;
	QFileSystemModel *fsmod, *snapmod;
	QMenu *contextMenu;

	//Internal variables
	QStringList snapDirs; //internal saved variable for the discovered zfs snapshot dirs
	QModelIndex CItem; //the item that was right-clicked (for the context menu)
	QSettings *settings;
	QShortcut *nextTabLShort, *nextTabRShort, *closeTabShort;
	QCompleter *dirCompleter;
	bool isUserWritable;

	//Simplification Functions
	void setupIcons(); 			//used during initialization
	void setupConnections(); 	//used during initialization
	void loadSettings(); 		//used during initialization

	void loadBrowseDir(QString);
	void loadSnapshot(QString);
	bool findSnapshotDir(); //returns true if the current dir has snapshots available
	
	//Functions to get/set the currently active directory
	QString getCurrentDir();
	void setCurrentDir(QString);

private slots:
	void slotSingleInstance(const QString &in){
	  this->show();
	  this->raise();
	  this->OpenDirs(in.split("\n"));
	}
	
	//General button check functions (start in a new thread!)
	void checkForMultimediaFiles();
	void checkForBackups();
	void checkForPictures();
	
	//General page switching
	void goToMultimediaPage();
	void goToRestorePage();
	void goToSlideshowPage();
	void goToBrowserPage();
	
	//Menu Actions
	void on_actionNew_Tab_triggered();
	void on_actionClose_triggered();
	void goToBookmark(QAction*);
	
	//Toolbar Actions
	void on_actionBack_triggered();
	void on_actionUpDir_triggered();
	void on_actionHome_triggered();
	void on_actionBookMark_triggered();

	//Browser Functions
	void startEditDir(QWidget *old, QWidget *now);
	void goToDirectory(); //go to a manually typed in directory
	void directoryLoaded();
	void on_tool_addToDir_clicked();
	void tabChanged(int tab);
	void tabClosed(int tab = -1);
	void prevTab();
	void nextTab();
	void ItemRun( const QModelIndex&);
	void OpenContextMenu(const QPoint&);

	//Slideshow Functions
	void showNewPicture();
	void firstPicture();
	void prevPicture();
	void nextPicture();
	void lastPicture();
	
	//ZFS Restore Functions
	void snapshotLoaded();
	void showSnapshot();
	void nextSnapshot();
	void prevSnapshot();
	void restoreItems();
	
	//Context Menu Actions
	void OpenItem(); //run "lumina-open" on it
	void OpenItemWith(); //run "lumina-open -select" on it
	void OpenDir(); //open the dir in a new tab
	void RemoveItem(); //Remove the item permanently
	void RunInMediaPlayer(); //open in the media player
	void RunInSlideShow(); //open in slideshow viewer
};

#endif