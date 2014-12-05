#include <qapplication.h>
#include <qtranslator.h>
#include <qlocale.h>
#include <QDebug>
#include "pbi.h"
#include "../config.h"

int main( int argc, char ** argv )
{
    QApplication a(argc, argv, TRUE);

    QTranslator translator( 0 );
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/PBI_" + langCode + ".qm" ) )
      langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("PBI_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;

    PBI w; 
    QString pbifile;
	
    if ( argc == 2)
        pbifile = argv[1];
    else
	return -1;
    
    qDebug() << "Loading PBI: " << pbifile;
    w.ProgramInit(pbifile);
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
