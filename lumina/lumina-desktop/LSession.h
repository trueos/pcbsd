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
#include <QTranslator>
#include <QSettings>

#include "Globals.h"
#include "AppMenu.h"
#include "SettingsMenu.h"
#include "SystemWindow.h"

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
	//Functions to be called during startup
	void setupSession();

	virtual bool x11EventFilter(XEvent *event);
	
	bool LoadLocale(QString);

	//System Tray Utilities
	static bool StartupSystemTray();
	static bool CloseSystemTray();
	//System Access
	static AppMenu* applicationMenu();
	static void systemWindow();
	static SettingsMenu* settingsMenu();

public slots:
	void launchStartupApps();

private slots:
	//Internal simplification functions
	void checkUserFiles();
	void loadStyleSheet();

	//system tray functions
	void parseClientMessageEvent(XClientMessageEvent *event);

signals:
	void NewSystemTrayApp(WId); //WinID
	void WindowListEvent(WId);
	void WindowListEvent();
	void LocaleChanged();
	
};

#endif
