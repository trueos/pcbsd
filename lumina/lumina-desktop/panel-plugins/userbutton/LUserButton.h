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
#include <QToolButton>
#include <QTimer>

// Lumina-desktop includes
#include "../../Globals.h"
#include "../LPPlugin.h" //main plugin widget
//#include "../LTBWidget.h" //visual plugin button

// libLumina includes
#include "LuminaXDG.h"

class LUserButton : public QToolButton{
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
};

// PANEL PLUGIN WRAPPER
class LUserButtonPlugin : public LPPlugin{
	Q_OBJECT
private:
	LUserButton *button;

public:
	LUserButtonPlugin(QWidget *parent = 0, QString id = "userbutton", bool horizontal=true) : LPPlugin(parent, id, horizontal){
	  this->setStyleSheet( "QToolButton::menu-indicator{ image: none; } QToolButton{padding: 0px;}");
	  button = new LUserButton(parent);
	  button->setAutoRaise(true);
	  button->setPopupMode(QToolButton::InstantPopup);
	  this->layout()->setContentsMargins(0,0,0,0);
	  this->layout()->addWidget(button);
	  QTimer::singleShot(0,this, SLOT(OrientationChange()));
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
	
	void OrientationChange(){
	  QSize sz;
	  if(this->layout()->direction()==QBoxLayout::LeftToRight){
	    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
	    button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	    sz = QSize(this->height(), this->height());
	  }else{
	    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
	    button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	    sz = QSize(this->width(), this->width());
	  }
	  button->setIconSize(sz);
	  this->layout()->update();
	}
};

#endif