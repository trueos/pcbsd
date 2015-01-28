#include <QTranslator>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QTextCodec>
#include <pcbsd-SingleApplication.h>
#include "mainUI.h"
#include <unistd.h>
#include <sys/types.h>

#ifndef PREFIX
#define PREFIX QString("/usr/local/")
#endif

int main(int argc, char ** argv)
{
    PCSingleApplication a(argc, argv);
    if(!a.isPrimaryProcess()){ return 0; }
    
    //Check for root
    if (0 != getuid())
    {
        QMessageBox msg;
        msg.setText(QObject::tr("This application requires administrator privileges for operation."));
        msg.exec();
        exit(2);
    }
    
    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    
    if ( ! QFile::exists(PREFIX + "/share/pcbsd/i18n/pc-bootconfig_" + langCode + ".qm" ) )  langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("pc-bootconfig_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;
    QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") ); //Force Utf-8 compliance

    mainUI w;
    QObject::connect(&a, SIGNAL(InputsAvailable(QStringList)), &w, SLOT(slotSingleInstance()) );
    w.show();

    int retCode = a.exec();
    return retCode;
}
