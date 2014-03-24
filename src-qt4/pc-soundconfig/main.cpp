#include <QDebug>
#include <QFile>
#include <qtranslator.h>
#include <qlocale.h>
#include "snddialog.h"
#include "ui_snddialog.h"
#include "../config.h"

#include <unistd.h>

int main( int argc, char ** argv )
{
   // Make sure we are running as root
   if (getuid() != 0) {
      qDebug("Error, needs to be run as root");
      exit(1);
   }
   
    QApplication a(argc, argv);

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/SoundConfig_" + langCode + ".qm" ) )
      langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("SoundConfig_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;

    sndDialog w;
    w.programInit();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    
    w.show();

    return a.exec();
}
