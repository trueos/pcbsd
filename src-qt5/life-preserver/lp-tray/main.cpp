#include <qtranslator.h>
#include <qlocale.h>
#include <pcbsd-SingleApplication.h>
#include <QDebug>
#include <QFile>
#include <QTextCodec>
#include <unistd.h>
#include <sys/types.h>

#include "LPTray.h"
//#include "../config.h"

#ifndef PREFIX
#define PREFIX QString("/usr/local/")
#endif

int main( int argc, char ** argv )
{
    PCSingleApplication a(argc, argv);
    if(!a.isPrimaryProcess()){ return 0; }

    //Check whether running as root
    if( getuid() == 0){
      qDebug() << "Life-Preserver Tray must not be started as root!";
      return 1;
    }
 
   bool ready = false;
   for(int i=0; i<60 && !ready; i++){
      ready = QSystemTrayIcon::isSystemTrayAvailable();
      if(!ready){
	//Pause for 5 seconds
        sleep(5); //don't worry about stopping event handling - nothing running yet
      }
   }
   if(!ready){
     qDebug() << "Could not find any available system tray after 5 minutes: exiting....";
     return 1;
   }    
    
    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/lifePreserver/i18n/LifePreserverTray_" + langCode + ".qm" ) )
      langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("LifePreserverTray_") + langCode, PREFIX + "/share/lifePreserver/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;
    QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") ); //Force Utf-8 compliance
    LPTray *w = new LPTray(); 
    w->show();

    QObject::connect( &a, SIGNAL( InputsAvailable(QStringList) ), w, SLOT( slotSingleInstance() ) );
    int ret = -1;
    while( w->isVisible() ){
      //Make sure the application does not exit while the tray is still open
      ret = a.exec();
    }
    return ret;
}
