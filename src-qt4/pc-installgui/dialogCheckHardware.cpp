/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QTextStream>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/sysctl.h>

#include <net/if.h>
#include <net/if_dl.h>
#include <net/if_media.h>
 
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#include "dialogCheckHardware.h"
#include "ui_dialogCheckHardware.h"

void dialogCheckHardware::programInit()
{
  connect(pushClose, SIGNAL(clicked()), this, SLOT(slotClose()));

  // Get the various hardware details
  getVideo();
  getNetwork();
  getSound();

}

void dialogCheckHardware::getVideo()
{
   QString tmp, res, driver;
   QProcess d;
   d.start(QString("xdpyinfo"), QStringList());
   while(d.state() == QProcess::Starting || d.state() == QProcess::Running) {
      d.waitForFinished(200);
      QCoreApplication::processEvents();
   }

   while (d.canReadLine()) {
     tmp = d.readLine().simplified();
     if ( tmp.indexOf("dimensions:") != -1 ) {
       res = tmp.section(" ", 1, 1);
       break;
     }
   }

   // Figure out the driver
   QFile file("/var/log/Xorg.0.log");
   if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
     QTextStream in(&file);
     while (!in.atEnd()) {
        QString line = in.readLine();
        if ( line.indexOf("Loading /usr/local/lib/xorg/modules/drivers/") != -1 ) {
           QFileInfo fi(line);
           driver = fi.fileName();
           driver = driver.section("_", 0, 0);
           break;
        }
     }
    file.close();
   }


  if ( driver != "vesa" ) {
     labelVideoDriverIcon->setPixmap(QPixmap(":/modules/images/ok.png"));
  } else {
     labelVideoDriverIcon->setPixmap(QPixmap(":/modules/images/failed.png"));
  } 
  labelVideoDriverIcon->setScaledContents(true);
  labelVideoDriver->setText(tr("Video driver:") + " (" + driver + ")" );
  labelVideoResolution->setText(tr("Video resolution:") + " (" + res + ")" );
  labelVideoResolutionIcon->setPixmap(QPixmap(":/modules/images/ok.png"));
  labelVideoResolutionIcon->setScaledContents(true);

}

void dialogCheckHardware::getNetwork()
{
   QStringList ifs;
   QString eth, wifi;
   bool isWifi;

   // Get a list of nics
   struct ifaddrs *ifap;
   getifaddrs(&ifap);
   char ifName[IFNAMSIZ];
   while (ifap != NULL)
   {
      strncpy(ifName, ifap->ifa_name, IFNAMSIZ);
      if (ifs.contains(ifName) == 0) ifs += ifName;
      ifap = ifap->ifa_next;
   }

   freeifaddrs(ifap);

   for ( QStringList::Iterator it = ifs.begin(); it != ifs.end(); ++it )
   {
       QString dev = *it;
       if (dev.indexOf("lo") == 0
           || dev.indexOf("fwe") == 0 
           || dev.indexOf("ipfw") == 0
           || dev.indexOf("lagg") == 0
           || dev.indexOf("plip") == 0
           || dev.indexOf("pfsync") == 0
           || dev.indexOf("pflog") == 0
           || dev.indexOf("usbus") == 0
           || dev.indexOf("vboxnet") == 0
           || dev.indexOf("wlan") == 0
           || dev.indexOf("tun") == 0)
	  continue;

        // Figure out the type of device
        struct ifmediareq ifm;
        memset(&ifm, 0, sizeof(struct ifmediareq));

        strncpy(ifm.ifm_name, dev.toAscii(), IFNAMSIZ);
        int s = socket(AF_INET, SOCK_DGRAM, 0);

        ioctl(s, SIOCGIFMEDIA, &ifm);

        isWifi = IFM_TYPE(ifm.ifm_active) == IFM_IEEE80211;

	if ( isWifi ) {
          if ( wifi.isEmpty() )
            wifi = dev;
        } else {
          if ( eth.isEmpty() )
            eth = dev;
        }
   }

   if ( eth.isEmpty() )
   {
     labelEthernet->setText(tr("No ethernet detected"));
     labelEthernetIcon->setPixmap(QPixmap(":/modules/images/failed.png"));
     labelEthernetIcon->setScaledContents(true);
   } else {
     labelEthernet->setText(tr("Ethernet device:") + " (" + eth + ")" );
     labelEthernetIcon->setPixmap(QPixmap(":/modules/images/ok.png"));
     labelEthernetIcon->setScaledContents(true);
   }
   if ( wifi.isEmpty() )
   {
     labelWifi->setText(tr("No wifi detected"));
     labelWifiIcon->setPixmap(QPixmap(":/modules/images/failed.png"));
     labelWifiIcon->setScaledContents(true);
   } else {
     labelWifi->setText(tr("Wifi device:") + " (" + wifi + ")" );
     labelWifiIcon->setPixmap(QPixmap(":/modules/images/ok.png"));
     labelWifiIcon->setScaledContents(true);
   }
}

void dialogCheckHardware::getSound()
{
   QString tmp, snd;
   // Figure out the driver
   QProcess d;
   d.start(QString("cat"), QStringList() << "/dev/sndstat");
   while(d.state() == QProcess::Starting || d.state() == QProcess::Running) {
      d.waitForFinished(200);
      QCoreApplication::processEvents();
   }

   while (d.canReadLine()) {
     tmp = d.readLine().simplified();
     if ( tmp.indexOf("default") != -1 ) {
        snd = tmp.section(":", 0, 0);
        break;
     }
   }

   if ( snd.isEmpty() )
   {
     labelSound->setText(tr("No sound detected"));
     labelSoundIcon->setPixmap(QPixmap(":/modules/images/failed.png"));
     labelSoundIcon->setScaledContents(true);
   } else {
     labelSound->setText(tr("Sound device:") + " (" + snd + ")" );
     labelSoundIcon->setPixmap(QPixmap(":/modules/images/ok.png"));
     labelSoundIcon->setScaledContents(true);
   }

}

void dialogCheckHardware::slotClose()
{
  close();
}
