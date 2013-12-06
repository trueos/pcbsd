#ifndef _MODBUILD_H
#define _MODBUILD_H

#include <QString>
#include <QStringList>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QDir>

class ModBuild{
	
private:
  //Internal variables
  QStringList progStruct, mkStruct, serverStruct, menuStruct, desktopStruct, mimeStruct, linksStruct, portStruct;
  QString modulePath, saveMenuFile, saveDesktopFile, saveMimeFile, savePortDir;
  //Internal functions
  QString assignPortMenuCategory(QString);
  int readMakeFile(QString);
  static bool emptyDirectory(QString);
  QString getFilenameFromPath(QString);
  QString checkYesNo(QString);

public:
  ModBuild();
  ~ModBuild();
  //Public variables
  QStringList currentBins, currentMimeTypes, currentIcons, validMenuCategories;
  bool isPortPBI, isLocalPBI;

  // --Regular functions--
    //Module startup functions
  bool createNewModule(QString, QString, QString);
  bool loadModule(QString);
  bool readPortInformation(QString);
    //pbi.conf functions
  bool loadPBIconf();
  bool writePBIconf();
    //desktop entry functions
  bool loadDesktop(QString);
  bool writeDesktop();
  bool removeDesktop();
    //menu entry functions
  bool loadMenu(QString);
  bool writeMenu();
  bool removeMenu();
    //mime type functions
  bool loadMime(QString);
  bool writeMime();
  bool removeMime();
    //external-links functions
  bool loadExternalLinks();
  bool writeExternalLinks();
  void addExternalLink(QString, QString, QStringList);
  void removeExternalLink(QString, QString);
  QStringList externalLinks();
    //resources functions
  bool addResource(bool, QString);
  bool removeResource(QString);
   //General purpose functions
  QString path(){ return modulePath; } //get the current module path
  QString readValue(QString); //get a variable from the module
  bool writeValue(QString,QString); //set a variable in the module
  QStringList filesAvailable(QString); //get the available files per category
  void compressModule();  //package the module for distribution
  bool createModuleDir();  //Create/overwrite module directory structure
   //FreeNAS Plugins
  bool writeSampleFreenasControl();
  bool writeSampleFreenasTweakRC();
  
  // --Static functions--
  static bool createDir(QString);
  static bool createFile(QString,QStringList);
  static QStringList readFile(QString);
  

};

#endif

