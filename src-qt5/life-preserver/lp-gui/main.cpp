#include <qtranslator.h>
#include <qlocale.h>
#include <pcbsd-SingleApplication.h>
#include <QDebug>
#include <QFile>
#include <unistd.h>
#include <sys/types.h>

#include "LPMain.h"
//#include "../config.h"

#ifndef PREFIX
#define PREFIX QString("/usr/local/")
#endif

int main( int argc, char ** argv )
{
    PCSingleApplication a(argc, argv);
    if( !a.isPrimaryProcess() ){ return 0; }

    //Check whether running as root
    if( getuid() != 0){
      qDebug() << "Life-Preserver must be started as root!";
      return 1;
    }
    
    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/lifePreserver/i18n/LifePreserver_" + langCode + ".qm" ) )
      langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("LifePreserver_") + langCode, PREFIX + "/share/lifePreserver/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;

    LPMain *w = new LPMain(); 
    w->show();

    QObject::connect( &a, SIGNAL( InputsAvailable(QStringList) ), w, SLOT( slotSingleInstance() ) );
    return a.exec();
}
