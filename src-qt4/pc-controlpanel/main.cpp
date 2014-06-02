#include "mainwindow.h"
#include <QApplication>

#include "pcbsd-ui.h"

#include "backend/cp-itemgroup.h"

int main(int argc, char *argv[])
{        
    PCSingleApplication a(argc, argv, "pc-controlpanel");

    if ( a.isRunning() )
              return !(a.sendMessage("show"));

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/pc-controlpanel_" + langCode + ".qm" ) )
              langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("pc-controlpanel_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );

    MainWindow w;       

    w.show();

    QObject::connect(&a, SIGNAL(messageReceived(const QString&)), &w, SLOT(slotSingleInstance()) );
    
    return a.exec();
}
