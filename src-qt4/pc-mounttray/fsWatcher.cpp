#include "fsWatcher.h"

FSWatcher::FSWatcher() : QObject(){
  //setup the timer
  timer = new QTimer();
  QObject::connect(timer, SIGNAL(timeout()), this, SLOT(checkFS()));
}

FSWatcher::~FSWatcher(){
}

void FSWatcher::start(int ms){ 
    timer->start(ms); 
    QTimer::singleShot(2000,this,SLOT(checkFS()) ); //make sure to perform a check when it starts up
}

void FSWatcher::stop(){
  timer->stop();
}

QStringList FSWatcher::getFSmountpoints(){
  //output line format: name::filesystem::totalspace::usedspace::percent
  // -- sizes all in K
  QStringList output; 
  //ZFS Checks
  QStringList zpools = runCMD("zpool list -o name");
  if(zpools.length() > 1){ 
    //Then there are ZFS pools available
    for(int i=1; i<zpools.length(); i++){ //skip the header line
      QStringList tmp = runCMD("zfs list -o available,used "+zpools[i]);
      //second line contains the data
      QString avail = tmp[1].section(" ",0,0,QString::SectionSkipEmpty);
      QString used = tmp[1].section(" ",1,1,QString::SectionSkipEmpty);
      int iUsed = displayToInt(used);
      int iTotal = displayToInt(avail) + iUsed;
      int percent = calculatePercentage(iUsed, iTotal);
      //qDebug() << "Percent calc: tot:"<<iTotal<<"used"<<iUsed<<"percent"<<percent;
      //format the output string and add it in
      output << zpools[i]+"::zfs::"+QString::number(iTotal)+"::"+QString::number(iUsed)+"::"+QString::number(percent);
    }
  }
  //Now get all the rest of the mounted filesystems
  QStringList dfout = runCMD("df -h -T");
  //Format: name, filesystem, size, used, available, percent, mountpoint 
  for(int i=1; i<dfout.length(); i++){
    //ignore certain filesystems
    if(dfout[i].startsWith("devfs")){}
    else if(dfout[i].startsWith("procfs")){}
    else if(dfout[i].startsWith("linprocfs")){}
    else{
      //Now parse out the info  
      dfout[i].replace("\t"," ");
      QString fs = dfout[i].section("  ",1,1,QString::SectionSkipEmpty).simplified();
      if(fs != "zfs"){  //ignore zfs filesystems (already taken care of)
        QString name = dfout[i].section("  ",6,6,QString::SectionSkipEmpty).simplified();
        QString total = dfout[i].section("  ",2,2,QString::SectionSkipEmpty).simplified();
        QString used = dfout[i].section("  ",3,3,QString::SectionSkipEmpty).simplified();
        //Calculate the percent
        int iUsed = displayToInt(used);
        int iTotal = displayToInt(total);
        int percent = calculatePercentage(iUsed, iTotal);
        //qDebug() << "df Item:" << dfout[i];
        //qDebug() << " - Detected:" << name << fs << iTotal << iUsed << percent;
        //format the output string and add it in
        output << name+"::"+fs+"::"+QString::number(iTotal)+"::"+QString::number(iUsed)+"::"+QString::number(percent);
      }
    }
  }
  //Return the results
  //qDebug() << "FS output:" << output;
  return output;
  
}

int FSWatcher::displayToInt(QString entry){
  //split the number from the size label
  //qDebug() << "Display to Int conversion:" << entry;
  QString units = entry.right(1); //last character
  entry.chop(1); //remove the unit
  double num = entry.toDouble();
  //qDebug() << "initial number:" << num << "units:" << units;
  if(units=="K"){} //Kilobytes (no change)
  else if(units=="M"){ num=num*1024; } //Megabytes to K
  else if(units=="G"){ num=num*1048576; } //Gigabytes to K
  else{ num=0; } //smaller than a KB
  //qDebug() << "number:" << num;
  return num;
}

QString FSWatcher::intToDisplay(int K){
  QString num;
  //qDebug() << "Int to Display:" << K;
  double kdb = K; //using pure integers causes errors with large numbers
  if( K > 1048576 ){ num = QString::number( int((kdb*100)/1048576)/100.0 ) +"G"; }
  else if(K > 1024){ num = QString::number( int((kdb*100)/1024)/100.0 ) +"M"; }
  else{ num = QString::number(K) +"K"; }
  //qDebug() << "Display:" << num;
  return num;
    	  
}

//====== Private Slot =======
void FSWatcher::checkFS(){
  QStringList devList = getFSmountpoints();
  QStringList badDevs;
  for(int i=0; i<devList.length(); i++){
    int percent = devList[i].section("::",4,4).toInt();
    if(percent > 90){
      //Device greater than 90% full, warn the user
      badDevs << devList[i].section("::",0,0); //list the mountpoint
      qDebug() << "WARNING: Device almost full:" << devList[i].section("::",0,0)+": "+QString::number(percent)+"% full";
    }
  }
  if(!badDevs.isEmpty()){
    QString title = tr("Disk(s) Almost Full");
    QString message = badDevs.join(", ");
    emit FSWarning(title,message);
  }
}

int FSWatcher::calculatePercentage(int used, int total){
  double U = used;
  double T = total;
  double result = (U/T)*100.0;
  return result; //will remove decimel places;
}
//====== Run System Command Function ======
QStringList FSWatcher::runCMD(QString command){
   QProcess p;
   QString outstr;
   //Make sure we use the system environment to properly read system variables, etc.
   QProcessEnvironment penv = QProcessEnvironment::systemEnvironment();
   penv.insert("BLOCKSIZE","K"); //make sure we use a 1KB block size
   p.setProcessEnvironment(penv);
   //Merge the output channels to retrieve all output possible
   p.setProcessChannelMode(QProcess::MergedChannels);   
   p.start(command);
   while(p.state()==QProcess::Starting || p.state() == QProcess::Running){
     p.waitForFinished(200);
     QCoreApplication::processEvents();
   }
   QString tmp = p.readAllStandardOutput();
   outstr.append(tmp);
   if(outstr.endsWith("\n")){outstr.chop(1);} //remove the newline at the end 
   QStringList out = outstr.split("\n");
   return out;
}
