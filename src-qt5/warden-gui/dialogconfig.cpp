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
#include "dialogconfig.h"
#include <qsettings.h>
#include <qfiledialog.h>
#include "pcbsd-utils.h"
#include "pcbsd-netif.h"


void dialogConfig::programInit()
{
    // Load NICS
    detectDev();

    // Read in the configuration and populate the widget
    readConfig();
}

void dialogConfig::detectDev()
{
    QString tmp;
    bool foundDev;

    comboNIC->clear();
    
    QStringList ifs = NetworkInterface::getInterfaces();
    for ( QStringList::Iterator it = ifs.begin(); it != ifs.end(); ++it )
    {
       foundDev = false;

       QString dev = *it;
       if (dev.indexOf("lo") == 0 
	   || dev.indexOf("fwe") == 0
	   || dev.indexOf("fwip") == 0
	   || dev.indexOf("ipfw") == 0
	   || dev.indexOf("plip") == 0
	   || dev.indexOf("pfsync") == 0
	   || dev.indexOf("pflog") == 0
	   || dev.indexOf("usbus") == 0
	   || dev.indexOf("tun") == 0)
           continue;

	NetworkInterface ifr(dev);
        if (ifr.isWireless() && dev.indexOf("wlan") != 0 ) 
           continue;

       comboNIC->addItem(dev);
   }

}

void dialogConfig::readConfig()
{
    QSettings settings("PCBSD");
    
    NIC = pcbsd::Utils::getValFromPCConf("/usr/local/etc/warden.conf", "NIC");
    TMPDIR = pcbsd::Utils::getValFromPCConf("/usr/local/etc/warden.conf", "WTMP");
    JailDir = pcbsd::Utils::getValFromPCConf("/usr/local/etc/warden.conf", "JDIR");
    
    lineJailDir->setText(JailDir);
    lineTMPDIR->setText(TMPDIR);
    for ( int i = 0; i < comboNIC->count(); ++i )
      if ( comboNIC->itemText(i) == NIC )
         comboNIC->setCurrentIndex(i);
}


void dialogConfig::slotChangeJailDir()
{
    QString tmp;
    
       tmp = QFileDialog::getExistingDirectory ( this, tr("Select Jail Directory"), JailDir, QFileDialog::ShowDirsOnly );
       if ( ! tmp.isEmpty() )
       {
	   lineJailDir->setText(tmp);
	   JailDir = tmp;
       }
}


void dialogConfig::slotChangeTMPDIR()
{
    QString tmp;
    
       tmp = QFileDialog::getExistingDirectory ( this, tr("Select Temp Directory"), TMPDIR, QFileDialog::ShowDirsOnly );
       if ( ! tmp.isEmpty() )
       {
	   lineTMPDIR->setText(tmp);
	   TMPDIR = tmp;
       }
}


void dialogConfig::slotSave()
{
    
    pcbsd::Utils::setValPCConf("/usr/local/etc/warden.conf", "NIC", comboNIC->currentText());
    pcbsd::Utils::setValPCConf("/usr/local/etc/warden.conf", "WTMP", lineTMPDIR->text());
    pcbsd::Utils::setValPCConf("/usr/local/etc/warden.conf", "JDIR", lineJailDir->text());
    
    emit saved();
    close();
}


void dialogConfig::slotCancel()
{
    close();
}
