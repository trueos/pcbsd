#include <QApplication>
#include <qtranslator.h>
#include <qlocale.h>
#include <qtsingleapplication.h>
#include <QDebug>
//#include <QMessageBox>
//#include <QSplashScreen>
#include <QProcess>
#include <QFile>

#include "MainUI.h"
//#include "migrateUI.h"

#include <unistd.h>
#include <sys/types.h>

//#include "../config.h"
#ifndef PREFIX
#define PREFIX QString("/usr/local")
#endif

int main( int argc, char ** argv )
{
    //Check for root permissions
    if( getuid() == 0){
      qDebug() << "pc-softwaremanager must not be started as root!";
      return 1;
    }
    qDebug() << "Starting Up the AppCafe";
    QtSingleApplication a(argc, argv);   
    if ( a.isRunning() ){
      qDebug() << " - an instance of the AppCafe is already running";
      return !(a.sendMessage("show"));
    }

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/SoftwareManager_" + langCode + ".qm" ) )
      langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("SoftwareManager_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;

    bool debug = false;
    if(argc > 1){ debug = ( QString(argv[1])=="--debug"); }
    
      //Launch the UI
      MainUI w(debug); 
      w.show();

      QObject::connect(&a, SIGNAL(messageReceived(const QString&)), &w, SLOT(slotSingleInstance()) );
      a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
      return a.exec();

}
