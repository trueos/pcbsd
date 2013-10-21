#ifndef _LP_TRAY_H
#define _LP_TRAY_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QString>
#include <QMessageBox>

#include "LPWatcher.h"
#include "LPMessages.h"

class LPTray : public QSystemTrayIcon{
		Q_OBJECT
public:
	LPTray();
	~LPTray();

private:
	LPWatcher *watcher;
	LPMessages *msgdlg;
	QMenu *menu;

	void updateTrayIcon();
	void updateToolTip();
	
private slots:
	void watcherMessage(QString type = "");

	void slotTrayClicked(QSystemTrayIcon::ActivationReason);
	void slotClose();
	void slotSingleInstance();
	void startGUI();
	void startMessageDialog();
	void refreshStatus();

};

#endif
