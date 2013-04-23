#include <qapplication.h>
#include <qtranslator.h>
#include <qtextcodec.h>
#include <qstylefactory.h>
#include <qdesktopwidget.h>
#include <QDebug>
#include <pcbsd-ui.h>
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
   if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/PackageManager_" + langCode + ".qm" ) )
     langCode.truncate(langCode.indexOf("_"));
   translator.load( QString("PackageManager_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
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

   QString chroot;
   if ( argc >= 2)
   {
       QString chkarg = argv[1];
       // Running in a chroot
       if ( chkarg == "-chroot" )
         if ( argc == 3 ) {
	   chroot = argv[2];
         } else {
           qDebug() << "Usage: -chroot <directory>";
           exit(1);
         }
   }

   w.ProgramInit(chroot);
   w.show();

   QObject::connect( &a, SIGNAL( messageReceived(const QString &) ), &w, SLOT( slotSingleInstance() ) );
   a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
   return a.exec();
}
