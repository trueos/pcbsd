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

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "pcdm-gui.h"
#include "pcdm-backend.h"
#include "pcdm-config.h"
#include "pcdm-xprocess.h"

#define TMPLANGFILE QString("/tmp/.PCDMLang")
#define TMPAUTOLOGINFILE QString("/tmp/.PCDMAutoLogin")
#define TMPAUTHFILE QString("/tmp/.PCDMAuth")
//Make sure that prefix is set
//#ifndef prefix
//#define prefix "/usr/local/"
//#endif

bool USECLIBS=false;

int runSingleSession(int argc, char *argv[]){
  //QTime clock;
  //clock.start();
  Backend::checkLocalDirs();  // Create and fill "/usr/local/share/PCDM" if needed
  Backend::openLogFile("/var/log/PCDM.log");  
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
	desktop.loginToXSession(user,pwd, dsk);
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

    // Set full-screen dimensions
    QRect dimensions = QApplication::desktop()->screenGeometry();
    int wid = dimensions.width();     // returns desktop width
    int hig = dimensions.height();    // returns desktop height
    w.setGeometry(0, 0, wid, hig);

    //Set the proper size on the Application
    w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint);
    w.setWindowState(Qt::WindowMaximized); //Qt::WindowFullScreen);

    //Setup the signals/slots to startup the desktop session
    if(USECLIBS){ QObject::connect( &w,SIGNAL(xLoginAttempt(QString,QString,QString)), &desktop,SLOT(setupDesktop(QString,QString,QString))); }
    else{ QObject::connect( &w,SIGNAL(xLoginAttempt(QString,QString,QString)), &desktop,SLOT(loginToXSession(QString,QString,QString)) ); }
    //Setup the signals/slots for return information for the GUI
    QObject::connect( &desktop, SIGNAL(InvalidLogin()), &w, SLOT(slotLoginFailure()) );
    QObject::connect( &desktop, SIGNAL(started()), &w, SLOT(slotLoginSuccess()) );
    QObject::connect( &desktop, SIGNAL(ValidLogin()), &w, SLOT(slotLoginSuccess()) );
    
    //qDebug() << "Showing GUI:" << QString::number(clock.elapsed())+" ms";
    w.show();
    a.exec();
  }  // end of PCDM GUI running
  int retcode = 0;
  //Wait for the desktop session to finish before exiting
  if(USECLIBS){ desktop.startDesktop(); }
  else{ desktop.waitForSessionClosed(); }
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
  
  
  return retcode;
}

int main(int argc, char *argv[])
{
 bool neverquit = TRUE;
 bool runonce = FALSE;
 if(argc==2){ if( QString(argv[1]) == "--once"){ runonce = TRUE; } }
  
 while(neverquit){
  if(runonce){ neverquit = FALSE; }
  qDebug() << " -- PCDM Session Starting...";
  system("xauth -f "+TMPAUTHFILE.toUtf8()+" generate :0 ."); //Using MIT-MAGIC-COOKIE-1 protocol
  //MIT-MAGIC-COOKIE-1 is good enough for our purposes since it is a single-user graphical system with no XDMCP
  int sid = -1;
  int pid = fork();
  if(pid < 0){
    qDebug() << "Error: Could not fork the PCDM session";
    return -1;
  }else if( pid ==0 ){
    //New Child Process
    sid = setsid(); //start a session
    qDebug() << "-- Session ID:" << sid;
    int retCode = runSingleSession(argc,argv);
    qDebug() << "-- PCDM Session Ended --";
    //check for special exit code
    if(retCode != 0){ neverquit=FALSE; }
    //Now kill the shild process (whole session)
    qDebug() << "Exiting child process";
    exit(3);
  }else{ 
    //Parent (calling) process
    int status;
    sleep(2);
    waitpid(sid,&status,0); //wait for the child (session) to finish
  }
  qDebug() << "-- PCDM Session Ended --";
  if(QFile::exists("/var/run/nologin")){ neverquit = FALSE; } 
 }
 system("xauth -f "+TMPAUTHFILE.toUtf8()+" remove :0");
 return 0;
}
