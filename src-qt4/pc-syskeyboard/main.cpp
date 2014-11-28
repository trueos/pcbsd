#include "mainwindow.h"
#include <QApplication>

#include "pcbsd-ui.h"

int main(int argc, char *argv[])
{
    PCSingleApplication a(argc,argv,"pc-syskeyboard");

    if ( a.isRunning() )
                  return !(a.sendMessage("show"));

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/pc-syskeyboard_" + langCode + ".qm" ) )
                  langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("pc-syskeyboard_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );

    MainWindow w;
    w.show();
    
    QObject::connect(&a, SIGNAL(messageReceived(const QString&)), &w, SLOT(slotSingleInstance()) );

    return a.exec();
}
