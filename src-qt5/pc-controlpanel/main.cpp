#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTranslator>
#include <QString>
#include <QTextCodec>

#include <pcbsd-SingleApplication.h>
#include "backend/cp-itemgroup.h"

#ifndef PREFIX
#define PREFIX QString("/usr/local")
#endif

int main(int argc, char *argv[])
{        
    PCSingleApplication a(argc, argv);
    if(!a.isPrimaryProcess()){ return 0; }

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/pc-controlpanel_" + langCode + ".qm" ) )
              langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("pc-controlpanel_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );
    QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") ); //Force Utf-8 compliance
    MainWindow w;       

    w.show();

    QObject::connect(&a, SIGNAL(InputsAvailable(QStringList)), &w, SLOT(slotSingleInstance()) );
    
    return a.exec();
}
