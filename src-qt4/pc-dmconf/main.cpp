#include <QMessageBox>
#include <qtranslator.h>
#include <qlocale.h>
#include <qtsingleapplication.h>
#include <QFile>
#include "mainwindow.h"
#include "../config.h"

#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{   
    QtSingleApplication a(argc, argv);

    if ( a.isRunning() )
          return !(a.sendMessage("show"));

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists(PREFIX + "/share/pcbsd/i18n/pc-dmconf_" + langCode + ".qm" ) )
        langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("pc-dmconf_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );    

    MainWindow w;
    w.show();

    //Check for root
    if (0 != geteuid())
    {
        QMessageBox msg;
        msg.setText(w.tr("You should run this application as root"));
        msg.exec();
        exit(2);
    }

    QObject::connect(&a, SIGNAL(messageReceived(const QString &)), &w, SLOT(slotSingleInstance()));

    return a.exec();
}
