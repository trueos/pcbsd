#include <QApplication>
#include <QDebug>
#include <QTranslator>
#include <pcbsd-ui.h>
#include <err.h>
#include "dialogwarden.h"

int main( int argc, char ** argv )
{
    QtSingleApplication a(argc, argv);
    if ( a.isRunning() )
      return !(a.sendMessage("show"));

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( "/usr/local/share/warden/i18n/Warden_" + langCode + ".qm" ) )
      langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("Warden_") + langCode, "/usr/local/share/warden/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;


    dialogWarden *w = new dialogWarden();
    w->programInit();
    w->show();
    QObject::connect(&a, SIGNAL(messageReceived(const QString&)), w, SLOT(slotSingleInstance()) );
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
