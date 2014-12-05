#include <qapplication.h>
#include <qtranslator.h>
#include <qtextcodec.h>
#include <QLocale>
#include <QDebug>
#include <QFile>
#include "crashhandler.h"
#include "../config.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/CrashHandler_" + langCode + ".qm" ) )
      langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("CrashHandler_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;
    
    CrashHandler w;
    if (a.argc() != 4) {
	//Not enough args, die
	return 1;
    }
    w.setup(a.argv()[1], a.argv()[2], a.argv()[3]);
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
