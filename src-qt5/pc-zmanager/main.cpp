#include "zmanagerwindow.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <qtranslator.h>
#include <qlocale.h>
#include <QTextCodec>

#include <pcbsd-SingleApplication.h>

#include <unistd.h>
#include <sys/types.h>

#ifndef PREFIX
#define PREFIX QString("/usr/local")
#endif

int main(int argc, char *argv[])
{
    // Make sure we are running as root
    if (getuid() != 0) {
      qDebug("Error, needs to be run as root");
      exit(1);
    }

   PCSingleApplication a(argc, argv);
    if ( !a.isPrimaryProcess() ){ return 0; }

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/ZManager_" + langCode + ".qm" ) )
      langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("ZManager_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;
    QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") ); //Force Utf-8 compliance
    
    ZManagerWindow w;


    w.ProgramInit();

    w.show();

    QObject::connect(&a, SIGNAL(InputsAvailable(QStringList)), &w, SLOT(slotSingleInstance()) );
//    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
