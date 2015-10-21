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
#include <QDesktopWidget>

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
  qDebug() << "Starting Up the AppCafe";
  PCSingleApplication a(argc, argv);   
  if ( !a.isPrimaryProcess() ){
    qDebug() << " - an instance of the AppCafe is already running";
    return 0;
  }

  QTranslator translator;
  QLocale mylocale;
  QString langCode = mylocale.name();
  if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/SoftwareManager_" + langCode + ".qm" ) )
     langCode.truncate(langCode.indexOf("_"));
  translator.load( QString("SoftwareManager_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
  a.installTranslator( &translator );
  qDebug() << "Locale:" << langCode;
  QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") ); //Force Utf-8 compliance
    
  bool debug = false;
  if(argc > 1){ debug = ( QString(argv[1])=="--debug"); }
    
  //Launch the UI
  MainUI w(debug); 
  if(QApplication::desktop()->availableGeometry().width() < 1024){
    w.showMaximized();
  }else{
    w.show();
  }

  QObject::connect(&a, SIGNAL(InputsAvailable(QStringList)), &w, SLOT(slotSingleInstance()) );
  a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
  return a.exec();
}
