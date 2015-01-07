#include <QCoreApplication>
#include <QDebug>
#include <QTextCodec>
#include <unistd.h>
#include <sys/types.h>

#include "MainClient.h"
//#include "../config.h"

#ifndef PREFIX
#define PREFIX QString("/usr/local/")
#endif

int main( int argc, char ** argv )
{
    //First loac the proper text encoding for localized keys
    QString enc = QString(getenv("LANG")).section(".",1,1);
    if(enc.isEmpty()){ enc = QString(getenv("LC_ALL")).section(".",1,1); }
    if(enc.isEmpty()){ enc = "UTF-8"; }
    if(!enc.isEmpty()){
      QTextCodec::setCodecForLocale( QTextCodec::codecForName(enc.toUtf8()) ); 
    }
    //Turn the inputs into a list
    QStringList inputs;
    for(int i=1; i<argc; i++){
      inputs << QString( argv[i] );
    }
    //Check whether running as root (if shutting down the daemon)
    if( getuid() != 0 && inputs.join("").simplified().contains("shutdowndaemon") ){
      qDebug() << "[ERROR] The pc-sysconfig daemon may only be stopped by root!";
      return 1;
    }
    
    QCoreApplication a(argc, argv);
    //Create the client and send requests
    MainClient *w = new MainClient(&a);
    if( w->parseInputs(inputs) ){
      return a.exec();
    }else{
      return 0; //printed usage info
    }
}
