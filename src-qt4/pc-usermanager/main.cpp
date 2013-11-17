#include <qtranslator.h>
#include <QApplication>
#include <qlocale.h>
#include <QDebug>
#include "pcbsdusermanager.h"
#include "../config.h"

int main( int argc, char ** argv )
{

    QApplication a(argc, argv);

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists(PREFIX + "/share/pcbsd/i18n/pcbsdusermanager_" + langCode + ".qm" ) )
      langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("pcbsdusermanager_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;

    PCBSDUserManager w; 

    if ( argc >= 2)
    {
       QString chkarg = argv[1];
       // Running in a warden jail?
       if ( chkarg == "-warden" ) {
         if ( argc == 4 ){
           w.setWardenMode(QString(argv[2]), QString(argv[3]));
         } else {
           qDebug() << "Usage: -warden <directory> <ip>";
           exit(1);
         }
       }
    }
	
    w.setupUMDialogs();
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
