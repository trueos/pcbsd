#include <QCoreApplication>
#include <QDebug>
#include <unistd.h>
#include <sys/types.h>

#include "MainClient.h"
//#include "../config.h"

#ifndef PREFIX
#define PREFIX QString("/usr/local/")
#endif

void printHelp(){
  qDebug() << "\
pc-sysconfig: Interface to set system configurations or retrieve system information\n\
\n\
Usage:\n\
  <Usage Tips Not Implemented Yet>\n\
";
  exit(1);
}

int main( int argc, char ** argv )
{
    //Turn the inputs into a list
    QStringList inputs;
    for(int i=1; i<argc; i++){
      inputs << QString( argv[i] );
    }
    //Check for help inputs and print usage info
    if(inputs.length()<1 || inputs.contains("help")){ printHelp(); }
    //Check whether running as root (if shutting down the daemon)
    if( getuid() != 0 && inputs.join("").simplified().contains("shutdowndaemon") ){
      qDebug() << "[ERROR] The pc-sysconfig daemon may only be stopped by root!";
      return 1;
    }
    
    QCoreApplication a(argc, argv);
    //Create the client and send requests
    MainClient *w = new MainClient(&a);
    w->parseInputs(inputs);
    return a.exec();
}
