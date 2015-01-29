 
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


int  main(int argc, char *argv[]) {

   QApplication a(argc, argv);

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
