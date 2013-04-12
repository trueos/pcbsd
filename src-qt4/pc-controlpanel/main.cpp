#include <qtranslator.h>
#include <QtGui/QApplication>
#include <qlocale.h>
#include <qtsingleapplication.h>
#include <QFile>
#include "mainwnd.h"
#include "../config.h"

int main(int argc, char *argv[])
{
    QtSingleApplication a(argc, argv);
    if ( a.isRunning() )
      return !(a.sendMessage("show"));

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists(PREFIX + "/share/pcbsd/i18n/pc-controlpanel_" + langCode + ".qm" ) )
    langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("pc-controlpanel_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );

    MainWnd w;
    w.show();

    QObject::connect(&a, SIGNAL(messageReceived(const QString &)), &w, SLOT(slotSingleInstance()));
    return a.exec();
}
