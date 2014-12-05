/***************************************************************************
 *   Copyright (C) 2011 by Ken Moore 					   *
 *   ken@pcbsd.org  							   *
 *                                                                         *
 *   Permission is hereby granted, free of charge, to any person obtaining *
 *   a copy of this software and associated documentation files (the       *
 *   "Software"), to deal in the Software without restriction, including   *
 *   without limitation the rights to use, copy, modify, merge, publish,   *
 *   distribute, sublicense, and/or sell copies of the Software, and to    *
 *   permit persons to whom the Software is furnished to do so, subject to *
 *   the following conditions:                                             *
 *                                                                         *
 *   The above copyright notice and this permission notice shall be        *
 *   included in all copies or substantial portions of the Software.       *
 *                                                                         *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/

#include <stdlib.h>

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>

#include "pcbsd-utils.h"
#include "pcbsd-hardware.h"

using namespace pcbsd;

// --------- BLUETOOTH HARDWARE ----------
QStringList Hardware::getBTdevNames(){
  //This function returns a list of: [localName,deviceName] (i.e. [MyPCBSD,ubt0] )
  QString devName;
  QString localName;
  QStringList tmpL = Utils::runShellCommand("hccontrol Read_Local_Name");
  QString chk = tmpL[0].section(":",0,0,QString::SectionSkipEmpty);
  if(chk=="hccontrol"){
    devName = "No Bluetooth device connected";
    localName = "Error:";
  }else{
    QString names = tmpL[0].section(":",1,1,QString::SectionSkipEmpty);
    localName = names.section("(",0,0,QString::SectionSkipEmpty);
    devName = names.section("(",1,1,QString::SectionSkipEmpty);
    devName = devName.section(")",0,0,QString::SectionSkipEmpty);
  }
  
  QStringList btNames;
  btNames << localName.simplified() << devName.simplified();
  return btNames;
}

void Hardware::setBTdevName(QString newName, QString devName){
  QString sysname = newName + " ("+devName+")";
  QString cmd = "hccontrol change_local_name \""+sysname+"\"";
  Utils::runShellCommand(cmd);
  return;
}

void Hardware::addBTdevice(QString bdaddr, QString name, QString key, QString pin){
  //Check inputs and format them properly
  if(bdaddr.isEmpty() ){return; } //do nothing if no bdaddr given
  
  if(name.isEmpty() ){ //if no name given, scan the device and get its name
    name = getBTRemoteName(bdaddr);
  }
  name = "\""+name+"\""; //put quotes around the name  
  
  if(key.isEmpty() ){ key="nokey";} //if no key, set to proper value
  else{ key = "\""+key+"\"";} //put quotes around given key

  if(pin.isEmpty() ){ pin="nopin";} //if no pin, set to proper value
  else{ pin = "\""+pin+"\"";} //put quotes around given pin
  


  //check to make sure the bdaddr is not already added
  bool newDevice = false;  
  QString cmd = "grep "+bdaddr+" /etc/bluetooth/hcsecd.conf";
  QStringList chk = Utils::runShellCommand(cmd);
  if( chk.isEmpty() ){newDevice = true;}
  
  //Remove the device entry if it already exists
  if(!newDevice){ rmBTdevice(bdaddr,false); }
  
  //copy the hcsecd.conf file for writing
  Utils::runShellCommand("cp /etc/bluetooth/hcsecd.conf /etc/bluetooth/hcsecd.conf.tmp");
  //Add to the temporary file
  QFile fileout( "/etc/bluetooth/hcsecd.conf.tmp" );
  if( fileout.open( QIODevice::Append ) ){
    QTextStream streamout( &fileout );
    streamout << "\ndevice {\n";
    streamout << " bdaddr  "+bdaddr+";\n";
    streamout << " name    "+name+";\n";
    streamout << " key     "+key+";\n";
    streamout << " pin     "+pin+";\n";
    streamout << "}\n";
    fileout.close();
  }

  //overwrite the original hcsecd.conf file since writing is finished
  Utils::runShellCommand("mv /etc/bluetooth/hcsecd.conf.tmp /etc/bluetooth/hcsecd.conf");
  //Set the proper permissions again
  Utils::runShellCommand("chmod 600 /etc/bluetooth/hcsecd.conf");  
  //restart hcsecd
  Utils::runShellCommand("/etc/rc.d/hcsecd restart");
  
  return;
}

QStringList Hardware::readAllSavedBTDevices(){
  //Return QStringList of all BDADDR's saved in hcsecd.conf
  QStringList bdaddrList;
  //scan the file and output all the devices
  QFile filein( "/etc/bluetooth/hcsecd.conf" );
  if( filein.open( QIODevice::ReadOnly ) ){
    QTextStream instream( &filein );
    while(!instream.atEnd() ){
      QString line = instream.readLine();
      if( line.contains("device {") && !line.contains("#") ){
	//grab the value for the bdaddr from the next line
	bdaddrList << getHcsecdDeviceValue( instream.readLine() );
      }
    } //end of loop over file
    filein.close();
  }
  return bdaddrList;
}

QStringList Hardware::readSavedBTDevice(QString bdaddr){
  //Returns QStringList of [bdaddr, name, key, pin] from the hcsecd.conf file
  
  QStringList settings;
  //scan the file and save only the desired device settings
  QFile filein( "/etc/bluetooth/hcsecd.conf" );
  if( filein.open( QIODevice::ReadOnly ) ){
    QTextStream instream( &filein );
    while(!instream.atEnd() ){
      QString line = instream.readLine();
	if( line.contains("device {") && !line.contains("#") ){
	  //grab the value for the bdaddr from the next line
	  QString bdaddrchk = getHcsecdDeviceValue( instream.readLine() );
	  if(bdaddrchk == bdaddr){//not entry to be removed
	    //save these device configurations
	    settings << bdaddrchk; //bdaddr
	    settings << getHcsecdDeviceValue( instream.readLine() ); //name
	    settings << getHcsecdDeviceValue( instream.readLine() ); //key
	    settings << getHcsecdDeviceValue( instream.readLine() ); //pin
	    break; //end the loop, data already found
	  }
	}
    } //end of loop over file
    filein.close();
  }
  return settings;
}

void Hardware::rmBTdevice(QString bdaddr,bool willRestart=true){
  //Disconnect the device first
  disconnectBTDevice(bdaddr);
  
  QStringList oldbdaddr, oldname, oldkey, oldpin;
  //scan the file and save all the devices except the one to be removed
  QFile filein( "/etc/bluetooth/hcsecd.conf" );
  if( filein.open( QIODevice::ReadOnly ) ){
    QTextStream instream( &filein );
    while(!instream.atEnd() ){
      QString line = instream.readLine();
	if( line.contains("device {") && !line.contains("#") ){
	  //grab the value for the bdaddr from the next line
	  QString bdaddrchk = getHcsecdDeviceValue( instream.readLine() );
	  if(bdaddrchk != bdaddr){//not entry to be removed
	    //save these device configurations
	    oldbdaddr << bdaddrchk;
	    oldname << getHcsecdDeviceValue( instream.readLine() );
	    oldkey << getHcsecdDeviceValue( instream.readLine() );
	    oldpin << getHcsecdDeviceValue( instream.readLine() );
	  }
	}
    } //end of loop over file
    filein.close();
  }
  
  
  //write a temporary file to not include the removed device
  QFile fileout( "/etc/bluetooth/hcsecd.conf.tmp" );
  if( fileout.open( QIODevice::WriteOnly ) ){
    QTextStream streamout( &fileout );
    for(int i=0; i<oldbdaddr.length(); i++){
      //Check the formatting of the name/key/pin (put quotes around item when needed)
      oldname[i] = "\""+oldname[i]+"\"";
      if(oldkey[i]!="nokey"){oldkey[i] = "\""+oldkey[i]+"\"";}
      if(oldpin[i]!="nopin"){oldpin[i] = "\""+oldpin[i]+"\"";}
      //write the device info to the file
      streamout << "\ndevice {\n";
      streamout << " bdaddr  "+oldbdaddr[i]+";\n";
      streamout << " name    "+oldname[i]+";\n";
      streamout << " key     "+oldkey[i]+";\n";
      streamout << " pin     "+oldpin[i]+";\n";
      streamout << "}\n";
    }
    fileout.close();
  }  
  //overwrite the original file
  Utils::runShellCommand("mv /etc/bluetooth/hcsecd.conf.tmp /etc/bluetooth/hcsecd.conf");
  //Set the proper permissions again
  Utils::runShellCommand("chmod 600 /etc/bluetooth/hcsecd.conf");
  
  //Restart the hcsecd if desired
  if(willRestart) { Utils::runShellCommand("/etc/rc.d/hcsecd restart"); }
}

QString Hardware::getHcsecdDeviceValue(QString rawline){
  //returns the value of the variable on a given raw line from the hcsecd.conf file
  QString line = rawline.simplified().section(";",0,0); //remove the ";" at the end of the line
  //qDebug() << line;
  line = line.section(" ",1,1,QString::SectionSkipEmpty); //remove the label at the beginning
  line.remove("\"");
  //qDebug() << rawline << "-->" << line;
  return line;
}

void Hardware::restartBT(){
  qDebug() << "Restarting hcsecd";
  Utils::runShellCommand("/etc/rc.d/hcsecd restart"); //restart hcsecd to make sure any changes are available
  //Try to connect to all the devices saved in hcsecd.conf that are not already connected
  QStringList savedBdaddrList = readAllSavedBTDevices();
  for(int i=0; i<savedBdaddrList.length(); i++){
    if( getBTConnectionHandle(savedBdaddrList[i]).isEmpty() ){ //if not currently connected
      qDebug() << "Initiating Connection: "+savedBdaddrList[i];
      QString result = pairBTDevice(savedBdaddrList[i]);
      if(!result.isEmpty()){qDebug() << result; }
      else{qDebug() << "Connection successful";}
    }
  }
  qDebug() << "Finished restarting Bluetooth Services";
}

QStringList Hardware::findBTdevices(){
  //Returns a QStringList of uniqe BD_ADDR discovered
  QStringList bdaddrlist;
  QStringList output = Utils::listShellCommandSearch("hccontrol -n ubt0hci inquiry","BD_ADDR");
  for(int i=0; i<output.length(); i++){
    QString bdaddr = output[i].section(":",1,7,QString::SectionSkipEmpty).simplified();
    if( !bdaddrlist.contains(bdaddr) ){bdaddrlist << bdaddr;} //add to the list if it is unique
  }
  return bdaddrlist;
}

QString Hardware::getBTRemoteName(QString bdaddr,bool tryagain){
  //tryagain is TRUE by default
    QString cmd = "hccontrol remote_name_request "+bdaddr;
    QString name = Utils::runShellCommandSearch(cmd,"Name:");
    name = name.section(":",1,1);
    name = name.simplified(); //remove extra white space
    if(name.isEmpty() && tryagain){
      name = getBTRemoteName(bdaddr,FALSE); //try one more time, false to avoid loop situation
    }
    return name;
}

QString Hardware::pairBTDevice(QString bdaddr){
  //Returns empty string if device connected successfully
  
  Utils::runShellCommand("hccontrol write_authentication_enable 1"); //Turn on computer initiated authentication
  //Initiate connection
  QString cmd = "hccontrol create_connection "+ bdaddr;
  QStringList result = Utils::runShellCommand(cmd);
  //Determine if connection accepted and prepare status output
  QString handle = getBTConnectionHandle(bdaddr); //should be a handle if connection successful
  QString status;
  if(handle.isEmpty()){
    //Did not connect, get the error
    QString junk = result.join(" ").section(":",1,1,QString::SectionSkipEmpty); //remove the "Status:" indicator
    junk = junk.section("[",0,0,QString::SectionSkipEmpty).simplified(); //remove the error code [0x...] from the end
    QString err = result.join(" ").section("[",1,1).section("]",0,0).simplified(); //Get the error code
    //Add extra information for known errors
    if(err == "0x18"){status = "Error: "+junk+" - Check PIN and KEY configuration"; } 
    else if(err == "0x4"){status = "Error: "+junk+" - Check that device is in range and turned on"; }
    //Add more known errors later
    else{ status = "Error: #"+err+" : "+junk; }
  }
  //Clean up settings
  Utils::runShellCommand("hccontrol write_authentication_enable 0"); //Turn off computer initated authentication
  
  return status;
}

void Hardware::disconnectBTDevice(QString bdaddr){
  //Find the connection handle from the BD_ADDR
  QString handle = getBTConnectionHandle(bdaddr);
  if(handle.isEmpty()){ return; } //Do nothing if the device is not currently connected
  //Send the disconnect signal
  QString cmd = "hccontrol disconnect "+handle;
  Utils::runShellCommand(cmd);
}

QString Hardware::getBTConnectionHandle(QString bdaddr){
  QStringList output = Utils::runShellCommand("hccontrol read_connection_list");
  QString handle = output.filter(bdaddr).join(";").section(" ",1,1,QString::SectionSkipEmpty);
  return handle;
}
