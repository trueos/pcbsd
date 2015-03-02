#ifndef _LP_TRAY_H
#define _LP_TRAY_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QString>
#include <QSettings>
#include <QDateTime>
#include <QCheckBox>
#include <QWidgetAction>
#include <QFileSystemWatcher>

//libpcbsd includes
#include <pcbsd-utils.h>

#include "SysStatus.h" //Includes the UPDATE_* definitions

#define PCBSD_CONF_FILE QString("/usr/local/etc/pcbsd.conf")
#define SYSCACHE_LOG_FILE QString("/var/log/pc-syscache.log")
/*#define UPDATE_LOG_FILE QString("/var/log/pc-updatemanager.log")
#define UPDATE_PROC_DIR QString("/tmp/.pcbsdflags")
#define UPDATE_PROC_FLAG_FILE_FILTER QString("pkg-update-available-*")
#define UPDATE_PROC_WORKING QString("updating")
#define UPDATE_PROC_FINISHED QString("success")*/

class TrayUI : public QSystemTrayIcon{
	Q_OBJECT
public:
	TrayUI();
	~TrayUI();

private:
	//System Status
	SysStatus CSTAT; //current status

	//Scheduler variables
	QDateTime lastDirCheck; //the last time the watched directory was checked
	QString AUNotice; //Auto-Update Setting (translated string)
	QString AUval; //Auto-Update setting (internal value)
	void UpdateAUNotice(); //update the notice and re-draw the icon/text

	//Passive System Watchers
	QFileSystemWatcher *watcher;
	bool PerformingCheck;

	//UI Elements
	QSettings *settings;
	QMenu *mainMenu;
	QCheckBox *runAtStartup, *showNotifications, *checkJails, *torMode;
	QWidgetAction *rasA, *snA, *cjA, *tmA; //actions for the checkboxes

private slots:
	//Internal slots
	void watcherDirChange();
	void watcherFileChange(QString);
	void checkForUpdates(); //updates current status
	void UpdateIcon(); //based on current status
	void ShowMessage();

	//UI Interaction Slots
	void BackendResync(); //will cause syscache to re-sync
	void launchApp(QString cmd = ""); //if empty it will launch the top-level app requiring attention
	void slotItemClicked(QAction*);
	void slotTrayClicked(QSystemTrayIcon::ActivationReason);
	void slotRunAtStartupClicked();
	void slotShowMessagesClicked();
	void slotCheckJailsClicked();
	void slotClose();
	void slotSingleInstance();
	void slotToggleTorMode();
	void slotCheckTorStatus();
};

#endif
