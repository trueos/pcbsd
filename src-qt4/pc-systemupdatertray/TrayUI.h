#ifndef _LP_TRAY_H
#define _LP_TRAY_H

#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QString>
#include <QMessageBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QWidgetAction>
#include <QSettings>
#include <QTime>

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
    QMenu *schedule_menu;
	QCheckBox *runAtStartup, *showNotifications;
    QRadioButton* disableAutoCheck, *checkEveryHour, *checkEvery6hrs, *checkEveryDay;
	QWidgetAction *rasAct, *snAct; //widget-action containers for the checkboxes
    QWidgetAction *dacAct, *dhAct, *d6hAct, *ddAct; //widget-action containers for the radio buttons
    QSettings *settings;
	QTimer *chktime;
	int PKGSTATUS, WARDENSTATUS, SYSSTATUS; // 0-up2date, 1-working, 2-updateavailable
	bool noInternet; //flag whether the update checks could even work
	bool wasworking; //internal status for determining when to show messages

    QTime  nextCheckTime;
    QTimer* checkTimer;
    int currentCheckInterval;

    void makeScheduleMenu();

	void updateTrayIcon();
	void updateToolTip();

	//System Information Checks
	bool rebootNeeded();

	//External Process Launchers
	void startPKGCheck();
	void startSYSCheck();
	void startWardenCheck();
    void setCheckInterval(int sec);

private slots:
	void checkForUpdates(); //simplification to start all checks
	void startupChecks(); //initial checks after app startup
	void launchApp(QString cmd = ""); //if empty it will launch the top-level app requiring attention

	void watcherMessage(SystemFlags::SYSFLAG, SystemFlags::SYSMESSAGE);

	void slotItemClicked(QAction*);
	void slotTrayClicked(QSystemTrayIcon::ActivationReason);
	void slotRunAtStartupClicked();
	void slotShowMessagesClicked();

	void slotClose();
	void slotSingleInstance();

    void slotCheckTimer();

    void slotParsePeriodControls();
};

#endif
