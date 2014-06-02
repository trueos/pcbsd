//#include <QtGui/QApplication>
#include "pcbsd-ui.h"
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    PCApplication a(argc, argv);

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/pc-bugreport_" + langCode + ".qm" ) )
              langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("pc-bugreport_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );

    MainWindow w;
    w.show();


    return a.exec();
}
