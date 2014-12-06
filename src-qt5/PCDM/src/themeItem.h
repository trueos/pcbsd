/* PCDM Login Manager:
*  Written by Ken Moore (ken@pcbsd.org) 2012/2013
*  Copyright(c) 2013 by the PC-BSD Project
*  Available under the 3-clause BSD license
*/

#ifndef _PCDM_THEME_ITEM_H
#define _PCDM_THEME_ITEM_H

#include <QString>
#include <QSize>

class ThemeItem{

public:
  ThemeItem(){ enabled=false; x1=-1; x2=-1; y1=-1; y2=-1; }
  //~ThemeItem(){}
  //values
  bool enabled;
  bool isVertical;
  QString value;
  int x1,x2,y1,y2;
  QString icon;
  QSize iconSize;

};

#endif
