//===========================================
//  Lumina-DE source code
//  Copyright (c) 2013, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef _LUMINA_TOOLBAR_WIDGET_H
#define _LUMINA_TOOLBAR_WIDGET_H

#include <QToolButton>
//#include <QTimer>
//#include <QMenu>
//#include <QMouseEvent>
#include <QEvent>
#include <QWheelEvent>

//#include <QObject>
//#include <QHBoxLayout>
#include "Globals.h"

class LTBWidget : public QToolButton{
	Q_OBJECT
private:
	Lumina::STATES cstate;

	void updateBackground(){
	  if(cstate == Lumina::NONE){ this->setBackgroundRole(QPalette::NoRole); }
	  else if(cstate == Lumina::VISIBLE){ this->setBackgroundRole(QPalette::Light); }
	  else if(cstate == Lumina::INVISIBLE){ this->setBackgroundRole(QPalette::Dark); }
	  else if(cstate == Lumina::ACTIVE){ this->setBackgroundRole(QPalette::Button); }
	  else if(cstate == Lumina::NOTIFICATION){ this->setBackgroundRole(QPalette::Highlight); }
	}
	
signals:

	void wheelScroll(int change);
	
public:
	LTBWidget(QWidget* parent) : QToolButton(parent){
	  this->setStyleSheet( this->styleSheet()+" LTBWidget::menu-indicator{image: none;}");
	  cstate = Lumina::NONE;
	  //this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	  this->setPopupMode(QToolButton::InstantPopup);
	  this->setAutoRaise(true);

	}
	
	~LTBWidget(){ 
	}
	
	void setState(Lumina::STATES newstate){
	  cstate = newstate;
	  updateBackground();
	}
	
public slots:
	

protected:
	void wheelEvent(QWheelEvent *event){
	  int change = event->delta()/120; // 1/15th of a rotation (delta/120) is usually one "click" of the wheel
	  emit wheelScroll(change);
	}

};

#endif
