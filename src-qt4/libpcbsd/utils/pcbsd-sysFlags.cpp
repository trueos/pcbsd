#include "pcbsd-sysFlags.h"

SystemFlagWatcher::SystemFlagWatcher(){
  CDT = QDateTime::currentDateTime();
  watcher = new QFileSystemWatcher(this);
  connect(watcher, SIGNAL(directoryChanged(QString)), this, SLOT(watcherNotification()) );
  connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(watcherNotification()) );
  QTimer::singleShot(1, this, SLOT(watchFlagDir()) ); 
}

SystemFlagWatcher::~SystemFlagWatcher(){
	
}

//========
//   PUBLIC
//========
void SystemFlagWatcher::checkForRecent(int minutes){
  CDT = QDateTime::currentDateTime();
	CDT = CDT.addMSecs( -(minutes*60000) );
  watcherNotification(); //Now manually check for flags since then
}

//=========
//    PRIVATE
//=========
void SystemFlagWatcher::watchFlagDir(){
  watcher->addPath(FLAGDIR);
  //Now manually run the detection routine
  watcherNotification();
  //Now check that the directory is being watched
  if(watcher->directories().isEmpty()){
    //flag dir does not exist yet, try again in 2 minutes
    QTimer::singleShot(120000, this, SLOT(watchFlagDir()) );
  }
}

void SystemFlagWatcher::watcherNotification(){
  //Does not specify the contents of a flag just yet - KenMoore 3/12/14
  QDir dir(FLAGDIR);
  if(!dir.exists()){ return; } //flag directory does not exist yet - do nothing
  QFileInfoList flags = dir.entryInfoList( QDir::Files | QDir::NoDotAndDotDot, QDir::Time);
  for(int i=0; i<flags.length(); i++){
     if(CDT < flags[i].lastModified()){
       //New flag - check which one and emit the proper signal
       if(flags[i].fileName() == NETWORKRESTARTED){
         emit FlagChanged(SystemFlags::NetRestart, "");
       }else if(flags[i].fileName() == PKGUPDATEAVAILABLE){
	 emit FlagChanged(SystemFlags::PkgUpdate, "");
       }else if(flags[i].fileName() == SYSUPDATEAVAILABLE){
	 emit FlagChanged(SystemFlags::SysUpdate, "");
       }else if(flags[i].fileName() == PBIUPDATEAVAILABLE){
	 emit FlagChanged(SystemFlags::PbiUpdate, "");
       }
     }
  }
  CDT = QDateTime::currentDateTime(); //Now update the last time flags were checked
}