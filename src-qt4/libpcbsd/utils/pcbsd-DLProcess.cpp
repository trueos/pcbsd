#include "pcbsd-DLProcess.h"

DLProcess::DLProcess(QObject* parent) : QProcess(parent){
  //Setup the process environment for downloads
  this->setProcessChannelMode(QProcess::MergedChannels);
  //Setup the internal signals/slots
  connect(this, SIGNAL(readyRead()), this, SLOT(newMessage()) );
  connect(this, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(ProcFinished()) );
  //Flag as no output parsing at the moment
  DLTYPE = -1;
  pipeFile.clear();
}

DLProcess::~DLProcess(){
	
}

void DLProcess::setWardenDir(QString wardendir){
  pipeFile = wardendir+"/tmp/pkg-fifo";
  if(QFile::exists(pipeFile)){
    //That pipe already exists: use a different one to prevent conflicts
    int num =2;
    while(QFile::exists(pipeFile+"-"+QString::number(num)) ){ num ++; }
    pipeFile.append("-"+QString::number(num));
  }
}

void DLProcess::setDLType(QString type){
  type = type.toLower();
  if(type=="pbi"){ 
    DLTYPE = 0;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
      env.insert("PBI_FETCH_PARSING","YES"); //For readable download notifications
    this->setProcessEnvironment(env);
	  
  }else if(type=="pkg"){ 
    DLTYPE = 1; 
    //For this type, need to run an additional process to watch an event-pipe
    if(pipeFile.isEmpty()){ setWardenDir(""); } //generate the pipe file on base system
    //Setup the pipe file on the system
    system("mkfifo "+pipeFile.toUtf8()+" ; sleep 1");
    watcher = new QProcess(this);
	  watcher->start("cat", QStringList() << "-u" << pipeFile );
          connect(watcher, SIGNAL(readyRead()), this, SLOT(newMessage()) );
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
      env.insert("PCFETCHGUI","YES"); //For readable download notifications
      env.insert("EVENT_PIPE", pipeFile);
    this->setProcessEnvironment(env);
    //Disconnect the main process from the parser
    disconnect(SIGNAL(readyRead()), this, SLOT(newMessage()) );
    
  }else if(type=="cdn"){
    DLTYPE = 2;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
      env.insert("GUI_FETCH_PARSING","YES"); //For readable download notifications
    this->setProcessEnvironment(env);
	  
  }else{
    DLTYPE = -1;
    this->setProcessEnvironment( QProcessEnvironment::systemEnvironment() );
  }
}

bool DLProcess::isRunning(){
  return (this->state() != QProcess::NotRunning);
}

// ===============
//  PRIVATE FUNCTIONS
// ===============
void DLProcess::calculateStats(QString current, QString total, QString speed, QString filename){
  //Assumes all values are in KB or KB/s
  bool totok, curok, spdok;
  double tot, cur, spd, percent;
  tot = total.toDouble(&totok);
  cur = current.toDouble(&curok);

  //qDebug() << " - DownloadStats:" << tot << cur << spd;
  if(totok && tot==0){totok=false;}
  if(curok && cur==0){curok=false;}
  //Now format the output string
  //Get percentage complete
  if(totok && curok){
    bool totErr = (tot==cur); //catch for a display error where the cur is always identical to the tot
    if(!totErr){	  
      //Calculate the percentage
      percent = (cur/tot)*100;
      percent = int(percent*10)/10.0;
    }else{
      //(Total = Current) bug: unknown percentage since process is still running
      percent = -1;
    }	    
  }else{
    percent = -1;
  }	
  //Check if total is known  
  if(!totok && curok){
    //Only Current is known
    tot = cur; //output current instead of total since no percent either
  }
  //OTHER input (speed, filename)
  QString other = "??";
  //Check the speed
  if(!speed.isEmpty()){
    spd = speed.toDouble(&spdok);
    if(spdok && spd<=0){ spdok=false; }
    if(spdok){
      other = kbToString(spd)+"/s";
    }
  //Check for a filename instead
  }else if(!filename.isEmpty() ){
    other = filename;
  }
  //Now emit the stats
  QString perc = "??";
  if(percent >= 0){ perc = QString::number(percent); }
  QString tota = "??";
  if(tot >= 0){ tota = kbToString(tot); }
  
  emit UpdatePercent(perc, tota, other);
}

void DLProcess::parsePBILine(QString line){
  //Quick checks for line formatting
  if( line.startsWith("FETCH:") ){ emit UpdateMessage(tr("Download Starting...")); return; }
  else if( line == "FETCHDONE"){ emit UpdateMessage(tr("Download Finished")); return; }
  else if(!line.startsWith("SIZE:")){ emit UpdateMessage(line); return; }
  //Now parse the PBI download line
  //qDebug() << "parse Download Line:" << line;
  //Line format: SIZE:  <KB> DOWNLOADED:  <KB> SPEED:  <KB/s> KB/s
  line = line.simplified();
  line.replace("SIZE: ","");
  line.replace("DOWNLOADED: ", "");
  line.replace("SPEED: ","");
  line.replace("KB/s","");
  //Now run the calculations and emit the signal
  calculateStats(line.section(" ",1,1), line.section(" ",0,0), line.section(" ",2,2), "");
}

void DLProcess::parsePKGLine(QString line){
     // KPM!!
     // TODO 12-12-2013
     // No JSON in Qt4, once we move to Qt5, replace this hack
     // with the new JSON parser
     // Moved 2/18/14 to this class from pc-pkgmanager by Ken Moore

     // Look for any "msg" lines
     if ( line.indexOf("\"msg") != -1 ) {
          line.remove(0, line.indexOf("\"msg") + 8);
          line.truncate(line.lastIndexOf("\""));
	  emit UpdateMessage(line);
	  return;
     }

     // Look for a download status update
     if ( line.indexOf("\"INFO_FETCH") != -1 && line.indexOf("\"url\"") != -1 ) {
       QString file, dl, tot;
          line.remove(0, line.indexOf("\"url") + 8);
          line.truncate(line.lastIndexOf("}"));

          // Get the file basename
          file = line;
          file.truncate(line.indexOf("\""));
	  file = file.section("/",-1).section(".",0,0); //replace the QFileInfo method below (Ken)
          //QFileInfo tFile;
          //tFile.setFile(file);
          //file = tFile.baseName();

          // Get the download / total
          dl = line.section(":", 2, 2).section(",", 0, 0);
          tot = line.section(":", 3, 3).section("}", 0, 0);
          dl = dl.simplified();
          tot = tot.simplified();
	     
	  //These are in bytes, need to convert to kilobytes before sending it on...
	  dl = QString::number( dl.toLongLong()/1024 );
	  tot = QString::number( tot.toLongLong()/1024 );
	     
        //Now calculate the stats and emit the signal
	calculateStats(dl, tot, "", file);
     }	
}

QString DLProcess::kbToString(double kb){
  double num = kb;
   QStringList lab; lab << "KB" << "MB" << "GB" << "TB" << "PB";
   int i=0;
   while( (i<lab.length()) && (num > 1024) ){
     num=num/1024; i++;	   
   }
   //Round to 2 decimel places
   num = int(num*100)/100.0;
   QString output = QString::number(num)+" "+lab[i];
   //qDebug() << "Size calculation:" << sizeK << output;
   return output;
}

// ================
//      PRIVATE SLOTS
// ================
void DLProcess::ProcFinished(){
  if(DLTYPE == 1){
    //PKG format - also kill the watcher process
    watcher->kill();
    QFile::remove(pipeFile); //remove the pipe file too
  }
}

void DLProcess::newMessage(){
  if(DLTYPE == 1){
    //PKG type pulls info from a different QProcess
    while(watcher->canReadLine()){
       QString line = watcher->readLine().simplified();
	if(line.isEmpty()){ continue; }
	parsePKGLine(line);
    }
    
  }else{	  
    //All other DL types pull from the main process
	  
    while(this->canReadLine()){
      QString line = this->readLine().simplified();
      if(line.isEmpty()){ continue; }
      if(DLTYPE == 0){ 
        // PBI Download format
        parsePBILine(line);
      }else if(DLTYPE == 2){
        // CDN Download format
        parsePBILine(line); //same parser as PBI's at the moment
      }else{
        //Don't have parsing rules for this type: just spit it out
        emit UpdateMessage(line);
      }
    }//End of loop over main process lines
    
  } //end check of PKG type
}
