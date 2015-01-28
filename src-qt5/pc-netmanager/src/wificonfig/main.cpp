#include <QApplication>
#include <qtranslator.h>
#include <QTextCodec>
#include <QDebug>
#include "wificonfigwidgetbase.h"
#include "ui_wificonfigwidgetbase.h"
#include "../../../config.h"

int main( int argc, char ** argv )
{
    QApplication a(argc, argv);

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/wificonfig_" + langCode + ".qm" ) )
      langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("wificonfig_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;
    QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") ); //Force Utf-8 compliance
    
    wificonfigwidgetbase w;
    //a.setMainWidget(&w);
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    
    if ( argc == 2)
    {
		QString Device = argv[1];
		w.setDevice(Device);
		w.show();
     } else if ( argc == 3  ) {
	 QString val = argv[1];
	 if ( val != "info" && val != "wpaonly" )
	   exit(1);  

         if ( val == "wpaonly" )
	   w.setModeWPAOnly(); 
	 
	 QString Device = argv[2];
	 w.setDevice(Device);
	 w.showInfo();
	 w.show();
     } else {
		exit(1);
     }

    return a.exec();
}
