//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Susanne Jaeckel
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LBattery.h"

LBattery::LBattery(QWidget *parent) : LPPlugin(parent, "battery"){
  //Setup the widget
  label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);
  this->layout()->addWidget(label);
  //Setup the timer
  timer = new QTimer();
  timer->setInterval(3000); //update every 3 seconds
  connect(timer,SIGNAL(timeout()), this, SLOT(updateBattery()) );
  updateBattery();
  timer->start();
}

LBattery::~LBattery(){
  timer->stop();
  delete timer;
}

void LBattery::updateBattery(){
  //Now update the display
  label->setText("19%");
  label->setToolTip("High: 19% ~23 Minuten");
}
