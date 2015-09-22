// ===============================
//  PC-BSD REST API Server
// Available under the 3-clause BSD License
// Written by: Ken Moore <ken@pcbsd.org> July 2015
// =================================
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QDir>
#include <unistd.h>
#include <sys/types.h>

#include "WebServer.h"

#ifndef PREFIX
#define PREFIX QString("/usr/local/")
#endif

#define DEBUG 1

QFile logfile("/var/log/syscache-webclient.log");
void MessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg){
  QString txt;
  switch(type){
  case QtDebugMsg:
  	  txt = msg;
  	  break;
  case QtWarningMsg:
  	  txt = QString("WARNING: %1").arg(msg);
          txt += "\n Context: "+QString(context.file)+" Line: "+QString(context.line)+" Function: "+QString(context.function);
  	  break;
  case QtCriticalMsg:
  	  txt = QString("CRITICAL: %1").arg(msg);
	  txt += "\n Context: "+QString(context.file)+" Line: "+QString(context.line)+" Function: "+QString(context.function);
  	  break;
  case QtFatalMsg:
  	  txt = QString("FATAL: %1").arg(msg);
	  txt += "\n Context: "+QString(context.file)+" Line: "+QString(context.line)+" Function: "+QString(context.function);
  	  break;
  }

  QTextStream out(&logfile);
  out << txt;
  if(!txt.endsWith("\n")){ out << "\n"; }
}

int main( int argc, char ** argv )
{
    QCoreApplication a(argc, argv);
    //Check whether running as root
    if( getuid() != 0){
      qDebug() << "syscache-webclient must be started as root!";
      return 1;
    }
      //Setup the log file
    if(DEBUG){
      qDebug() << "Log File:" << logfile.fileName();
      if(QFile::exists(logfile.fileName()+".old")){ QFile::remove(logfile.fileName()+".old"); }
      if(logfile.exists()){ QFile::rename(logfile.fileName(), logfile.fileName()+".old"); }
      //Make sure the parent directory exists
      if(!QFile::exists("/var/log")){
        QDir dir;
        dir.mkpath("/var/log");
      }
      logfile.open(QIODevice::WriteOnly | QIODevice::Append);
      qInstallMessageHandler(MessageOutput);
    }
      
    //Create and start the daemon
    qDebug() << "Starting the PC-BSD syscache websocket client interface....";
    WebServer *w = new WebServer(); 
    if( w->startServer() ){
      //Now start the event loop
      int ret = a.exec();
      logfile.close();
      return ret;
    }else{
      return 1;
    }
}
