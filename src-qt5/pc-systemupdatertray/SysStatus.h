#ifndef _PCBSD_SYSTEM_STATUS_H
#define _PCBSD_SYSTEM_STATUS_H

#include <QString>
#include <QIcon>

#define UPDATE_LOG_FILE QString("/var/log/pc-updatemanager.log")
#define UPDATE_PROC_DIR QString("/tmp/.pcbsdflags")
#define UPDATE_PROC_FLAG_FILE_FILTER QString("pkg-update-available-*")
#define UPDATE_PROC_WORKING QString("updating")
#define UPDATE_PROC_FINISHED QString("success")

//Simplification class for managing all the different system status's
class SysStatus{
public:
	//All the flags which can be used
	bool updating, complete, pkg, sys, sec, jail;
	QString sysupver; //Available OS update version

	SysStatus();
	~SysStatus();
		
	//Set all the flag appropriately
	void checkSystem(bool checkjails = false);
	//Determine if running in TOR mode
	bool InTorMode();
	//See if there has been any difference from an older status 
	bool changedFrom(SysStatus old);
	//Get the tooltip string for the current status
	QString tooltip();
	//Get the icon for the current status
	QIcon icon();
};
#endif
