#include <qapplication.h>
#include <qtranslator.h>
#include <qtextcodec.h>
#include "adsldap.h"
#include <QDebug>
#include <QFile>
#include <QTextCodec>
#include <unistd.h>
#include <sys/types.h>

int main( int argc, char ** argv )
{
   QApplication a(argc, argv);
   
   QTranslator translator;
   QLocale mylocale;
   QString langCode = mylocale.name();
   if ( ! QFile::exists( "/usr/local/share/pcbsd/i18n/PCADSLDAP_" + langCode + ".qm" ) )
     langCode.truncate(langCode.indexOf("_"));
   translator.load( QString("PCADSLDAP_") + langCode, "/usr/local/share/pcbsd/i18n/" );
   a.installTranslator( &translator );
   qDebug() << "Locale:" << langCode;
    QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") ); //Force Utf-8 compliance
   // Make sure we are running as root
   if (getuid() != 0) {
      qDebug("Error, needs to be run as root");
      exit(1);
   }


   pcADSLDAP w;

   w.programInit();

   w.show();
   a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
   return a.exec();
}
