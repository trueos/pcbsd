#include <qapplication.h>
#include <qtranslator.h>
#include <QLocale>
#include <QTextCodec>
#include <QFile>
#include <QDebug>
#include "pfmanagerdlg.hpp"
#include "../config.h"
#include <unistd.h>
#include <sys/types.h>

int main ( int argc, char **argv )
{
    QApplication app(argc, argv);

    QLocale mylocale;
    QTranslator translator;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/PFManager_" + langCode + ".qm" ) )
     langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("PFManager_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    app.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;
    QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") ); //Force Utf-8 compliance
    
    // Make sure we are running as root
    if (getuid() != 0) {
       qDebug("Error, needs to be run as root");
       exit(1);
    }


    PFManagerDlg wnd;

    wnd.load();

    //app.setMainWidget(&wnd);
    wnd.show();
    app.connect(&app, SIGNAL(lastWindowClosed()),
                &app, SLOT(quit()));

    return app.exec();
}
