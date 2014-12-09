#include <pcbsd-utils.h>
#include "menuItem.h"


MenuItem::MenuItem(QWidget* parent, DevCheck *chk, QString newdevice, QString newlabel, QString newtype, QString newfs) : QWidgetAction(parent)
{
  AMFILE= QDir::homePath() + "/.pc-automounttray";   //File to save/load all the devices to be automounted
  //Set the device info variables
  if( !newdevice.startsWith(DEVICEDIR) ){ newdevice.prepend(DEVICEDIR); }
  device = newdevice;
  devType = newtype;
  filesystem = newfs;
  DEVCHECK = chk;
  //currentUser = user;
  mountedHere = false; //not mounted by this app (yet)
  rootRequired = false; //assume user device for the moment
  //Create the layout
  QGridLayout* layout = new QGridLayout();
  QHBoxLayout* hlayout = new QHBoxLayout();
  //Create the gui items
  devLabel = new QLabel;
    devLabel->setToolTip(device);
    devLabel->setText(newlabel);
  devIcon = new QLabel;
    devIcon->setToolTip(device);
  currentSpace = new QProgressBar;
    currentSpace->setMinimum(0);
  pushMount = new QPushButton;
  checkAutomount = new QCheckBox;
    checkAutomount->setChecked( checkSavedAutoMount() );
    checkAutomount->setText( tr("Auto-mount this device") );
  //Add the widgets to the layout
  layout->addWidget(devIcon,0,0,3,1);	
  hlayout->addWidget(devLabel);
  hlayout->addWidget(pushMount);
  layout->addLayout(hlayout,0,1);
  layout->addWidget(currentSpace,1,1);
  layout->addWidget(checkAutomount,2,1);
  //Set the layout
  frame = new QWidget();
  frame->setLayout(layout);
  this->setDefaultWidget(frame);

  //Connect the signals/slots
  connect(pushMount,SIGNAL(clicked()), this, SLOT(slotMountClicked()));
  connect(checkAutomount,SIGNAL(toggled(bool)),this,SLOT(slotAutoMountToggled(bool)));

  //Setup the device Icon based on the type
  if(devType == "USB"){ baseicon = QPixmap(":icons/usb.png"); }
  else if(devType == "SATA"){ baseicon = QPixmap(":icons/harddrive.png"); }
  else if(devType == "SD"){ baseicon = QPixmap(":icons/sdcard.png"); }
  else if(devType == "CD9660"){ baseicon = QPixmap(":icons/dvd.png"); }
  else if(devType == "ISO"){baseicon = QPixmap(":icons/dvd.png"); rootRequired = true;}
  else if(devType == "SCSI"){baseicon = QPixmap(":icons/harddrive.png"); }
  devIcon->setPixmap(baseicon);
  //Start the automount procedure if necessary
  if(checkAutomount->isChecked() || devType=="ISO" || filesystem=="AVDISK" ){
    QTimer::singleShot(500,this,SLOT( slotAutoMount() ));
  }
  //Update the Item based upon current device status
  updateItem();

}

MenuItem::~MenuItem(){
}

/*
  PUBLIC FUNCTIONS
*/

void MenuItem::updateItem(){
  //Update the item visuals, based upon current device status
  if( isConnected() ){
    if(filesystem == "AVDISK" || (filesystem=="UDF" && devType=="CD9660") ){
      //non-mountable audio/video disk (cd/dvd usually)
      devIcon->setEnabled(true);  //Make the icon full color
      devIcon->setToolTip(device+"\n"+tr("Audio/Video Disk"));
      pushMount->setText(tr("Play"));
      pushMount->setIcon(QIcon(":icons/play.png"));
      checkAutomount->setVisible(false);
    }else if( isMounted() ){
      if(mountpoint.isEmpty()){
      	//detect the current mountpoint
      	QString output = pcbsd::Utils::runShellCommandSearch("mount",device);
        mountpoint = output.section(" on ",1,1).section(" (",0,0).replace(" ","-");
      }
      devIcon->setEnabled(true);  //Make the icon full color
      devIcon->setToolTip(device+"\n"+QString(tr("Mounted at %1")).arg(mountpoint));
      pushMount->setText(tr("Eject"));
      pushMount->setIcon(QIcon(":icons/eject.png"));
      if(devType != "ISO"){ checkAutomount->setVisible(true); }
      else{ checkAutomount->setVisible(false); }
    }else{	  
      devIcon->setEnabled(false); //Grey out the icon if not mounted
      devIcon->setToolTip(device);
      pushMount->setText(tr("Mount"));
      pushMount->setIcon(QIcon(":icons/mount.png"));
      checkAutomount->setVisible(false);
    }
  }else{
    emit itemRemoved(device);
    return;
  }
  //Set visibility and sizes on progressbar
  updateSizes();
}
	
//Getters
QString MenuItem::getDeviceName(){
  return devLabel->text();	
}
	
//Device information
bool MenuItem::isConnected(){
  if( QFile::exists(device) ){ return true; }
  else{ return false; }
}

bool MenuItem::isMounted(){
  //Check if device is mounted
  QStringList chk = pcbsd::Utils::runShellCommand("mount");
  bool mounted=false;
  for(int i=0; i<chk.length(); i++){
    mounted = chk[i].contains(device) || chk[i].contains(devLabel->text()) || chk[i].contains(devLabel->text().replace(" ","-"));
    if(!mounted && chk[i].startsWith("/dev/fuse on ") ){
      //No method of probing fuse devices to find the original parent (yet)
    }
    if(mounted){ 
      //Save the mountpoint if it is mounted
      mountpoint = chk[i].section(" on ",1,10).section("(",0,0).simplified();
      break; 
    }
  }
  //Set the rootRequired flag as appropriate
  if(mounted){
    //Only check the mountpoint owner if not already flagged for root perms
    if(!rootRequired && !mountedHere){ 
      QString home = QDir::homePath();
      QString shorthome = home; shorthome.replace("/usr/home/", "/home/");
      //Check if the mountpoint is in the current user's home directory
      rootRequired = !( mountpoint.startsWith(home) || mountpoint.startsWith(shorthome) );
      //qDebug() << "Mountpoint directory in non-user directory:" << rootRequired << mountpoint << home;
    }else if(mountedHere && devType == "ISO"){
      rootRequired = true; //Requires root to remove the MD device, not necessary to mount it though
    }
  }else{
    //Not mounted and passed known user device limitations
    rootRequired = false;
  }
  //qDebug() << "Check for root:" << device << rootRequired;
  //Now update the icon
    if(rootRequired && filesystem=="UNKNOWN"){
      //Add the root-overlay to the base icon
      QPixmap tmp = baseicon;
      QPixmap overlay1(":icons/root-overlay.png");
      QPixmap overlay2(":icons/question-overlay.png");
      QPainter paint(&tmp);
	    paint.drawPixmap(devIcon->width()-20, devIcon->height()-20, overlay1 ); //put it in the bottom-right corner
	    paint.drawPixmap(devIcon->width()-24,0, overlay2 ); //put it in the top-right corner
      devIcon->setPixmap(tmp);	    
    }else if(rootRequired){
      //Add the root-overlay to the base icon
      QPixmap tmp = baseicon;
      QPixmap overlay(":icons/root-overlay.png");
      QPainter paint(&tmp);
	    paint.drawPixmap(devIcon->width()-20, devIcon->height()-20, overlay ); //put it in the bottom-right corner
      devIcon->setPixmap(tmp);
    }else if(filesystem=="UNKNOWN"){
      //Add the root-overlay to the base icon
      QPixmap tmp = baseicon;
      QPixmap overlay(":icons/question-overlay.png");
      QPainter paint(&tmp);
	   paint.drawPixmap(devIcon->width()-24,0, overlay ); //put it in the top-right corner
      devIcon->setPixmap(tmp);
    }else{
      devIcon->setPixmap(baseicon); //base icon w/ no overlay
    }

  return mounted;
}

//Cleanup function
void MenuItem::cleanup(){
  if( isMounted() ){
    unmountItem(); //unmount and remove mountpoint
  }else{
    //Just check for mountpoint removal
    if(QFile::exists(mountpoint) && mountedHere){
      qDebug() << "Removing old mountpoint:" << mountpoint;
      QString output = systemCMD("rmdir "+mountpoint).join(" ");
      if(!output.isEmpty()){ qDebug() << " -Error:" <<output; }
    }
  }
}
/*
  PRIVATE FUNCTIONS
*/
void MenuItem::slotMountClicked(){
  //Hide the parent menu
  emit itemWorking();
  //Now 
  if( isConnected() ){
    if(filesystem=="AVDISK"){
      emit openAVDisk("Audio");
    }else if(filesystem=="UDF" && devType=="CD9660"){
      emit openAVDisk("Video");
    }else if( !isMounted() ){
      mountItem();
    }else{
      unmountItem();  
    }
  }else{
    emit itemRemoved(device);	  
  }
  updateItem();
}

void MenuItem::slotAutoMount(){
//Just like slotMountClicked, but will only mount the device if appropriate (no removals);
  if( isConnected() ){
     if(filesystem=="AVDISK"){
      emit openAVDisk("Audio");
    }else if(filesystem=="UDF" && devType=="CD9660"){
      emit openAVDisk("Video");
    }else if( !isMounted() ){
      mountItem();
    }
  }else{
    emit itemRemoved(device);	  
  }
  updateItem();	
}

void MenuItem::slotAutoMountToggled(bool checked){
  qDebug() << "Auto-mount toggled for:" << device << checked;
  QString entry = devLabel->text()+":::"+devType+":::"+filesystem;
  if(checked){
    //Add this entry to the auto-mount file
    QString cmd = "echo \""+entry+"\" >> "+AMFILE;
    system( cmd.toUtf8() );
  }else{
    //Remove this entry from the automount file
    QString tmpFile = AMFILE+".tmp";
    QString cmd = "cat "+AMFILE+" | grep -v "+entry+" > "+tmpFile+"; mv "+tmpFile+" "+AMFILE;
    system( cmd.toUtf8() );
  }
}

bool MenuItem::checkSavedAutoMount(){
  if(QFile::exists(AMFILE)){
    QString cmd = "cat "+AMFILE;
    QString search = devLabel->text() +" "+ devType +" "+ filesystem;
    QString chk = pcbsd::Utils::runShellCommandSearch(cmd, search);
    if( chk.isEmpty() ){ return false; }
    else{ return true; }
  }else{
    return false;
  }
}

void MenuItem::mountItem(){
  //Mount the device
  
  //Create the full path to the mountpoint
  QString deviceName = devLabel->text();
  QString mntpoint = MOUNTDIR + deviceName.replace(" ","-"); //take into account spaces in the name

  //Create the fileystem specific command for mounting
  QString tmpFileSystem;
  QString cmd;
  if(filesystem=="UNKNOWN"){
    //prompt for filesystem
    bool selected = false;
    tmpFileSystem = QInputDialog::getItem(0, deviceName+"("+devType+")", tr("Mount as:"), DEVCHECK->AvailableFS(), 0, false, &selected);
    if( !selected || tmpFileSystem.isEmpty() ){ return; } //cancelled
    //Now get the mount command
    cmd = DEVCHECK->getMountCommand(tmpFileSystem, device, mntpoint);
  }else{
    cmd = DEVCHECK->getMountCommand(filesystem, device, mntpoint);
  }

  //Make sure the mntpoint is available
  QDir mpd(mntpoint);
  if(mpd.exists() && !rootRequired){
    //Remove the existing directory (will work only if it is empty)
    mpd.cdUp();
    mpd.rmdir(mntpoint);
  }
  //Prepare the mount command to run
  //QString cmd = fstype + " " +fsopts + " " + device + " " + mntpoint;
  qDebug() << "Mounting device" << device << "on" << mntpoint << "("<<filesystem<<")";
  if(DEBUG_MODE){ qDebug() << " - command:" << cmd; }
  //Generate the run script
  QString runscript = createRunScript( mntpoint, cmd);
  //Now run the script 
  bool ok = !runscript.isEmpty();
  bool tryroot = false;
  QStringList outL("ERROR:SCRIPT");
  //Run the mounting commands
  if(ok && !rootRequired){
    outL.clear();
    outL = systemCMD(runscript);
    //qDebug() << "Mount return code 1:" << outL;
    //if it could not mount device with permissions issues - try as root
    if( !outL.filter("Permission denied").isEmpty() || !outL.filter("not permitted").isEmpty() ){ 
      qDebug() << " - Permissions issue, try as root";
      tryroot = true; 
    } 
  }
  if( (ok && rootRequired) || tryroot ){
    outL.clear();
    outL = systemCMD("pc-su "+runscript);
    //qDebug() << "Mount return code 2:" << outL;
  }
  //Now parse the return code
  QString result, title;
  mountedHere = true; //need to set this before running isMounted to update icons right
  ok = isMounted();
  mountedHere = ok; //now make sure it is the proper value
  if( ok ){
	title = tr("Success");
	result = QString( tr("%1 mounted at %2") ).arg(deviceName).arg(mntpoint);
	if(tryroot){ rootRequired = true; } //flag this as requiring root for later
  }else if( !outL.filter("ERROR:MOUNTPOINT").isEmpty() ){
	title = tr("Failure");
	result = QString( tr("Could not create mountpoint: %1") ).arg(mntpoint);
  }else if( !outL.filter("ERROR:MOUNTING").isEmpty() ){
	title = tr("Failure");
	result = QString( tr("Could not mount device %1 on %2 (%3)") ).arg(deviceName, mntpoint, filesystem);
  }else{
	QString tmp = outL.join("");
	  tmp.remove("password:"); //pc-su sometimes outputs this
	if(!tmp.simplified().isEmpty() || !(rootRequired || tryroot) ){ //check for pc-su cancellation
	  qDebug() << "General Error output:" << outL;
	  title = tr("General Error");
	  result = tr("Could not create/run the device mounting script");
	}
  }
  qDebug() << "pc-mounttray: "<<title << result;
  if(DEBUG_MODE){ qDebug() << " - output:" << outL; }
  
  //Output the proper signals depending upon success
  if(ok){
    emit itemMounted(mntpoint);
    mountpoint = mntpoint;
    if( !tmpFileSystem.isEmpty() ){ filesystem = tmpFileSystem; } //this one worked - use it in the future
  }else{
    mountpoint.clear();
  }
  if( !checkAutomount->isChecked()  && !(title.isEmpty() && result.isEmpty()) ){
    emit newMessage(title, result); //suppress the output message if it was automounted
  }
  //Now remove the runscript
  //if(ok)  //only for testing purposes
    QFile::remove(runscript);
  
}

void MenuItem::unmountItem(bool force){
  //Unmount the device
	
  //Check to see if the current mountpoint exists or if it is somewhere else
  if( !QFile::exists(mountpoint) ){
    if( !isMounted() ){  //double check that it is actually mounted (and fix the mountpoint)
      //it is not mounted to begin with
      return;
    }
  }
  bool ok = umount(force, mountpoint);
  QString result, title;
  if(ok){
    title = QString( tr("%1 has been successfully unmounted.") ).arg(devLabel->text());
    if(devType == "ISO"){
      result = tr("The ISO file has been detached from the system.");
    }else{
      result = tr("It is now safe to remove the device");
    }
  }else{
    title = QString( tr("Error: %1 was not unmounted") ).arg(devLabel->text());
  }
  //emit the proper signals
  if(ok && !isMounted()){ //definitely unmounted
    qDebug() << " *Success*";
    mountpoint.clear();
    emit itemUnmounted(device);
    emit newMessage(title, result);
  }else if(!ok){ //error unmounting
    qDebug() << " *Failure*";
    emit newMessage(title, result);
  }//Will not emit anything if the unmount was cancelled for some reason
  
}

void MenuItem::updateSizes(){
  //this method only works if the device is currently mounted
  bool ok = false;
  if(isMounted()){
    QString cmd = "df \""+mountpoint+"\"";
    QStringList output = systemCMD(cmd); //make sure we use the one with a 1K blocksize
    if(output.length() > 1){
      //parse the output (1K blocks) and save them
      QString line = output[1].replace("\t"," ");
      //qDebug() << "df output:" << output << cmd;
      maxSize = line.section(" ",1,1,QString::SectionSkipEmpty).simplified();
      currentSize = line.section(" ",2,2,QString::SectionSkipEmpty).simplified();
      ok=true;
    }else{
      maxSize.clear();
      currentSize.clear();	    
    }
  }else{
    maxSize.clear();
    currentSize.clear();
  }
  //Now setup the display progressbar display
  if(ok){
    currentSpace->setMaximum( maxSize.toInt() );
    currentSpace->setValue( currentSize.toInt() );
    currentSpace->setVisible(true);
    //display the actual size available in the tooltip
    QString diskAvailable = getSizeDisplay( maxSize.toInt() - currentSize.toInt() );
    //qDebug() << "MaxSize:" << maxSize << maxSize.toInt();
    //qDebug() << "CurrentSize:" << currentSize << currentSize.toInt();
    //qDebug() << "Disk Available:" << diskAvailable;
    currentSpace->setToolTip( QString( tr("%1 of disk space available") ).arg(diskAvailable) );
  }else{
    currentSpace->setVisible(false);
  }
  	
}

QString MenuItem::getSizeDisplay(int kb){
  //convert from KB to human readable output
  if( kb > 1048576 ){ //display in GB
    return QString::number( double(int( (kb*100)/1048576 )/100.0) )+"GB";
  }else if( kb > 1024 ){ //display in MB
    return QString::number( double(int( (kb*100)/1024 )/100.0) )+"MB";	  
  }else{ //display in KB
    return QString::number( kb )+"KB";	  
  }
	
}

QStringList MenuItem::systemCMD(QString command){ 
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

QString MenuItem::createRunScript(QString mntpoint, QString mntcmd){
  //generate the run script filename
  QString filename = QDir::homePath()+"/.mounttrayrunscript";
  if(QFile::exists(filename)){
    int i=2;
    while(QFile::exists(filename+QString::number(i))){ i++; }
    filename = filename+QString::number(i);
  }
  //Now generate the run script
  QFile file(filename);
  if( !file.open(QFile::WriteOnly | QFile::Text) ){
    return ""; //could not create run script
  }
  QTextStream out(&file);
  out << "#!/bin/sh\n";
    //Create the mountpoint
    out << "mkdir \""+mntpoint+"\"\n";
    out << "if [ $? -ne 0 ]; then\n  echo ERROR:MOUNTPOINT\n  exit 1\nfi\n";
    //Now set the mountpoint permissions
    if(filesystem != "FAT"){ //FAT command sets permissions itself
      out << "chmod 755 "+mntpoint+"\n";
      out << "if [ $? -ne 0 ]; then\n  echo ERROR:PERMS\nfi\n";
    }
    //run the mount command
    out << mntcmd + "\n";
    out << "if [ $? -ne 0 ]; then\n  rmdir "+mntpoint+"\n  echo ERROR:MOUNTING\n  exit 2\nfi\n";
  
  //Now exit with the success signal
  out << "exit 0";
  file.close();
  QFile::setPermissions(filename, QFile::permissions(filename) | QFile::ExeOwner); //make it executable
  return filename;
}

QString MenuItem::createRemoveScript(QString mntpoint, bool force){
 //generate the run script filename
  QString filename = QDir::homePath()+"/.mounttrayrunscript";
  if(QFile::exists(filename)){
    int i=2;
    while(QFile::exists(filename+QString::number(i))){ i++; }
    filename = filename+QString::number(i);
  }
  //Now generate the run script
  QFile file(filename);
  if( !file.open(QFile::WriteOnly | QFile::Text) ){
    return ""; //could not create run script
  }
  QTextStream out(&file);
  out << "#!/bin/sh\n";
     //Unmount all the NULLFS mountpoints first (in case it has been mounted into a PBI container)
    QStringList nullfs = systemCMD("mount").filter(mntpoint).filter("nullfs");
    bool ok= true;
    for(int i=0; i<nullfs.length() && ok; i++){
      QString nfspoint = nullfs[i].section(" on ",1,10).section("(",0,0).simplified();
      if(!force){ out << "umount \""+nfspoint+"\"\n"; }
      else{        out << "umount -f \""+nfspoint+"\"\n"; }
      out << "if [ $? -ne 0 ]; then\n  echo ERROR:UNMOUNT\n exit 1\nfi\n";
      qDebug() << " - will unmount nullfs point:" << nfspoint;
    }
    //Unmount the device
    if(!force){ out << "umount \""+mntpoint+"\"\n";  }
    else{        out << "umount -f \""+mntpoint+"\"\n";  }
    out << "if [ $? -ne 0 ]; then\n  echo ERROR:UNMOUNT\n  exit 1\nfi\n";
    //Remove the mountpoint if appropriate
    if(mountedHere){
      qDebug() << " - will remove mountpoint directory";
      out << "rmdir \""+mntpoint+ "\"\n";
      out << "if [ $? -ne 0 ]; then\n  echo ERROR:RMDIR\nfi\n";
    }
    //If an MD device, also remove it
    if(devType=="ISO" && device.section("/",-1).startsWith("md") && mountedHere ){
      //Get the md number
      QString num = device.section("/md",-1).simplified();
      //also remove the MD device from the system using "mdconfig"
      qDebug() << " - will detach memory disk #"<< num;
      out << "mdconfig -d -u "+num+"\n";
      out << "if [ $? -ne 0 ]; then\n  echo ERROR:MDDETACH\nfi\n";
    }
  //Now exit with the success signal
  out << "exit 0";
  file.close();
  QFile::setPermissions(filename, QFile::permissions(filename) | QFile::ExeOwner); //make it executable
  return filename;
}

bool MenuItem::umount(bool force, QString mntpoint){
  qDebug() << "Unmounting device from" << mntpoint;
  if(rootRequired){ qDebug() << " - prompt for root"; }
  QString runscript = createRemoveScript(mntpoint, force);
  //Run the commands
  QStringList output;
  QString result, title;
  bool ok = !runscript.isEmpty();
  if(ok && rootRequired){ output = systemCMD("pc-su "+runscript); }
  else if(ok){ output = systemCMD(runscript); }
  else{ return false; } //could not even create the runscript
  //Check output
  if(output.filter("ERROR:UNMOUNT").isEmpty() && !isMounted() ){
    //unmounting successful
    if( !output.filter("ERROR:RMDIR").isEmpty() ){
      qDebug() << "pc-mounttray: Error removing mountpoint:" << mountpoint;
      qDebug() << " - Error message:" << output;
    }
    if( !output.filter("ERROR:MDDETACH").isEmpty() ){
      qDebug() << "pc-mounttray: Error detaching MD device";
      qDebug() << " - Error message:" << output;
    }
    ok = true;
    mountedHere = false; //not mounted by this app anymore
  }else if( !output.filter("Device busy").isEmpty() && !force){
    qDebug() << " - Device Busy";
    //Ask whether to force the removal
    if(QMessageBox::Yes == QMessageBox::question(0,tr("Device Busy"),
	         tr("The device appears to be busy. Would you like to unmount it anyway?")+"\n\n"+tr("NOTE: This is generally not recommended unless you are sure that you don't have any applications using the device."),
                 QMessageBox::Yes | QMessageBox::No, QMessageBox::No) ){
        ok = umount(true, mntpoint); //force the unmount recursively
    }
  }else{
    qDebug() << "pc-mounttray: Error unmounting mountpoint:" << mountpoint;
    qDebug() << " - Error message:" << output;
    ok = false;
  }
  if(DEBUG_MODE){ qDebug() << " - output:" << output; }
  QFile::remove(runscript);
  
  return ok;
}

