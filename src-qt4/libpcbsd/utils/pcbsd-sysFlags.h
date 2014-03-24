#ifndef _PCBSD_SYSTEM_FLAGS_H
#define _PCBSD_SYSTEM_FLAGS_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QString>
#include <QDateTime>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QTimer>

#include <unistd.h>

#include "pcbsd-utils.h"

// DEFINE THE FLAG FILES
#define FLAGDIR QString("/tmp/.pcbsdflags")
#define NETWORKRESTARTED QString("network-restarted")
#define PKGUPDATEAVAILABLE QString("pkg-update-available")
#define SYSUPDATEAVAILABLE QString("sys-update-available")
#define PBIUPDATEAVAILABLE QString("pbi-update-available")

class SystemFlags{
public:
	enum SYSFLAG{ NetRestart, PkgUpdate, SysUpdate, PbiUpdate};
	
	static void setFlag( SYSFLAG flag, QString contents = ""){
	  if(!QFile::exists(FLAGDIR)){
	    pcbsd::Utils::runShellCommand("mkdir -p -m 777 "+FLAGDIR);
	    if( !QFile::exists(FLAGDIR) ){ return; }
	  }
	  QString cmd;
	  if(contents.isEmpty()){ cmd = "touch %1"; }
	  else{ cmd = "echo '%2' > %1"; cmd = cmd.arg("%1", contents); }
	  //Now get the flag type
	  cmd = cmd.arg(FLAGDIR+"/");
	  QString user = "-"+QString( getlogin() );
	  switch(flag){
	    case NetRestart:
		cmd = cmd.arg(NETWORKRESTARTED+user);
		break;
	    case PkgUpdate:
		cmd = cmd.arg(PKGUPDATEAVAILABLE+user);
		break;
	    case SysUpdate:
		cmd = cmd.arg(SYSUPDATEAVAILABLE+user);
		break;
	    case PbiUpdate:
		cmd = cmd.arg(PBIUPDATEAVAILABLE+user);
		break;
	    default:
		return;
	  }
	  pcbsd::Utils::runShellCommand(cmd);
	}

};

class SystemFlagWatcher : public QObject{
	Q_OBJECT
public:
	SystemFlagWatcher();
	~SystemFlagWatcher();

	void checkForRecent(int minutes);

private:
	QDateTime CDT;  //current date time
	QFileSystemWatcher *watcher;

private slots:
	void watchFlagDir();
	void watcherNotification();
	QString quickRead(QString filepath);

signals:
	void FlagChanged(SystemFlags::SYSFLAG, QString);
};

#endif