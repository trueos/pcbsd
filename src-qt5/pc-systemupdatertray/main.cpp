
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

#include "TrayUI.h"
#include "../config.h"

int  main(int argc, char ** argv)
{
   QtSingleApplication a(argc, argv);
   if ( a.isRunning() )
     return !(a.sendMessage("show"));

   QTranslator translator;
   QLocale mylocale;
   QString langCode = mylocale.name();
   if ( ! QFile::exists(PREFIX + "/share/pcbsd/i18n/SystemUpdaterTray_" + langCode + ".qm" ) )
     langCode.truncate(langCode.indexOf("_"));
   translator.load( QString("SystemUpdaterTray_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
   a.installTranslator( &translator );
   qDebug() << "Locale:" << langCode;

   TrayUI tray;
   QApplication::setQuitOnLastWindowClosed(false);

   // Init our program
   QObject::connect(&a, SIGNAL(messageReceived(const QString&)), &tray, SLOT(slotSingleInstance()) );
   tray.show();
   //tray.programInit();
   return  a.exec();
}
