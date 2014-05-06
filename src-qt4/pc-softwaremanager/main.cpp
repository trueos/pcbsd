#include <QApplication>
#include <qtranslator.h>
#include <qlocale.h>
#include <qtsingleapplication.h>
#include <QDebug>
#include <QSplashScreen>

#include "mainUI.h"
#include "migrateUI.h"

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

    //Check for the old PBI system, and prompt to migrate if needed
    QDir dir("/var/db/pbi/installed");
    if( !dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot).isEmpty() ){
      //Still on the old system - prompt to migrate to PBI-NG
      MigrateUI w;
      w.show();
      QObject::connect(&a, SIGNAL(messageReceived(const QString&)), &w, SLOT(slotSingleInstance()) );
      a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
      return a.exec();
    }else{
     QPixmap pix(":/icons/splash.png");
     QSplashScreen SS(pix);
	SS.showMessage(QObject::tr("Starting Up.."), Qt::AlignHCenter | Qt::AlignBottom);
	SS.show();
	a.processEvents();
	a.processEvents();

      //Already on PBI-NG
      MainUI w; 
      w.ProgramInit();
      w.show();
      SS.finish(&w);
      QObject::connect(&a, SIGNAL(messageReceived(const QString&)), &w, SLOT(slotSingleInstance()) );
      a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
      return a.exec();
    }
}
