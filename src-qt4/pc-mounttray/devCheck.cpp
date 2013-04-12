#include <pcbsd-utils.h>
#include "devCheck.h"


DevCheck::DevCheck(){
  //Initialize the lists of valid device types
  validDevs << "da" << "ad" << "mmcsd" << "cd" << "acd";
  validDevTypes << "USB" << "SATA" << "SD" << "CD9660" << "CD9660";
  for(int i=0; i<validDevs.length(); i++){
    devFilter << validDevs[i]+"*";
  }
  //Initialize lists of filesystems and detection strings
  fsDetection.clear();
  fsMatch.clear();
  fsDetection << "FAT" << "NTFS" << "EXT" << "ISO 9660" << "Unix Fast File system" << "Reiser" << "XFS"; //string to match for a particular filesystem
  fsMatch << "FAT" << "NTFS" << "EXT" << "CD9660" << "UFS" << "REISERFS" << "XFS"; //internal labels for the filesystems
  fsFilter << "fat" << "ntfs" << "ext" << "cdrom" << "ufs" << "reiser" << "xfs"; //label categories in /dev/
  //Initialize the device directory
  devDir = QDir(DEVICEDIR);
}

DevCheck::~DevCheck(){
}

/*
  PUBLIC FUNCTIONS
*/

bool DevCheck::isValid(QString node){
  bool ok = FALSE;
  for(int i=0; i<validDevs.length(); i++){
    if(node.startsWith(validDevs[i])){ 
	ok = TRUE; 
	break; 
    }
  }
  return ok;
}

QStringList DevCheck::devChildren(QString node){
  devDir.cd(DEVICEDIR);
  QStringList subdevs;
  if(node.isEmpty()){
    subdevs = devDir.entryList(devFilter, QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::System, QDir::NoSort);
  }else{
    subdevs = devDir.entryList(QStringList(node+"*"), QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::System, QDir::NoSort);
  }
  //Clean up the list of children
  for(int i=0; i<subdevs.length(); i++){
    if( (subdevs[i] == node) || subdevs[i].isEmpty() ){
      subdevs.removeAt(i);
      i--;
    }
  }
  return subdevs;
}

QString DevCheck::devLabel(QString node, QString filesystem){
  QString dlabel;
  QStringList glout = Utils::runShellCommand("glabel list");
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
  if(!QFile::symLinkTarget(fullDev).isEmpty()){ return FALSE; }
  
  //Double check for valid device types (just in case)
  QString detType;
  for(int i=0; i<validDevs.length(); i++){
    if(node.startsWith(validDevs[i])){ 
	detType = validDevTypes[i]; 
	break; 
    }
  }
  
  //Make sure we quit before running commands on any invalid device nodes
  if(detType.isEmpty() || !QFile::exists(fullDev) ){return FALSE;}
  else{type->append(detType);}
  bool isCD=FALSE;
  if(detType == "CD9660"){ isCD=TRUE; }
  
  //Read the Device Info using "file -s <device>"
  QString cmd = "file -s "+fullDev;
  QString output = Utils::runShellCommand(cmd).join(" ");
  //if(isCD){ qDebug() << "File -s output:" << output; }
  
  // ** Get the max storage size **
  int kb = 0;
  bool hasPartitions = FALSE; 
  bool isMounted = FALSE;
  if( !isCD ){
    QStringList tmp = output.split(",");
    if( !tmp.filter("partition ").isEmpty() ){
      //Check for actual sub-devices (*s[#][a/b/c/....])
      if( devChildren(node).length() > 1 ){ hasPartitions = TRUE; } //the main device will always make it length 1
    }
    if( !tmp.filter("last mounted on /").isEmpty() && (detType == "SATA")){
      isMounted = TRUE;
    }
    //Now try to get the size of the disk
    if( !tmp.filter("number of data blocks").isEmpty() ){
      tmp = tmp.filter("number of data blocks");
      kb = tmp[0].remove("number of data blocks").simplified().toInt();
    }else if( !tmp.filter("number of blocks").isEmpty() ){
      tmp = tmp.filter("number of blocks");
      kb = tmp[0].remove("number of blocks").simplified().toInt();
    }else if( !tmp.filter("hidden sectors").isEmpty()){
      tmp = tmp.filter("hidden sectors");
      kb = tmp[0].remove("hidden sectors").simplified().toInt();
    }else if( !tmp.filter("sectors").isEmpty()){
      tmp = tmp.filter("sectors");
      //qDebug() << "Det Sectors line:"<<tmp;
      int num=0;
      for(int i=0; i<tmp.length(); i++){
        int n = tmp[i].remove("sectors").section("(",0,0).simplified().toInt();
        if(n > num){ num = n; }
      }
      kb = num;
    }else{ kb = -1; }
  } //end non-CD size and isMounted detection
  bool oksize = (kb > 0);
  //First try to get the device label using the "file -s" output
  QString dlabel;
  if(isCD){
    if( !output.contains("ERROR:") ){
      dlabel = output.section("'",-2).remove("'").simplified();
      //qDebug() << dlabel;
      //while( dlabel.endsWith(" ") || dlabel.endsWith("\n") ){ dlabel.chop(1); }
      //qDebug() << dlabel;
    }
  }else{
   dlabel = output.section("label: \"",1,1).section("\"",0,0).simplified(); //device name
  }
  // - trim the label out of the output line for filesystem type detection
  QString devFSsec, devFSsec2;
  if(!isCD){
    devFSsec = output.section("label:",0,0);
    devFSsec2 = output.section("label:",1,3).section(",",1,1,QString::SectionSkipEmpty);
  }else{
    devFSsec = output.simplified();	  
  }
  // ** Find the filesystem type for the device **
  QString filesys;
  for(int i=0; i<fsDetection.length(); i++){
    if(devFSsec.contains(fsDetection[i]) || devFSsec2.contains(fsDetection[i]) ){
      filesys = fsMatch[i]; 
    }
  }
  //If the filesystem could not be detected or is not supported
  bool hasFS = TRUE;
  if(filesys.isEmpty()){ filesys = "UNKNOWN"; hasFS=FALSE; }
  
  //Now get the device label (if there is one) using glabel
  bool hasLabel = FALSE;
  QString glabel;
  if(!isCD){ glabel = devLabel(node, filesys); }
  //Check to see if we have a label, otherwise assign one
  if( !glabel.isEmpty() ){ dlabel = glabel; hasLabel = TRUE; } //glabel
  else if(!dlabel.isEmpty()){ hasLabel = TRUE; } //file -s label
  else{
    //Assign a device label
    if(isCD){
      dlabel = "Optical_Disk";
    }else{
      dlabel = detType+"-Device"; //this is not a "detected" label
    }
  }
  
  //Now perform the final checks to see if it is a good device
  bool good = FALSE;
  if( isMounted ){}//Always ignore this device (local FreeBSD installation that is being used)
  else if( hasPartitions ){} //Ignore devices that have partitions accessible as seperate devices
  else if( hasFS && isCD ){ good = TRUE; } //CD/DVD data disks don't have as much info
  //Allow devices that match 2 of the 3 criteria
  else if( hasFS && oksize ){ good = TRUE; } //This will catch most good devices
  else if( hasLabel && oksize ){ good = TRUE; } //allow unknown filesystems if there is a good size reading
  else if( hasFS && hasLabel ){ good = TRUE; } // allow device if it has a known label and filesystem
  
  //Now setup the outputs as appropriate
  maxsize->append( QString::number(kb) );
  label->append(dlabel);
  filesystem->append(filesys);
  
  if(!good){
    qDebug() << "Invalid Device:" << node << detType << dlabel << filesys << kb; 
    if(DEBUG_MODE){qDebug() << " -Detected Flags:" << isMounted << hasPartitions << hasLabel << hasFS << oksize;} 
  }
  return good;
}

/*
  PRIVATE FUNCTIONS
*/


