#include <qtranslator.h>
#include <qlocale.h>
#include <QDebug>
#include <QFile>
#include <QApplication>
#include "networkman.h"
#include "../../../config.h"

int main( int argc, char ** argv )
{
    QApplication a(argc, argv);

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists(PREFIX + "/share/pcbsd/i18n/NetworkManager_" + langCode + ".qm" ) )
      langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("NetworkManager_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;

    NetworkMan w;

    w.Init();

    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
