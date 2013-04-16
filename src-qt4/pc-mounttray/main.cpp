
#include <qtranslator.h>
#include <qtextcodec.h>
#include <qlocale.h>
#include <QDebug>
#include <QFile>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QMessageBox>
#include <QtGui>
#include <QProcessEnvironment>
#include <qtsingleapplication.h>

#include "mountTray.h"
#include "../config.h"

bool DEBUG_MODE;
QString DEVICEDIR;
QString MOUNTDIR;

int  main(int argc, char ** argv)
{
   //Check for root permissions
   QString id = QProcessEnvironment::systemEnvironment().toStringList().filter("LOGNAME=").join(" ").remove("LOGNAME=").simplified();
   //qDebug() << id;
    if( id != "root" ){
      qDebug() << "pc-mounttray requires root permissions for device management!";
      qDebug() << " - HINT: run 'sudo pc-mounttray' instead";
      exit(1);
    }
   //Check for "-v" flag for debugging
   QString flag = QString(argv[1]);
   if( flag == "-v" || flag == "-debug" ){ DEBUG_MODE=TRUE; }
   else{ DEBUG_MODE=FALSE; }
   
   //Now start the application
   DEVICEDIR = "/dev/";
   MOUNTDIR = "/media/";
   QtSingleApplication a(argc, argv);
   if ( a.isRunning() )
     return !(a.sendMessage("show"));

   QTranslator translator;
   QLocale mylocale;
   QString langCode = mylocale.name();
   if ( ! QFile::exists(PREFIX + "/share/pcbsd/i18n/MountTray_" + langCode + ".qm" ) )  langCode.truncate(langCode.indexOf("_"));
   translator.load( QString("MountTray_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
   a.installTranslator( &translator );
   qDebug() << "Locale:" << langCode;
   
   MountTray tray;
   QApplication::setQuitOnLastWindowClosed(false);

   // Init our program
   QObject::connect(&a, SIGNAL(messageReceived(const QString&)), &tray, SLOT(slotSingleInstance()) );
   QObject::connect(&a, SIGNAL(aboutToQuit()),&tray,SLOT(closeTray()));
   tray.programInit();
   return  a.exec();
}
