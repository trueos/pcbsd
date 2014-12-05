
#include <qtranslator.h>
#include <qtextcodec.h>
#include <qlocale.h>
#include <QDebug>
#include <QFile>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QMessageBox>
#include <QtGui>
#include <qtsingleapplication.h>

#include "btTray.h"
#include "../config.h"

#include <unistd.h>
#include <sys/types.h>

int  main(int argc, char ** argv)
{
	
   //Make sure this is running as root (required to restart particular BT services)
    if( getuid() != 0){
      qDebug() << "pc-bluetoothtray must be started as root!";
      system("pc-su pc-bluetoothtray &");
      return 1;
    }
	
   QtSingleApplication a(argc, argv);
   if ( a.isRunning() )
     return !(a.sendMessage("show"));

   QTranslator translator;
   QLocale mylocale;
   QString langCode = mylocale.name();
   if ( ! QFile::exists(PREFIX + "/share/pcbsd/i18n/pc-bluetoothtray_" + langCode + ".qm" ) )  langCode.truncate(langCode.indexOf("_"));
   translator.load( QString("pc-bluetoothtray_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
   a.installTranslator( &translator );
   qDebug() << "Locale:" << langCode;
   
   BluetoothTray tray;
   QApplication::setQuitOnLastWindowClosed(false);

   // Init our program
   QObject::connect(&a, SIGNAL(messageReceived(const QString&)), &tray, SLOT(slotSingleInstance()) );

   tray.programInit();
   return  a.exec();
}
