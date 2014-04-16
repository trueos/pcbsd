//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef _LUMINA_DESKTOP_TASK_MANAGER_PLUGIN_H
#define _LUMINA_DESKTOP_TASK_MANAGER_PLUGIN_H

// Qt includes
#include <QWidget>
#include <QList>
#include <QString>
#include <QDebug>
#include <QTimer>
#include <QEvent>

// libLumina includes
#include <LuminaX11.h>

// Local includes
#include "LTaskButton.h"
#include "LWinInfo.h"
#include "../LPPlugin.h"
#include "../../LSession.h" //keep this last

class LTaskManagerPlugin : public LPPlugin{
	Q_OBJECT
public:
	LTaskManagerPlugin(QWidget *parent=0);
	~LTaskManagerPlugin();

private:
	QList<LTaskButton*> BUTTONS; //to keep track of the current buttons
	QTimer *timer;
	bool updating; //quick flag for if it is currently working

private slots:
	void UpdateButtons();
	void checkWindows();

public slots:
	void LocaleChange(){
	  UpdateButtons();
	}
	void ThemeChange(){
	  UpdateButtons();
	}

};
#endif