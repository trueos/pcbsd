#include <QApplication>
#include <QDebug>
#include <QTranslator>
#include <QTextCodec>

#include <pcbsd-ui.h>
#include <pcbsd-SingleApplication.h>
#include <err.h>
#include "dialogwarden.h"

int main( int argc, char ** argv )
{
    PCSingleApplication a(argc, argv);
    if ( !a.isPrimaryProcess() ){ return 0; }

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( "/usr/local/share/pcbsd/i18n/Warden_" + langCode + ".qm" ) )
      langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("Warden_") + langCode, "/usr/local/share/pcbsd/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;
    QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") ); //Force Utf-8 compliance

    dialogWarden *w = new dialogWarden();
    w->programInit();
    w->show();
    QObject::connect(&a, SIGNAL(InputsAvailable(QStringList)), w, SLOT(slotSingleInstance()) );
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
