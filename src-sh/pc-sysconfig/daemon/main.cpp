#include <QCoreApplication>
#include <QDebug>
#include <unistd.h>
#include <sys/types.h>

#include "MainDaemon.h"
//#include "../config.h"

#ifndef PREFIX
#define PREFIX QString("/usr/local/")
#endif

int main( int argc, char ** argv )
{
    QCoreApplication a(argc, argv);
    //Check whether running as root
    if( getuid() != 0){
      qDebug() << "The PC-BSD device daemon must be started as root!";
      return 1;
    }
    //Create and start the daemon
    qDebug() << "Starting the PC-BSD Device Daemon....";
    MainDaemon *w = new MainDaemon(&a); 
    w->startServer();
    return a.exec();
}
