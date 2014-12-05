#include <qtranslator.h>
#include <qtextcodec.h>
#include <qlocale.h>
#include <QDebug>
#include <QFile>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QMessageBox>
#include <QtGui>
#include <qtsingleapplication.h>

#include "btmaingui.h"
#include "../config.h"

int  main(int argc, char ** argv)
{
   QtSingleApplication a(argc, argv);
   if ( a.isRunning() )
     return !(a.sendMessage("show"));

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
   QObject::connect(&a, SIGNAL(messageReceived(const QString&)), &w, SLOT(slotSingleInstance()) );
   w.show();
   //w.firstRun();
   return  a.exec();
}
