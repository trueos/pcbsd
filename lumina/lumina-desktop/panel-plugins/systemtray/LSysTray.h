//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef _LUMINA_DESKTOP_SYSTRAY_H
#define _LUMINA_DESKTOP_SYSTRAY_H

//Qt includes
#include <QFrame>
#include <QHBoxLayout>
#include <QDebug>
#include <QX11Info>
#include <QX11EmbedContainer>

//Local includes
#include "../../LSession.h"
#include "../LPPlugin.h"
#include "TrayIcon.h"

class LSysTray : public LPPlugin{
	Q_OBJECT
public:
	LSysTray(QWidget *parent = 0, QString id="systemtray", bool horizontal=true);
	~LSysTray();

	void start();
	void stop();
	
private:
	bool isRunning;
	QList<TrayIcon*> trayIcons;
	
	
	
private slots:
	void addTrayIcon(WId win);
	void updateStatus();
	void trayAppClosed();
	
};

#endif
