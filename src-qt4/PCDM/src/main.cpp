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
#include <QX11Info>
//#include <X11/Xlib.h>

#include "pcdm-gui.h"
#include "pcdm-backend.h"
#include "pcdm-config.h"
#include "pcdm-xprocess.h"

#define TMPLANGFILE QString("/tmp/.PCDMLang")
#define TMPAUTOLOGINFILE QString("/tmp/.PCDMAutoLogin")

//Make sure that prefix is set
//#ifndef prefix
//#define prefix "/usr/local/"
//#endif

int runSingleSession(int argc, char *argv[]){
  //QTime clock;
  //clock.start();
  Backend::checkLocalDirs();  // Create and fill "/usr/local/share/PCDM" if needed
  Backend::openLogFile("/usr/local/share/PCDM/PCDM.log");  
  //qDebug() << "Backend Checks Finished:" << QString::number(clock.elapsed())+" ms";
  //Check for the flag to try and auto-login
  bool ALtriggered = FALSE;
  if(QFile::exists(TMPAUTOLOGINFILE)){ ALtriggered=TRUE; QFile::remove(TMPAUTOLOGINFILE); }
  
  QString changeLang; 
  // Load the configuration file
  QString confFile = "/usr/local/etc/pcdm.conf";
  if(!QFile::exists(confFile)){ 
    qDebug() << "PCDM: Configuration file missing:"<<confFile<<"\n  - Using default configuration";
    confFile = ":samples/pcdm.conf"; 
  }
  
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
  QCoreApplication::processEvents(); //Process the splash screen event immediately
  //qDebug() << "SplashScreen Started:" << QString::number(clock.elapsed())+" ms";
  //Initialize the xprocess
  XProcess desktop;
  
  //*** STARTUP THE PROGRAM ***
  bool goodAL = FALSE; //Flag for whether the autologin was successful
  // Start the autologin procedure if applicable
  if( ALtriggered && Config::useAutoLogin() ){
    //Setup the Auto Login
    QString user = Backend::getALUsername();
    QString pwd = Backend::getALPassword();
    QString dsk = Backend::getLastDE(user);
    if( user.isEmpty() || dsk.isEmpty() ){
	 goodAL=FALSE;   
    }else{
	desktop.loginToXSession(user,pwd, Backend::getUserHomeDir(user), dsk);
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
      w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint);
      w.setWindowState(Qt::WindowMaximized); //Qt::WindowFullScreen);

    //Setup the signals/slots to startup the desktop session
    QObject::connect( &w,SIGNAL(xLoginAttempt(QString,QString,QString,QString)), &desktop,SLOT(loginToXSession(QString,QString,QString,QString)) );
    //Setup the signals/slots for return information for the GUI
    QObject::connect( &desktop, SIGNAL(InvalidLogin()), &w, SLOT(slotLoginFailure()) );
    QObject::connect( &desktop, SIGNAL(started()), &w, SLOT(slotLoginSuccess()) );
    
    //qDebug() << "Showing GUI:" << QString::number(clock.elapsed())+" ms";
    w.show();
    a.exec();
  }  // end of PCDM GUI running
  int retcode = 0;
  //Wait for the desktop session to finish before exiting
  desktop.waitForSessionClosed();
  splash.show(); //show the splash screen again
  splash.showMessage(QObject::tr("System Shutting Down"), Qt::AlignHCenter | Qt::AlignBottom, Qt::white);
  //check for shutdown process
  if(QFile::exists("/var/run/nologin")){
    //Pause for a few seconds to prevent starting a new session during a shutdown
    QTime wTime = QTime::currentTime().addSecs(30);
    while( QTime::currentTime() < wTime ){ 
      //Keep processing events during the wait (for splashscreen)
      QCoreApplication::processEvents(QEventLoop::AllEvents, 100); 
    }
    //set the return code for a shutdown
    retcode = -1; //make sure it does not start a new session
  }
  
  //Clean up Code
  delete &desktop;
  delete &a;
  delete &splash;
  //XSetCloseDownMode(QX11Info::display(), DestroyAll);
  //XCLoseDisplay(QX11Info::display());
  
  
  return retcode;
}

int main(int argc, char *argv[])
{
 bool neverquit = TRUE;
 bool runonce = TRUE; //looping is currently not working yet - needs to restart X each time?
 if(argc==2){ if( QString(argv[1]) == "--once"){ runonce = TRUE; } }
  
 while(neverquit){
  if(runonce){ neverquit = FALSE; }
  qDebug() << " -- PCDM Session Starting...";
  int retCode = runSingleSession(argc,argv);
  if(retCode != 0){ neverquit=FALSE; }
 }
 return 0;
}
