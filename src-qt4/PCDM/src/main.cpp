/* PCDM Login Manager:
*  Written by Ken Moore (ken@pcbsd.org) 2012/2013
*  Copyright(c) 2013 by the PC-BSD Project
*  Available under the 3-clause BSD license
*/

#include <QtGui/QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDesktopWidget>
#include <QFile>
#include <QSplashScreen>
#include <QTime>
#include <QDebug>

#include "pcdm-gui.h"
#include "pcdm-backend.h"
#include "pcdm-config.h"
#include "pcdm-xprocess.h"

#define TMPLANGFILE QString("/tmp/.PCDMLang")

//Make sure that prefix is set
//#ifndef prefix
//#define prefix "/usr/local/"
//#endif

int main(int argc, char *argv[])
{
  int returnCode = 0;
  //QTime clock;
  //clock.start();
  Backend::checkLocalDirs();  // Create and fill "/usr/local/share/PCDM" if needed
  Backend::openLogFile("/usr/local/share/PCDM/PCDM.log");  
  //qDebug() << "Backend Checks Finished:" << QString::number(clock.elapsed())+" ms";
  //Check for the flag to try and auto-login
  bool ALtriggered = FALSE;
  if(QString(argv[1]) == "-AutoLogin"){ ALtriggered=TRUE; }
  
  QString changeLang; 
  // Load the configuration file
  QString confFile = "/usr/local/share/PCDM/pcdm.conf";
  if(!QFile::exists(confFile)){ confFile = ":samples/pcdm.conf"; }
  Config::loadConfigFile(confFile);
  //qDebug() << "Config File Loaded:" << QString::number(clock.elapsed())+" ms";
  // Startup the main application
  QApplication a(argc,argv); 
  
  // Show our splash screen, so the user doesn't freak that that it takes a few seconds to show up
  QSplashScreen splash;
  if(!Config::splashscreen().isEmpty()){
    splash.setPixmap( QPixmap(Config::splashscreen()) ); //load the splashscreen file
  }
  splash.show();
  //qDebug() << "SplashScreen Started:" << QString::number(clock.elapsed())+" ms";
  //Initialize the xprocess
  XProcess desktop;
  
  //*** STARTUP THE PROGRAM ***
  bool goodAL = FALSE; //Flag for whether the autologin was successful
  
  // Start the autologin procedure if applicable
  if( ALtriggered && Config::useAutoLogin() ){
    //Setup the Auto Login
    QString user = Backend::getALUsername();
    QString dcmd = Backend::getALDesktopCmd();
    QString pwd = Backend::getALPassword();
    if( user.isEmpty() || dcmd.isEmpty() ){
	 goodAL=FALSE;   
    }else{
	desktop.loginToXSession(user,pwd, Backend::getUserHomeDir(user), dcmd);
	splash.close();
	if(desktop.isRunning()){
	  goodAL=TRUE; //flag this as a good login to skip the GUI
	}
    }
  }
  //qDebug() << "AutoLogin Finished:" << QString::number(clock.elapsed())+" ms";
  if(!goodAL){
    // ------START THE PCDM GUI-------
    
    // Check what directory our app is in
    QString appDir = "/usr/local/share/PCDM";
    // Load the translator
    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    //Check for a language change detected
    if ( ! changeLang.isEmpty() )       
       langCode = changeLang;
    //Load the proper locale for the translator
    if ( QFile::exists(appDir + "/i18n/PCDM_" + langCode + ".qm" ) ) {
      translator.load( QString("PCDM_") + langCode, appDir + "/i18n/" );
      a.installTranslator(&translator);
      Backend::log("Loaded Translation:" + appDir + "/i18n/PCDM_" + langCode + ".qm");
    } else {
      Backend::log("Could not find: " + appDir + "/i18n/PCDM_" + langCode + ".qm");
      langCode = "";
    }
    //qDebug() << "Translation Finished:" << QString::number(clock.elapsed())+" ms";

    Backend::log("Starting up PCDM interface");
    PCDMgui w;
    //qDebug() << "Main GUI Created:" << QString::number(clock.elapsed())+" ms";
    splash.finish(&w); //close the splash when the GUI starts up
    //Set the proper size on the Application
      w.setWindowFlags(w.windowFlags() ^Qt::WindowSoftkeysVisibleHint);
      w.setWindowState(Qt::WindowFullScreen);

    //Setup the signals/slots to startup the desktop session
    QObject::connect( &w,SIGNAL(xLoginAttempt(QString,QString,QString,QString)), &desktop,SLOT(loginToXSession(QString,QString,QString,QString)) );
    //Setup the signals/slots for return information for the GUI
    QObject::connect( &desktop, SIGNAL(InvalidLogin()), &w, SLOT(slotLoginFailure()) );
    QObject::connect( &desktop, SIGNAL(started()), &w, SLOT(slotLoginSuccess()) );
    
    //qDebug() << "Showing GUI:" << QString::number(clock.elapsed())+" ms";
    w.show();
    returnCode = a.exec();

  }  // end of PCDM GUI running
  
  //Wait for the desktop session to finish before exiting
  desktop.waitForSessionClosed();
  
  return returnCode;
}
