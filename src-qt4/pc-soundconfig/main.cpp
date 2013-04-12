#include <QDebug>
#include <QFile>
#include <qtranslator.h>
#include <qlocale.h>
#include "snddialog.h"
#include "ui_snddialog.h"
#include "../config.h"

int main( int argc, char ** argv )
{
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
