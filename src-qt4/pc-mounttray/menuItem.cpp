#include <pcbsd-utils.h>
#include "menuItem.h"


MenuItem::MenuItem(QWidget* parent, QString newdevice, QString newlabel, QString newtype, QString newfs) : QWidgetAction(parent)
{
  AMFILE= QDir::homePath() + "/.pc-automounttray";   //File to save/load all the devices to be automounted
  //Set the device info variables
  if( !newdevice.startsWith(DEVICEDIR) ){ newdevice.prepend(DEVICEDIR); }
  device = newdevice;
  devType = newtype;
  filesystem = newfs;
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
  if(devType == "USB"){ devIcon->setPixmap(QPixmap(":icons/usb.png")); }
  else if(devType == "SATA"){ devIcon->setPixmap(QPixmap(":icons/harddrive.png")); }
  else if(devType == "SD"){ devIcon->setPixmap(QPixmap(":icons/sdcard.png")); }
  else if(devType == "CD9660"){ devIcon->setPixmap(QPixmap(":icons/dvd.png")); }
  else if(devType == "ISO"){devIcon->setPixmap(QPixmap(":icons/dvd.png")); }
  //Start the automount procedure if necessary
  if(checkAutomount->isChecked() || devType=="ISO"){
    QTimer::singleShot(500,this,SLOT( slotMountClicked() ));
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
    if( isMounted() ){
      if(mountpoint.isEmpty()){
      	//detect the current mountpoint
      	QString output = pcbsd::Utils::runShellCommandSearch("mount",device);
        mountpoint = output.section(" on ",1,1).section(" (",0,0).replace(" ","-");
      }
      devIcon->setEnabled(TRUE);  //Make the icon full color
      devIcon->setToolTip(device+"\n"+QString(tr("Mounted at %1")).arg(mountpoint));
      pushMount->setText(tr("Eject"));
      pushMount->setIcon(QIcon(":icons/eject.png"));
      if(devType != "ISO"){ checkAutomount->setVisible(TRUE); }
      else{ checkAutomount->setVisible(FALSE); }
    }else{	  
      devIcon->setEnabled(FALSE); //Grey out the icon if not mounted
      devIcon->setToolTip(device);
      pushMount->setText(tr("Mount"));
      pushMount->setIcon(QIcon(":icons/mount.png"));
      checkAutomount->setVisible(FALSE);
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
  if( QFile::exists(device) ){ return TRUE; }
  else{ return FALSE; }
}

bool MenuItem::isMounted(){
  //Check if device is mounted
  QStringList chk = pcbsd::Utils::runShellCommand("mount");
  bool mounted=false;
  for(int i=0; i<chk.length(); i++){
    mounted = chk[i].contains(device) || chk[i].contains(devLabel->text()) || chk[i].contains(devLabel->text().replace(" ","-"));
    if(mounted){ 
      //Save the mountpoint if it is mounted
      mountpoint = chk[i].section(" on ",1,10).section("(",0,0).simplified();
      break; 
    }
  }
  return mounted;
}

//Cleanup function
void MenuItem::cleanup(){
  if( isMounted() ){
    unmountItem(); //unmount and remove mountpoint
  }else{
    //Just check for mountpoint removal
    if(QFile::exists(mountpoint)){
      qDebug() << "Removing old mountpoint:" << mountpoint;
      QString output = pcbsd::Utils::runShellCommand("rmdir "+mountpoint).join(" ");
      if(!output.isEmpty()){ qDebug() << " -Error:" <<output; }
    }
  }
}
/*
  PRIVATE FUNCTIONS
*/
void MenuItem::slotMountClicked(){
  if( isConnected() ){
    if( !isMounted() ){
      mountItem();
    }else{
      unmountItem();  
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
    if( chk.isEmpty() ){ return FALSE; }
    else{ return TRUE; }
  }else{
    return FALSE;
  }
}

void MenuItem::mountItem(){
  //Mount the device
  
  //Create the full path to the mountpoint
  QString deviceName = devLabel->text();
  QString mntpoint = MOUNTDIR + deviceName.replace(" ","-"); //take into account spaces in the name

  //Create the fileystem specific command for mounting
  QString fstype;
  QString fsopts="";
  if( filesystem == "FAT" ){ fstype = "mount -t msdosfs"; fsopts = "-o large,longnames,-m=644,-M=777"; }
  else if(filesystem == "NTFS"){ fstype = "ntfs-3g"; }
  else if(filesystem == "EXT"){ fstype = "mount -t ext2fs"; }
  else if(filesystem == "CD9660"){ fstype = "mount -t cd9660"; }
  else if(filesystem == "UFS"){ fstype = "mount -t ufs"; }
  else if(filesystem == "REISERFS"){ fstype = "mount -t reiserfs"; }
  else if(filesystem == "XFS"){ fstype = "mount -t xfs"; }
  else{
    qDebug() << "Unknown device filesystem:" << device << filesystem << " attempting mount_auto command";
    fstype = "mount_auto";
    //QMessageBox::warning(this,tr("Unknown Device Filesystem"),tr("The filesystem on this device is unknown and cannot be mounted at this time") );
    //return FALSE;
  }
  //Make sure the mntpoint is available
  QDir mpd(mntpoint);
  if(mpd.exists()){
    //Remove the existing directory (will work only if it is empty)
    mpd.cdUp();
    mpd.rmdir(mntpoint);
  }
  //Prepare the commands to run
  QString cmd1 = "mkdir " + mntpoint;
  QString cmd2 = fstype + " " +fsopts + " " + device + " " + mntpoint;
  QString cmd3 = "chmod 777 " + mntpoint; //to set full user/root access
  
  qDebug() << "Mounting device" << device << "on" << mntpoint << "("<<filesystem<<")";
  if(DEBUG_MODE){ qDebug() << " - command:" << cmd2; }
  
  bool ok = FALSE;
  QString result, title;
  //Run the mounting commands
  QStringList output = pcbsd::Utils::runShellCommand(cmd1);
  if( output.join(" ").simplified().isEmpty() ){
    //directory created, run the next commands
    system(cmd3.toUtf8()); //set directory permissions before mounting device
    output = pcbsd::Utils::runShellCommand(cmd2);
    if( output.join(" ").simplified().isEmpty() ){
      title = tr("Success");
      result = QString( tr("%1 mounted at %2") ).arg(deviceName).arg(mntpoint);
      ok = TRUE;
    }else{
      qDebug() << "pc-mounttray: Error mounting device:" << device;
      qDebug() << " - Error message:" << output;
      title = QString( tr("Error mounting %1 at %2") ).arg(deviceName).arg(mntpoint);
      result =  output.join(" ");
      //Remove the mount point just created
      pcbsd::Utils::runShellCommand("rmdir "+mntpoint);
    }
  }else{
    qDebug() << "pc-mounttray: Error creating mountpoint:" << mntpoint;
    qDebug() << " - Error message:" << output;
    title = QString( tr("Error mounting %1") ).arg(deviceName);
    result =  QString( tr("Could not create mount point at %1") ).arg(mntpoint);
  }
  //Output the proper signals depending upon success
  if(ok){
    emit itemMounted(mntpoint);
    mountpoint = mntpoint;
  }else{
    mountpoint.clear();
  }
  if( !checkAutomount->isChecked() ){
    emit newMessage(title, result); //suppress the output message if it was automounted
  }
  
}

void MenuItem::unmountItem(){
  //Unmount the device
	
  //Check to see if the current mountpoint exists or if it is somewhere else
  if( !QFile::exists(mountpoint) ){
    if( !isMounted() ){  //double check that it is actually mounted (and fix the mountpoint)
      //it is not mounted to begin with
      return;
    }
  }
  //Make sure there are no spaces in the mounpoint path
  QString cmd1 = "umount \"" + mountpoint +"\"";
  QString cmd2 = "rmdir \"" + mountpoint +"\"";
  qDebug() << "Unmounting device from" << mountpoint;
  //Run the commands
  QStringList output;
  QString result, title;
  bool ok = FALSE;
  output = pcbsd::Utils::runShellCommand(cmd1);
  if(output.join(" ").simplified().isEmpty()){
    //unmounting successful, remove the mount point directory
    output = pcbsd::Utils::runShellCommand(cmd2);
    if(!output.join(" ").simplified().isEmpty()){
      qDebug() << "pc-mounttray: Error removing mountpoint:" << mountpoint;
      qDebug() << " - Error message:" << output;
    }
    ok = TRUE;
    title = QString( tr("%1 has been successfully unmounted.") ).arg(devLabel->text());
    if(devType == "ISO"){
      result = tr("The ISO file has been completely detached from the system.");
    }else{
      result = tr("It is now safe to remove the device");
    }
  }else{
    qDebug() << "pc-mounttray: Error unmounting mountpoint:" << mountpoint;
    qDebug() << " - Error message:" << output;
    title = QString( tr("Error: %1 could not be unmounted") ).arg(devLabel->text());
    result = output.join(" ");
  }
  //emit the proper signals
  if(ok){
    mountpoint.clear();
    if(devType=="ISO" && device.section("/",-1).startsWith("md") ){
      //Get the md number
      QString num = device.section("/md",-1).simplified();
      //also remove the MD device from the system using "mdconfig"
      qDebug() << "Detaching Memory Disk:" << num;
      QString cmd = "mdconfig -d -u "+num;
      system(cmd.toUtf8());
    }
    emit itemUnmounted(device);
  }
  emit newMessage(title, result);
}

void MenuItem::updateSizes(){
  //this method only works if the device is currently mounted
  bool ok = FALSE;
  if(isMounted()){
    QString cmd = "df \""+mountpoint+"\"";
    QStringList output = systemCMD(cmd); //make sure we use the one with a 1K blocksize
    if(output.length() > 1){
      //parse the output (1K blocks) and save them
      QString line = output[1].replace("\t"," ");
      //qDebug() << "df output:" << output << cmd;
      maxSize = line.section(" ",1,1,QString::SectionSkipEmpty).simplified();
      currentSize = line.section(" ",2,2,QString::SectionSkipEmpty).simplified();
      ok=TRUE;
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
    currentSpace->setVisible(TRUE);
    //display the actual size available in the tooltip
    QString diskAvailable = getSizeDisplay( maxSize.toInt() - currentSize.toInt() );
    //qDebug() << "MaxSize:" << maxSize << maxSize.toInt();
    //qDebug() << "CurrentSize:" << currentSize << currentSize.toInt();
    //qDebug() << "Disk Available:" << diskAvailable;
    currentSpace->setToolTip( QString( tr("%1 of disk space available") ).arg(diskAvailable) );
  }else{
    currentSpace->setVisible(FALSE);
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
