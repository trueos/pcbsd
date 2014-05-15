//===========================================
//  Lumina-DE source code
//  Copyright (c) 2013, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef _LUMINA_TOOLBAR_WIDGET_H
#define _LUMINA_TOOLBAR_WIDGET_H

#include <QToolButton>
#include <QEvent>
#include <QWheelEvent>

#include "Globals.h"

class LTBWidget : public QToolButton{
	Q_OBJECT
private:
	Lumina::STATES cstate;
	QString rawstyle;
	void updateBackground(){
	  QString background = "background: transparent; "; //default value
	  QString border = "border: 1px solid transparent;";
	  if(cstate == Lumina::NONE){ } //just use the defaults
	  else if(cstate == Lumina::VISIBLE){ background = "background: rgba(170,170,170,120);"; border="border: 1px solid grey;"; }
	  else if(cstate == Lumina::INVISIBLE){}
	  else if(cstate == Lumina::ACTIVE){ background= "background: rgba(175,238,238,255);"; border ="border: 1px solid grey;"; }
	  else if(cstate == Lumina::NOTIFICATION){ background= "background: rgba(255,250,205,255);"; border="border: 1px solid grey"; }
	  QString raw = rawstyle;
	  this->setStyleSheet( raw.replace("%1",background).replace("%2", border) );
	}
	
signals:

	void wheelScroll(int change);
	
public:
	LTBWidget(QWidget* parent) : QToolButton(parent){
	  //this->setStyleSheet( this->styleSheet()+" LTBWidget::menu-indicator{image: none;}");
	  cstate = Lumina::NONE;
	  this->setPopupMode(QToolButton::InstantPopup);
	  this->setAutoRaise(true);
	  rawstyle = "LTBWidget{ %1 %2 border-radius: 5px;}  LTBWidget::menu-indicator{image: none;} LTBWidget::hover{ %1 border: 1px solid lightblue; border-radius: 5px; } LTBWidget::menu-button{ background: transparent; width: 15px; } LTBWidget[popupMode=\"1\"]{%1 %2 border-radius: 5px; padding-right: 15px;} LTBWidget[popupMode=\"1\"]::hover{%1 border: 1px solid lightblue; border-radius: 5px; padding-right: 15px}";
	  updateBackground();
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
