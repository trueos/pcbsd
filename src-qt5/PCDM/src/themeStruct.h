/* PCDM Login Manager:
*  Written by Ken Moore (ken@pcbsd.org) 2012/2013
*  Copyright(c) 2013 by the TrueOS Project
*  Available under the 3-clause BSD license
*/

#ifndef _PCDM_THEME_STRUCT_H
#define _PCDM_THEME_STRUCT_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QFile>
#include <QDebug>
#include <QSize>
#include "themeItem.h"

class ThemeStruct{

private:
  QStringList itemNames;
  QList<ThemeItem> items;
  QString applicationStyleSheet;

public:
  ThemeStruct(){}
  ~ThemeStruct();
  //How to fill a structure with items
  void loadThemeFile(QString);
  //import/export functions for passing items between different theme structures
  ThemeItem exportItem(QString);
  void importItem(QString, ThemeItem);

  //Main Interface Functions
  bool validItem(QString);
  QStringList invalidItems();
  bool itemIsEnabled(QString);
  bool itemIsVertical(QString);
  QString itemValue(QString);
  QString itemIcon(QString);
  QSize itemIconSize(QString);
  int itemLocation(QString, QString variable);
  QString styleSheet();
  QStringList getSpacers();

};

#endif

