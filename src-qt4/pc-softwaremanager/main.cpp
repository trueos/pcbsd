#include <QApplication>
#include <qtranslator.h>
#include <qlocale.h>
#include <qtsingleapplication.h>
#include <QDebug>
#include "mainUI.h"
//#include "../config.h"
#ifndef PREFIX
#define PREFIX QString("/usr/local")
#endif

int main( int argc, char ** argv )
{
    //Check for root permissions
    if( getuid() != 0){
      qDebug() << "pc-softwaremanager must be started as root!";
      return 1;
    }
    
    QtSingleApplication a(argc, argv);   
    if ( a.isRunning() )
      return !(a.sendMessage("show"));

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/SoftwareManager_" + langCode + ".qm" ) )
      langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("SoftwareManager_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;

    MainUI w; 

    /*if ( argc >= 2)
    {
       QString chkarg = argv[1];
       // Running in a warden jail?
       if ( chkarg == "-warden" ){
	 if ( argc == 4 ){
           w.setWardenMode(QString(argv[2]), QString(argv[3]));
	 }else {
	   qDebug() << "Usage: -warden <directory> <ip>";
	   exit(1);
         }
       }
       // Show the installed tab?
       //if ( chkarg == "-installed" ){
         //w.showInstalledTab();
       //}
    }*/

    w.ProgramInit();
	
    w.show();

    QObject::connect(&a, SIGNAL(messageReceived(const QString&)), &w, SLOT(slotSingleInstance()) );
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
