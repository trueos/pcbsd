#include "fsWatcher.h"

FSWatcher::FSWatcher() : QObject(){
  //setup the timer
  timer = new QTimer();
  timer->setSingleShot(true);
  QObject::connect(timer, SIGNAL(timeout()), this, SLOT(checkFS()));
}

FSWatcher::~FSWatcher(){
}

void FSWatcher::start(int ms){ 
    timer->stop();
    timer->setInterval(ms); //max time between system checks
    timer->start(); 
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
      tmp[1] = tmp[1].replace("\t"," "); //ensure no tab characters in the output
      QString avail = tmp[1].section(" ",0,0,QString::SectionSkipEmpty);
      QString used = tmp[1].section(" ",1,1,QString::SectionSkipEmpty);
      double iUsed = floor(displayToDouble(used));
      double iTotal = floor(displayToDouble(avail)) + iUsed;
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
    else if(dfout[i].startsWith("linsysfs")){}
    else if(dfout[i].startsWith("fdescfs")){}
    else if(dfout[i].contains("/boot/efi")){}
    else{
      //Now parse out the info  
      dfout[i].replace("\t"," ");
      QString fs = dfout[i].section("  ",1,1,QString::SectionSkipEmpty).simplified();
      if(fs != "zfs" && fs!="cd9660" && fs!="nullfs" && fs!="fusefs" && fs!="autofs"){  //ignore zfs filesystems (already taken care of)
        QString name = dfout[i].section("  ",6,6,QString::SectionSkipEmpty).simplified();
        QString total = dfout[i].section("  ",2,2,QString::SectionSkipEmpty).simplified();
        QString used = dfout[i].section("  ",3,3,QString::SectionSkipEmpty).simplified();
        //Calculate the percent
        double iUsed = displayToDouble(used);
        double iTotal = displayToDouble(total);
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

int FSWatcher::displayToDouble(QString entry){
  //split the number from the size label
  //qDebug() << "Display to Int conversion:" << entry;
  QString units = entry.right(1); //last character
  entry.chop(1); //remove the unit
  entry = entry.replace(",","."); //replace any comma's with period's (Europe/US difference)
  double num = entry.toDouble();
  //qDebug() << "initial number:" << num << "units:" << units;
  QStringList unitL; unitL << "K" << "M" << "G" << "T" << "P" << "E" << "Z" << "Y";
  bool ok = false;
  for(int i=0; i< unitL.length(); i++){
    if(units == unitL[i]){ num = num*pow(1024.0,i); ok = true; break;}
  }
  if(!ok){num=0; }
  //qDebug() << "number:" << num;
  return num;
}

QString FSWatcher::doubleToDisplay(double K){
  QString num;
  //qDebug() << "Int to Display:" << K;
  double kdb = K; //using pure integers causes errors with large numbers
  QStringList units; units << "K" << "M" << "G" << "T" << "P" << "E" << "Z" << "Y";
  int i=0;
  while( (kdb > 1000) && (i < 8) ){
    kdb = kdb/1024;
    i++;
  }
  if(i<8){
    num = QString::number( int((kdb*100))/100.0) + units[i];
  }else{
    num = "??";
  }
  //qDebug() << "Display:" << num;
  return num;
    	  
}

//====== Public Slot =======
void FSWatcher::checkFS(){
  QStringList devList = getFSmountpoints();
  QStringList badDevs;
  for(int i=0; i<devList.length(); i++){
    int percent = devList[i].section("::",4,4).toInt();
    if(percent > 90){
      //Device greater than 90% full, warn the user
      badDevs << devList[i].section("::",0,0); //list the mountpoint
      qDebug() << "WARNING: Device almost full:" << devList[i].section("::",0,0)+": "+QString::number(percent)+"% full: Time: "+QTime::currentTime().toString();
    }
  }
  if(!badDevs.isEmpty()){
    //check to make sure these are new "bad" devices
    bool newFound = false;
    for(int i=0; i<badDevs.length(); i++){
      if( oldBadDevs.indexOf(badDevs[i]) == -1){ newFound = true; }	    
    }
    if(newFound){
      QString title = tr("Disk(s) Almost Full");
      QString message = badDevs.join(", ");
      emit FSWarning(title,message);
    }
  }
  //Save the current badDevs as the old list
  oldBadDevs = badDevs;
  //Reset the timer for the next time this function is to be called
  timer->start(); //reset the timer again
}

//===== Calculate Percentages =====
int FSWatcher::calculatePercentage(double used, double total){
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
