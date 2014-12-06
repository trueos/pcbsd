/***************************************************************************
 *   Copyright (C) 2008 by Tim McCormick   *
 *   tim@pcbsd.org   *
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
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. *
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/sysctl.h>
#include <unistd.h>

#include <net/if.h>
#include <net/if_dl.h>
#include <net/if_media.h>

#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#include <qregexp.h>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QString>
//#include <QMessageBox> //for debugging purposes

#include "pcbsd-netif.h"
#include "pcbsd-utils.h"

using namespace pcbsd;

QStringList NetworkInterface::getInterfaces()
{
   QStringList result;
   struct ifaddrs *ifap;
   getifaddrs(&ifap);
   char ifName[IFNAMSIZ];
   
   while (ifap != NULL)
   {
      strncpy(ifName, ifap->ifa_name, IFNAMSIZ);
      if (result.contains(ifName) == 0) result += ifName;
      ifap = ifap->ifa_next;
   }

   freeifaddrs(ifap);
   return result;
}

NetworkInterface::NetworkInterface(QString devName)
{
   name = devName;
}

QString NetworkInterface::ipAsString()
{
   struct ifreq ifr;
   memset(&ifr, 0, sizeof(struct ifreq));

   strncpy(ifr.ifr_name, name.toLocal8Bit(), IFNAMSIZ);
   int s = socket(PF_INET, SOCK_DGRAM, 0);
   
   ioctl(s, SIOCGIFADDR, &ifr);
   struct in_addr in = ((sockaddr_in *) &ifr.ifr_addr)->sin_addr;

   return QString(inet_ntoa(in));
}

QString NetworkInterface::netmaskAsString()
{
   struct ifreq ifr;
   memset(&ifr, 0, sizeof(struct ifreq));

   strncpy(ifr.ifr_name, name.toLocal8Bit(), IFNAMSIZ);
   int s = socket(PF_INET, SOCK_DGRAM, 0);
   
   ioctl(s, SIOCGIFNETMASK, &ifr);
   struct in_addr in = ((sockaddr_in *) &ifr.ifr_addr)->sin_addr;

   return QString(inet_ntoa(in));
}

QString NetworkInterface::macAsString()
{
   int mib[6];
   size_t len;
   char *buf;
   struct sockaddr_dl *sdl;
   char *ptr;

   mib[0] = CTL_NET;
   mib[1] = AF_ROUTE;
   mib[2] = 0;
   mib[3] = AF_LINK;
   mib[4] = NET_RT_IFLIST;
   mib[5] = if_nametoindex(name.toLocal8Bit());

   //First find the size of the return, so we can adjust buf accordingly
   sysctl(mib, 6, NULL, &len, NULL, 0);
   buf = (char *) malloc(len);
   sysctl(mib, 6, buf, &len, NULL, 0);

   sdl = (sockaddr_dl *)(((if_msghdr *)buf)+1);
   ptr = (char *) LLADDR(sdl);
   
   QString mac;
   for (uint i=0; i < 6; i++)
   {
      mac += QString::number(*(ptr+i), 16).right(2).rightJustified(2, '0');
      if (i<5) mac += ":";
   }
   
   return mac;
}

QString NetworkInterface::mediaStatusAsString()
{
   struct ifmediareq ifm;
   memset(&ifm, 0, sizeof(struct ifmediareq));

   strncpy(ifm.ifm_name, name.toLocal8Bit(), IFNAMSIZ);
   int s = socket(AF_INET, SOCK_DGRAM, 0);

   ioctl(s, SIOCGIFMEDIA, &ifm);
   QString status;

   switch (IFM_TYPE(ifm.ifm_active))
   {
      case IFM_FDDI:
      case IFM_TOKEN:
         if (ifm.ifm_status & IFM_ACTIVE) status = "inserted";
         else status = "no ring";
         break;
      case IFM_IEEE80211:
         if (ifm.ifm_status & IFM_ACTIVE) status = "associated";
         else status = "no carrier";
         break;
      default:
         if (ifm.ifm_status & IFM_ACTIVE) status = "active";
         else status = "no carrier";
   }
   return status;
}

QString NetworkInterface::desc()
{
   return Utils::sysctl("dev." + devName() + "." + QString::number(devNum()) + ".%desc");
}

QString NetworkInterface::devName()
{
   uint pos = name.indexOf(QRegExp("[0-9]+$"));
   QString result = name;
   result.truncate(pos);
   return result;
}

bool NetworkInterface::isWireless()
{
   struct ifmediareq ifm;
   memset(&ifm, 0, sizeof(struct ifmediareq));

   strncpy(ifm.ifm_name, name.toLocal8Bit(), IFNAMSIZ);
   int s = socket(AF_INET, SOCK_DGRAM, 0);

   ioctl(s, SIOCGIFMEDIA, &ifm);

   return IFM_TYPE(ifm.ifm_active) == IFM_IEEE80211;
}

bool NetworkInterface::isUp()
{
   struct ifreq ifr;
   memset(&ifr, 0, sizeof(struct ifreq));

   strncpy(ifr.ifr_name, name.toLocal8Bit(), IFNAMSIZ);
   int s = socket(AF_INET, SOCK_DGRAM, 0);

   ioctl(s, SIOCGIFFLAGS, &ifr);

   return (ifr.ifr_flags & IFF_UP) ? 1 : 0;
}

long NetworkInterface::packetsRx()
{
   int mib[6];
   size_t len;
   char *buf;
   struct if_msghdr *ifm;

   mib[0] = CTL_NET;
   mib[1] = AF_ROUTE;
   mib[2] = 0;
   mib[3] = AF_LINK;
   mib[4] = NET_RT_IFLIST;
   mib[5] = if_nametoindex(name.toLocal8Bit());

   //First find the size of the return, so we can adjust buf accordingly
   sysctl(mib, 6, NULL, &len, NULL, 0);
   buf = (char *) malloc(len);
   sysctl(mib, 6, buf, &len, NULL, 0);

   ifm = (if_msghdr *) buf;

   return ifm->ifm_data.ifi_ipackets;
}

long NetworkInterface::packetsTx()
{
   int mib[6];
   size_t len;
   char *buf;
   struct if_msghdr *ifm;

   mib[0] = CTL_NET;
   mib[1] = AF_ROUTE;
   mib[2] = 0;
   mib[3] = AF_LINK;
   mib[4] = NET_RT_IFLIST;
   mib[5] = if_nametoindex(name.toLocal8Bit());

   //First find the size of the return, so we can adjust buf accordingly
   sysctl(mib, 6, NULL, &len, NULL, 0);
   buf = (char *) malloc(len);
   sysctl(mib, 6, buf, &len, NULL, 0);

   ifm = (if_msghdr *) buf;

   return ifm->ifm_data.ifi_opackets;
}

long NetworkInterface::errorsRx()
{
   int mib[6];
   size_t len;
   char *buf;
   struct if_msghdr *ifm;

   mib[0] = CTL_NET;
   mib[1] = AF_ROUTE;
   mib[2] = 0;
   mib[3] = AF_LINK;
   mib[4] = NET_RT_IFLIST;
   mib[5] = if_nametoindex(name.toLocal8Bit());

   //First find the size of the return, so we can adjust buf accordingly
   sysctl(mib, 6, NULL, &len, NULL, 0);
   buf = (char *) malloc(len);
   sysctl(mib, 6, buf, &len, NULL, 0);

   ifm = (if_msghdr *) buf;

   return ifm->ifm_data.ifi_ierrors;
}

long NetworkInterface::errorsTx()
{
   int mib[6];
   size_t len;
   char *buf;
   struct if_msghdr *ifm;

   mib[0] = CTL_NET;
   mib[1] = AF_ROUTE;
   mib[2] = 0;
   mib[3] = AF_LINK;
   mib[4] = NET_RT_IFLIST;
   mib[5] = if_nametoindex(name.toLocal8Bit());

   //First find the size of the return, so we can adjust buf accordingly
   sysctl(mib, 6, NULL, &len, NULL, 0);
   buf = (char *) malloc(len);
   sysctl(mib, 6, buf, &len, NULL, 0);

   ifm = (if_msghdr *) buf;

   return ifm->ifm_data.ifi_oerrors;
}

uint NetworkInterface::devNum()
{
   uint pos = name.indexOf(QRegExp("[0-9]+$"));
   return name.mid(pos).toInt();
}

void NetworkInterface::restart(){
  //restart all network connections
  system("(sync; ifconfig lagg0 destroy; route -n flush; /etc/rc.d/netif restart) &");
}

QStringList NetworkInterface::parseWifiScanLine( QString linein, bool isverbose){
  /*
    This function works given a single line of the command:
      "ifconfig (wifi device) list scan" (verbose or not)
    Output: QStringList[ssid,bssid,channel,rate,signal%,INT,CAPS,etc]
  */
  int n; //index for end of the SSID section
  if(isverbose){n=33;} //verbose can display full ssid names between 16-33 characters long
  else{n=16;}
  QString line = linein; //make sure we do not change the line outside the function
  //Parse the line into its components
  QString ssid = line.left(n); //Entire SSID/MESH ID column
    ssid = ssid.trimmed(); //remove excess whitespace at the ends
    line.remove(0,n); //remove the ssid from the line (multiple words mess up later parts otherwise)
  QString bssid = line.section(" ",0,0,QString::SectionSkipEmpty); //BSSID column
  QString chan = line.section(" ",1,1,QString::SectionSkipEmpty); //CHAN column
  QString rate = line.section(" ",2,2,QString::SectionSkipEmpty); //RATE column
  QString signoise = line.section(" ",3,3,QString::SectionSkipEmpty); //S:G column
  QString inte = line.section(" ",4,4,QString::SectionSkipEmpty); //INT column
  QString caps = line.section(" ",5,5,QString::SectionSkipEmpty); //CAPS column
  QString etc = line.section(" ",6,50,QString::SectionSkipEmpty); //Everything at the end (security info) 

    //change signoise into signal strength(%)
    QString sig = signoise.section(":",0,0); //determine signal
    QString noise = signoise.section(":",1,1); //determine noise
    //determine the signal strength
    int signal = ( sig.toInt()-noise.toInt() )*4;
    if(signal<0){signal=0;}
    if(signal>100){signal=100;}
    //output result as a QString percentage
    QString strength = QString::number(signal);
    strength.append("%");

  //get the security type
    QString secType = parseWifiSecurity(caps,etc);
  //put into QStringList for output
  QStringList out;
  out << ssid << bssid << chan << rate << strength << inte << secType;
  return out;
}

QString NetworkInterface::getWifiSecurity( QString SSID, QString deviceName ){
  //Make sure the SSID is short enough (non verbose will be used to search)
  QString smSSID;
  if(SSID.size() > 10){
    smSSID = SSID.left(10);
  }else{
    smSSID = SSID; 
  }
  //Scan the wifi to determine the security type
  //If there are multiple wifi's with the same SSID, it will use the first one only
  QString command = "ifconfig " + deviceName + " list scan";
  QString ifconfOut = Utils::runShellCommandSearch(command,smSSID);
  QStringList parsed = NetworkInterface::parseWifiScanLine(ifconfOut,false);
  //get the security type
  QString output = parsed[6];
  return output;
}

QString NetworkInterface::parseWifiSecurity( QString caps, QString etc ){
  // INPUT: caps and etc are the last two fields output from "ifconfig (device) list scan"
  
  QStringList secure = etc.split(" "); //put each security type in a different list element
    
  //Examine the CAPS and Security data to determine the wifi encryption type
  bool isDisabled = !caps.contains("P") && caps.contains("ES"); //CAPS = "ES" only -> no wifi encryption
  bool isWEP = caps.contains("EP"); //CAPS = "EP" or "EPS" only (could also be RADIUS security type)
  bool isWPA = secure.contains("WPA"); //both WPA personal and enterprise
  bool isWPA2 = secure.contains("RSN"); //both WPA2 personal and enterprise
  
  //qDebug() << "caps: "+caps+"\netc: "+etc;

  //Output the security type
  QString securityType;
  if(isDisabled){ 
    securityType = "None";
  }else if(isWPA2){ 
    securityType = "WPA2";
  }else if(isWPA){ 
    securityType = "WPA";
  }else if(isWEP){ 
    securityType = "WEP";
  }else{ 
    securityType = "ERROR";
  }
  return securityType;
}

QString NetworkInterface::getFirstWiredDevice()
{
  QString tmp;

  QStringList ifs = getInterfaces();
  for ( QStringList::Iterator it = ifs.begin(); it != ifs.end(); ++it )
  {
     // List of devices to exclude not real ethernet
     QString tmpDev = *it;
     if (tmpDev.indexOf("lo0") == -1
         && tmpDev.indexOf("lagg") == -1
         && tmpDev.indexOf("lo1") == -1
         && tmpDev.indexOf("lo2") == -1
         && tmpDev.indexOf("lo3") == -1
         && tmpDev.indexOf("fwe") == -1
         && tmpDev.indexOf("plip") == -1
         && tmpDev.indexOf("pfsync") == -1
         && tmpDev.indexOf("pflog") == -1
         && tmpDev.indexOf("ipfw") == -1
         && tmpDev.indexOf("tun") == -1
         && tmpDev.indexOf("usbus") == -1
         && tmpDev.indexOf("vboxnet") == -1
         && tmpDev.indexOf("wlan") == -1 )
     {
        NetworkInterface ifr(tmpDev);
        if (! ifr.isWireless()) {
          return tmpDev;
        }
     }
  }

  return tmp;
}

QString NetworkInterface::getWifiParent(QString dev)
{
   dev.remove(0, dev.size() -1 );
   QString DevNum = dev;
   return Utils::sysctl("net.wlan." + DevNum + ".%parent");
}

void NetworkInterface::wifiQuickConnect(QString SSID, QString netKey, QString DeviceName, bool WEPHex){
  /* 
     This function uses a set of defaults to connect to a wifi access point with a minimum
     of information from the user. It does *NOT* (currently) support the WPA-Enterprise encryption
  */
  
    //do nothing if no SSID given
    if( SSID.isEmpty() ){
      return;
    }
  
    QString tmp;
    QString ifConfigLine;
    
    //Set defaults for quick-connect
    ifConfigLine="SYNCDHCP"; //Use DHCP

    //setup for not using the lagg interface
      Utils::setConfFileValue( "/etc/rc.conf", "ifconfig_lagg0", "", -1);
      Utils::setConfFileValue( "/etc/rc.conf", "ifconfig_" + DeviceName, \
		 "ifconfig_" + DeviceName + "=\"WPA " + ifConfigLine + "\"", -1);

	
    //Determine if the wpa_supplicant file exists already or is empty
    bool newWPASup = true;
    bool existingSSID = false;
    QStringList tmpFileList;
    QString tmpEntry;
    QFile fileout( "/etc/wpa_supplicant.conf" );
    if( fileout.open( QIODevice::ReadOnly ) ){
      QTextStream streamtmp(&fileout);
      streamtmp.setCodec("UTF-8");
        QString line;
        bool inEntry = false;
        bool eStart, eEnd;
        while ( !streamtmp.atEnd() ) {
            eStart = false; 
            eEnd = false;	
            line = streamtmp.readLine();
	    if ( line.contains("ctrl_interface=/var/run/wpa_supplicant") ) {
   		newWPASup = false;
	    }else if(line.contains("ssid=") && line.contains(SSID)){
	    	existingSSID=true;
	    }else if(line.contains("network={")){eStart = true;}
	    else if(line.contains("}")){eEnd = true; }
	    
	    //Save the file by entry temporarily
	    if(eStart){ tmpEntry = line; inEntry = true; }
	    else if(eEnd){ tmpEntry.append(" ::: "+line); tmpFileList << tmpEntry; inEntry=false; }	    
	    else if(inEntry){ tmpEntry.append(" ::: "+line); }
	    else{ tmpFileList << line; }
	}
	fileout.close();
    }
    //If the desired SSID already has an entry, remove it from wpa_supplicant.conf
    if(existingSSID){
      QFile tmpFile( "/etc/wpa_supplicant.conf.tmp" );
      if(tmpFile.open(QIODevice::WriteOnly | QIODevice::Text )){
        QTextStream oStr(&tmpFile);
        for(int i=0; i<tmpFileList.length(); i++){
          if(tmpFileList[i].contains("network={")){
            QStringList tmp = tmpFileList[i].split(" ::: ");
            //skip it if the new SSID
	    int idx = tmp.indexOf("ssid=");
            if( (idx!= -1) && !tmp[idx].contains(SSID) ){
              for(int j=0; j<tmp.length(); j++){
            	oStr << tmp[j] + "\n";    	    
              }
            }
          }else{
            oStr << tmpFileList[i] + "\n";	  
          }			
        }
      }
      tmpFile.close();
      Utils::runShellCommand("mv /etc/wpa_supplicant.conf.tmp /etc/wpa_supplicant.conf");
    }
    
    // Create the wpa_supplicant file based on saved configuration    
    if ( fileout.open( QIODevice::Append ) ) {
       QTextStream streamout( &fileout );

       // Fix to prevent kernel panic
       if(newWPASup)
	   streamout << "ctrl_interface=/var/run/wpa_supplicant\n\n";
       //Use SSID for network connection
       streamout << "\nnetwork={\n ssid=\"" + SSID + "\"\n";
       streamout << " priority=" << 146 << "\n";
       streamout << " scan_ssid=1\n";

       //Determine the security type for the given SSID
       QString SecType = getWifiSecurity(SSID,DeviceName);
       
       //Configure the wifi Security Settings for the proper type
       if ( SecType.contains("None") ){
          streamout << " key_mgmt=NONE\n";
	  
       } else if ( SecType.contains("WEP") ) {
	  //Set WEP Defaults
	  int WEPIndex = 0;
          //bool WEPHex = true; //Use Hex WEP key
          streamout << " key_mgmt=NONE\n";
          streamout << " wep_tx_keyidx=" + tmp.setNum(WEPIndex) + "\n";
          // Check if we are using a plaintext WEP or not
          if ( WEPHex ) {
            streamout << " wep_key" + tmp.setNum(WEPIndex) + "=" + netKey + "\n";
          } else {
            streamout << " wep_key" + tmp.setNum(WEPIndex) + "=\"" + netKey + "\"\n";
          }

       } else if ( SecType.contains("WPA") ) {
          streamout << " psk=\"" + netKey + "\"\n";	
       } 

       streamout << "}\n\n";

       fileout.close();
     } 

    // Flush to disk, so that when we restart network it picks up the changes
    sync();

    Utils::restartNetworking();
   
   return;
}

// Function which sets up the lagg0 interface for the specified wlan[0-9] device
void NetworkInterface::enableLagg(QString dev)
{
  // Get the first wired device, we will enable lagg0 to use it
  QString wiredDev, wifiParent, wifiConf, tmp, newWifiConf;
  wiredDev = NetworkInterface::getFirstWiredDevice();
  wifiParent = NetworkInterface::getWifiParent(dev);

  // If no wired device on this box or no valid wifi parent we can return, no need to setup lagg
  if ( wiredDev.isEmpty() || wifiParent.isEmpty() )
     return;

  // Get the config for this wifi device
  wifiConf = Utils::getConfFileValue( "/etc/rc.conf", "ifconfig_" + dev + "=", 1 );
  if ( wifiConf.isEmpty() )
     wifiConf = Utils::getConfFileValue( "/etc/rc.conf", "ifconfig_lagg0=", 1 );
  else if ( wifiConf == "WPA" ) {
     // If it just equals WPA, check if anything already exists in lagg0 line
     tmp = Utils::getConfFileValue( "/etc/rc.conf", "ifconfig_lagg0=", 1 );
     if ( ! tmp.isEmpty() )
        wifiConf = tmp;
  }

  // No valid config? We can't save this..
  if ( wifiConf.isEmpty() )
     return;

  // Setup the ethernet mac address cloning for this device
  Utils::setConfFileValue( "/etc/rc.conf", "ifconfig_" + wifiParent, "ifconfig_" + wifiParent + "=\"`ifconfig " + wiredDev + " ether`\"", 1);
  Utils::setConfFileValue( "/etc/rc.conf", "ifconfig_" + wifiParent, "ifconfig_" + wifiParent + "=\"ether ${ifconfig_" + wifiParent + "##*ether }\"", 2);
  Utils::setConfFileValue( "/etc/rc.conf", "wlans_" + wifiParent, "wlans_" + wifiParent + "=\"" + dev + "\"", -1);

  wifiConf = wifiConf.simplified();

  // Check if we have a WPA setting, and put it in the right device slot
  if ( wifiConf.indexOf("WPA") != -1 )
  {
    newWifiConf="WPA";
    wifiConf = wifiConf.replace("WPA", "");
  }

  // Check if we have a Country code setting, and put it in the right device slot
  if ( wifiConf.indexOf("country ") != -1 )
  {
    tmp = wifiConf;
    tmp.remove(0, tmp.lastIndexOf("country") + 8);
    tmp = tmp.section(" ", 0,0);

    newWifiConf+=" country " + tmp;
    wifiConf = wifiConf.remove(wifiConf.indexOf("country"), wifiConf.indexOf("country") + 10 );
  }


  // Save the new wifi config line
  newWifiConf = newWifiConf.simplified();
  Utils::setConfFileValue( "/etc/rc.conf", "ifconfig_" + dev, "ifconfig_" + dev + "=\"" + newWifiConf + "\"", -1);

  // Set the wired device to UP
  Utils::setConfFileValue( "/etc/rc.conf", "ifconfig_" + wiredDev, "ifconfig_" + wiredDev + "=\"UP\"", -1);

  // Enable the lagg0 interface
  wifiConf = wifiConf.simplified();
  Utils::setConfFileValue( "/etc/rc.conf", "cloned_interfaces", "cloned_interfaces=\"lagg0\"", -1);
  Utils::setConfFileValue( "/etc/rc.conf", "ifconfig_lagg0", "ifconfig_lagg0=\"laggproto failover laggport " + wiredDev + " laggport " + dev + " " + wifiConf + "\"", -1);

}

int NetworkInterface::enableWirelessAccessPoint(QString wdev, QString name, QString password, bool persist, QString ip, QString mode, int channel, QString netmask, QString country){
  //Return Codes: 0=success, 1=General Error, -1=Unsupported device (no AP support in driver)
	
  //Check that wlan0 is not currently in use
  QString ret = Utils::runShellCommand("ifconfig wlan0").join("\n").simplified();
  if(ret.contains( "<UP,") ){ return 1; } //wlan0 currently in use
  else if(!ret.contains("wlan0 does not exist")){ 
    //wlan0 already in use: remove it for now (not sure if associated with the device given)
    Utils::runShellCommand("ifconfig wlan0 destroy");
  }
  
  //Check that wlan0/wdev support AP mode
  Utils::runShellCommand("ifconfig wlan0 create wlandev "+wdev);
  ret = Utils::runShellCommand("ifconfig wlan0 list caps").join("\n");
  Utils::runShellCommand("ifconfig wlan0 destroy"); //clean up
  if(!ret.contains(",HOSTAP,")){
    //This device does not support access point mode
    return -1;
  }
  
  //Re-create the new wlan0 in AP mode
  Utils::runShellCommand("ifconfig wlan0 create wlandev "+wdev+" wlanmode hostap");
  QString cmd = "ifconfig wlan0 inet "+ip+" netmask "+netmask+" ssid \""+name+"\" mode "+mode+" channel "+QString::number(channel);
  if( !country.isEmpty() ){ cmd.append(" country "+country); }
  Utils::runShellCommand(cmd);
  
  //Enable WPA-PSK encryption for the Access Point (if password given)
  if( !password.isEmpty() ){
    QStringList contents;
	contents << "interface=wlan0" \
		<< "debug=1" \
		<< "ctrl_interface=/var/run/hostapd" \
		<< "ctrl_interface_group=wheel" \
		<< "ssid="+name \
		<< "wpa=1" \
		<< "wpa_passphrase="+password \
		<< "wpa_key_mgmt=WPA-PSK" \
		<< "wpa_pairwise=CCMP TKIP";
    QFile file("/etc/hostapd.conf");
    if( file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate) ){
      QTextStream out(&file);
      out << contents.join("\n");
      file.close();
      Utils::runShellCommand("service hostapd forcestart");
    }else{
      qDebug() << "[WARNING] Could not enable WPA encryption for the new Access Point - is now an open network!!";
      password.clear(); //Could not enable encryption
    }
  }
  
  //Save the AP configuration to /etc/rc.conf
  if(persist){
    Utils::setConfFileValue("/etc/rc.conf", "wlans_"+wdev, "wlans_"+wdev+"=\"wlan0\"", -1);
    Utils::setConfFileValue("/etc/rc.conf", "create_args_wlan0", "create_args_wlan0=\"wlanmode hostap\"", -1);
    Utils::setConfFileValue("/etc/rc.conf", "ifconfig_wlan0", "ifconfig_wlan0=\"inet "+ip+" netmask "+netmask+" ssid "+name+" mode "+mode+" channel "+QString::number(channel)+"\"", -1);
    if(!password.isEmpty()){
      Utils::setConfFileValue("/etc/rc.conf", "hostapd_enable", "hostapd_enable=\"YES\"", -1);
    }
  }else{
    Utils::setConfFileValue("/etc/rc.conf","wlans_"+wdev, "");
    Utils::setConfFileValue("/etc/rc.conf","create_args_wlan0", "");
    Utils::setConfFileValue("/etc/rc.conf","ifconfig_wlan0", "");
  }
  return 0;
}

void NetworkInterface::disableWirelessAccessPoint(QString wdev){
  Utils::runShellCommand("service hostapd forcestop");
  QFile::remove("/etc/hostapd.conf");
  Utils::setConfFileValue("/etc/rc.conf", "wlans_"+wdev, "", -1);
  Utils::setConfFileValue("/etc/rc.conf", "create_args_wlan0", "", -1);
  Utils::setConfFileValue("/etc/rc.conf", "ifconfig_wlan0", "", -1);
  Utils::setConfFileValue("/etc/rc.conf", "hostapd_enable", "hostapd_enable=\"NO\"", -1);
  Utils::runShellCommand("ifconfig wlan0 destroy");
}
