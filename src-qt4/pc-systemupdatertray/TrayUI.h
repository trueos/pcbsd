#ifndef _LP_TRAY_H
#define _LP_TRAY_H

#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QString>
#include <QMessageBox>
#include <QCheckBox>
#include <QWidgetAction>
#include <QSettings>

//libpcbsd includes
#include <pcbsd-sysFlags.h>

class TrayUI : public QSystemTrayIcon{
		Q_OBJECT
public:
	TrayUI();
	~TrayUI();

private:
	SystemFlagWatcher *watcher;
	QMenu *menu;
	QCheckBox *runAtStartup, *showNotifications;
	QWidgetAction *rasAct, *snAct; //widget-action containers for the checkboxes
	QSettings *settings;
	int PBISTATUS, PKGSTATUS, WARDENSTATUS, SYSSTATUS; // 0-up2date, 1-working, 2-updateavailable
	bool noInternet; //flag whether the update checks could even work
	bool wasworking; //internal status for determining when to show messages

	void updateTrayIcon();
	void updateToolTip();

	//System Information Checks
	bool rebootNeeded();

	//External Process Launchers
	void startPBICheck();
	void startPKGCheck();
	void startSYSCheck();
	void startWardenCheck();
	void checkForUpdates(); //simplification for all the above checks

private slots:
	void launchApp(QString cmd = ""); //if empty it will launch the top-level app requiring attention

	void watcherMessage(SystemFlags::SYSFLAG, SystemFlags::SYSMESSAGE);

	void slotItemClicked(QAction*);
	void slotTrayClicked(QSystemTrayIcon::ActivationReason);
	void slotRunAtStartupClicked();
	void slotShowMessagesClicked();

	void slotClose();
	void slotSingleInstance();

};

#endif
