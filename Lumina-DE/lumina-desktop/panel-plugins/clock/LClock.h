//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef _LUMINA_DESKTOP_CLOCK_H
#define _LUMINA_DESKTOP_CLOCK_H

#include <QTimer>
#include <QDateTime>
#include <QLabel>
#include <QWidget>
#include <QString>

#include "../LPPlugin.h"

class LClock : public LPPlugin{
	Q_OBJECT
public:
	LClock(QWidget *parent = 0);
	~LClock();
	
private:
	QTimer *timer;
	QLabel *label;
	
private slots:
	void updateTime();
	
};

#endif
