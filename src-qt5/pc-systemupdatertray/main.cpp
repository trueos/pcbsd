
#include <qtranslator.h>
#include <qtextcodec.h>
#include <qlocale.h>
#include <QDebug>
#include <QFile>
#include <pcbsd-SingleApplication.h>

#include "TrayUI.h"
#include "../config.h"

int  main(int argc, char ** argv)
{
   PCSingleApplication a(argc, argv);
   if ( !a.isPrimaryProcess() ){ return 0; }

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
