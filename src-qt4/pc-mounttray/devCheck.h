/*
 Class for getting device information
*/

#ifndef DEVICE_CHECK_H
#define DEVICE_CHECK_H

#include <QDir>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QTimer>

extern bool DEBUG_MODE;
extern QString DEVICEDIR;
extern QString MOUNTDIR;

class DevCheck
{

  public:
	DevCheck();
	~DevCheck();

	bool isValid(QString);
	QStringList devChildren(QString);
	QString devLabel(QString, QString);
	bool devInfo(QString, QString*, QString*, QString*, QString*);
	
	QStringList AvailableFS();
	QString getMountCommand(QString FS, QString dev, QString mntpoint);
  
  private:
  	QStringList validDevs, validDevTypes, devFilter;
  	QStringList fsDetection, fsMatch, fsFilter;
	QStringList fsManual, fsCMD, fsBinCheck;
  	QDir devDir;

};
#endif
