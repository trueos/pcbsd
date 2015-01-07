#ifndef _PCBSD_SYSTEM_CONFIG_BACKEND_H
#define _PCBSD_SYSTEM_CONFIG_BACKEND_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QCoreApplication>

#define DELIM QString("::::")

#include "DevDB.h"

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
	  QStringList outputs;
	  if(req.length()==1){
	    if(req[0] == "listremdev"){ outputs = listAllRemDev(); }
	    else if(req[0] == "listmounteddev"){ outputs = listMountedNodes(); }
	    else if(req[0] == "supportedfilesystems"){ outputs = DEVDB::knownFilesystems(); }
	  }else if(req.length() ==2){
	    if(req[0] == "devinfo"){ outputs = getRemDevInfo(req[1]); }
	  
	  }else if(req.length() == 3){
	    if(req[0] == "devinfo"){ outputs = getRemDevInfo(req[1], req[2].toLower()=="skiplabel"); }
	  }
	  
	  if(outputs.isEmpty()){ return "[NO INFO]"; }
	  else{ return outputs.join(", "); }
	}
	
private:
	QStringList CPART; //Currently running partition/device nodes
	QString CUSER, CLOCALE; //Current user/local for the request

	//===========
	//Full backend functionality by system (details in Backend-<system>.cpp)
	//===========
	QStringList runShellCommand(QString cmd){
	  QProcess p;  
	  //Make sure we use the system environment to properly read system variables, etc.
	  p.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
	  //Merge the output channels to retrieve all output possible
	  p.setProcessChannelMode(QProcess::MergedChannels);   
	  p.start(cmd);
	  while(p.state()==QProcess::Starting || p.state() == QProcess::Running){
	    p.waitForFinished(200);
	    QCoreApplication::processEvents();
	  }
	  QString tmp = p.readAllStandardOutput();
	  if(tmp.endsWith("\n")){tmp.chop(1);} //remove the newline at the end 
	  return tmp.split("\n");
	}

	//REMOVABLE DEVICES (remdev)
	QStringList IntMountPoints; //Internal Mount points created by this utility (will be removed on cleanup)
	void updateIntMountPoints(); //Update the internal list

	QStringList devChildren(QString dev);
	QStringList findActiveDevices(); //list FreeBSD device nodes that are active
	QStringList listAllRemDev();
	QStringList getRemDevInfo(QString node, bool skiplabel = false);
	QStringList disktypeInfo(QString node); //use "disktype" for probing device
	bool specialFileInfo(QString fulldev, QString *filesystem, QString *label); //use "file -s" for probing device
	QStringList listMountedNodes();
	
	QString mountRemDev(QString node, QString mntdir, QString fs, QString username, QString locale);
	bool unmountRemDev(QString nodedir); //can use node *or* mntdir

	//BLUETOOTH (bluetooth)

	//NETWORKING (network)

	//NETWORK SHARES (netshare)

	
};

#endif