
#include <qtranslator.h>
#include <qtextcodec.h>
#include <qlocale.h>
#include <QDebug>
#include <QFile>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QMessageBox>
#include <QtGui>
#include <QTextCodec>
#include <QSystemTrayIcon>

#include <QProcessEnvironment>
#include <pcbsd-SingleApplication.h>

#include "mountTray.h"
#include "../config.h"

#include <unistd.h>

bool DEBUG_MODE;
QString DEVICEDIR;
//QString MOUNTDIR;
//QString ROOTMOUNTDIR;
//QString USERNAME;

int  main(int argc, char ** argv)
{
   //Check for root permissions
   QString id = QProcessEnvironment::systemEnvironment().toStringList().filter("LOGNAME=").join(" ").remove("LOGNAME=").simplified();
   //qDebug() << id;
    if( id == "root" ){
      qDebug() << "pc-mounttray should not be started with root permissions";
      exit(1);
    }/*else{
      USERNAME = id;
    }*/
   //Check for "-v" flag for debugging
   QString flag = QString(argv[1]);
   if( flag == "-v" || flag == "-debug" ){ DEBUG_MODE=true; }
   else{ DEBUG_MODE=false; }
   
   //Now start the application
   DEVICEDIR = "/dev/";
   /*MOUNTDIR = QDir::homePath()+"/Media/";
   //ROOTMOUNTDIR = "/media/";
   if(!QFile::exists(MOUNTDIR)){
     QDir dir(MOUNTDIR);
     bool ok = dir.mkpath(dir.absolutePath());
     if(!ok){
       qDebug() << "Error: Could not create the user media directory:" << MOUNTDIR;
       qDebug() << " - No way to mount devices: exiting....";
       exit(1);
     }
   }*/

   
   PCSingleApplication a(argc, argv);
   if ( !a.isPrimaryProcess() ){ return 0; }

   bool ready = false;
   for(int i=0; i<60 && !ready; i++){
      ready = QSystemTrayIcon::isSystemTrayAvailable();
      if(!ready){
	//Pause for 5 seconds
        sleep(5); //don't worry about stopping event handling - nothing running yet
      }
   }
   if(!ready){
     qDebug() << "Could not find any available system tray after 5 minutes: exiting....";
     return 1;
   }
   
   QTranslator translator;
   QLocale mylocale;
   QString langCode = mylocale.name();
   if ( ! QFile::exists(PREFIX + "/share/pcbsd/i18n/MountTray_" + langCode + ".qm" ) )  langCode.truncate(langCode.indexOf("_"));
   translator.load( QString("MountTray_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
   a.installTranslator( &translator );
   qDebug() << "Locale:" << langCode;
   QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") ); //Force Utf-8 compliance
   
   MountTray tray;
   QApplication::setQuitOnLastWindowClosed(false);

   // Init our program
   QObject::connect(&a, SIGNAL(InputsAvailable(QStringList)), &tray, SLOT(slotSingleInstance()) );
   QObject::connect(&a, SIGNAL(aboutToQuit()),&tray,SLOT(closeTray()));
   tray.programInit();
   return  a.exec();
}
