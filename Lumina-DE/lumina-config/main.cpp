#include <QTranslator>
#include <qtsingleapplication.h>
#include <QtGui/QApplication>
#include <QDebug>
#include "MainUI.h"
#define PREFIX QString("/usr/local")

int main(int argc, char ** argv)
{
    QtSingleApplication a(argc, argv);
    if( a.isRunning() )
      return !(a.sendMessage("show"));
  
    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    
    if ( ! QFile::exists(PREFIX + "/share/lumina/i18n/" + langCode + ".qm" ) )  langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("lumina-config_") + langCode, PREFIX + "/share/lumina/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;
    

    MainUI w;
    QObject::connect(&a, SIGNAL(messageReceived(const QString&)), &w, SLOT(slotSingleInstance()) );
    w.show();

    int retCode = a.exec();
    return retCode;
}
