#include <QApplication>
#include <qlocale.h>
#include <pcbsd-SingleApplication.h>
#include <QDebug>
#include <QFile>
#include <QObject>
#include <QString>
#include <QTranslator>
#include "networkman.h"
#include "../../../config.h"

int main( int argc, char ** argv )
{
    PCSingleApplication a(argc, argv);
    if (!a.isPrimaryProcess()){ return 0; }

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

    // Are we running via the system-installer?
    if ( argc == 2 ) {
       QString tmp = argv[1];
       if ( tmp == "-installer" )
         w.setInstallerMode();
    }

    w.show();

    a.connect( &a, SIGNAL( InputsAvailable(QStringList) ), &w, SLOT( slotSingleInstance() ) );
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
