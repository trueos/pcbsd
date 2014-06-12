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

// libLumina includes
#include <LuminaXDG.h>

// Local includes

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
	QFileSystemModel *fsmod;
	QMenu *contextMenu;
	QSettings *settings;

	//Internal variables
	QStringList snapDirs; //internal saved variable for the discovered zfs snapshot dirs
	QStringList snaps; //names of the snapshots corresponding to snapDirs
	QModelIndex CItem; //the item that was right-clicked (for the context menu)

	//Simplification Functions
	void setupIcons(); 		//used during initialization
	void setupConnections(); 	//used during initialization
	void loadBrowseDir(QString);
	void loadSnapshot(QString);
	bool findSnapshotDir(); //returns true if the current dir has snapshots available
	QString getCurrentDir();
	void setCurrentDir(QString);

private slots:
	void slotSingleInstance(const QString &in){
	  this->show();
	  this->raise();
	  this->OpenDirs(in.split("\n"));
	}
	
	//General page switching
	void goToMultimediaPage();
	void goToRestorePage();
	void goToSlideshowPage();
	void goToBrowserPage();
	
	//Menu Actions
	void on_actionNew_Tab_triggered();
	
	//Toolbar Actions
	void on_actionBack_triggered();
	void on_actionUpDir_triggered();
	void on_actionHome_triggered();
	void on_actionBookMark_triggered();

	//Browser Functions
	void tabChanged(int tab);
	void tabClosed(int tab);
	void ItemRun( const QModelIndex&);
	void OpenContextMenu(const QPoint&);

	//Context Menu Actions
	void OpenItem(); //run "lumina-open" on it
	void OpenItemWith(); //run "lumina-open -select" on it
	void OpenDir(); //open the dir in a new tab
	void RunInMediaPlayer(); //open in the media player
	void RunInSlideShow(); //open in slideshow viewer
};

#endif