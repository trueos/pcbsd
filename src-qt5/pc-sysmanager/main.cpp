#include <qapplication.h>
#include <qtranslator.h>
#include <qtextcodec.h>
#include <qstylefactory.h>
#include <qdesktopwidget.h>
#include <QDebug>
#include <unistd.h>
#include <sys/types.h>
#include "pbsystemtab.h"
#include "../config.h"

int main( int argc, char ** argv )
{
   QApplication a(argc, argv);

   QTranslator translator;
   QLocale mylocale;
   QString langCode = mylocale.name();
   if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/PBSystem_" + langCode + ".qm" ) )
     langCode.truncate(langCode.indexOf("_"));
   translator.load( QString("PBSystem_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
   a.installTranslator( &translator );
   qDebug() << "Locale:" << langCode;
   QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") ); //Force Utf-8 compliance
   
   // Make sure we are running as root
   if (getuid() != 0) {
      qDebug("Error, needs to be run as root");
      exit(1);
   }

   PBSystemTab w;

   w.ProgramInit();

   QString Tmp = QString(argv[1]);
   if ( Tmp == "-updates" )
     w.changeOpenTab(1);

   w.show();
   a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
   return a.exec();
}
