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
#include <QGroupBox>
#include <QString>
#include <QRect>
#include <QDebug>
#include <QSettings>
#include <QTimer>
#include <QMoveEvent>
#include <QResizeEvent>

class LDPlugin : public QGroupBox{
	Q_OBJECT
	
private:
	QString plugintype;
	QRect validRect;
	bool isMoving;
	
public:
	QSettings *settings;

	LDPlugin(QWidget *parent = 0, QRect rect = QRect(), QString ptype="unknown") : QGroupBox(parent){
	  plugintype=ptype;
	  validRect = rect;
	  settings = new QSettings("desktop-plugins",ptype);
		this->setMouseTracking(true);
	  setMovable(true);
	}
	
	~LDPlugin(){
	  //delete mvtimer;
	  delete settings;
	}
	
	QString type(){
	  return plugintype;
	}
	
	void updateValidRect(QRect rect){
	  validRect = rect;
	  QTimer::singleShot(0, this, SLOT(validateGeometry()) );
	}
	
	void setMovable(bool movable){
	  isMoving = movable;
	  this->setFlat(movable);
	  if(movable){ 
	    this->setTitle(plugintype);
	    this->setStyleSheet( "LDPlugin{ background-color: transparent; }" );
	  }else{ 
	    this->setTitle(""); 
	    this->setStyleSheet( "LDPlugin{ background-color: rgb(255,255,255); }" );
	  }
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

	void showPlugin(){
	  validateGeometry(false);
	  this->setGeometry( settings->value("location/x",0).toInt(), settings->value("location/y",0).toInt(), settings->value("location/width",this->sizeHint().width()).toInt(), settings->value("location/height",this->sizeHint().height()).toInt() );
	  this->show();
	}
	
	virtual void LocaleChange(){
	  //This needs to be re-implemented in the subclassed plugin
	    //This is where all text is set/translated
	}
	virtual void ThemeChange(){
	  //This needs to be re-implemented in the subclassed plugin
	    //This is where all the visuals are set if using Theme-dependant icons.
	}

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
	
	virtual void MousePressEvent(QMouseEvent *event){
	  qDebug() << "Mouse Down";
		event->ignore();
	}
	
	virtual void MouseReleaseEvent(QMouseEvent *event){
	  qDebug() << "Mouse Up";
		event->ignore();
	}
	
	virtual void MouseMoveEvent(QMouseEvent *event){
	  if(isMoving && event->buttons()!=Qt::NoButton){
	    qDebug() << "Moving Plugin:";
	    this->move( event->globalX() - event->pos().x(), event->globalY() - event->pos().y());
	    event->accept();
	  }else if(isMoving){
	    qDebug() << "Move: No Buttons Pressed";
	  }
	}
};

#endif