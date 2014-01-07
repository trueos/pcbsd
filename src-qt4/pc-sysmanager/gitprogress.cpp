#include "gitprogress.h"


gitProc::gitProc(){
  //initialize the internal worker process
  process = new QProcess;
  process->setProcessChannelMode(QProcess::MergedChannels);
  connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(parseUpdate()) );
  connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(procDone()) );
}

gitProc::~gitProc(){
  //Clean up the internal worker process
  delete process;
}
	
// ========================
//     PUBLIC WRAPPER FUNCTIONS
// ========================
bool gitProc::startInitPorts(){
  running = true;
  if( !quickCMD("", "rm -rf /usr/ports/*") ){ running = false; return false;}
  if( !quickCMD("/usr/ports", "git init") ){ running = false; }
  else{
    //start the long process
    process->setWorkingDirectory("/usr/ports/.git");
    process->start("git remote add origin https://www.github.com/pcbsd/freebsd-ports.git");
  }
  return running;
}

bool gitProc::startPorts(){
  running = true;
  if( !quickCMD("", "/usr/ports/.git" "git fetch --depth=1") ){ running = false; return false;}
  else{
    //start the long process
    process->setWorkingDirectory("/usr/ports");
    process->start("git checkout -t origin/master");
  }
  return running;
}

bool gitProc::startInitSource(){
  running = true;
  if( !quickCMD("", "rm -rf /usr/src/*") ){ running = false; return false;}
  if( !quickCMD("/usr/src", "git init") ){ running = false; }
  else{
    //start the long process
    process->setWorkingDirectory("/usr/src/.git");
    process->start("git remote add origin https://www.github.com/pcbsd/freebsd.git");
  }
  return running;
}

bool gitProc::startSource(){
  running = true; 
  if( !quickCMD("", "/usr/src/.git" "git fetch --depth=1") ){ running = false; return false;}
  else{
    //start the long process
    process->setWorkingDirectory("/usr/ports");
    process->start("git checkout -t origin/master");
  }
  return running;
}

// ====================
//   STATIC PUBLIC FUNCTIONS
// ====================
bool gitProc::quickCMD(QString dir, QString cmd, QStringList args){
  //Run a quick command without looking for output other than success/failure
  QProcess *proc = new QProcess;
  if( !dir.isEmpty() && QFile::exists(dir) ){ proc->setWorkingDirectory(dir); }
  if(args.isEmpty()){ proc->start(cmd); }
  else{ proc->start(cmd, args); }
  while(!proc->waitForFinished(300)){ QCoreApplication::processEvents(); }
  return (proc->exitCode() == 0);
}

// ==============
//     PRIVATE SLOTS
// ==============
void gitProc::parseUpdate(){
  while(process->canReadLine()){
    QString output = process->readLine().simplified();
    //could add a check here to not send the message for empty lines
    emit ProcMessage(output);
  }
}

void gitProc::procDone(){
  running = false;
  emit ProcFinished(process->exitCode() == 0);
}
