#include <qtranslator.h>
#include <qtextcodec.h>
#include <qlocale.h>
#include <QDebug>
#include <QFile>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QMessageBox>
#include <QtGui>
#include <pcbsd-SingleApplication.h>

#include "btmaingui.h"
#include "../config.h"

int  main(int argc, char ** argv)
{
   PCSingleApplication a(argc, argv);
   if ( !a.isPrimaryProcess() ){ return 0; }

   QTranslator translator;
   QLocale mylocale;
   QString langCode = mylocale.name();
   if ( ! QFile::exists(PREFIX + "/share/pcbsd/i18n/pc-bluetoothmanager_" + langCode + ".qm" ) )  langCode.truncate(langCode.indexOf("_"));
   translator.load( QString("pc-bluetoothmanager_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
   a.installTranslator( &translator );
   qDebug() << "Locale:" << langCode;

   btmaingui w;
   //QApplication::setQuitOnLastWindowClosed(false);

   // Init our program
   QObject::connect(&a, SIGNAL(InputsAvailable(QStringList)), &w, SLOT(slotSingleInstance()) );
   w.show();
   //w.firstRun();
   return  a.exec();
}
