#ifndef _LP_TRAY_H
#define _LP_TRAY_H

#include <QSystemTrayIcon>
#include <QMenu>
//#include <QFileSystemWatcher>
#include <QString>
//#include <QTimer>
//#include <QProcess>
//#include <QFile>
//#include <QTextStream>
#include <QMessageBox>

#include "mainUI.h"
#include "LPWatcher.h"

class LPTray : public QSystemTrayIcon{
		Q_OBJECT
public:
	LPTray();
	~LPTray();

private:
	LPWatcher *watcher;
	QMenu *menu;
	mainUI *GUI;
	//bool initPhase;


	void updateTrayIcon();
	void updateToolTip();
	
private slots:
	void watcherMessage(QString type = "");

	void slotTrayClicked(QSystemTrayIcon::ActivationReason);
	void slotClose();
	void slotSingleInstance();
	void startGUI();

};

#endif
