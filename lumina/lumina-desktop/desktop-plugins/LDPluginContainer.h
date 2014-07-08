//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
//  This class is the generic container for a desktop plugin that handles
//    saving/restoring all the movement and sizing
//===========================================
#ifndef _LUMINA_DESKTOP_DESKTOP_PLUGIN_CONTAINER_H
#define _LUMINA_DESKTOP_DESKTOP_PLUGIN_CONTAINER_H

#include <QObject>
#include <QMdiSubWindow>
#include <QSettings>
#include <QMoveEvent>
#include <QResizeEvent>

#include "LDPlugin.h"

class LDPluginContainer : public QMdiSubWindow{
	Q_OBJECT
	
private:
	QSettings *settings;
	bool locked, setup;
	
public:
	LDPluginContainer(LDPlugin *plugin = 0, bool islocked = true) : QMdiSubWindow(){
	  locked = islocked;
	  setup=true;
	  if(locked){ this->setWindowFlags(Qt::FramelessWindowHint); }
	  else{ this->setWindowFlags(Qt::CustomizeWindowHint); }
	  settings = plugin->settings; //save this pointer for access later
	  this->setContentsMargins(0,0,0,0);
	  if(!locked){
	    this->setWindowTitle(plugin->type());
	    this->setWidget( new QWidget() );
	    //this->setStyleSheet("QMdiSubWindow{ padding: 0px; background: lightgrey; border: 2px solid grey; border-radius: 1px;} QMdiSubWindow::title{ background-color: lightgrey; height: 10px;  border: none; font: bold 8;}");
	  }else{
	    this->setStyleSheet("LDPluginContainer{ padding: 0px; background: transparent; border: none;}");
	    this->setWidget(plugin);
	  }
	}
	
	~LDPluginContainer(){
	}

	void loadInitialPosition(){
	  QRect set(settings->value("location/x",0).toInt(), settings->value("location/y",0).toInt(), settings->value("location/width",this->widget()->sizeHint().width()).toInt(), settings->value("location/height",this->widget()->sizeHint().height()).toInt());
	  qDebug() << "Initial Plugin Location:" << set.x() << set.y() << set.width() << set.height();
	    if(set.height() < 10){ set.setHeight(10); } //to prevent foot-shooting
	    if(set.width() < 10){ set.setWidth(10); } //to prevent foot-shooting
	    this->setGeometry(set);
	  setup=false; //done with setup
	}
	
protected:
	void moveEvent(QMoveEvent *event){
	  //Save this location to the settings
	  if(!locked && !setup){
	    //qDebug() << "DP Move:" << event->pos().x() << event->pos().y();
	    settings->setValue("location/x", event->pos().x());
	    settings->setValue("location/y", event->pos().y());
	    settings->sync();
	  }
	  event->ignore();
	}
	void resizeEvent(QResizeEvent *event){
	  //Save this size info to the settings
	  if(!locked && !setup){
	    //qDebug() << "DP Resize:" << event->size().width() << event->size().height();
	    settings->setValue("location/width", event->size().width());
	    settings->setValue("location/height", event->size().height());
	    settings->sync();
	  }
	  event->ignore();
	}
	
};

#endif
