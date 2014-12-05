#include "pcbsd-DLProcess.h"

DLProcess::DLProcess(QObject* parent) : QProcess(parent){
  //Setup the process environment for downloads
  this->setProcessChannelMode(QProcess::MergedChannels);
  //Setup the internal signals/slots
  connect(this, SIGNAL(readyRead()), this, SLOT(newProcessMessage()) );
  connect(this, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(ProcFinished()) );
  //Flag as no output parsing at the moment
  DLTYPE = -1;
  parentW = 0; //no parent widget for now
  pipeFile.clear();
}

DLProcess::~DLProcess(){
	
}

void DLProcess::setParentWidget(QWidget *par){
  parentW = par;	
}

void DLProcess::setWardenDir(QString wardendir){
  wDir = wardendir;
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
          connect(watcher, SIGNAL(readyRead()), this, SLOT(newPipeMessage()) );
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
      env.insert("PCFETCHGUI","YES"); //For readable download notifications
      env.insert("EVENT_PIPE", pipeFile);
    this->setProcessEnvironment(env);    
    
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
      //Calculate the percentage
      percent = (cur/tot)*100;
      percent = int(percent*10)/10.0;
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
     //qDebug() << " -- pkg line:" << line;
     // Look for a download status update
     static QString running = ""; //filename being downloaded
     if ( line.indexOf("\"INFO_FETCH") != -1 && line.indexOf("\"url\"") != -1 ) {
       QString file, dl, tot;
          line.remove(0, line.indexOf("\"url") + 8);
          line.truncate(line.lastIndexOf("}"));

          // Get the file basename
          file = line;
	  //qDebug() << "DL File:" << file;
          file.truncate(line.indexOf("\""));
	  file = file.section("/",-1).section(".txz",0,0); //replace the QFileInfo method below (Ken)
	  running = file;
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
     else if ( line.indexOf("PKGCONFLICTS: ") == 0 ) {
	QString tmp = line; 
     	tmp.replace("PKGCONFLICTS: ", "");
        ConflictList = tmp;
     }
     else if ( line.indexOf("PKGREPLY: ") == 0 ) {
	QString ans;
	QString tmp = line; 
     	tmp.replace("PKGREPLY: ", "");
        QMessageBox msgBox(parentW);
 	msgBox.setText(tr("The following packages are causing conflicts with the selected changes and can be automatically removed. Continue?") + "\n" + ConflictList);
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
   	msgBox.setDetailedText(getConflictDetailText());
        msgBox.setDefaultButton(QMessageBox::No);
        if ( msgBox.exec() == QMessageBox::Yes) {
	  // We will try to fix conflicts
	  ans="yes";
        } else {
	  // We will fail :(
          QMessageBox::warning(parentW, tr("Package Conflicts"),
          tr("You may need to manually fix the conflicts before trying again."),
          QMessageBox::Ok,
          QMessageBox::Ok);
	  ans="no";
        }

        QFile pkgTrig( tmp );
        if ( pkgTrig.open( QIODevice::WriteOnly ) ) {
           QTextStream streamTrig( &pkgTrig );
           streamTrig << ans;
	   pkgTrig.close();
	   ConflictList.clear(); //already sent an answer - clear the internal list
	}
     }else if(line.startsWith("{ \"type\":")){
	//Determine if this is the start or tick
	if(line.contains("\"INFO_PROGRESS_START\"")){
	  //starting download not output via pkg yet (update "running" variable)
	  emit UpdatePercent("0.0","??",running); //go back to start
	}else if(line.contains("\"INFO_PROGRESS_TICK\"")){
	  //get the current/total download progress
	  QString cur = line.section("current\":",1,1).section(",",0,0).simplified();
	  QString tot = line.section("total\" :",1,1).section("}",0,0).simplified();
	  //These are in bytes, need to convert to kilobytes before sending it on...
	  cur = QString::number( cur.toLongLong()/1024 );
	  tot = QString::number( tot.toLongLong()/1024 );
	  //Now perform the calculations
	  //Now calculate the stats and emit the signal
	  calculateStats(cur, tot, "", running);
	}
	     
     }else{
     	  //Just emit the message
          //line.remove(0, line.indexOf("\"msg") + 8);
          //line.truncate(line.lastIndexOf("\""));
	  emit UpdateMessage(line);
	  return;
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

QString DLProcess::getConflictDetailText() {

  QStringList ConList = ConflictList.split(" ");
  QStringList tmpDeps;
  QString retText;

  for (int i = 0; i < ConList.size(); ++i) {
    QProcess p;
    tmpDeps.clear();

    if ( wDir.isEmpty() )
      p.start("pkg", QStringList() << "rquery" << "%rn-%rv" << ConList.at(i));
    else
      p.start("chroot", QStringList() << wDir << "pkg" "rquery" << "%rn-%rv" << ConList.at(i) );

    if(p.waitForFinished()) {
      while (p.canReadLine()) {
        tmpDeps << p.readLine().simplified();
      }
    }
    retText+= ConList.at(i) + " " + tr("required by:") + "\n" + tmpDeps.join(" ");
  }

  return retText;
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

void DLProcess::newPipeMessage()
{
    //PKG type pulls info from a different QProcess
    while(watcher->canReadLine())
    {
        QString line = watcher->readLine().simplified();
        if(line.isEmpty()){ continue; }
        parsePKGLine(line);
    }
}

void DLProcess::newProcessMessage()
{
    while(this->canReadLine()){
      QString line = this->readLine().simplified();
      if(line.isEmpty()){ continue; }
      if(DLTYPE == 0){ 
        // PBI Download format
        parsePBILine(line);
      }else if(DLTYPE == 1){
      	//PKG Download format
      	//qDebug() << "pkg message: "<< line;
      	parsePKGLine(line);
      }else if(DLTYPE == 2){
        // CDN Download format
        parsePBILine(line); //same parser as PBI's at the moment
      }else{
        //Don't have parsing rules for this type: just spit it out
        emit UpdateMessage(line);
      }
    }//End of loop over main process lines
}
