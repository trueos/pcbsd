#include "SysStatus.h"

#include <QObject>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <pcbsd-utils.h>


SysStatus::SysStatus(){ 
  //Initialize all flags to false
  updating = complete = pkg = sys = sec = jail = false; 
}
SysStatus::~SysStatus(){}
	
void SysStatus::checkSystem(bool checkjails){
  complete = QFile::exists("/tmp/.rebootRequired");
  if(!complete){ 
    //Get all the possible flag files and only take the most recent (latest flag - they overwrite each other)
    QStringList upinfo = pcbsd::Utils::runShellCommand("syscache needsreboot isupdating");
    if(upinfo.length() < 2 || upinfo.join("").contains("[ERROR]") ){
      //Fallback method in case syscache is not working for some reason
      QDir procdir(UPDATE_PROC_DIR);
      QFileInfoList files = procdir.entryInfoList(QStringList() << UPDATE_PROC_FLAG_FILE_FILTER, QDir::Files, QDir::Time);
      QStringList tmp; for(int i=0; i<files.length(); i++){ tmp << files[i].absoluteFilePath(); }
      QString flag;
      if(!files.isEmpty()){ flag = pcbsd::Utils::readTextFile(files.first().absoluteFilePath()).simplified().toLower(); }
      //qDebug() << "No syscache running - use flags:" << tmp << flag;
      complete = (UPDATE_PROC_FINISHED == flag );
      updating = (UPDATE_PROC_WORKING == flag );
    }else{
      //Use the syscache info
      complete = (upinfo[0]=="true");
      updating = (upinfo[1]=="true");
    }
    if(!updating && !complete){
      //Run syscache to probe for updates that are available
      QString cmd = "syscache hasmajorupdates hassecurityupdates haspcbsdupdates \"pkg #system hasupdates\" \"jail list\"";
      QStringList info = pcbsd::Utils::runShellCommand(cmd);
      if(info.length() < 5){ return; } //no info from syscache
      sys = (info[0] == "true");
      sec = (info[1] == "true") || (info[2] == "true"); //combine security updates with pcbsd patches for notifications
      pkg = (info[3] == "true");
      //Now look for jail updates
      if(checkjails && !info[4].simplified().isEmpty() ){
	QStringList jls = info[4].split(", ");
	cmd = "syscache";
	for(int i=0; i<jls.length(); i++){
	  cmd.append(" \"pkg "+jls[i]+" hasupdates\"");
	}
	QStringList jinfo = pcbsd::Utils::runShellCommand(cmd);
	jail = jinfo.contains("true");
      }
    }
  }
}
	
bool SysStatus::changedFrom(SysStatus old){
  //See if the current status is different from an old status
  if(old.complete  != complete){ return true; }
  else if(complete){ return false; } //both complete - no real difference
  else if(old.updating != updating){ return true; }
  else if(updating){ return false; } //both updating - no real difference
  else if(sys != old.sys){ return true; }
  else{
    return ((pkg || sec || jail) == (old.pkg || old.sec || old.jail)); //if update availability changed (don't care which type)
  }
}
	
QString SysStatus::tooltip(){
  //Generate the tooltip based on the current status
  if(complete){ return QObject::tr("Reboot required to finish updates"); }
  else if(updating){ return QObject::tr("Performing background updates"); }
  else if(sys){  return QString( QObject::tr("OS Update Available: %1") ).arg(sysupver); }
  else if(pkg || sec || jail){
    QStringList msg;
    if(sec){ msg << QObject::tr("System updates available"); }
    if(pkg){ msg << QObject::tr("Package updates available"); }
    if(jail){ msg << QObject::tr("Jail updates available"); }
    return msg.join("\n");
  }else{
    return QObject::tr("System up to date!");
  }
}
	
QIcon SysStatus::icon(){
  //Generate the icon based on the current status
  if(complete){ return QIcon(":images/restart.png"); }
  else if(updating){ return QIcon(":images/updating.png"); }
  else if(sys || sec){ return QIcon(":images/sysupdates.png"); }
  else if(pkg){ return QIcon(":images/pkgupdates.png"); }
  else if(jail){ return QIcon(":images/warden.png"); }
  else{ return QIcon(":images/updated.png");}
}