//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef _LUMINA_DESKTOP_LDESKTOP_H
#define _LUMINA_DESKTOP_LDESKTOP_H

#include <QCoreApplication>


#include <QSettings>
#include <QFile>
#include <QList>
#include <QDebug>
#include <QTimer>

#include <LuminaXDG.h>

#include "LPanel.h"
#include "Globals.h"
#include "LSession.h"
#include "desktop-plugins/LDPlugin.h"
#include "desktop-plugins/NewDP.h"

class LDesktop : public QObject{
	Q_OBJECT
public:
	LDesktop(int deskNum=0);
	~LDesktop();
	
public slots:
	void SystemLogout(){ QCoreApplication::exit(0); }
	void SystemRestart(){ SYSTEM::restart(); QCoreApplication::exit(0); }
	void SystemShutdown(){ SYSTEM::shutdown(); QCoreApplication::exit(0); }
	void SystemTerminal(){ QProcess::startDetached("xterm"); }
	
private:
	QSettings *settings;
	QTimer *bgtimer;
	QDesktopWidget *desktop;
	QString DPREFIX;
	int desktopnumber;
	int xoffset;
	bool defaultdesktop;
	QList<LPanel*> PANELS;
	QWidget *bgWindow;
	QMenu *deskMenu;
	AppMenu *appmenu;
	QList<LDPlugin*> PLUGINS;
	
private slots:
	void UpdateMenu();
	void ShowMenu(){
	  deskMenu->popup(QCursor::pos());
	}
	
	void UpdateDesktop();
	
	void UpdatePanels();

	void UpdateBackground();

};
#endif
