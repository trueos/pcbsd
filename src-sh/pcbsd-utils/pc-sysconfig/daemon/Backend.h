#ifndef _PCBSD_SYSTEM_CONFIG_BACKEND_H
#define _PCBSD_SYSTEM_CONFIG_BACKEND_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QCoreApplication>
#include <QTextStream>
#include <QDebug>

#define DELIM QString("::::")

#include "DevDB.h"

class Backend : public QObject{
	Q_OBJECT
public:
	Backend(QObject *parent = 0) : QObject(parent){
	  //Fill the general/unchanging info
	  CPART = findActiveDevices(); //detect which partition/device is currently in use
	  updateIntMountPoints(); //update the internal list of mount points
	  cleanMediaDir();
	}
	~Backend(){
		
	}
	
	QString runRequest(QStringList req, QString user="", QString locale="en_US"){
	  CUSER = user;
	  CLOCALE = locale;
	  QStringList outputs;
	  //qDebug() << "Run Request:" << req;
	  if(req.length()==1){
	    if(req[0] == "list-remdev"){ outputs = listAllRemDev(); }
	    else if(req[0] == "list-mounteddev"){ outputs = listMountedNodes(); }
	    else if(req[0] == "list-audiodev"){ outputs = ListAudioDevices(); }
	    else if(req[0] == "probe-netdrives"){ outputs = listNetworkDrives(); }
	    else if(req[0] == "list-mountednetdrives"){ outputs = listMountedNetDrives(); }
	    else if(req[0] == "supportedfilesystems"){ outputs = getUsableFileSystems(); }
	    else if(req[0] == "usingtormode"){ outputs << getTorModeStatus(); }
	    else if(req[0] == "getscreenbrightness"){ outputs << getScreenBrightness(); }
	    else if(req[0] == "systemcansuspend"){ outputs << checkSuspendAbility(); }
	    else if(req[0] == "suspendsystem"){ suspendSystem(); outputs << "Suspending system..."; }
	  }else if(req.length() ==2){
	    if(req[0] == "devinfo"){ outputs = getRemDevInfo(req[1]); }
	    else if(req[0] == "devsize"){ outputs << getDeviceSizeInfo(req[1]); }
	    else if(req[0] == "mount"){ outputs << mountRemDev(req[1],"",""); } //fully-auto mounting of device "mount <dev>"
	    else if(req[0] == "unmount"){ outputs << unmountRemDev(req[1],false); } //"unmount <dev or dir>"
	    else if(req[0] == "unmountnet"){ outputs << autoUnmountNetDrive(req[1]); } //IP
	    else if(req[0] == "load-iso"){ outputs << createMemoryDiskFromISO(req[1]); }
	    else if(req[0] == "setdefaultaudiodevice"){ outputs << setDefaultAudioDevice(req[1]); }
	    else if(req[0] == "setscreenbrightness"){ outputs << setScreenBrightness(req[1]); }
	  }else if(req.length() == 3){
	    if(req[0] == "devinfo"){ outputs = getRemDevInfo(req[1], req[2].toLower()=="skiplabel"); }
	    else if(req[0] == "mount"){ outputs << mountRemDev(req[1],"",req[2]); } //"mount <dev> <fs>"
	    else if(req[0] == "unmount"){ outputs << unmountRemDev(req[1],req[2].toLower()=="force"); } //"unmount <dev or dir> force"
	    else if(req[0] == "mountnet"){ outputs << autoMountNetDrive(req[1],req[2]); } //IP, name
	  }else if(req.length() == 4){
	    if(req[0] == "mount"){ outputs << mountRemDev(req[1],req[3],req[2]); } //"mount <dev> <fs> <dir>"
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

	// RUN A SHELL COMMAND
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
	  p.close();
	  if(tmp.endsWith("\n")){tmp.chop(1);} //remove the newline at the end 
	  return tmp.split("\n");
	}
	// READ A TEXT FILE
	QStringList readFile(QString filepath){
	  QStringList out;
	  QFile file(filepath);
	  if(file.open(QIODevice::Text | QIODevice::ReadOnly)){
	    QTextStream in(&file);
	    while(!in.atEnd()){
	      out << in.readLine();
	    }
	    file.close();
	  }
	  return out;
	}
	// WRITE A TEXT FILE
	bool writeFile(QString filepath, QStringList contents, bool overwrite){
	  QFile file(filepath);
	  if(file.exists() && !overwrite){ return false; }
	  bool ok = false;
	  if( file.open(QIODevice::WriteOnly | QIODevice::Truncate) ){
	  QTextStream out(&file);
	  out << contents.join("\n");
	  file.close();
	  ok = true;
	  }
	  return ok;
	}
	
	//REMOVABLE DEVICES (remdev)
	QStringList IntMountPoints; //Internal Mount points created by this utility (will be removed on cleanup)
	void updateIntMountPoints(); //Update the internal list
	void cleanMediaDir(); //To be run on startup - clean up any leftover mountpoint (in case of crash, etc)
	
	QStringList devChildren(QString dev);
	QStringList findActiveDevices(); //list FreeBSD device nodes that are active
	QStringList listAllRemDev();
	QStringList getRemDevInfo(QString node, bool skiplabel = false);
	QStringList disktypeInfo(QString node); //use "disktype" for probing device
	bool VerifyDevice(QString fulldev, QString type); //returns "true" if device is valid (has something connected)
	QStringList listMountedNodes();
	QString generateGenericLabel(QString type);
	QString getDeviceSizeInfo(QString nodedir);
	QStringList getUsableFileSystems();
	QStringList getSwapDevices();
	QStringList getPersonaCryptDevices();
	QStringList getAvailableZFSPools(); //output item: <pool name>::::<device node> (can be multiple devices per pool)
	QStringList getCurrentZFSPools(); //list of pools by name
	QStringList getCurrentZFSDevices(QString pool = ""); //list of all used device nodes for ZFS pool(s)
	
	QString mountRemDev(QString node, QString mntdir, QString fs);
	QString unmountRemDev(QString nodedir, bool force = false, bool internal = false); //can use node *or* mntdir
	QString createMemoryDiskFromISO(QString isoFile);
	
	//BLUETOOTH (bluetooth)

	//NETWORKING (network)
	QStringList listNetworkDrives(); //output format: [<Name> (<IP>)]
	QStringList listMountedNetDrives(); //output format: [<Name> (<IP>) on <dir>]
	QString autoMountNetDrive(QString driveIP, QString name);
	QString autoUnmountNetDrive(QString driveIP);
	
	//NETWORK SHARES (netshare)

	//AUDIO (audio)
	QString setDefaultAudioDevice(QString pcmdevice); 
	QStringList ListAudioDevices();
	
	//SYSTEM CONFIG STATUS
	QString getTorModeStatus(){
	  if( 0 == QProcess::execute("enable-tor-mode -c") ){ return "TRUE"; }
	  else{ return "FALSE"; }
	}
	
	//SYSTEM SUSPEND SUPPORT
	QString checkSuspendAbility(){
	  //Check the supported CPU sleep states and ensure that S3 is listed
	  QStringList info = runShellCommand("sysctl hw.acpi.supported_sleep_state");
	  bool ok = false;
	  if(!info.isEmpty()){
    	    ok = info.first().section("=",1,1).split(" ").contains("S3");
	  }
	  return ok ? "TRUE": "FALSE";
	}
	void suspendSystem(){
	  QProcess::startDetached("acpiconf -s 3");
	}

	//SCREEN CONTROL (screen)
	QString getScreenBrightness(); //returns: 0-100 (%), or [ERROR] for any error
	QString setScreenBrightness(QString percent); //returns: [SUCCESS] or [ERROR]
	  //Input Note: "+" or "-" may be prepended on the percentage to cause a difference from the current setting (+5 will increase by 5)
};

#endif
