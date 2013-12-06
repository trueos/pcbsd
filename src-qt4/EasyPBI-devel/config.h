#ifndef _CONFIG_H
#define _CONFIG_H

#include <QString>
#include <QStringList>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QList>
#include <QCoreApplication>
#include <QProcessEnvironment>

class Config{
	
private:
  //Internal variables
  QStringList badPackages, exProgStruct, defaultSettings, valueStruct, detStruct;
  QList<bool> TFstruct;
 
  //Internal functions
  QString readSavedValue(QString,QString);
  void checkStructures();

public:
  Config();
  ~Config();
  // --Regular functions--
  void readMachineArch();
  bool checkDirectoryStructure();
  void loadSettingsFile();
  void saveSettingsFile();
  void scanForExternalUtilities();
  void returnToDefaults();

  //--Get/Set Data--
  QString value(QString);
  void setValue(QString, QString);

  QStringList list(QString);
  void setList(QString, QStringList);

  bool check(QString);
  void setCheck(QString, bool);

};

#endif

