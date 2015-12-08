//===========================================
//  PC-BSD source code
//  Copyright (c) 2015, PC-BSD Software/iXsystems
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
// Note: This was almost entirely written by Tim McCormick in 2009 for
//   the first PC-BSD library, and copied here by Ken Moore in 2015
//===========================================
#include "pcbsd-network.h"
#include "pcbsd-general.h"

#include "global.h"

using namespace pcUtil;

//====================
//   STATIC LISTING FUNCTION 
//====================
QStringList NetDevice::listNetDevices(){
  QStringList result;
  struct ifaddrs *ifap;
  getifaddrs(&ifap);
  char ifName[IFNAMSIZ];
   //Probe the system for each device (one at a time)
  while (ifap != NULL){
     strncpy(ifName, ifap->ifa_name, IFNAMSIZ);
     if (result.contains(ifName) == 0) result += ifName;
     ifap = ifap->ifa_next;
  }
  //Close the 
  freeifaddrs(ifap);
  return result;
}

//=====================
//          CLASS FUNCTIONS
//=====================
//Get the name of the device (taking off any numbers)
QString NetDevice::devName(){
   uint pos = name.indexOf(QRegExp("[0-9]+$"));
   QString result = name;
   result.truncate(pos);
   return result;
}

//Return just the number of the device (removing the name)
uint NetDevice::devNum(){
   uint pos = name.indexOf(QRegExp("[0-9]+$"));
   return name.mid(pos).toInt();
}

//Fetch the IP and return it as a QString
QString NetDevice::ipAsString(){
   struct ifreq ifr;
   memset(&ifr, 0, sizeof(struct ifreq));

   strncpy(ifr.ifr_name, name.toLocal8Bit(), IFNAMSIZ);
   int s = socket(PF_INET, SOCK_DGRAM, 0);
   
   ioctl(s, SIOCGIFADDR, &ifr);
   struct in_addr in = ((sockaddr_in *) &ifr.ifr_addr)->sin_addr;

   return QString(inet_ntoa(in));
}

//Fetch the IPv6 and return it as a QString
QString NetDevice::ipv6AsString(){
  return "unknown";
   //Note: New on 6/24/15 - still needs testing
   /*struct ifreq ifr;
   memset(&ifr, 0, sizeof(struct ifreq));

   strncpy(ifr.ifr_name, name.toLocal8Bit(), IFNAMSIZ);
   int s = socket(PF_INET6, SOCK_DGRAM, 0);
   
   ioctl(s, SIOCGIFADDR, &ifr);
   struct in6_addr in = ((sockaddr_in6 *) &ifr.ifr_addr)->sin6_addr;

   return QString(inet_ntoa(in));*/
}

//Fetch the netmask and return it as a QString
QString NetDevice::netmaskAsString(){
   struct ifreq ifr;
   memset(&ifr, 0, sizeof(struct ifreq));

   strncpy(ifr.ifr_name, name.toLocal8Bit(), IFNAMSIZ);
   int s = socket(PF_INET, SOCK_DGRAM, 0);
   
   ioctl(s, SIOCGIFNETMASK, &ifr);
   struct in_addr in = ((sockaddr_in *) &ifr.ifr_addr)->sin_addr;

   return QString(inet_ntoa(in));
}

//Returns the description string for the device
QString NetDevice::desc(){
   return General::sysctl("dev." + devName() + "." + QString::number(devNum()) + ".%desc");
}

//Fetch the mac address as a QString
QString NetDevice::macAsString(){
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
   for (uint i=0; i < 6; i++){
      mac += QString::number(*(ptr+i), 16).right(2).rightJustified(2, '0');
      if(i<5){ mac += ":"; }
   }
   return mac;
}

//Get the status of the device (active, associated, etc...)
QString NetDevice::mediaStatusAsString(){
   struct ifmediareq ifm;
   memset(&ifm, 0, sizeof(struct ifmediareq));

   strncpy(ifm.ifm_name, name.toLocal8Bit(), IFNAMSIZ);
   int s = socket(AF_INET, SOCK_DGRAM, 0);

   ioctl(s, SIOCGIFMEDIA, &ifm);
   QString status;

   switch (IFM_TYPE(ifm.ifm_active)){
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

QString NetDevice::gatewayAsString(){
  QString info = General::RunCommand("nice netstat -n -r").split("\n").filter(name).filter("default").join("\n");
  if(info.isEmpty()){ return ""; }
  //Pull the gateway out of the first line (<default> <gateway>)
  info = info.replace("\t"," ").section("\n",0,0).simplified(); //ensure proper parsing
  return  info.section(" ",1,1);
}

//Check if a device is wireless or not
bool NetDevice::isWireless(){
   struct ifmediareq ifm;
   memset(&ifm, 0, sizeof(struct ifmediareq));

   strncpy(ifm.ifm_name, name.toLocal8Bit(), IFNAMSIZ);
   int s = socket(AF_INET, SOCK_DGRAM, 0);

   ioctl(s, SIOCGIFMEDIA, &ifm);

   return IFM_TYPE(ifm.ifm_active) == IFM_IEEE80211;
}

//Get the parent device (if this is a wireless wlan)
QString NetDevice::getWifiParent(){
   if(!name.contains("wlan")){ return ""; }
   return General::sysctl("net.wlan." + QString::number(this->devNum()) + ".%parent");
}

//See if the device is setup to use DHCP
bool NetDevice::usesDHCP(){
  //The system does not keep track of how the device's address was assigned
  //  so the closest we can get to this is to see if the system is setup to use 
  //  DHCP on startup (in /etc/rc.conf) (Ken Moore - 6/24/15)
  return !Network::readRcConf().filter(name).filter("DHCP").isEmpty();
}

//See if the device is currently in use
bool NetDevice::isUp(){
   struct ifreq ifr;
   memset(&ifr, 0, sizeof(struct ifreq));

   strncpy(ifr.ifr_name, name.toLocal8Bit(), IFNAMSIZ);
   int s = socket(AF_INET, SOCK_DGRAM, 0);

   ioctl(s, SIOCGIFFLAGS, &ifr);

   return (ifr.ifr_flags & IFF_UP) ? 1 : 0;
}

//Determine the number of packets received by the device
long NetDevice::packetsRx(){
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

//Determine the number of packets transmitted by the device
long NetDevice::packetsTx(){
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

//Determine the number of errors received
long NetDevice::errorsRx(){
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

//Determine the number of errors transmitted
long NetDevice::errorsTx(){
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

//=========================
//   SETTING FUNCTIONS (requires root)
//=========================
void NetDevice::setUp(bool up){
  //This only sets it up/down for the current session - does not change usage on next boot
  General::RunCommand("ifconfig "+name+" "+ (up ? "up": "down") );
}
