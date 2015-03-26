#include <QApplication>
#include <qtranslator.h>
#include <qlocale.h>
#include <pcbsd-SingleApplication.h>
#include <QDebug>
#include <QMessageBox>
//#include <QSplashScreen>
#include <QProcess>
#include <QFile>
#include <QTextCodec>

#include "MainUI.h"

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
    qDebug() << "Starting Up the webkitviewer";
    PCSingleApplication a(argc, argv);   
    if ( !a.isPrimaryProcess() ){
      qDebug() << " - an instance of the AppCafe is already running";
      return 0;
    }

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/webkitviewer_" + langCode + ".qm" ) )
      langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("webkitviewer_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;
    QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") ); //Force Utf-8 compliance
    
    bool debug = false;
    QString fileURL;
    if(argc > 1){ 
      for(int i=1; i<argc; i++){
        if(!debug){ 
	  debug = ( QString(argv[i])=="--debug"); 
	  if(debug){ continue; }
        }
        fileURL = QString(argv[i]);
      }
    }
    if(fileURL.isEmpty()){ qDebug() << "No File/URL supplied! exiting..."; return 1; }
    else{ qDebug() << "Opening:" << fileURL; }
      //Launch the UI
      MainUI w(debug, fileURL); 
      w.show();

      QObject::connect(&a, SIGNAL(InputsAvailable(QStringList)), &w, SLOT(slotSingleInstance()) );
      a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
      return a.exec();

}
