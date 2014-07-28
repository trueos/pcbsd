#include <QCoreApplication>
#include <QDebug>
#include <unistd.h>
#include <sys/types.h>

#include "syscache-client.h"
//#include "../config.h"

#ifndef PREFIX
#define PREFIX QString("/usr/local/")
#endif

int main( int argc, char ** argv )
{
    QCoreApplication a(argc, argv);
    //Turn the inputs into a list
    QStringList inputs;
    for(int i=1; i<argc; i++){
      inputs << QString( argv[i] );
    }
    //Check whether running as root (if shutting down the daemon)
    if( getuid() != 0 && inputs.join("").simplified()=="shutdowndaemon"){
      qDebug() << "[ERROR] The syscache daemon may only be stopped by root!";
      return 1;
    }
    //Create and start the daemon
    SysCacheClient *w = new SysCacheClient(&a);
    w->parseInputs(inputs);
    return a.exec();
}
