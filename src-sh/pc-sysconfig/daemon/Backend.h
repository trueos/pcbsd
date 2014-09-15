#ifndef _PCBSD_SYSTEM_CONFIG_BACKEND_H
#define _PCBSD_SYSTEM_CONFIG_BACKEND_H

#include <QObject>
#include <QDir>
#include <QFile>

class Backend : public QObject{
	Q_OBJECT
public:
	Backend(QObject *parent = 0) : QObject(parent){
	  //Fill the general/unchanging info
	  CPART = findActiveDevices(); //detect which partition/device is currently in use
	}
	~Backend(){
		
	}
	
	QString runRequest(QStringList req){
		
	  return "Not Implemented Yet";
	}
	
private:
	QStringList CPART; //Currently running partition/device nodes

	//Full backend functionality by system (details in Backend-<system>.cpp)

	//REMOVABLE DEVICES (remdev)
	QStringList IntMountPoints; //Mount points created by this utility (will be removed on cleanup)

	QStringList findActiveDevices(); //list FreeBSD device nodes that are active
	QStringList listAllRemDev();
	QString getRemDevInfo(QString node);
	QStringList disktypeInfo(QString node);
	QStringList listMountedNodes();

	//BLUETOOTH (bluetooth)

	//NETWORKING (network)

	//NETWORK SHARES (netshare)

	
};

#endif