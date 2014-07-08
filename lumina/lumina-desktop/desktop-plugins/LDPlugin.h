//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
//  This class is the generic container layout for all desktop plugins
//  Simply subclass this when creating a new plugin to enable correct
//    visibility and usage within the desktop window
//===========================================
#ifndef _LUMINA_DESKTOP_DESKTOP_PLUGIN_H
#define _LUMINA_DESKTOP_DESKTOP_PLUGIN_H

#include <QObject>
#include <QMdiSubWindow>
#include <QString>
#include <QRect>
#include <QDebug>
#include <QSettings>
#include <QTimer>
#include <QMoveEvent>
#include <QResizeEvent>

class LDPlugin : public QWidget{
	Q_OBJECT
	
private:
	QString plugintype;
	QRect validRect;
	
public:
	QSettings *settings;

	LDPlugin(QWidget *parent = 0, QRect rect = QRect(), QString ptype="unknown") : QWidget(parent){
	  plugintype=ptype;
	  validRect = rect;
	  settings = new QSettings("desktop-plugins",ptype);
	}
	
	~LDPlugin(){
	  delete settings;
	}
	
	QString type(){
	  return plugintype;
	}
	
	void updateValidRect(QRect rect){
	  validRect = rect;
	  QTimer::singleShot(0, this, SLOT(validateGeometry()) );
	}
	
public slots:
	void validateGeometry(bool canmove = true){
	  //This checks the current location of the widget against the valid area
	  qDebug() << "DP geom:" << this->rect().x() << this->rect().y() << this->rect().width() << this->rect().height();
	  if(!validRect.contains(this->rect())){
	    qDebug() << "Invalid Geometry:" << this->rect().x() << this->rect().y() << this->rect().width() << this->rect().height();
	    qDebug() << " - Valid:" << validRect.x() << validRect.y() << validRect.width() << validRect.height();
	    //Move to just within the valid region
	    if(canmove){
		    
	    }
	  }
	}
	
	virtual void LocaleChange(){
	  //This needs to be re-implemented in the subclassed plugin
	    //This is where all text is set/translated
	}
	virtual void ThemeChange(){
	  //This needs to be re-implemented in the subclassed plugin
	    //This is where all the visuals are set if using Theme-dependant icons.
	}
/*
protected:
	virtual void MoveEvent(QMoveEvent *event){
	  //Save this location to the settings
	  qDebug() << "DP Move:" << event->pos().x() << event->pos().y();
	  settings->setValue("location/x", event->pos().x());
	  settings->setValue("location/y", event->pos().y());
	}
	virtual void ResizeEvent(QResizeEvent *event){
	  //Save this size info to the settings
	  qDebug() << "DP Resize:" << event->size().width() << event->size().height();
	  settings->setValue("location/width", event->size().width());
	  settings->setValue("location/height", event->size().height());
	}
*/	
};

#endif