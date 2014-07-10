//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
//  This panel plugin is the main button that allow the user to run 
//    applications or logout of the desktop
//===========================================
#ifndef _LUMINA_DESKTOP_USER_MENU_H
#define _LUMINA_DESKTOP_USER_MENU_H

// Qt includes
#include <QMenu>
#include <QAction>
#include <QDir>
#include <QSettings>
#include <QCoreApplication>

// Lumina-desktop includes
#include "../../Globals.h"
#include "../LPPlugin.h" //main plugin widget
#include "../LTBWidget.h" //visual plugin button
#include "../../LSession.h"

// libLumina includes
#include "LuminaXDG.h"

class LUserButton : public LTBWidget{
	Q_OBJECT
public:
	LUserButton(QWidget *parent = 0);
	~LUserButton();

	void UpdateMenu();

private:
	QDir *homedir;
	QMenu *mainMenu; 
	QSettings *settings;

	void goToDir(QString);

private slots:
	void buttonClicked(){
	  UpdateMenu();
	  mainMenu->popup(this->mapToGlobal( QPoint(0,this->height()) ) );
	}
	
	void menuButtonClicked(QAction *act){
	  if(act->parent() != mainMenu){ return; }
	  if(!act->whatsThis().isEmpty()){
	    goToDir( act->whatsThis() );
	  }
	}
	
	void Logout();
	//void Restart();
	//void Shutdown();
};

// PANEL PLUGIN WRAPPER
class LUserButtonPlugin : public LPPlugin{
	Q_OBJECT
private:
	LUserButton *button;

public:
	LUserButtonPlugin(QWidget *parent = 0, QString id = "userbutton", bool horizontal=true) : LPPlugin(parent, id, horizontal){
	  button = new LUserButton(parent);
	  this->layout()->addWidget(button);
	}
	~LUserButtonPlugin(){
	}
public slots:
	void LocaleChange(){
	  button->UpdateMenu();
	}
	void ThemeChange(){
	  button->UpdateMenu();
	}
};

#endif