//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef _LUMINA_DESKTOP_SESSION_H
#define _LUMINA_DESKTOP_SESSION_H

#include <QApplication>
#include <QDebug>
#include <QString>
#include <QX11Info>
#include <QEvent>

#include "Globals.h"
#include "AppMenu.h"
#include "SettingsMenu.h"

//LibLumina X11 class
#include <LuminaX11.h>

//SYSTEM TRAY STANDARD DEFINITIONS
#define SYSTEM_TRAY_REQUEST_DOCK 0
#define SYSTEM_TRAY_BEGIN_MESSAGE 1
#define SYSTEM_TRAY_CANCEL_MESSAGE 2


class LSession : public QApplication{
	Q_OBJECT
public:
	LSession(int &argc, char **argv);
	~LSession();
	
	virtual bool x11EventFilter(XEvent *event);
	
	//System Tray Utilities
	static bool StartupSystemTray();
	static bool CloseSystemTray();
	//System Access
	static AppMenu* applicationMenu();
	static void SystemWindow();
	static SettingsMenu* settingsMenu();
	

private slots:
	//system tray functions
	void parseClientMessageEvent(XClientMessageEvent *event);

signals:
	void NewSystemTrayApp(WId); //WinID
	void WindowListEvent();
	
};

#endif
