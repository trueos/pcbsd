#include <QCoreApplication>
#include <QDebug>
#include <unistd.h>
#include <sys/types.h>
#include <QFile>

#include "syscache-daemon.h"
//#include "../config.h"

#ifndef PREFIX
#define PREFIX QString("/usr/local/")
#endif

int main( int argc, char ** argv )
{
    QCoreApplication a(argc, argv);
    //Check whether running as root
    if( getuid() != 0){
      qDebug() << "The syscache daemon must be started as root!";
      return 1;
    }
    //Create and start the daemon
    qDebug() << "Starting the System Cache Daemon....";
    if(QFile::exists("/var/log/pc-syscache.log")){ QFile::remove("/var/log/pc-syscache.log"); }
    SysCacheDaemon *w = new SysCacheDaemon(&a); 
    w->startServer();
    return a.exec();
}
