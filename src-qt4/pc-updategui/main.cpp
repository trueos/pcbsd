#include <qapplication.h>
#include <qtranslator.h>
#include <qtextcodec.h>
#include <qstylefactory.h>
#include <qdesktopwidget.h>
#include <qtsingleapplication.h>
#include <QDebug>
#include "mainWin.h"
#include "../config.h"

int main( int argc, char ** argv )
{
   QtSingleApplication a(argc, argv);
   if (a.isRunning())
     return !(a.sendMessage("show"));

   QTranslator translator;
   QLocale mylocale;
   QString langCode = mylocale.name();
   if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/UpdateGUI_" + langCode + ".qm" ) )
     langCode.truncate(langCode.indexOf("_"));
   translator.load( QString("UpdateGui_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
   a.installTranslator( &translator );
   qDebug() << "Locale:" << langCode;

   // Check the language we are running in, and set the correct font
   if (  langCode == "zh_CN" ||  langCode == "zh_TW" )
   {
     QFont f( "mingunittf", 10);
     a.setFont( f);
   } else if ( langCode == "ja" ) {
     QFont f( "vlgothic", 10);
     a.setFont( f);
   }
 
   mainWin w;

   QString chroot, ip;
   if ( argc >= 2)
   {
       QString chkarg = argv[1];
       // Running in a warden jail?
       if ( chkarg == "-warden" ) {
         if ( argc == 4 ){
	   chroot = argv[2];
	   ip = argv[3];
         } else {
           qDebug() << "Usage: -warden <directory> <ip>";
           exit(1);
         }
       }
   }

   w.ProgramInit(chroot, ip);
   w.show();

   QObject::connect( &a, SIGNAL( messageReceived(const QString &) ), &w, SLOT( slotSingleInstance() ) );
   a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
   return a.exec();
}
