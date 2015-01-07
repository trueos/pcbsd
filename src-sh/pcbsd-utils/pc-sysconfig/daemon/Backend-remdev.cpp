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



//REMOVABLE DEVICES (remdev)
void Backend::updateIntMountPoints(){
  //Format: 
	
  //First run "mount" to make sure and get the complete list of mounted devices
  QStringList info = runShellCommand("mount");
  //Verify that the current entries are still valid
  for(int i=0; i<IntMountPoints.length(); i++){
    QString node = IntMountPoints[i].section(DELIM,0,0);
    if(!node.isEmpty() && !node.startsWith("/dev/")){ node.prepend("/dev/"); }
    QString mntdir = IntMountPoints[i].section(DELIM,2,2);
    bool invalid = false;
    if(!node.isEmpty() && !QFile::exists(node)){ invalid = true; }
    else if(mntdir.isEmpty()){ invalid = true; } //required for unmounting
    else{
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
      IntMountPoints.removeAt(i);
      i--;
    }    
  }
  
  //Parse the mount info and find anything that was mounted externally
  QStringList fsfilter = DEVDB::knownFilesystems();
  for(int i=0; i<info.length(); i++){
    //filter out unknown filesystems
    QString fs = info[i].section("(",1,1).section(",",0,0);
    if( !fsfilter.contains(fs.toUpper()) ){ continue; }
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
  qDebug() << "Active Devices:" << activeDevs;
  return activeDevs;
}

QStringList Backend::listAllRemDev(){
  //Find/list all removable devices connected to the system
  QStringList out;
  QStringList badlist = DEVDB::invalidDeviceList();
  QDir devDir("/dev");
  QStringList subdevs = devDir.entryList(DEVDB::deviceFilter(), QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::System, QDir::NoSort);
  qDebug() << "Detected Devices:" << subdevs;
  for(int i=0; i<subdevs.length(); i++){
    //Filter out any devices that are always invalid
    if(badlist.contains(subdevs[i])){ continue; }
    //Make sure it is not an active partition
    bool ok = true;
    for(int j=0; j<CPART.length(); j++){
      if( subdevs[i].startsWith(CPART[j]) ){ ok = false; break; }
    }
    //Make sure this is a bottom-level device
    if(ok){ 
      QStringList filter = subdevs.filter(subdevs[i]); 
      for(int f=0; f<filter.length(); f++){ 
        if( filter[f].startsWith(subdevs[i]) && (filter[f]!=subdevs[i]) ){ ok = false; break; }
      }
    }
   
    //If ok, add it to the output list
    if(ok){ out << subdevs[i]; }
  }
  return out;
}

QStringList Backend::getRemDevInfo(QString node, bool skiplabel){
  //Overarching device info retrieval
  //Output: [FILESYSTEM, LABEL, TYPE]
  QStringList out;
  QString fs, label, type;
  //  - First determine the type by the name of the node (simple/fast)
  type = DEVDB::deviceTypeByNode(node);
  //qDebug() << "Device Type By Node:" << node << type;
  //  - Run "File -s" and determine if it is a valid device (and anything else that we can)
  //bool isok = specialFileInfo("/dev/"+node, &fs, &label);
  //qDebug() << " - Special File Info:" << isok << fs << label;
  //if(!isok){ return QStringList() << "" << "" << ""; } //invalid device (not connected?)
  //  - Run Disktype
  QStringList dtype = disktypeInfo(node);
  fs = dtype[0];  label = dtype[1];
  if(!dtype[2].isEmpty()){ type = dtype[2]; } //replace type if necessary
  //  - If CD/DVD, try to determine the full type (empty/audio/video/
  if(type=="CD" && fs.isEmpty()){
    //Run  cdcontrol (or cd-info) to determine the type of CD/DVD
	  
  }
  // - Determine the label if necessary
  if(!skiplabel){
    //Run glabel/ntfslabel/other as needed
    if(fs=="NTFS"){
      QString lab = runShellCommand("ntfslabel /dev/"+node).join("");
      if(!lab.isEmpty()){ label = lab; }
    }else{
      QStringList labs = runShellCommand("glabel list "+node).filter("Name: ");
      if(labs.length() > 0){
        label = labs[0].section(":",-1).section("/",-1).simplified();
	label.replace("%20", " "); //this field code is often not replaced properly in glabel output
      }
    }
  }
  //Format the output
  out << fs << label << type;
  return out;
}

QStringList Backend::disktypeInfo(QString node){
  //Run disktype on the device and return any info
  QStringList info = runShellCommand("disktype /dev/"+node);
  QStringList dsDetection = DEVDB::disktypeDetectionStrings();
  QString bytes, fs, type, label; 
  bool blankDisk=false;
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
        if(tmp.contains(dsDetection[j].section("::::",0,0))){ fs = dsDetection[j].section("::::",1,1); break; }
      }
    }else if( info[i].contains("Volume name") ){
      label = info[i].section("\"",1,1).section("\"",0,0).simplified(); //name is within quotes
    }
    //stop if all info found (size is always the first to be found in info)
    if(!fs.isEmpty() && !label.isEmpty()){ break; }
  }
  if(blankDisk && (node.startsWith("cd") || node.startsWith("acd")) ){ type = "CD-Empty"; }
  //Format the outputs
  return (QStringList() << fs << label << type);
}

bool Backend::specialFileInfo(QString fulldev, QString *filesystem, QString *label){
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
}

QStringList Backend::listMountedNodes(){
  QStringList out;
  for(int i=0; i<IntMountPoints.length(); i++){
    out << IntMountPoints[i].section(DELIM,0,0);
  }
  return out;
}

QString Backend::mountRemDev(QString node, QString mntdir, QString fs, QString username, QString locale){
  //Adjust inputs (as needed)
  if(!node.startsWith("/dev/")){ node.prepend("/dev/"); }
  if(fs.isEmpty() || fs.toLower()=="auto"){
    //automatically determine the filesystem to mount (not implemented yet)
    QStringList info = getRemDevInfo(node,true);
    if(info.length() > 2){ 
      if(!fs.isEmpty()){ fs = info[0]; }
      else if(fs.startsWith("CD-")){ fs = "CD9660"; } //try this since mount requested
      else{ return ("[ERROR-0] No filesystem detected -- "+node); }
    }
  }
  QDir dir(mntdir);
  //Verify Inputs
  if(!QFile::exists(node)){ return ("[ERROR-1] Invalid device node -- "+node); }
  else if(!DEVDB::isFSSupported(fs)){ return ("[ERROR-2] Filesystem not supported -- "+fs); }
  else if(dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot).length() > 0){ return ("[ERROR-3] Mount point already in use -- "+mntdir); }
  
  //Now get the mounting command for the device
  QStringList cmds = DEVDB::MountCmdsForFS(fs);
  //Replace any special field flags in the commands
  for(int i=0; i<cmds.length(); i++){
    if(cmds[i].contains("%3")){ cmds[i] = cmds[i].arg(node, mntdir, locale); }
    else{ cmds[i] = cmds[i].arg(node, mntdir); }
  }
  
  //Create the directory if necessary
  bool dircreated = false;
  if(!dir.exists()){
    dircreated = dir.mkpath(mntdir);
    if(!dircreated){ return ("[ERROR-4] Mount point could not be created -- "+mntdir); }
  }
  //Now set permissions on the mount point (if just created)
  if(dircreated){
    //Set 777 permissions
    QFile::setPermissions(mntdir, QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner | QFile::ReadUser | QFile::WriteUser | QFile::ExeUser | QFile::ReadGroup | QFile::WriteGroup | QFile::ExeGroup | QFile::ReadOther | QFile::WriteOther | QFile::ExeOther );
    //Set the given user as the owner
    if(!CUSER.isEmpty()){ runShellCommand("chown "+username+":operator "+mntdir); }
  }
  
  //Mount the device
  bool ok = true;
  for(int i=0; i<cmds.length() && ok; i++){
    ok = !(runShellCommand(cmds[i]).length() > 0);
  }
  if( !ok ){
    //Error mounting the device
    dir.rmpath(mntdir); //clean up the unused mountpoint that was just created
    return ("[ERROR-5] Device could not be mounted -- "+node+" -- "+fs+" -- "+mntdir);
  }
  
  //Now save this entry internally [node, filesystem, mntdir, user, (canremove/noremove), internal]
  node.remove("/dev/"); //only save the node
  IntMountPoints << node+DELIM+fs+DELIM+mntdir+DELIM+CUSER+DELIM+(dircreated ? "canremove": "noremove")+DELIM+"internal";
  return "[SUCCESS]";
}

bool Backend::unmountRemDev(QString nodedir){
  //can use node *or* mntdir
  updateIntMountPoints();
  QStringList found = IntMountPoints.filter(nodedir+DELIM);
  QString mntdir, fs, user;
  bool rmdir = false;
  if(found.length()>0){
    mntdir=found[0].section(DELIM,2,2);
    rmdir = (found[0].section(DELIM,4,4)=="canremove");
    fs = found[0].section(DELIM,1,1);
    user = found[0].section(DELIM, 3,3);
  }
  if(mntdir.isEmpty()){ return false; }
  //Unmount the device
  QStringList cmds = DEVDB::UnmountCmdsForFS(fs);
  bool ok = true;
  for(int i=0; i<cmds.length() && ok; i++){
    cmds[i].replace("%1", user).replace("%2", mntdir);
    ok = !(runShellCommand(cmds[i]).length() > 0);
  }
  if( !ok ){
    //Error unmounting the device
    return false;
  }
  //Remove the mount point (if necessary)
  if(rmdir){
    QDir dir;
    dir.rmdir(mntdir);
  }
  //Now remove that entry from the internal list
  IntMountPoints.removeAll(found[0]);
  return true;
}
