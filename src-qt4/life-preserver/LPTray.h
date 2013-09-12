#ifndef _LP_TRAY_H
#define _LP_TRAY_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QFileSystemWatcher>
#include <QString>
#include <QTimer>
#include <QProcess>
#include <QFile>
#include <QTextStream>

#include "mainUI.h"
#include "LPWatcher.h"

class LPTray : public QSystemTrayIcon{
		Q_OBJECT
public:
	LPTray();
	~LPTray();

private:
	//QFileSystemWatcher *watcher;
	LPWatcher *watcher;
	QMenu *menu;
	//QTimer *timer;
	mainUI *GUI;
	//QString sFile; //location of the replication status file
	//QFile *logFile, *statFile;
	//QTextStream *LFStream, *SFStream;
	//QString repTotK, lastSize;
	//int wNum; //internal tracking of which frame of the icon animation we are on
	bool initPhase;

	//void parseLogMessage(QString, bool quiet = false);
	//void parseStatusMessage(QString);
	//void setIdleToolTip();
	//void startWorkingIcon();
	//void stopWorkingIcon();
	//double displayToDoubleK(QString);

	void updateTrayIcon();
	void updateToolTip();
	
private slots:
	void watcherMessage(QString);
	//void firstCheck();
	//void slotNewLogMessage(QString);
	void slotTrayClicked(QSystemTrayIcon::ActivationReason);
	void slotClose();
	void slotSingleInstance();
	void startGUI();
	//void displayWorkingIcon();
};

#endif
