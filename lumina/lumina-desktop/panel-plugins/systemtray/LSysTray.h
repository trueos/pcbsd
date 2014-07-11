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
	QFrame *frame;
	QBoxLayout *LI; //layout items
	
private slots:
	void addTrayIcon(WId win);
	void updateStatus();
	void trayAppClosed();

public slots:
	virtual void OrientationChange(){
	   //make sure the internal layout has the same orientation as the main widget
	   LI->setDirection( this->layout()->direction() );
	   //Re-adjust the maximum widget size
	  int sz;
	  if(this->layout()->direction()==QBoxLayout::LeftToRight){
	    this->setMaximumSize( trayIcons.length()*this->height(), 10000);
	    sz = this->height()-2*frame->frameWidth();
	  }else{
	    this->setMaximumSize(10000, trayIcons.length()*this->width());
	    sz = this->width()-2*frame->frameWidth();
	  }
	  for(int i=0; i<trayIcons.length(); i++){
	    trayIcons[i]->setSizeSquare(sz);
	    trayIcons[i]->updateIcon();
	  }
	}
};

#endif
