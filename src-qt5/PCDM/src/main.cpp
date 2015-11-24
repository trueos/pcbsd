/* PCDM Login Manager:
*  Written by Ken Moore (ken@pcbsd.org) 2012/2013
*  Copyright(c) 2013 by the PC-BSD Project
*  Available under the 3-clause BSD license
*/

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDesktopWidget>
#include <QFile>
#include <QSplashScreen>
#include <QTime>
#include <QDebug>
#include <QX11Info>
#include <QTextCodec>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "pcdm-gui.h"
#include "pcdm-backend.h"
#include "pcdm-config.h"
#include "pcdm-xprocess.h"
#include "pcdm-logindelay.h"

#define TMPLANGFILE QString("/tmp/.PCDMLang")
#define TMPAUTOLOGINFILE QString("/tmp/.PCDMAutoLogin")
#define TMPAUTHFILE QString("/tmp/.PCDMAuth")
#define TMPSTOPFILE QString("/tmp/.PCDMstop")
//Make sure that prefix is set
//#ifndef prefix
//#define prefix "/usr/local/"
//#endif

//bool USECLIBS=false;

//Setup any qDebug/qWarning/qError messages to get saved into this log file directly
QFile logfile("/var/log/PCDM.log");
void MessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg){
  QString txt;
  switch(type){
  case QtDebugMsg:
  	  txt = QString("Debug: %1").arg(msg);
  	  break;
  case QtWarningMsg:
  	  txt = QString("Warning: %1").arg(msg);
  	  break;
  case QtCriticalMsg:
  	  txt = QString("CRITICAL: %1").arg(msg);
  	  break;
  case QtFatalMsg:
  	  txt = QString("FATAL: %1").arg(msg);
  	  break;
  }

  QTextStream out(&logfile);
  out << txt;
  if(!txt.endsWith("\n")){ out << "\n"; }
}
int runSingleSession(int argc, char *argv[]){
  //QTime clock;
  //clock.start();
  Backend::checkLocalDirs();  // Create and fill "/usr/local/share/PCDM" if needed
  //Setup the log file
  qDebug() << "PCDM Log File: /var/log/PCDM.log"; //This does not go into the log
    if(QFile::exists(logfile.fileName()+".old")){ QFile::remove(logfile.fileName()+".old"); }
    if(logfile.exists()){ QFile::rename(logfile.fileName(), logfile.fileName()+".old"); }
      //Make sure the parent directory exists
      if(!QFile::exists("/var/log")){
        QDir dir;
        dir.mkpath("/var/log");
      }
    logfile.open(QIODevice::WriteOnly | QIODevice::Append);
  //Backend::openLogFile("/var/log/PCDM.log"); 

  //qDebug() << "Backend Checks Finished:" << QString::number(clock.elapsed())+" ms";
  //Setup the initial system environment (locale, keyboard)
  QString lang, kmodel, klayout, kvariant;
  Backend::readDefaultSysEnvironment(lang,kmodel,klayout,kvariant);
  //setenv("LANG", lang.toUtf8(), 0);
  lang = lang.section(".",0,0); //just in case it also had the encoding saved to the file
  setenv("LANG", lang.toUtf8(), 0);
  Backend::changeKbMap(kmodel,klayout,kvariant);
  //Check for the flag to try and auto-login
  bool ALtriggered = false;
  if(QFile::exists(TMPAUTOLOGINFILE)){ ALtriggered=true; QFile::remove(TMPAUTOLOGINFILE); }
  
  //QString changeLang; 
  // Load the configuration file
  QString confFile = "/usr/local/etc/pcdm.conf";
  if(!QFile::exists(confFile)){ 
    qDebug() << "PCDM: Configuration file missing:"<<confFile<<"\n  - Using default configuration";
    QFile::copy(":samples/pcdm.conf", confFile);
  }
  
  Config::loadConfigFile(confFile);
  // Now set the backend functionality of which usernames are allowed
  Backend::allowUidUnder1K(Config::allowUnder1KUsers(), Config::excludedUserList());
  //qDebug() << "Config File Loaded:" << QString::number(clock.elapsed())+" ms";
  // Startup the main application
  QApplication a(argc,argv); 
    //Setup Log File
    qInstallMessageHandler(MessageOutput);
  int retCode = 0; //used for UI/application return
  // Show our splash screen, so the user doesn't freak that that it takes a few seconds to show up
  /*QSplashScreen splash;
  if(!Config::splashscreen().isEmpty()){
    splash.setPixmap( QPixmap(Config::splashscreen()) ); //load the splashscreen file
  }
  splash.show();*/
  QCoreApplication::processEvents(); //Process the splash screen event immediately
  //qDebug() << "SplashScreen Started:" << QString::number(clock.elapsed())+" ms";
  //Initialize the xprocess
  XProcess desktop;
  
  // Check what directory our app is in
    QString appDir = "/usr/local/share/pcbsd";
    // Load the translator
    QTranslator translator;
    QString langCode = lang;
    //Check for a language change detected
    //if ( ! changeLang.isEmpty() )       
       //langCode = changeLang;
    //Load the proper locale for the translator
    if ( QFile::exists(appDir + "/i18n/PCDM_" + langCode + ".qm" ) ) {
      translator.load( QString("PCDM_") + langCode, appDir + "/i18n/" );
      a.installTranslator(&translator);
      qDebug() <<"Loaded Translation:" + appDir + "/i18n/PCDM_" + langCode + ".qm";
    } else {
      qDebug() << "Could not find: " + appDir + "/i18n/PCDM_" + langCode + ".qm";
      langCode = "en_US"; //always default to US english
    }


    QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") ); //Force Utf-8 compliance
    //qDebug() << "Translation Finished:" << QString::number(clock.elapsed())+" ms";
    
  //*** STARTUP THE PROGRAM ***
  bool goodAL = false; //Flag for whether the autologin was successful
  // Start the autologin procedure if applicable
  if( ALtriggered && Config::useAutoLogin() ){
    //Setup the Auto Login
    QString user = Backend::getALUsername();
    QString pwd = Backend::getALPassword();
    QString dsk = Backend::getLastDE(user);
    if( user.isEmpty() || dsk.isEmpty() || QFile::exists("/var/db/personacrypt/"+user+".key") ){
	//Invalid inputs (or a PersonaCrypt user)
	 goodAL=false;   
    }else{
	//Run the time delay for the autologin attempt
	if(Config::autoLoginDelay() > 1){
	  loginDelay dlg(Config::autoLoginDelay(), user);
	  //splash.close();
	  dlg.start();
	    dlg.activateWindow();
	  dlg.exec();
	  goodAL = dlg.continueLogin;
	}else{
	  goodAL = true;
	}
	//now start the autologin if appropriate
	if(goodAL){
	  desktop.loginToXSession(user,pwd, dsk,langCode,"",false);
	  //splash.close();
	  if(desktop.isRunning()){
	    goodAL=true; //flag this as a good login to skip the GUI
	  }
        }
    }
  }
  //qDebug() << "AutoLogin Finished:" << QString::number(clock.elapsed())+" ms";
  if(!goodAL){
    // ------START THE PCDM GUI-------

    qDebug() << "Starting up PCDM interface";
    PCDMgui w;

    QLocale locale(lang); //Always use the "lang" saved from last login - even if the "langCode" was reset to en_US for loading PCDM translations
    w.setLocale(locale);
    //qDebug() << "Main GUI Created:" << QString::number(clock.elapsed())+" ms";
    //splash.finish(&w); //close the splash when the GUI starts up

    //Setup the signals/slots to startup the desktop session 
    QObject::connect( &w,SIGNAL(xLoginAttempt(QString,QString,QString,QString, QString, bool)), &desktop,SLOT(loginToXSession(QString,QString,QString,QString, QString,bool)) ); 
    //Setup the signals/slots for return information for the GUI
    QObject::connect( &desktop, SIGNAL(InvalidLogin()), &w, SLOT(slotLoginFailure()) );
    QObject::connect( &desktop, SIGNAL(started()), &w, SLOT(slotLoginSuccess()) );
    QObject::connect( &desktop, SIGNAL(ValidLogin()), &w, SLOT(slotLoginSuccess()) );
    
    //qDebug() << "Showing GUI:" << QString::number(clock.elapsed())+" ms";
    w.show();

    //Now start the event loop until the window closes
    retCode = a.exec();
  }  // end of PCDM GUI running
  //Wait for the desktop session to finish before exiting
    desktop.waitForSessionClosed(); 
    qDebug() << "PCDM Session finished";
    logfile.close();
  //splash.show(); //show the splash screen again
  //Now wait a couple seconds for things to settle
  QTime wTime = QTime::currentTime().addSecs(2);
  while( QTime::currentTime() < wTime ){
    QCoreApplication::processEvents(QEventLoop::AllEvents,100);
  }
  //check for shutdown process
  if( QFile::exists(TMPSTOPFILE) || QFile::exists("/var/run/nologin") || retCode > 0){
    //splash.showMessage(QObject::tr("System Shutting Down"), Qt::AlignHCenter | Qt::AlignBottom, Qt::white);
    QCoreApplication::processEvents();
    //Pause for a few seconds to prevent starting a new session during a shutdown
    wTime = QTime::currentTime().addSecs(30);
    while( QTime::currentTime() < wTime ){ 
      //Keep processing events during the wait (for splashscreen)
      QCoreApplication::processEvents(QEventLoop::AllEvents, 100); 
    }
  }
  
  //Clean up Code
  delete &desktop;
  delete &a;
  //delete &splash;
  
  
  return retCode;
}

int main(int argc, char *argv[])
{
 bool neverquit = true;
 bool runonce = true;  //Always set this for now until the internal repeater works properly
 setenv("MM_CHARSET", "UTF-8", 1); //ensure UTF-8 text formatting
 if(argc==2){ if( QString(argv[1]) == "-once"){ runonce = true; } }
  
 while(neverquit){
  if(runonce){ neverquit = false; }
  qDebug() << " -- PCDM Session Starting...";
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
    if(retCode == -1){ neverquit=true; } //make sure we go around again at least once
    else if(retCode != 0){ neverquit=false; }
    //Now kill the shild process (whole session)
    qDebug() << "Exiting child process";
    exit(3);
  }else{ 
    //Parent (calling) process
    int status;
    sleep(2);
    waitpid(sid,&status,0); //wait for the child (session) to finish
    //NOTE: the parent will eventually become a login-watcher daemon process that
    //   can spawn multiple child sessions on different TTY displays
  }
  qDebug() << "-- PCDM Session Ended --";
  if(QFile::exists("/var/run/nologin") || QFile::exists(TMPSTOPFILE) ){ neverquit = false; } 
 }
 return 0;
}
