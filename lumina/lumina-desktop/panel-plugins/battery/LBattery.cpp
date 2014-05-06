//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Susanne Jaeckel
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LBattery.h"

LBattery::LBattery(QWidget *parent) : LPPlugin(parent, "battery"){
  iconOld = -1;
  //Setup the widget
  label = new LTBWidget(this);
    //label->setAlignment(Qt::AlignCenter);
  this->layout()->addWidget(label);
  //Setup the timer
  timer = new QTimer();
  timer->setInterval(5000); //update every 3 seconds
  connect(timer,SIGNAL(timeout()), this, SLOT(updateBattery()) );
  updateBattery();
  timer->start();
}

LBattery::~LBattery(){
  timer->stop();
  delete timer;
}

void LBattery::updateBattery(){
  // Get current state of charge
  QStringList result = LUtils::getCmdOutput("/usr/sbin/apm", QStringList() << "-al");
  int charge = result.at(1).toInt();
//qDebug() << "1: " << result.at(0).toInt() << " 2: " << result.at(1).toInt();
  int icon = 0;
  if (charge > 100) { icon = 5; }
  else if (charge > 90) { icon = 4; }
  else if (charge > 70) { icon = 3; }
  else if (charge > 50) { icon = 2; }
  else if (charge > 30) { icon = 1; }
  icon = icon + result.at(0).toInt() * 10;
  if (icon != iconOld) {
    switch (icon) {
      case 0:
        label->setIcon( LXDG::findIcon("battery-caution", ":/images/battery-caution.png") );
        break;
      case 1:
        label->setIcon( LXDG::findIcon("battery-040", ":/images/battery-040.png") );
        break;
      case 2:
        label->setIcon( LXDG::findIcon("battery-060", ":/images/battery-060.png") );
        break;
      case 3:
        label->setIcon( LXDG::findIcon("battery-080", ":/images/battery-080.png") );
        break;
      case 4:
        label->setIcon( LXDG::findIcon("battery-100", ":/images/battery-100.png") );
        break;
      case 10:
        label->setIcon( LXDG::findIcon("battery-charging-caution", ":/images/battery-caution.png") );
        break;
      case 11:
        label->setIcon( LXDG::findIcon("battery-charging-040", ":/images/battery-040.png") );
        break;
      case 12:
        label->setIcon( LXDG::findIcon("battery-charging-060", ":/images/battery-060.png") );
        break;
      case 13:
        label->setIcon( LXDG::findIcon("battery-charging-080", ":/images/battery-080.png") );
        break;
      case 14:
        label->setIcon( LXDG::findIcon("battery-charging-100", ":/images/battery-100.png") );
        break;
      default:
        label->setIcon( LXDG::findIcon("battery-missing", ":/images/battery-missing.png") );
        break;
    }
    iconOld = icon;
  }
  //Now update the display
  label->setToolTip(QString("%1 %").arg(charge).append(icon > 9 && icon < 15 ? " and charging" : ""));
}
