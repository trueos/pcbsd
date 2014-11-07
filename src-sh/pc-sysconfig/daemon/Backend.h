#ifndef _PCBSD_SYSTEM_CONFIG_BACKEND_H
#define _PCBSD_SYSTEM_CONFIG_BACKEND_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QString>
#include <QStringList>

#define DELIM QString("::::")

class Backend : public QObject{
	Q_OBJECT
public:
	Backend(QObject *parent = 0) : QObject(parent){
	  //Fill the general/unchanging info
	  CPART = findActiveDevices(); //detect which partition/device is currently in use
	  updateIntMountPoints(); //update the internal list of mount points
	}
	~Backend(){
		
	}
	
	QString runRequest(QStringList req, QString user="", QString locale="en_US"){
	  CUSER = user;
	  CLOCALE = locale;
	  return "Not Implemented Yet";
	}
	
private:
	QStringList CPART; //Currently running partition/device nodes
	QString CUSER, CLOCALE; //Current user/local for the request

	//===========
	//Full backend functionality by system (details in Backend-<system>.cpp)
	//===========

	//REMOVABLE DEVICES (remdev)
	QStringList IntMountPoints; //Internal Mount points created by this utility (will be removed on cleanup)
	void updateIntMountPoints(); //Update the internal list

	QStringList findActiveDevices(); //list FreeBSD device nodes that are active
	QStringList listAllRemDev();
	QStringList getRemDevInfo(QString node, bool skiplabel = false);
	QStringList disktypeInfo(QString node);
	QStringList listMountedNodes();
	bool mountRemDev(QString node, QString mntdir, QString fs);
	bool unmountRemDev(QString nodedir); //can use node *or* mntdir

	//BLUETOOTH (bluetooth)

	//NETWORKING (network)

	//NETWORK SHARES (netshare)

	
};

#endif