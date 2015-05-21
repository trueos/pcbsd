 
#include <err.h>

#include <qtimer.h>
#include <QTranslator>
#include <QDebug>
#include <QFile>
#include <qtextcodec.h>
#include <QApplication>
#include <QLocale>
#include <QTextCodec>

#include <NetworkTray.h>
#include "../../../config.h"
#include <unistd.h>



int  main(int argc, char *argv[]) {

   QApplication a(argc, argv);

   bool ready = false;
   for(int i=0; i<60 && !ready; i++){
      ready = QSystemTrayIcon::isSystemTrayAvailable();
      if(!ready){
	//Pause for 5 seconds
        sleep(5); //don't worry about stopping event handling - nothing running yet
      }
   }
   if(!ready){
     qDebug() << "Could not find any available system tray after 5 minutes: exiting....";
     return 1;
   }
   
   QTranslator translator;
   QLocale mylocale;
   QString langCode = mylocale.name();
   if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/NetworkTray_" + langCode + ".qm" ) )
     langCode.truncate(langCode.indexOf("_"));
   translator.load( QString("NetworkTray_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
   a.installTranslator( &translator );
   qDebug() << "Locale:" << langCode;
   QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") ); //Force Utf-8 compliance
   
   NetworkTray tray;
   QApplication::setQuitOnLastWindowClosed(false);

   if ( argc == 2)
   {
       QString Device = argv[1];
       tray.programInit(Device);
   } else {
      exit(1);
   }
 

   return  a.exec();
}
