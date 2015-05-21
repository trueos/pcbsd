
#include <qtranslator.h>
#include <qtextcodec.h>
#include <qlocale.h>
#include <QDebug>
#include <QFile>
#include <pcbsd-SingleApplication.h>

#include "TrayUI.h"
#include "../config.h"

#include <unistd.h>

int  main(int argc, char ** argv)
{
   PCSingleApplication a(argc, argv);
   if ( !a.isPrimaryProcess() ){ return 0; }

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
   if ( ! QFile::exists(PREFIX + "/share/pcbsd/i18n/SystemUpdaterTray_" + langCode + ".qm" ) )
     langCode.truncate(langCode.indexOf("_"));
   translator.load( QString("SystemUpdaterTray_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
   a.installTranslator( &translator );
   qDebug() << "Locale:" << langCode;
   QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") ); //Force Utf-8 compliance
   
   TrayUI tray;
   QApplication::setQuitOnLastWindowClosed(false);

   // Init our program
   QObject::connect(&a, SIGNAL(InputsAvailable(QStringList)), &tray, SLOT(slotSingleInstance()) );
   tray.show();
   //tray.programInit();
   return  a.exec();
}
