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
  	QStringList fsDetection, dsDetection, fsMatch, fsFilter;
	QStringList fsManual, fsCMD, fsBinCheck;
	QStringList activeDevs;
  	QDir devDir;

	//Function to scan the system and save which devices are currently active for skipping
	void findActiveDevices();
	//Functions to run disk detection methods and return the info/validity
	bool getDiskInfo(QString fulldev, QString *filesystem, QString *label);
	bool getSpecialFileInfo(QString fulldev, QString*filesystem, QString *label);
};
#endif
