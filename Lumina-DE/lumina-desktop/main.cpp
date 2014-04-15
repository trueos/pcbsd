//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include <QDebug>
//#include <QMainWindow>
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QString>
#include <QTextStream>
#include <QDesktopWidget>
#include <QSettings>
#include <QList>
#include <QTranslator>

#include "WMProcess.h"
#include "Globals.h"
#include "LDesktop.h"
#include "LSession.h"

#include <LuminaXDG.h> //from libLuminaUtils

#ifndef PREFIX
#define PREFIX QString("/usr/local")
#endif

QFile logfile(QDir::homePath()+"/.lumina/logs/runtime.log");
void MessageOutput(QtMsgType type, const char *msg){
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
int main(int argc, char ** argv)
{
    //Setup any pre-QApplication initialization values
    LXDG::setEnvironmentVars();
    setenv("DESKTOP_SESSION","LUMINA",1);
    setenv("XDG_CURRENT_DESKTOP","LUMINA",1);
    //Check is this is the first run
    bool firstrun = false;
    if(!QFile::exists(logfile.fileName())){ firstrun = true; }
    //Setup the log file
    qDebug() << "Lumina Log File:" << logfile.fileName();
    if(logfile.exists()){ logfile.remove(); } //remove any old one
      //Make sure the parent directory exists
      if(!QFile::exists(QDir::homePath()+"/.lumina/logs")){
        QDir dir;
        dir.mkpath(QDir::homePath()+"/.lumina/logs");
      }
    logfile.open(QIODevice::WriteOnly | QIODevice::Append);
    //Startup the Application
    LSession a(argc, argv);
    //Setup Log File
    qInstallMsgHandler(MessageOutput);
    //Setup the QSettings
    QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, QDir::homePath()+"/.lumina/settings");
    qDebug() << "Initializing Lumina";
    //Start up the Window Manager
    qDebug() << " - Start Window Manager";
    WMProcess WM;
    WM.startWM();
    QObject::connect(&WM, SIGNAL(WMShutdown()), &a, SLOT(closeAllWindows()) );
    //Load the initial translations
    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    
    if ( ! QFile::exists(PREFIX + "/share/Lumina-DE/i18n/lumina-desktop_" + langCode + ".qm" ) )  langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("lumina-desktop_") + langCode, PREFIX + "/share/Lumina-DE/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;
    //Now start the desktop
    QDesktopWidget DW;
    QList<LDesktop*> screens;
    for(int i=0; i<DW.screenCount(); i++){
      qDebug() << " - Start Desktop " << i;
      screens << new LDesktop(i);
      a.processEvents();
    }
    qDebug() << " --exec";
    int retCode = a.exec();
    qDebug() << "Stopping the window manager";
    WM.stopWM();
    qDebug() << "Finished Closing Down Lumina";
    logfile.close();
    return retCode;
}
