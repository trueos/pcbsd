#include "zmanagerwindow.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <qtranslator.h>
#include <qlocale.h>
#include <qtsingleapplication.h>

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

    QtSingleApplication a("Disk Manager",argc, argv);
    if ( a.isRunning() )
      return !(a.sendMessage("show"));

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/ZManager_" + langCode + ".qm" ) )
      langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("ZManager_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;

    ZManagerWindow w;


    w.ProgramInit();

    w.show();

    QObject::connect(&a, SIGNAL(messageReceived(const QString&)), &w, SLOT(slotSingleInstance()) );
//    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
