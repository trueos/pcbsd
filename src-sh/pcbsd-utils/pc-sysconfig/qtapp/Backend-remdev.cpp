#include "Backend.h"
#include "DevDB.h" //Device Database Backend

#include <QDebug>

/*
NOTE about IntMountPoints list:
  Format: [node, filesystem, mntdir, user, (canremove/noremove), (internal/external)]
   node: 		The raw device name (/dev/da0 has a node of "da0") (optional)
   filesystem: 	The filesystem used for mounting the device (optional)
   mntdir: 		Directory where the device is mounted (required)
   user:  		Used for logging and initializing the mountpoint for a device
   [can/no]remove: 	Flag for whether the mountpoint should be cleaned up when unmounted
   [internal/external]: Flag for whether the device was mounted by this utility or not (will unmount internal device on service stop)
*/

/*
NOTES about memory disks (ISO type or md* node names)
1) If there are multiple partitions/slices on the disk, they need to be mounted/unmounted individually
2) In order to remove a memory device, it must be given the base md number (not the partition/slice number)
3) Make sure that all partitions are unmounted before removing a memory disk!
*/

//REMOVABLE DEVICES (remdev)
void Backend::updateIntMountPoints(bool passive){
  //Format: 
  //qDebug() << "Updating internal mountpoints";
  //First run "mount" to make sure and get the complete list of mounted devices
  QStringList info = runShellCommand("mount");
  QStringList zinfo = runShellCommand("zpool list -H -o name,altroot");
  QStringList mtpinfo = getMtpfsDevices(); //<number>::::<name>
  //qDebug() << "MTPFS Info:" << mtpinfo;
  //qDebug() << "zpool list:" << zinfo;
  //Verify that the current entries are still valid
  for(int i=0; i<IntMountPoints.length(); i++){
    QString node = IntMountPoints[i].section(DELIM,0,0);
    QString fs = IntMountPoints[i].section(DELIM,1,1).toLower();
    if(!node.isEmpty() && !node.startsWith("/dev/") && fs!="zfs" && fs!="mtpfs"){ node.prepend("/dev/"); }
    QString mntdir = IntMountPoints[i].section(DELIM,2,2);
    //qDebug() << "Check MountPoint:" << node << fs << mntdir;
    bool invalid = false;
    if(fs=="mtpfs"){
      QStringList filter = mtpinfo.filter(node).filter(mntdir.section("/",-1));
      //qDebug() << " - Check MTPFS filter:" << filter;
      if(filter.isEmpty() || !QFile::exists(mntdir) || info.filter(mntdir).isEmpty() ){ invalid = true; }
      else{
	//Already mounted - go ahead and remove it from the list
        for(int i=0; i<filter.length(); i++){ mtpinfo.removeAt( mtpinfo.indexOf(filter[i]) ); }	      
      }
    }
    else if(!node.isEmpty() && ( (!QFile::exists(node) && fs!="zfs") || (fs=="zfs" && zinfo.filter(node).isEmpty()) )){ 
       invalid = true; 
       if( info.filter(mntdir).length()==1  ){ //This is the only entry for this mounttpoint (don't have multiple things mounted on the same dir)
	  //qDebug() << "Unmounting directory:" << mntdir;
         //Device unplugged while it was mounted, unmount it
	  unmountRemDev(mntdir, false, true); //this is an internal change (no user interaction)
	  //Need to be careful about the internal list, since the unmount routine can modify it
	  // better to just restart at the beginning again and stop here
	  updateIntMountPoints();
	  return;
       }
    }
    else if(mntdir.isEmpty()){ invalid = true; } //required for unmounting
    else if( info.filter(mntdir).isEmpty() ){ //not currently listed by "mount"
      //qDebug() << "Mount Dir not listed as active - checking for removal:" << mntdir;
      QDir dir(mntdir);
      if(!dir.exists()){ invalid = true; }
      else if( dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot).length() < 1 && info.filter(mntdir).isEmpty() ){
	invalid = true;
        if( IntMountPoints[i].section(DELIM,4,4)=="canremove" ){
	  //Unmounted some other way - still clean up the old mountpoint
	  dir.rmdir(mntdir);
	}
      }
    }
    if(invalid){
      //Remove this entry from the list
      //qDebug() << "Removing Internal Mount Info:" << IntMountPoints[i];
      IntMountPoints.removeAt(i);
      i--;
    }    
  } //end loop over current DB entries
  
  //Parse the mount info and find anything that was mounted externally
  QStringList fsfilter = DEVDB::knownFilesystems();
  for(int i=0; i<info.length(); i++){
    //filter out unknown filesystems
    QString fs = info[i].section("(",1,1).section(",",0,0);
    if(fs=="msdosfs"){ fs="fat"; } //special catch for an alternate display
    if( !fsfilter.contains(fs.toUpper()) || fs=="zfs" || fs=="fusefs"){ continue; }
    else if( info[i].contains(" /boot/") || info[i].contains("personahome.eli") ){ continue; } //skip any boot devices or home dirs
    QString mpoint = info[i].section(" on ",1,50).section(" (",0,0);
    if(!mpoint.isEmpty() && IntMountPoints.filter(DELIM+mpoint+DELIM).isEmpty()){
      //Externally Mounted device: add it to the internal list
      QString node = info[i].section(" on ",0,0);
        if(!node.startsWith("/dev/")){ node.clear(); } //not a valid device node: just skip this
	else{
	  node.remove("/dev/");
	  //Make sure this is not an active device node (for the currently running OS partition)
	  if(CPART.contains(node)){ continue; }
	}
      IntMountPoints << node+DELIM+fs+DELIM+mpoint+DELIM+""+DELIM+"noremove"+DELIM+"external";
      //qDebug() << "New Internal Mount Info:" << IntMountPoints.last();
    }
  }
  if(passive){ return; } //don't run any mounting routines below
  //Special Check for MTPFS devices (no associated device node - so auto-mount them so they are in the DB uniquely)
  /*for(int i=0; i<mtpinfo.length(); i++){
    //qDebug() << "Check MTPFS info:" << mtpinfo[i] << IntMountPoints.filter("mtpfs");
    mountRemDev(mtpinfo[i].section("::::",0,0), mtpinfo[i].section("::::",1,-1), "mtpfs");
  }*/
  //qDebug() << " - Current IntMountPoints:" << IntMountPoints;
}

void Backend::cleanMediaDir(){
  updateIntMountPoints();
  QDir media("/media");
  QStringList dirs = media.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
  for(int i=0; i<dirs.length(); i++){
    if( !IntMountPoints.contains(DELIM+media.absoluteFilePath(dirs[i])) ){
      //Nothing mounted here (or on a child directory)
      // This will fail if there is anything in the directory (don't bother duplicating Qt checks)
      media.rmdir("/media/"+dirs[i]); 
    }
  }
  
}

QStringList Backend::devChildren(QString dev){
  QDir dir("/dev/");
  return dir.entryList(QStringList() << dev+"*", QDir::System | QDir::Files | QDir::NoDotAndDotDot | QDir::CaseSensitive, QDir::Name);
}

QStringList Backend::findActiveDevices(){
  QStringList activeDevs;
  //Now find any active partitions and ignore it and any children of it
  //NOTE: gpart does not work for root on ZFS, especially with boot blocks on every device for BE-support
  /*QStringList info = runShellCommand("gpart show -p");
  info = info.filter("freebsd").filter("[active]");
  for(int i=0; i<info.length(); i++){
    info[i].remove("=>");
    info[i] = info[i].replace("\t"," ").simplified();
    QString dev = info[i].section(" ",2,2,QString::SectionSkipEmpty);
    activeDevs << dev;
    //Now get any child devices
    activeDevs << devChildren(dev);
  }*/
	
  //Use "zpool status" to get the active ZFS devices/partitions
  QStringList info = runShellCommand("zpool status");
  int start = info.indexOf("config:");
  //qDebug() << "Zpool info start:" << start;
  for(int i = start+1; i<info.length(); i++){
    //qDebug() << "Check Line:" << info[i] << i;
    if(info[i].isEmpty() || (info[i].contains("NAME") && info[i].contains("STATE")) ){ continue; }
    else if(info[i].startsWith("errors:")){ break; } //start of the next section
    info[i].replace("\t"," ");
    QString dev = info[i].section(" ",0,0,QString::SectionSkipEmpty).simplified();
    //qDebug() << "Device:" << dev << info[i];
    if(QFile::exists("/dev/"+dev)){ activeDevs << dev; }
  }
  activeDevs.removeDuplicates();
  //qDebug() << "Active Devices:" << activeDevs;
  return activeDevs;
}

QStringList Backend::listAllRemDev(){
  //Find/list all removable devices connected to the system
  QStringList out = listMountedNodes(); //start with everything currently still mounted
  
  QStringList badlist = DEVDB::invalidDeviceList();
  //qDebug() << "Initial Bad List:" << badlist;
  badlist << getSwapDevices();
  //qDebug() << " - with swap:" << badlist;
  badlist << getPersonaCryptDevices();
  //qDebug() << " - with PC:" << badlist;
  QDir devDir("/dev");
  QStringList subdevs = devDir.entryList(DEVDB::deviceFilter(), QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::System, QDir::NoSort);
  //First look for any ZFS devices that are available to import
  QStringList zinfo = getAvailableZFSPools();
  for(int i=0; i<zinfo.length(); i++){
    out << zinfo[i].section("::::",0,0); //add the pool itself to the available list
    badlist << zinfo[i].section("::::",1,1); //skip any devices associated with this pool later
  }
  //Now filter out any devices associated with currently mounted pools
  badlist << getCurrentZFSDevices();
  //Now look for any MTPFS devices
  QStringList mtpinfo = getMtpfsDevices();
  for(int i=0; i<mtpinfo.length(); i++){
    out << mtpinfo[i].section("::::",0,0);
  }
  out.removeDuplicates(); //since some pools can have multiple devices
  //qDebug() << "Detected Devices:" << subdevs << "\nBad List:" <<badlist;
  //Now scan all the 
  QStringList badmd;
  for(int i=0; i<subdevs.length(); i++){
    //Filter out any devices that are always invalid
    QString base = subdevs[i].section("p",0,0).section("s",0,0); //base device (if needed)
    if(badlist.contains(subdevs[i]) || badlist.contains(base) || out.contains(subdevs[i]) || QFileInfo(subdevs[i]).isSymLink() ){ continue; }
    //Make sure it is not an active partition
    bool ok = true;
    for(int j=0; j<CPART.length(); j++){
      if( subdevs[i].startsWith(CPART[j]) ){ ok = false; break; }
    }
    //Make sure this is a bottom level device for non-memory disks
    if(ok){ 
	if( subdevs[i].startsWith("md") ){
	  //loaded ISO files Memory disks need to be top-level devices:
	  if(subdevs[i].contains("p") || subdevs[i].contains("s") ){ ok = false; break; }
	}else{
          QStringList filter = subdevs.filter(subdevs[i]); 
          for(int f=0; f<filter.length(); f++){ 
            if( filter[f].startsWith(subdevs[i]) && (filter[f]!=subdevs[i]) ){ ok = false; break; }
          }
        }
    }
    //Finally, ensure that there is actually something attached to the device 
    // (existance is not good enough for things like CD drives or USB card readers/hubs)
   if(ok && !subdevs[i].startsWith("md") ){ 
      ok = VerifyDevice("/dev/"+subdevs[i], DEVDB::deviceTypeByNode(subdevs[i]) ); 
      //if(!ok && subdevs[i].startsWith("md") && (subdevs[i].contains("p") || subdevs[i].contains("s")) ){ badmd << subdevs[i]; } //add ot the list for later
   }
    //If ok, add it to the output list
    if(ok){ out << subdevs[i]; }
  }
  //Special check for memory disk device trees - sometimes the bottom-level children are not usable, so we need the top-level device instead
  for(int i=0; i<badmd.length(); i++){
    QString base = badmd[i].section("s",0,0).section("p",0,0);
    bool found = false;
    for(int g=0; g<out.length(); g++){ 
      if(out[g].startsWith(base)){ found = true; break; }
    }
    if(!found && VerifyDevice("/dev/"+base, "ISO") ){
      out << base; //add this base device to the output list (no children usable, but base is usable
    }
  }
  return out;
}

QStringList Backend::getRemDevInfo(QString node, bool skiplabel){
  //Overarching device info retrieval
  //Output: [FILESYSTEM, LABEL, TYPE [, mountpoint] ]
  QStringList out;
  QString fs, label, type;
  if(node.startsWith("/dev/")){ node.remove("/dev/"); }
  
  // - Quick finish if the device is already mounted 
  //  (same info - no need to re-probe the device and cause activity on it)
  updateIntMountPoints();
  QStringList info = IntMountPoints.filter(node);
  for(int i=0; i<info.length(); i++){
    if(info[i].startsWith(node+DELIM)){
      //Already mounted - return the info from the internal database
      fs = info[i].section(DELIM,1,1).toUpper();
      label = info[i].section(DELIM,2,2).section("/",-1); //mountpoint directory name
      QString mtpoint = info[i].section(DELIM, 2,2);
      if(fs.toUpper()=="ZFS"){ type = DEVDB::deviceTypeByNode( getCurrentZFSDevices(node).first() ); }
      else if(fs.toUpper()=="MTPFS"){ type = "USB"; }
      else{ type = DEVDB::deviceTypeByNode(node); }
      if(mtpoint.isEmpty()){
        return (QStringList() << fs << label << type);
      }else{
	return (QStringList() << fs << label << type << mtpoint);
      }
    }
  }
  
  if(!QFile::exists("/dev/"+node)){
    // - Check if this is an available ZFS pool first (instead of a device node)
    QStringList zinfo = getAvailableZFSPools();
    //qDebug() << " - Available ZFS Pools:" << zinfo;
    for(int i=0; i<zinfo.length(); i++){
      if(zinfo[i].startsWith(node+"::::") ){
        //First exact match for the pool - use this device for determining type
        fs = "ZFS"; label = node; 
        type = DEVDB::deviceTypeByNode(zinfo[i].section("::::",1,1) );
        return (QStringList() << fs << label << type); //already have everything necessary
      }else if(zinfo[i].endsWith("::::"+node)){
        //First exact match for a device node - use this pool for output
        fs = "ZFS"; label = zinfo[i].section("::::",0,0);
        type = DEVDB::deviceTypeByNode(node);
        return (QStringList() << fs << label << type); //already have everything necessary
      }
    }
    //Not ZFS pool - check for MTPFS device number
    QStringList mtpinfo = getMtpfsDevices();
    for(int i=0; i<mtpinfo.length(); i++){
      if(mtpinfo[i].section("::::",0,0)==node){
        return (QStringList()<<"MTPFS" << mtpinfo[i].section("::::",1,-1) << "USB");
      }
    }
  }

  
  //Non-ZFS device given - try to figure it out
  //  - Now determine the type by the name of the node (simple/fast)
  type = DEVDB::deviceTypeByNode(node);
  //qDebug() << "Device Type By Node:" << node << type;
  
  //  - Run Disktype
  QStringList dtype = disktypeInfo(node);
  fs = dtype[0];  label = dtype[1];
  if(!dtype[2].isEmpty()){ type = dtype[2]; } //replace type if necessary
  //  - If CD/DVD, try to determine the full type (empty/audio/video/data)
  if(type=="CD"){
	  
    if(fs=="CD9660"){ 
      type = "CD-DATA"; //data CD
    }else if(fs=="UDF"){
      //Could be VIDEO (DVD/BlueRay) or DATA
      QStringList cdinfo = runShellCommand("cd-info --no-cddb --no-device-info  --no-disc-mode --dvd --no-header -q /dev/"+node);
      if(cdinfo.filter("Track List").isEmpty()){ type = "CD-DATA"; }
      else{ type = "CD-VIDEO"; } //either dvd or blueray, no distinction at the moment
    }else{
      //No filesystem, so it must be either nothing or an audio CD
     /*if( QFile::exists("/usr/local/bin/cdparanoia") ){
        //cdparanoia is much better about not disturbing the cd if it is running than cd-info
        QStringList cdinfo = runShellCommand("cdparanoia -Q -d /dev/"+node);
        if( !cdinfo.filter("(audio tracks only)").isEmpty() ){ type = "CD-AUDIO"; }
        else{ type = "CD-NONE"; }	      
      }else{*/
	QStringList cdinfo = runShellCommand("cdcontrol -f "+node+" status audio");
        if( !cdinfo.filter("Audio status =").isEmpty() ){ type = "CD-AUDIO"; }
	else{ type = "CD-NONE"; }
	
	//The cd-info method is rather invasive, and takes a while to read the disk (causing delays in any app currently using it)
        /*QStringList cdinfo = runShellCommand("cd-info -T --no-cddb --no-device-info  --no-disc-mode --dvd --no-header -q /dev/"+node);
        if( !cdinfo.filter("TRACK").filter("1").isEmpty() ){type = "CD-AUDIO"; }
        else{ type = "CD-NONE"; }*/
      //}

    }
  }/*else if( fs.isEmpty() ){
    if(QFile::exists("/usr/local/bin/simple-mtpfs")){
      QStringList info = runShellCommand("simple-mtpfs -l").filter("(MTP)");
      for(int i=0; i<info.length(); i++){
        if( IntMountPoints.filter(info[i].section(" (",0,0)).isEmpty() ){
	  //New 
	}
      }
    }
  }*/
  // - Determine the label using other tools if necessary
  //qDebug() << "End of disktype:" << node << dtype << fs << label << type;
  if(!skiplabel && label.isEmpty()){
    //Run glabel/ntfslabel/other as needed
    if(fs=="NTFS"){
      QStringList lab = runShellCommand("ntfslabel /dev/"+node);
      //qDebug() << "ntfslabel:" << node << lab;
      if(lab.length()==2 && lab[0].startsWith("Failed ") ){ label = lab[2]; } //special catch for Windows 8
      else if(!lab.isEmpty()){ label = lab[0]; }
      //qDebug() << "label:" << label;
    }
    if(label.isEmpty()){
      QStringList labs = runShellCommand("glabel list "+node).filter("Name: ");
      if(labs.length() > 0){
        labs[0] = labs[0].section(":",-1).section("/",-1).simplified();
	if(!labs[0].isEmpty()){ label = labs[0]; }
	
      }
    }
    //Final check for getting a device label: camctl (based on type of device)
    if(label.isEmpty()){
      QString camctl;
      if(type=="SATA"){
        camctl = runShellCommand("camcontrol identify "+node.section("s",0,0) ).join(" ");
      }else if(type=="USB"){
	camctl = runShellCommand("camcontrol inquiry "+node.section("s",0,0) ).join(" ");
      }
      if(camctl.contains("camcontrol")){ camctl.clear(); } //error reported
      if(!camctl.isEmpty()){
	 label = camctl.section(">",0,0).section("<",-1).section(" ",0,0).simplified();
	 if(!label.isEmpty() && node.contains("s")){ label.append("-"+node.section("s",-1)); }
      }
    }
  }
  if(label.isEmpty()){
    label = generateGenericLabel(type); //final option - just create a generic name
  }
  label.replace("%20", " "); //this field code is often not replaced properly in glabel output
  if(fs.isEmpty()){ fs = "NONE"; }
  //Format the output
  out << fs << label << type;
  //qDebug() << " - done:" << out;
  return out;
}

QStringList Backend::disktypeInfo(QString node){
  //Run disktype on the device and return any info
  QStringList info = runShellCommand("disktype /dev/"+node);
  //In case there are multiple partitions on the device, remove all the invalid ones (EFI boot partitions for instance)
  QStringList parts = info.join("\n").split("\nPartition ");
  //qDebug() << "DiskType Partitions:" << node << parts;
  if(parts.filter("Volume name ").length()>0){
    parts = parts.filter("Volume name ");
    if(parts.length()>1 && parts.filter("file system").isEmpty()){ parts = parts.filter("file system"); }
  }
  for(int i=0; i<parts.length(); i++){
    if(parts[i].contains("Partition GUID ") ){ parts.removeAt(i); i--; } //EFI boot partition
  }
  if(!parts.isEmpty()){
    info = parts[0].split("\n"); //only use the first partition with both a label and a file system
  }
  //qDebug() << " - Filtered:" << info;
  //qDebug() << "Disktype Detection:" << node;
  QStringList dsDetection = DEVDB::disktypeDetectionStrings();
  QString bytes, fs, type, label; 
  bool blankDisk=false;
  for(int i=0; i<info.length(); i++){
    //qDebug() << "Line:" << info[i];
    if(info[i].isEmpty() || info[i].startsWith("---")){ continue; } //skip this line
    else if( info[i].startsWith("Character device,") && !info[i].contains("unknown size") ){
      //Get the size if possible
      QString tmp = info[i].section("(",1,1).section(")",0,0);
      if(tmp.contains("bytes")){ bytes = tmp.section(" ",0,0,QString::SectionSkipEmpty); }
      //qDebug() << " - bytes:" << bytes;
    }else if( info[i].contains("Blank disk/medium") ){ 
      blankDisk = true;
      //qDebug() << " - Blank disk";
    }else if( info[i].contains("file system") ){
      if(fs.isEmpty() || !fs.contains("(hints")){
        QString tmp = info[i].section("file system",0,0);
        for(int j=0; j<dsDetection.length(); j++){
          if(tmp.contains(dsDetection[j].section("::::",0,0))){ fs = dsDetection[j].section("::::",1,1); break; }
        }
      }
      //qDebug() << " - File System:" << fs;
    }else if( info[i].contains("Volume name") ){
      label = info[i].section("\"",1,1).section("\"",0,0).simplified(); //name is within quotes
      //qDebug() << " - Label:" << label;
    }
    //stop if all info found (size is always the first to be found in info)
    if(!fs.isEmpty() && !label.isEmpty()){ break; }
  }
  if( (blankDisk || (bytes.toInt()<2049) ) && (node.startsWith("cd") || node.startsWith("acd")) && label.isEmpty() && fs.isEmpty() ){ type = "CD-BLANK"; }
  if( (node.startsWith("cd")||node.startsWith("acd")) && (bytes.isEmpty()) ){ type = "CD-NONE"; }
  //Format the outputs
  //qDebug() << "Results:" << fs << label << type;
  return (QStringList() << fs << label << type);
}

bool Backend::VerifyDevice(QString fulldev, QString type){
  QString info = runShellCommand("file -s "+fulldev).join("");
  if(info.contains( fulldev+": symbolic link to ")){ return false; } //do not allow symbolic links through
  if(type.startsWith("CD") || type=="USB" || type=="SATA" || type=="SD"){
    bool good = info.simplified()!=(fulldev + ": data");
    if(good){ good = !info.contains("(Device not configured)"); } //special output when nothing actually in the tray
    if(good && !type.startsWith("CD")){ good = !info.contains("(Invalid argument)"); } //other output when nothing actually in the tray
    return good; 
  }else if(type=="ISO"){
    return info.simplified()!=(fulldev+": data");
  }else{
    return true; //assume valid for other types of devices 
    //(need to add parsing for new types here at a later date)
  }
}

/*bool Backend::specialFileInfo(QString fulldev, QString *filesystem, QString *label){
//This function will run the "file -s" utility and parses the output
  //  -- it will return "true" if there is a valid size for the device and it is NOT "active"
  //  -- filesystem/label detection is very sketchy with this utility
  filesystem->clear(); label->clear();
  QString info = runShellCommand("file -s "+fulldev).join("").simplified();
  qDebug() << "File Info:" << info;
  //Quick exit if invalid device
  if(info.contains("ERROR:") || info.contains("Device not configured") ){ return false; }
  //Get ready to look for info
  QStringList infoL = info.split(",");
  bool hasblocks = false;
  bool active = false;
  //Check for the filesystem
  //for(int i=0; i<fsDetection.length(); i++){
    //if(info.contains(fsDetection[i])){ filesystem->append( fsMatch[i] ); break; }
  //}
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
}*/

QStringList Backend::listMountedNodes(){
  QStringList out;
  updateIntMountPoints(false); //make sure the internal list is up to date (active check - will mount some devices as needed)
  for(int i=0; i<IntMountPoints.length(); i++){
    QString node = IntMountPoints[i].section(DELIM,0,0);
    if(node.isEmpty()){ node = IntMountPoints[i].section(DELIM,2,2); } //mountpoint instead
    out << node;
  }
  return out;
}

QString Backend::generateGenericLabel(QString type){
  //Generate a generic label name based on the type of device
  if(type=="CD-EMPTY"){ return tr("Blank Disk"); }
  else if(type=="CD-AUDIO"){ return tr("Audio Disk"); }
  else if(type=="CD-VIDEO"){ return tr("Video Disk"); }
  else if(type=="CD-DATA"){ return tr("Data Disk"); }
  else if(type=="SATA"){ return tr("Hard Drive"); }
  else if(type=="USB"){ return tr("USB Device"); }
  else if(type=="SD"){ return tr("SD Card"); }
  else if(type=="ISO"){ return tr("ISO File"); }
  else{ return tr("Unknown Device"); }
}

QString Backend::getDeviceSizeInfo(QString nodedir){
  //can use node *or* mntdir
  updateIntMountPoints();
  QStringList found = IntMountPoints.filter(nodedir+DELIM);
  QString out = "%1/%2 (%3)"; //<used>/<total> (<percent>)
  if(found.length()>0){
    QString dir = found[0].section(DELIM,2,2);
    //This might not work properly for ZFS file systems (FIX LATER)
    QStringList info = runShellCommand("df -h \""+dir+"\"");
    if(info.length()>1){ 
      //first line is the labels (Filesystem, Size, Used, Avail, Capacity, Mounted On)
      info[1] = info[1].replace("\t"," ");
      QStringList stats = info[1].split(" ", QString::SkipEmptyParts);
      if(stats.length() >= 5){
        out = out.arg(stats[2], stats[1], stats[4]);
      }else{
        out = out.arg("??","??","??");
      }
    }else{
      out = out.arg("??","??","??");
    }
  }else{
    out = out.arg("??","??","??");
  }
  return out;
}

QStringList Backend::getUsableFileSystems(){
  QStringList out, fslist;
  fslist = DEVDB::knownFilesystems();
  for(int i=0; i<fslist.length(); i++){
    if(DEVDB::isFSSupported(fslist[i])){ out << fslist[i]; }
  }
  return out;
}

QStringList Backend::getSwapDevices(){
  //Use "mdconfig" to filter out any SWAP devices
  QStringList info = runShellCommand("mdconfig -l -v").filter("/swap");
  QStringList devs;
  for(int i=0; i<info.length(); i++){
    info[i].replace("\t", " ");
    devs << info[i].section(" ",0,0).simplified();
  }
  //Also check for EFI/SWAP partitions with glabel
  info = runShellCommand("glabel status");
  for(int i=1; i<info.length(); i++){
    info[i].replace("\t"," ");
    QString lab = info[i].section(" ",0,0);
    if(lab.contains("label/efi") || lab.contains("/swap") ){
      devs << info[i].section(" ",2,2,QString::SectionSkipEmpty);
    }
  }
  devs.removeDuplicates();
  return devs;
}

QStringList Backend::getPersonaCryptDevices(){
  QStringList info = runShellCommand("personacrypt list");
  QStringList devs;
  for(int i=0; i<info.length(); i++){
    if(info[i].contains(":") && QFile::exists("/dev/"+info[i].section(":",0,0)) ){
      devs << info[i].section(":",0,0);
    }
  }
  return devs;
}

QStringList Backend::getMtpfsDevices(){
  //output item: <device number>::::<device name>
  if(!QFile::exists("/usr/local/bin/simple-mtpfs")){ return QStringList(); }
  QStringList info = runShellCommand("simple-mtpfs -l").filter(":");
  for(int i=0; i<info.length(); i++){
    info[i] = info[i].section(":",0,0)+"::::"+info[i].section(":",1,-1).section("(",0,0).simplified();
  }
  return info;
}

QStringList Backend::getAvailableZFSPools(){
  // Output format: <pool name>::::<device node>
  //Seperate them by different pools
  QStringList info = runShellCommand("zpool import").join("<br>").replace("\t"," ").split(" pool: ");
  QStringList output;
  for(int i=0; i<info.length(); i++){
     //Now look at the info for this particular pool
     QStringList pinfo = info[i].split("<br>");
     if(pinfo.length() < 5){ continue; } //not a full pool info (at least 5 lines - [pool:, id:, state:, action:, config:], usually more)
     QString pool = pinfo[0].simplified(); //this is how we divided it up earlier, with "pool:" as the first line
     pinfo = pinfo.filter("ONLINE"); //only look for available (ONLINE) pools to import
     for(int j=0; j<pinfo.length(); j++){
       if(pinfo[j].contains("state: ONLINE")){ continue; } //skip this line
       QString dev = pinfo[j].section("ONLINE",0,0).simplified();
	if(dev!=pool && QFile::exists("/dev/"+dev) ){
	  output << pool+"::::"+dev;
	}
    }
  }
  return output;
}

QStringList Backend::getCurrentZFSPools(){
  //This just returns the names of all the currently used ZFS pools
  QStringList pools = runShellCommand("zpool list -H -o name");
  return pools;
}

QStringList Backend::getCurrentZFSDevices(QString pool){
  //This lists all the raw devices associated with mounted zpools
  //Seperate them by different pools
  QStringList info = runShellCommand("zpool status "+pool).join("<br>").replace("\t"," ").split(" pool: ");
  QStringList output;
  for(int i=0; i<info.length(); i++){
     //Now look at the info for this particular pool
     QStringList pinfo = info[i].split("<br>");
     if(pinfo.length() < 5){ continue; } //not a full pool info (at least 5 lines - [pool:, id:, state:, action:, config:], usually more)
     QString pool = pinfo[0].simplified(); //this is how we divided it up earlier, with "pool:" as the first line
     pinfo = pinfo.filter("ONLINE"); //only look for available (ONLINE) pools to import
     for(int j=0; j<pinfo.length(); j++){
       if(pinfo[j].contains("state: ONLINE")){ continue; } //skip this line
       QString dev = pinfo[j].section("ONLINE",0,0).simplified();
	if(dev!=pool && QFile::exists("/dev/"+dev) ){
	  output << dev;
	}
    }
  }
  return output;
}

QString Backend::mountRemDev(QString node, QString mntdir, QString fs){
  //See if we need to probe the device here and adjust inputs
  if(fs.toLower()=="none" || fs.toLower()=="auto"){ fs.clear(); } //special input flags
  if(mntdir.isEmpty() || fs.isEmpty()){
    QStringList info = getRemDevInfo(node,!mntdir.isEmpty()); //<fs>,<label>,<type>
    //qDebug() << "Detected Info:" << info;
    if(info.length() > 2){ 
      if(fs.isEmpty()){ fs = info[0]; }
      if(fs.isEmpty() && info[2].startsWith("CD-")){ fs = "CD9660"; }
      if(mntdir.isEmpty()){
	if(info[1].isEmpty()){
	  //no label either, give it a generic label based on it's type
	  info[1] = generateGenericLabel(info[2]);
	}
        //Assign a mountpoint for the device based on it's label
	QString tmp = "/media/"+info[1]; //put it in this base directory
	//Make sure to verify that the mountpoint does not already exist
	if(QFile::exists(tmp)){
	  int i=2; //this would technically be the second device with this label
	  while( QFile::exists(tmp+"-"+QString::number(i)) ){ i++; }
	  tmp = tmp+"-"+QString::number(i);
	}
	mntdir = tmp;
      }
    } //info length check
  }
  
  //Final check for valid inputs/detections
  if(!node.startsWith("/dev/") && fs.toLower()!="zfs" && fs.toLower()!="mtpfs"){ node.prepend("/dev/"); }
  if(fs.isEmpty() || fs.toLower()=="none"){ return ("[ERROR-0] No filesystem detected -- "+node); }
  if(!mntdir.startsWith("/")){ mntdir.prepend("/media/"); }
  //qDebug() << "Mount Device:" << node << fs << mntdir;
  // - mntdir will always be auto-created as necessary
  QDir dir(mntdir);
  //Verify Inputs
  if(!QFile::exists(node) && fs.toLower()!="zfs" && fs.toLower()!="mtpfs"){ return ("[ERROR-1] Invalid device node -- "+node); }
  else if(!DEVDB::isFSSupported(fs)){ return ("[ERROR-2] Filesystem not supported -- "+fs); }
  else if(dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot).length() > 0){ return ("[ERROR-3] Mount point already in use -- "+mntdir); }
  
  //Create the directory if necessary
  bool dircreated = false;
  if(fs.toLower()=="zfs"){ dircreated = false; } //ZFS automatically handles the dir creation
  else if(!dir.exists()){
    dircreated = dir.mkpath(mntdir);
    if(!dircreated){ return ("[ERROR-4] Mount point could not be created -- "+mntdir); }
  }

  //Now get the mounting commands for the device
  QStringList cmds = DEVDB::MountCmdsForFS(fs, CLOCALE!="en_US");
    //if(fs.toLower()=="mtpfs"){ qDebug() << "Mount Commands:" << cmds; }
  bool usedlocale = cmds.join(" ").contains("%3");
  //Mount the device
  bool ok = true;
  bool done = false;
  QString errline;
  QString basedir = mntdir.section("/",0,-2);
  QProcess proc;
    proc.setProcessChannelMode(QProcess::MergedChannels);
  while(!done){
    for(int i=0; i<cmds.length() && ok; i++){
      //qDebug() << "Mountpoint:" << mntdir << basedir << mntdir.section("/",-1);
      cmds[i].replace("%1", node).replace("%2a", "\""+basedir+"\"").replace("%2b","\""+mntdir.section("/",-1)+"\"").replace("%2", "\""+mntdir+"\"").replace("%3", CLOCALE+".UTF-8");
      if(cmds[i].contains("%4")){
        cmds[i].replace("%4", runShellCommand("id -u "+CUSER).join("").simplified() );	    
      }
      if(cmds[i].contains("%5")){
        cmds[i].replace("%5", runShellCommand("id -g operator").join("").simplified() );
      }
      proc.start(cmds[i]);
      proc.waitForFinished(-1);
      ok = ( 0==proc.exitCode() ); //look for a return code of 0 for success for the command
      //if(fs.toLower()=="mtpfs"){ qDebug() << "Mount Command:" << cmds[i] << ok; }
      if(!ok){ errline = " -- on command: "+cmds[i]+" ("+proc.readAllStandardOutput()+")"; }
    }
    //Check for success and use any fallback methods
    if(!ok && usedlocale){
      //Failure with custom locale - try it without the localization flags
      ok = true; usedlocale = false; //reset flags for next run
      cmds = DEVDB::MountCmdsForFS(fs, false);
      errline.clear();
    }else{
      done = true;
    }
  } //end with "done" loop
  if( !ok ){
    //Error mounting the device
    dir.rmpath(mntdir); //clean up the unused mountpoint that was just created
    return ("[ERROR-5] Device could not be mounted -- "+node+" -- "+fs+" -- "+mntdir+errline);
  }
  
  //Now set permissions on the mount point (if just created)
  // NOTE: this chown usage is *NOT* recursive
  if(dircreated){
    //Set 777 permissions
    QFile::setPermissions(mntdir, QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner | QFile::ReadUser | QFile::WriteUser | QFile::ExeUser | QFile::ReadGroup | QFile::WriteGroup | QFile::ExeGroup | QFile::ReadOther | QFile::WriteOther | QFile::ExeOther );
    //Set the given user as the owner
    if(!CUSER.isEmpty()){ runShellCommand("chown "+CUSER+":operator \""+mntdir+"\""); }
  }
  
  //Now save this entry internally [node, filesystem, mntdir, user, (canremove/noremove), internal]
  node.remove("/dev/"); //only save the node
  IntMountPoints << node+DELIM+fs+DELIM+mntdir+DELIM+CUSER+DELIM+(dircreated ? "canremove": "noremove")+DELIM+"internal";
  return ("[SUCCESS] "+mntdir);
}

QString Backend::unmountRemDev(QString nodedir, bool force, bool internal){
  //can use node *or* mntdir
  if(nodedir.startsWith("/dev/")){ nodedir = nodedir.section("/dev/",-1); }
  if(!internal){ updateIntMountPoints(); }
  QStringList found = IntMountPoints.filter(nodedir+DELIM);
  if(QFile::exists("/dev/"+nodedir)){
    //node given
    if(nodedir.startsWith("md")){ 
      //memory disk, might have a different node in the mounting log (get all entries)
      found.clear(); //going to rebuild this list with a different criteria
      QString shortnode = nodedir.section("p",0,0).section("s",0,0).simplified();
      for(int i=0; i<IntMountPoints.length(); i++){
	if(IntMountPoints[i].startsWith(nodedir+DELIM)){
	  found.prepend(IntMountPoints[i]); //exact match, put it at the top
        }else if(IntMountPoints[i].startsWith(shortnode)){
	  found << IntMountPoints[i]; //partial match (other partition/slice is mounted)
	}
      }
    }
  }
  QString mntdir, fs, user, dev;
  bool rmdir = false;
  if(found.length()>0){
    dev=found[0].section(DELIM,0,0);
    mntdir=found[0].section(DELIM,2,2);
    rmdir = (found[0].section(DELIM,4,4)=="canremove");
    fs = found[0].section(DELIM,1,1);
    user = found[0].section(DELIM, 3,3);
  }else if(QFile::exists(nodedir)){
    mntdir = nodedir;
    rmdir = false; //not internally managed
  }
  if(mntdir.isEmpty()){ return ("[ERROR] Inputs Invalid: "+nodedir); }
  //Unmount the device
  QStringList cmds = DEVDB::UnmountCmdsForFS(fs, force);
  bool ok = true;
  QString errline;
  QString basedir = mntdir.section("/",0,-2);
  for(int i=0; i<cmds.length() && ok; i++){
    cmds[i].replace("%1", "/dev/"+dev).replace("%2a", "\""+basedir+"\"").replace("%2b","\""+mntdir.section("/",-1)+"\"").replace("%2", "\""+mntdir+"\"").replace("%3", CLOCALE);
    if(cmds[i].contains("%4")){
      cmds[i].replace("%4", runShellCommand("id -u "+CUSER).join("").simplified() );	    
    }
    if(cmds[i].contains("%5")){
      cmds[i].replace("%5", runShellCommand("id -g operator").join("").simplified() );
    }
    ok = ( 0==QProcess::execute(cmds[i]) ); //return code of 0 means success
    if(!ok){ errline = "[ERROR] Command Run: "+cmds[i]; }
  }
  if(!ok && internal){
    //force it anyway
    return unmountRemDev(nodedir, true, internal);
  }
  if( !ok ){
    //Error unmounting the device
    if(!errline.isEmpty()){ return errline; }
    else{ return "[ERROR]"; }
  }
  //Remove the mount point (if necessary)
  if(rmdir){
    QDir dir;
    dir.rmdir(mntdir);
    if(DEVDB::deviceTypeByNode(dev)=="ISO" && found.length()==1){
      //Also unload the memory disk if the last partition/slice was just unmounted
      // NOTE: this only works with the main md device number (not partitions/slices)
      QProcess::execute("mdconfig -d -u "+dev.section("md",-1).section("p",0,0).section("s",0,0).simplified() );
    }
  }
  //Now remove that entry from the internal list
  IntMountPoints.removeAll(found[0]);
  return "[SUCCESS]";
}

QString Backend::createMemoryDiskFromISO(QString isoFile){
  //Verify the input file name/path
  if(!QFile::exists(isoFile) || !isoFile.endsWith(".iso") ){ return "[ERROR] Invalid Input ISO file"; }
  //Now find the first available MD device number
  int num = 1;
  while(QFile::exists("/dev/md"+QString::number(num)) ){ num++; }
  //Create/run the command to create the memory disk
  QString cmd = "mdconfig -a -f \""+isoFile+"\" -u "+QString::number(num);
  bool ok = (0==QProcess::execute(cmd) );
  if(!ok){ return ("[ERROR] Could not create memory disk:" + QString::number(num)+",  "+isoFile); }
  else{ return "[SUCCESS]"; }
}
