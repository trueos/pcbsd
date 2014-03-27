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
  if(QFile::exists(FLAGDIR)){ //just to silence all the warnings about missing directory
    watcher->addPath(FLAGDIR);
  }
  //Now manually run the detection routine
  watcherNotification();
  //Now check that the directory is being watched
  if(watcher->directories().isEmpty()){
    //flag dir does not exist yet, try again in 2 minutes
    QTimer::singleShot(120000, this, SLOT(watchFlagDir()) );
  }
}

void SystemFlagWatcher::watcherNotification(){
  QDir dir(FLAGDIR);
  if(!dir.exists()){ return; } //flag directory does not exist yet - do nothing
  QFileInfoList flags = dir.entryInfoList( QDir::Files | QDir::NoDotAndDotDot, QDir::Time);
  for(int i=0; i<flags.length(); i++){
     if(CDT < flags[i].lastModified()){
       QString contents = quickRead(flags[i].absoluteFilePath());
	SystemFlags::SYSMESSAGE msg;
	if(contents==MWORKING){ msg = SystemFlags::Working; }
	else if(contents==MERROR){ msg = SystemFlags::Error; }
	else if(contents==MSUCCESS){ msg = SystemFlags::Success; }
	else if(contents==MUPDATE){ msg = SystemFlags::UpdateAvailable; }
	else{ continue; } //invalid message - skip this flag
       //New flag - check which one and emit the proper signal
       if(flags[i].fileName().startsWith(NETWORKRESTARTED) ){
         emit FlagChanged(SystemFlags::NetRestart, msg);
       }else if(flags[i].fileName().startsWith(PKGUPDATEAVAILABLE) ){
	 emit FlagChanged(SystemFlags::PkgUpdate, msg);
       }else if(flags[i].fileName().startsWith(SYSUPDATEAVAILABLE) ){
	 emit FlagChanged(SystemFlags::SysUpdate, msg);
       }else if(flags[i].fileName().startsWith(PBIUPDATEAVAILABLE) ){
	 emit FlagChanged(SystemFlags::PbiUpdate, msg);
       }
     }
  }
  CDT = QDateTime::currentDateTime(); //Now update the last time flags were checked
}

QString SystemFlagWatcher::quickRead(QString filepath){
  //only need one line from the file
  if(filepath.isEmpty()){ return ""; }
  QFile file(filepath);
  if( !file.open(QIODevice::ReadOnly | QIODevice::Text) ){ return ""; }
  QStringList ret;
  QTextStream in(&file);
  while( !in.atEnd() ){
    ret << QString( in.readLine() );
  }
  file.close();
  return ret.join("\n");
}
