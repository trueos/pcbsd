#ifndef _PCBSD_UPDATE_MANAGER_UI_H
#define _PCBSD_UPDATE_MANAGER_UI_H

#include <QMainWindow>
#include <QObject>
#include <QString>
#include <QFileSystemWatcher>

#include <pcbsd-utils.h>

#define UPDATE_LOG_FILE QString("/var/log/pc-updatemanager.log")

namespace Ui{
	class MainUI;
};

class MainUI : public QMainWindow{
	Q_OBJECT
public:
	MainUI();
	~MainUI();

public slots:
	void slotSingleInstance();

private:
	Ui::MainUI *ui;
	QFileSystemWatcher *watcher;

	void InitUI(); //initialize the UI (widgets, options, menus, current values)
	
private slots:
	void CloseUI(){
	  this->close();
	}
	
	void UpdateUI(); //refresh the entire UI , and system status structure
	// (generally only for initialization or after an update was started/stopped)
	
	//Update tab
	void updateSelChange(); //update selection changed
	void updateDetailsChange(); //update details changed
	void startUpdates(); //Start selected update
	//Patches tab
	void patchSelChange(); //patch selection changed
	void startPatches(); //Start installing the selected patches
	//Log tab
	void updateLogChanged(); //this is connected to a file watcher for changes
	//Configure tab
	void autoUpChange(); //auto-update option changed


};
#endif
