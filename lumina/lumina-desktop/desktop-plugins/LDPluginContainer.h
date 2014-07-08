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
	bool locked;
	
public:
	LDPluginContainer(LDPlugin *plugin = 0, bool islocked = true) : QMdiSubWindow(){
	  this->setWidget(plugin);
	  locked = islocked;
	  settings = plugin->settings; //save this pointer for access later
	  this->setContentsMargins(0,0,0,0);
	  QRect set(settings->value("location/x",0).toInt(), settings->value("location/y",0).toInt(), settings->value("location/width",plugin->sizeHint().width()).toInt(), settings->value("location/height",plugin->sizeHint().height()).toInt());
	  qDebug() << "Initial Plugin Location:" << set.x() << set.y() << set.width() << set.height();
	  if(!locked){
	    this->setWindowTitle(tr("Drag to move"));
	    //this->setStyleSheet("QMdiSubWindow{ padding: 0px; background: lightgrey; border: 2px solid grey; border-radius: 1px;} QMdiSubWindow::title{ background-color: lightgrey; height: 10px;  border: none; font: bold 8;}");
	    QRect ipt = this->widget()->geometry();
	    this->setGeometry( set.x()-ipt.x(), set.y()-ipt.y(), set.width()+(this->frameGeometry().width()-ipt.width()), set.height()+(this->frameGeometry().height()-ipt.height()) );
	  }else{
	    this->setStyleSheet("LDPluginContainer{ padding: 0px; background: transparent; border: none;}");
	    this->setGeometry( settings->value("location/x",0).toInt(), settings->value("location/y",0).toInt(), settings->value("location/width",plugin->sizeHint().width()).toInt(), settings->value("location/height",plugin->sizeHint().height()).toInt());
	  }
	}
	
	~LDPluginContainer(){
	}

	void loadInitialPosition(){
	  //qDebug() << "Initial Plugin Location:" << settings->value("location/x",0).toInt() << settings->value("location/y",0).toInt() << settings->value("location/width",this->widget()->sizeHint().width()).toInt() << settings->value("location/height",this->widget()->sizeHint().height()).toInt();
	  if(!locked){
	    this->setGeometry( settings->value("location/x",0).toInt()-2, settings->value("location/y",0).toInt()-12, settings->value("location/width",this->widget()->sizeHint().width()).toInt()+4, settings->value("location/height",this->widget()->sizeHint().height()+14).toInt());
	  }else{
	    this->setGeometry( settings->value("location/x",0).toInt(), settings->value("location/y",0).toInt(), settings->value("location/width",this->widget()->sizeHint().width()).toInt(), settings->value("location/height",this->widget()->sizeHint().height()).toInt());
	  }
	}
	
protected:
	void moveEvent(QMoveEvent *event){
	  //Save this location to the settings
	  if(!locked){
	    //qDebug() << "DP Move:" << event->pos().x() << event->pos().y();
	    settings->setValue("location/x", event->pos().x()+2);
	    settings->setValue("location/y", event->pos().y()+12);
	    settings->sync();
	  }
	}
	void resizeEvent(QResizeEvent *event){
	  //Save this size info to the settings
	  if(!locked){
	    //qDebug() << "DP Resize:" << event->size().width() << event->size().height();
	    settings->setValue("location/width", event->size().width()-4);
	    settings->setValue("location/height", event->size().height()-14);
	    settings->sync();
	  }
	}
	
};

#endif
