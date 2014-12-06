#include <qtranslator.h>
#include <qlocale.h>
#include <qtsingleapplication.h>
#include <QDebug>
#include <QFile>
#include <unistd.h>
#include <sys/types.h>

#include "MixerTray.h"
#include "MixerGUI.h"

//#include "../config.h"

#ifndef PREFIX
#define PREFIX QString("/usr/local/")
#endif

int main( int argc, char ** argv )
{
    QtSingleApplication a(argc, argv);
    if (a.isRunning())
      return !(a.sendMessage("show"));

    //Check whether running as root
    if( getuid() == 0){
      qDebug() << "pc-mixer must not be started as root!";
      return 1;
    }
    
    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/pc-mixer_" + langCode + ".qm" ) )
      langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("pc-mixer_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;

    if(argc>1 && QString(argv[1])=="-notray"){
      //Start up the GUI (no system tray mode)
      MixerGUI *w = new MixerGUI();
      w->updateGUI();
      w->show();
      QObject::connect( &a, SIGNAL( messageReceived(const QString &) ), w, SLOT( slotSingleInstance() ) );
    }else{
      //Start up the tray
      MixerTray *w = new MixerTray(); 
      w->show();
      QObject::connect( &a, SIGNAL( messageReceived(const QString &) ), w, SLOT( slotSingleInstance() ) );
    }
    
    int ret = a.exec();
    return ret;
}
