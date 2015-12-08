#include <QCoreApplication>
#include <QDebug>

#include <sysadm-network.h>

int main( int argc, char ** argv )
{
  //Run a simple test of all the sysadm backend functions
  QStringList devs = sysadm::NetDevice::listNetDevices();
  qDebug() <<"Devices:" << devs;
  for(int i=0; i<devs.length(); i++){
    sysadm::NetDevice D(devs[i]);
    qDebug() << "Check Device:" << D.device();
    qDebug() << " - name:" << D.devName();
    qDebug() << " - number:" << D.devNum();
    qDebug() << " - IPv4:" << D.ipAsString();
    qDebug() << " - IPv6:" << D.ipv6AsString();
    qDebug() << " - netmask:" << D.netmaskAsString();
    qDebug() << " - description:" << D.desc();
    qDebug() << " - MAC:" << D.macAsString();
    //qDebug() << " - Media Type:" << D.mediaTypeAsString();
    qDebug() << " - Media Status:" << D.mediaStatusAsString();
    bool iswifi = D.isWireless();
    qDebug() << " - Is Wireless:" << iswifi;
    if(iswifi){
      qDebug() << " -- Wifi Parent:" << D.getWifiParent();
    }
    qDebug() << " - Is Active:" << D.isUp();
    qDebug() << " - Packets Rx:" << D.packetsRx() << "Errors:" << D.errorsRx();
    qDebug() << " - Packets Tx:" << D.packetsTx() << "Errors:" << D.errorsTx();
  }
  return 0;
}
