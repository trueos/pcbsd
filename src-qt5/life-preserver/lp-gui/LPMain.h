#ifndef _LP_MAIN_H
#define _LP_MAIN_H

#include <QMainWindow>
#include <QComboBox>
#include <QToolButton>
#include <QLabel>
#include <QString>
#include <QStringList>
#include <QRadioButton>
#include <QWidgetAction>
#include <QFileSystemModel>
#include <QInputDialog>
#include <QDateTime>
#include <QTimer>
#include <QFileDialog>
#include <QFileSystemWatcher>
#include <QSettings>

#include "LPBackend.h"
#include "LPContainers.h"
#include "LPGUtils.h"
#include "LPWizard.h"
#include "LPConfig.h"
#include "LPClassic.h"
#include "LPISCSIWizard.h"

namespace Ui{
	class LPMain;
};

class LPMain : public QMainWindow{
	Q_OBJECT
public:
	LPMain(QWidget *parent = 0);
	~LPMain();

public slots:
	void slotSingleInstance();

private:
	Ui::LPMain *ui;
	QRadioButton *viewBasic, *viewAdvanced;
	QFileSystemModel *fsModel;
	bool poolSelected;
	LPDataset POOLDATA;
	QMessageBox *waitBox;
	QFileSystemWatcher *watcher;
	QTimer *timer;
	QSettings *settings;
	LPClassic *classicDLG;

	void showErrorDialog(QString title, QString message, QString errors);
	void showWaitBox(QString message);
	void hideWaitBox();

private slots:
	void updatePoolList();  //re-load available pools
	void viewChanged();
	void updateTabs();      //load current pool info and update tabs
	void updateDataset();  //restore dataset changed
	void updateSnapshot(); //selected snapshot changed
	void nextSnapshot();
	void prevSnapshot();
	void setFileVisibility();
	void restoreFiles();
	void openConfigGUI();
	void autoRefresh();
	// -- Menu Actions --
	//File Menu
	void menuAddPool(QAction*);
	void menuRemovePool(QAction*);
	void menuSaveSSHKey();
	void menuSetupISCSI(QAction*);
	void menuCloseWindow();
	//Classic Backups
	void menuCompressHomeDir(QAction*);
	void menuExtractHomeDir();
	//Disk Menu
	void menuAddDisk();
	void menuRemoveDisk(QAction*);
	void menuOfflineDisk(QAction*);
	void menuOnlineDisk(QAction*);
	void menuStartScrub();
	void menuStopScrub();
	//Snapshots Menu
	void menuNewSnapshot();
	void menuRemoveSnapshot(QAction*);
	void menuStartReplication();

protected:
	
};

#endif
