
#include <pcbsd-utils.h>
#include "devCheck.h"


DevCheck::DevCheck(){
  //Initialize the lists of valid device types
  validDevs << "da" << "ad" << "mmcsd" << "cd" << "acd" << "md" << "da";
  validDevTypes << "USB" << "SATA" << "SD" << "CD9660" << "CD9660" << "ISO" << "SCSI";
  for(int i=0; i<validDevs.length(); i++){
    devFilter << validDevs[i]+"*";
  }
  //Initialize lists of filesystems and detection strings for automatic detection
  fsDetection.clear();
  fsMatch.clear();
  fsFilter.clear();
  fsDetection << "FAT" << "NTFS" << "EXT" << "ISO 9660" << "Unix Fast File system" << "Reiser" << "XFS"; //string to find in "file -s" output
  dsDetection << "FAT" << "NTFS" << "EXT" << "ISO9660" << "UFS" << "Reiser" << "XFS" << "UDF"; //string to find in "diskinfo" output
  fsMatch << "FAT" << "NTFS" << "EXT" << "CD9660" << "UFS" << "REISERFS" << "XFS" << "UDF"; //internal labels for the filesystems
  fsFilter << "fat" << "ntfs" << "ext" << "cdrom" << "ufs" << "reiser" << "xfs" << "udf"; //label categories in /dev/
  //Initialize lists of Manual Filesystems that might be available
  fsManual.clear(); fsCMD.clear(); fsBinCheck.clear();
  fsManual << "FAT"  << "EXFAT" << "NTFS" << "EXT" << "EXT4" << "CD9660" << "UFS" << "REISERFS" << "XFS" << "UDF";
  //fsCMD: %1 becomes device path, %2 becomes mointpoint path
  QString langCode = QString(getenv("LANG"));
  if(langCode.startsWith("C.") || langCode.startsWith("en")){
    fsCMD << "mount -t msdosfs -o large,longnames,-m=755 %1 %2"; //FAT w/o locale code
  }else{
    fsCMD << "mount -t msdosfs -o large,longnames,-m=755,-L="+langCode+" %1 %2"; //FAT
  }
  fsCMD << "mount.exfat-fuse %1 %2"; //EXFAT
  fsCMD << "ntfs-3g %1 %2"; //NTFS
  fsCMD << "mount -t ext2fs %1 %2"; //EXT
  fsCMD << "ext4fuse %1 %2"; //EXT4
  fsCMD << "mount -t cd9660 %1 %2"; //CD9660
  fsCMD << "mount -t ufs %1 %2"; //UFS
  fsCMD << "mount -t reiserfs %1 %2"; //REISERFS
  fsCMD << "mount -t xfs %1 %2"; //XFS
  fsCMD << "mount -t udf %1 %2"; //UDF
  fsBinCheck << "/sbin/mount_msdosfs" << "/usr/local/bin/mount.exfat-fuse" << "/usr/local/bin/ntfs-3g" << "/sbin/mount" \
		<< "/usr/local/bin/ext4fuse" << "/sbin/mount_cd9660" << "/sbin/mount" << "/sbin/mount" << "/sbin/mount" << "/sbin/mount_udf";
  //Initialize the device directory
  devDir = QDir(DEVICEDIR);
  //Find all the currently active devices (that system is booted from)
  findActiveDevices();
}

DevCheck::~DevCheck(){
}

/*
  PUBLIC FUNCTIONS
*/

bool DevCheck::isValid(QString node){
  bool ok = false;
  if(activeDevs.contains(node)){ return false; } //currently active node - skip it
  for(int i=0; i<validDevs.length(); i++){
    if(node.startsWith(validDevs[i]) && node!="mdctl"){ 
	ok = true; 
	break; 
    }
  }
  return ok;
}

QStringList DevCheck::devChildren(QString node){
  devDir.cd(DEVICEDIR);
  QStringList subdevs = devDir.entryList(devFilter, QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::System, QDir::NoSort);
  //Clean up the list of children
  for(int i=0; i<subdevs.length(); i++){
    if( (subdevs[i] == node) || subdevs[i].isEmpty() || !subdevs[i].startsWith(node) ){
      subdevs.removeAt(i);
      i--;
    }
  }
  //qDebug() << "[DEBUG] " <<node << "Device Children:" << subdevs;
  return subdevs;
}

QString DevCheck::devLabel(QString node, QString filesystem){
  QString dlabel;
  if(filesystem.toLower()=="ntfs"){
    //Use ntfslabel for ntfs filesystems
    QStringList tmp = pcbsd::Utils::runShellCommand("ntfslabel "+devDir.absoluteFilePath(node) );
    if(tmp.length() ==1){
      dlabel = tmp[0]; //good result
    }else if(tmp.length()==2 && tmp[0].startsWith("Failed ")){
      dlabel = tmp[1]; //also good result, just warning for first line (Windows 8)
    }
    //skip outputs if 0 or >2 lines - (errors without detected label)
  }else{
    //All other filesystems
    QStringList glout = pcbsd::Utils::runShellCommand("glabel list");
    int index = glout.indexOf("Geom name: "+node);
    while(index != -1){
      for(int i=index; i<glout.length(); i++){ 
        if(glout[i].contains("Name: ")){ index = i; break;} //should only be 1 or 2 lines to find this
      }
      if(index == -1){ break; } //quick check to make sure we don't error out
      QString path = glout[index].section("Name:",1,10,QString::SectionSkipEmpty).simplified();  
      int fschk = fsMatch.indexOf(filesystem);
      if(fschk != -1){
        if(fsFilter[fschk] == path.section("/",0,0).simplified() ){
          //good label
          dlabel = path.section("/",-1).simplified();
          break;
        }
      }
      index = glout.indexOf("Geom name: "+node, index); //move to the next index if there is one
    }
    dlabel.replace("%20", " "); //quick check to make sure it does not have that special character
  }
  return dlabel;
}

bool DevCheck::devInfo(QString dev, QString* type, QString* label, QString* filesystem, QString* maxsize){
  //OUTPUT: bool isGoodDevice
  //INPUTS/OUTPUTS: type, label, filesystem, maxsize
  //INPUT: dev = device node (da0, not /dev/da0)
	
  //Clear the output variables
  type->clear(); label->clear(); filesystem->clear(); maxsize->clear();
  //make sure to have both the full path and just node
  QString node;
  QString fullDev;
  if(dev.startsWith(DEVICEDIR)){ fullDev = dev; node = dev.section("/",-1); }
  else{ node = dev; fullDev = DEVICEDIR + dev; }
  //Do not allow sym-links
  if(!QFile::symLinkTarget(fullDev).isEmpty()){ return false; }
  //Do not allow currently active devices
  if(activeDevs.contains(node)){ return false; }
  //Double check for valid device types (just in case)
  QString detType;
  for(int i=0; i<validDevs.length(); i++){
    if(node.startsWith(validDevs[i]) && node != "mdctl"){ 
	detType = validDevTypes[i]; 
	break; 
    }
  }
  //Make sure it is a no-child device (except memory disks or CD's, those need to be the parent)
  int children = devChildren(node).length();
  if( !node.startsWith("md") && detType!="CD9660" && detType!="ISO" && children>0 ){ return false; }
  //Make sure we quit before running commands on any invalid device nodes
  if(detType.isEmpty() || !QFile::exists(fullDev) ){return false;}
  //Get the camctl info and refine the device type if possible
  QString camctl;
  if(detType == "USB" && QFile::exists(fullDev)){
    //make sure that it is not a SCSI device
    camctl = pcbsd::Utils::runShellCommand( QString("camcontrol inquiry ")+node.section("s",0,0) ).join(" ");
    if(camctl.contains(" Fixed Direct Access SCSI")){ detType = "SCSI"; }
    if(camctl.contains("camcontrol")){ camctl.clear(); } //error or invalid device type
  }else if(detType == "SATA" && QFile::exists(fullDev)){
    camctl = pcbsd::Utils::runShellCommand( QString("camcontrol identify ")+node.section("s",0,0) ).join(" ");
    if(camctl.contains("camcontrol")){ camctl.clear(); } //error or invalid device type
  }
  if(!camctl.isEmpty()){
    //Alternate Device name for label later (if needed)
    camctl = camctl.section(">",0,0).section("<",-1).section(" ",0,0).simplified();
    QString partition = node.section("s",1,1);
    if(!partition.isEmpty()){ camctl.append("-"+partition); }
  }
  //Save the device type
  type->append(detType);
  
  bool isCD= (detType == "CD9660" || detType == "ISO"); //simplification for later
  //Run either "file -s" or "disktype" to get device info
  bool valid = false;
  QString fs, dlabel;
  if(QFile::exists("/usr/local/bin/disktype")){
    valid = getDiskInfo(fullDev, &fs, &dlabel);
  }else{
    // "file -s" should always work - use it as the backup method
    valid = getSpecialFileInfo(fullDev, &fs, &dlabel);
  }
  if(detType=="ISO" && fs.isEmpty() ){ valid = false; } //skip ISO's with unknown filesystems (swap, etc)
  else if(detType=="CD9660" &&  fs.isEmpty() ){
    //Try to determine which type of cd/dvd this is since it is not a data disk (which can be mounted)
    fs = "AVDISK"; //audio/video disk (not mountable)
  }
  if(!valid){ //don't bother continuing - already invalid
    if(DEBUG_MODE){ qDebug() << "Invalid Device:" << node << detType << dlabel << fs; } 
    return false; 
  } 
 
  //If the filesystem could not be detected or is not supported
  if(fs.isEmpty()){ fs = "UNKNOWN"; }
  
  //Now get the device label (if there is one) using glabel
  QString glabel;
  //Don't use glabel for SATA devices right now because it is inconsistent
  if(!isCD){ glabel = devLabel(node, fs); }
  //Check to see if we have a label, otherwise assign one
  if( !glabel.isEmpty() ){ dlabel = glabel; } //glabel
  else if(!dlabel.isEmpty()){ } //device info label
  else if( !camctl.isEmpty() ){ dlabel = camctl; } //camcontrol device ID
  else{
    //Assign a device label
    if(isCD){
      if(detType == "ISO"){
	dlabel = "ISO_File";
      }else{
        dlabel = "Optical_Disk";
      }
    }else{
      dlabel = detType+"-Device"; //this is not a "detected" label
    }
  }
  //make sure that a device label does not contain "(" or ")"
  if(dlabel.contains("(")){ dlabel = dlabel.remove("(").simplified(); }
  if(dlabel.contains(")")){ dlabel = dlabel.remove(")").simplified(); }
  dlabel = dlabel.simplified();

  //Setup the outputs
  label->append(dlabel);
  filesystem->append(fs);
  return valid;
}

QStringList DevCheck::AvailableFS(){
  QStringList avail;
  for(int i=0; i<fsBinCheck.length(); i++){
    if( QFile::exists(fsBinCheck[i]) ){ avail << fsManual[i]; }
  }
  return avail;
}

QString DevCheck::getMountCommand(QString FS, QString dev, QString mntpoint){
  QString CMD = "mount_auto "+dev+" "+mntpoint;
  int index = fsManual.indexOf(FS);
  if(index != -1){
    CMD = fsCMD[index].arg(dev, mntpoint);
  }else{
    qDebug() << "Using mount_auto: " << dev <<mntpoint;
  }
  return CMD;
}

/*
  PRIVATE FUNCTIONS
*/

void DevCheck::findActiveDevices(){
  activeDevs.clear();
  //Now find any active partitions and ignore it and any children of it
  QStringList info = pcbsd::Utils::runShellCommand("gpart show -p");
  info = info.filter("freebsd").filter("[active]");
  for(int i=0; i<info.length(); i++){
    info[i].remove("=>");
    info[i] = info[i].replace("\t"," ").simplified();
    QString dev = info[i].section(" ",2,2,QString::SectionSkipEmpty);
    activeDevs << dev;
    activeDevs << DevCheck::devChildren(dev);
  }
  
  activeDevs.removeDuplicates();
  //qDebug() << "Active Devices:" << activeDevs;
}

bool DevCheck::getDiskInfo(QString fulldev, QString *filesystem, QString *label){
  //This function will run the "disktype" utility and parses the output
  //  -- it will return "true" if there is a valid size for the device
  //  -- NOTE: Can only detect filesystem/size/label, and NOT whether it is active
  filesystem->clear(); label->clear();
  QStringList info = pcbsd::Utils::runShellCommand("disktype "+fulldev);
  QString bytes; bool blankDisk=false;
  for(int i=0; i<info.length(); i++){
    if(info[i].isEmpty() || info[i].startsWith("---")){ continue; } //skip this line
    else if( info[i].startsWith("Character device,") && !info[i].contains("unknown size") ){
      //Get the size if possible
      QString tmp = info[i].section("(",1,1).section(")",0,0);
      if(tmp.contains("bytes")){ bytes = tmp.section(" ",0,0,QString::SectionSkipEmpty); }
    }else if( info[i].contains("Blank disk/medium") ){ 
      blankDisk = true;
    }else if( info[i].contains("file system") ){
      QString tmp = info[i].section("file system",0,0);
      for(int j=0; j<dsDetection.length(); j++){
        if(tmp.contains(dsDetection[j])){ filesystem->clear(); filesystem->append(fsMatch[j]); break; }
      }
    }else if( info[i].contains("Volume name") ){
      QString tmp = info[i].section("\"",1,1).section("\"",0,0).simplified(); //name is within quotes
      label->append(tmp);
    }
    //stop if all info found (size is always the first to be found in info)
    if(!filesystem->isEmpty() && !label->isEmpty()){ break; }
  }
  return !(bytes.isEmpty() || blankDisk);
}

bool DevCheck::getSpecialFileInfo(QString fulldev, QString *filesystem, QString *label){
  //This function will run the "file -s" utility and parses the output
  //  -- it will return "true" if there is a valid size for the device and it is NOT "active"
  //  -- filesystem/label detection is very sketchy with this utility
  filesystem->clear(); label->clear();
  QString info = pcbsd::Utils::runShellCommand("file -s "+fulldev).join("").simplified();
  //Quick exit if invalid device
  if(info.contains("ERROR:") || info.contains("Device not configured") ){ return false; }
  //Get ready to look for info
  QStringList infoL = info.split(",");
  bool hasblocks = false;
  bool active = false;
  //Check for the filesystem
  for(int i=0; i<fsDetection.length(); i++){
    if(info.contains(fsDetection[i])){ filesystem->append( fsMatch[i] ); break; }
  }
  //Check if it is active
  if( !infoL.filter("active").isEmpty() ){ active = true; }
  //Check for listed number of blocks
  if( !infoL.filter("blocks").isEmpty() || !infoL.filter("sectors").isEmpty() ){ hasblocks = true; }
  else if( filesystem->contains("CD9660") ){ hasblocks = true; } //CD/DVD devices do not report size info when filled
  //Few checks for a disk label
  if( !infoL.filter("volume name").isEmpty() ){
    label->append( infoL.filter("volume name").join("").section("volume name",0,0,QString::SectionSkipEmpty).simplified() );
  }else if( !infoL.filter("label: \"").isEmpty() ){
    label->append( infoL.filter("label: \"").join("").section("label: \"",1,1).section("\"",0,0).simplified() );
  }else if( (infoL.length() == 1) && filesystem->contains("CD9660") && info.contains("\'") ){
    label->append( info.section("\'",1,1).section("\'",0,0).simplified() );
  }
  return (hasblocks && !active);  
}
