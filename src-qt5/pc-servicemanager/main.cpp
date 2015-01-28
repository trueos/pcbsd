#include <qapplication.h>
#include <qtranslator.h>
#include <qtextcodec.h>
#include <qstylefactory.h>
#include <qdesktopwidget.h>
#include <QDebug>
#include <unistd.h>
#include <sys/types.h>
#include "servicemanager.h"
#include "../config.h"

int main( int argc, char ** argv )
{
   QApplication a(argc, argv);

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/ServiceManager_" + langCode + ".qm" ) )
      langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("ServiceManager_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;
    QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") ); //Force Utf-8 compliance
    
    QString chroot, ip;
    if ( argc >= 2)
    {
       QString chkarg = argv[1];
       // Running in a warden jail?
       if ( chkarg == "-warden" ){
         if ( argc == 4 ) {
           chroot = argv[2];
           ip = argv[3];
         }else{
           qDebug() << "Usage: -warden <directory> <ip>";
           exit(1);
         }
       }
    }

    // Make sure we are running as root
    if (getuid() != 0) {
       qDebug("Error, needs to be run as root");
       exit(1);
    }

    //Init GUI
    ServiceManager w;

    w.ProgramInit(chroot, ip);

    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
