#include <qtranslator.h>
#include <qlocale.h>
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <unistd.h>
#include <sys/types.h>
#include "mainwindow.h"
#include "../config.h"

int main(int argc, char *argv[])
{   
    QApplication a(argc, argv);

    if ( qApp->argc() <= 1 ) {
       qDebug() << "Error: Need to specify argument to run!";
       exit(1);
    }

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists(PREFIX + "/share/pcbsd/i18n/qsudo_" + langCode + ".qm" ) )
        langCode.truncate(langCode.indexOf("_"));

    if ( QFile::exists(PREFIX + "/share/pcbsd/i18n/qsudo_" + langCode + ".qm" ) ) {
      translator.load( QString("qsudo_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
      a.installTranslator( &translator );
    }

    MainWindow w;
    w.ProgramInit();
    w.show();

    return a.exec();
}
