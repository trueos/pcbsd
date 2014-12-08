#include <pcbsd-SingleApplication.h>
#include <QTranslator>
#include <QString>
#include <QLocale>
#include <QFile>
#include "mainwindow.h"

#ifndef PREFIX
#define PREFIX QString("/usr/local")
#endif

int main(int argc, char *argv[])
{
    PCSingleApplication a(argc,argv);
    if ( !a.isPrimaryProcess() ){ return 0; }

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/pc-syskeyboard_" + langCode + ".qm" ) )
                  langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("pc-syskeyboard_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );

    MainWindow w;
    w.show();
    
    QObject::connect(&a, SIGNAL(InputsAvailable(QStringList)), &w, SLOT(slotSingleInstance()) );

    return a.exec();
}
