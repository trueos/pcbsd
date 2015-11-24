#include "SysStatus.h"

#include <QObject>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <pcbsd-utils.h>


SysStatus::SysStatus(){ 
  //Initialize all flags to false
  updating = complete = pkg = sys = sec = jail = error = false; 
}
SysStatus::~SysStatus(){}
	
void SysStatus::checkSystem(bool checkjails){
  complete = QFile::exists("/tmp/.rebootRequired");
  error = false;
  if(!complete){ 
    //Get all the possible flag files and only take the most recent (latest flag - they overwrite each other)
    QStringList upinfo = pcbsd::Utils::runShellCommand("syscache needsreboot isupdating");
    if(upinfo.length() < 2 || upinfo.join("").contains("[ERROR]") ){
      //Is syscache not running?
      updating = (0==QProcess::execute("pgrep -F /tmp/.updateInProgress"));
      if(!updating){
        //Can't restart syscache here - running with user permissions but needs root
	// Prompt that an error has occured instead
	error = true;
      }
      
    }else{
      //Use the syscache info
      complete = (upinfo[0]=="true");
      updating = (upinfo[1]=="true");
      //Check for syscache return errors
      qDebug() << "SYSCACHE Update Info:" << upinfo;
      if(!complete && upinfo[0]!="false"){ error=true; }
      if(!updating && upinfo[1]!="false"){ error=true; }
    }
    if(!updating && !complete){
      //Run syscache to probe for updates that are available
      QString cmd = "syscache hasmajorupdates hassecurityupdates haspcbsdupdates \"pkg #system hasupdates\" \"jail cages\"";
      QStringList info = pcbsd::Utils::runShellCommand(cmd);
      qDebug() << "SYSCACHE Info:" << info;
      if(info.length() < 5){error = true; return; } //no info from syscache
      else if(info.join(" ").contains("[ERROR]") ){ 
        for(int i=0; i<info.length()-1; i++){ //skip the last "jails" update - might not have jails on this system
	  if(info[i].contains("[ERROR]")){ error = true; return; }
	}
      }
      sys = (info[0] == "true");
      sec = (info[1] == "true") || (info[2] == "true"); //combine security updates with pcbsd patches for notifications
      pkg = (info[3] == "true");
      //Now look for jail updates
      if(checkjails && !info[4].simplified().isEmpty() && !info[4].contains("[ERROR]") ){
	QStringList jls = info[4].split(", ");
	  //Note: jls format: [<origin> <ID>]
	cmd = "syscache";
	for(int i=0; i<jls.length(); i++){
	  cmd.append(" \"pkg "+jls[i].section(" ",1,1)+" hasupdates\"");
	}
	QStringList jinfo = pcbsd::Utils::runShellCommand(cmd);
	jail = jinfo.contains("true");
      }
    }
  }
}

bool SysStatus::InTorMode(){
  return ("TRUE" == pcbsd::Utils::runShellCommand("pc-sysconfig usingtormode").join("").simplified() );
}

bool SysStatus::changedFrom(SysStatus old){
  //See if the current status is different from an old status
  if( (old.error != error) && error){ return true; } //new error discovered (ignore if fixed)
  else if(old.complete  != complete){ return true; }
  else if(complete){ return false; } //both complete - no real difference
  else if(old.updating != updating){ return true; }
  else if(updating){ return false; } //both updating - no real difference
  else if(sys != old.sys){ return true; }
  else{
    return ((pkg || sec || jail) != (old.pkg || old.sec || old.jail)); //if update availability changed (don't care which type)
  }
}
	
QString SysStatus::tooltip(){
  //Generate the tooltip based on the current status
  if(error){ return QObject::tr("Unable to determine system status"); }
  else if(complete){ return QObject::tr("Reboot required to finish updates"); }
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
  if(error){ return QIcon(":images/connecterror.png"); }
  else if(complete){ return QIcon(":images/restart.png"); }
  else if(updating){ return QIcon(":images/updating.png"); }
  else if(sys || sec){ return QIcon(":images/sysupdates.png"); }
  else if(pkg){ return QIcon(":images/pkgupdates.png"); }
  else if(jail){ return QIcon(":images/warden.png"); }
  else{ return QIcon(":images/updated.png");}
}