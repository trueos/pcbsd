//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LClock.h"

LClock::LClock(QWidget *parent) : LPPlugin(parent, "clock"){
  //Setup the widget
  label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);
  this->layout()->addWidget(label);
  //Setup the timer
  timer = new QTimer();
  timer->setInterval(1000); //update once a second
  connect(timer,SIGNAL(timeout()), this, SLOT(updateTime()) );
  updateTime();
  timer->start();
}

LClock::~LClock(){
  timer->stop();
  delete timer;
}

void LClock::updateTime(){
  QDateTime CT = QDateTime::currentDateTime();
  //Now update the display
  label->setText( CT.toString("h:mm AP") );
  label->setToolTip(CT.toString("ddd M/d/yy"));
}
