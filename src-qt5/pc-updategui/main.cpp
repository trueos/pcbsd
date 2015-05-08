#include <QTranslator>
#include <qtextcodec.h>
#include <qstylefactory.h>
#include <qdesktopwidget.h>
#include <pcbsd-SingleApplication.h>
#include <QDebug>
//#include "mainWin.h"
#include "mainUI.h"
#include "../config.h"

#include <unistd.h>
#include <sys/types.h>


int main( int argc, char ** argv )
{
   PCSingleApplication a(argc, argv);
   if (!a.isPrimaryProcess()){ return 0; }

    // Make sure we are running as root
    if (getuid() != 0) {
       qDebug("Error, needs to be run as root");
       exit(1);
    }
    
   QTranslator translator;
   QLocale mylocale;
   QString langCode = mylocale.name();
   if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/UpdateGUI_" + langCode + ".qm" ) )
     langCode.truncate(langCode.indexOf("_"));
   translator.load( QString("UpdateGui_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
   a.installTranslator( &translator );
   qDebug() << "Locale:" << langCode;
   QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") ); //Force Utf-8 compliance
   
   // Check the language we are running in, and set the correct font
   if (  langCode == "zh_CN" ||  langCode == "zh_TW" )
   {
     QFont f( "mingunittf", 10);
     a.setFont( f);
   } else if ( langCode == "ja" ) {
     QFont f( "vlgothic", 10);
     a.setFont( f);
   }
 
   //mainWin w;
   MainUI w;

   /*QString chroot, ip;
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
   }*/

   //w.ProgramInit(chroot, ip);
   w.show();

   QObject::connect( &a, SIGNAL( InputsAvailable(QStringList) ), &w, SLOT( slotSingleInstance() ) );
   //a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
   return a.exec();
}
