#include <QStringList>
#include <QDebug>
#include <QTextCodec>
#include <QCoreApplication>

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

#include "Backend.h"
//#include "../config.h"

void showUsage(){
  system("man pc-sysconfig");
  exit(0);
}

int main( int argc, char ** argv ){
    //First check for any man-page info
    if(argc<2){showUsage(); }
    else{
      for(int i=1; i<argc; i++){
        if(QString::fromLocal8Bit(argv[i])=="-h" || QString::fromLocal8Bit(argv[i])=="--help"){
	  showUsage();
	}
      }
    }
    //Get the current env settings
    QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") ); //Force Utf-8 compliance
    QString lang = QString::fromLocal8Bit(getenv("LC_ALL"));
    if(lang.isEmpty()){ lang = QString::fromLocal8Bit(getenv("LANG")); }
    QString user = QString::fromLocal8Bit(getlogin());
    setuid(0); //need to run as user for this
    //Create/run the request
    //qDebug() << "Open QApplication";
    QCoreApplication app(argc,argv); //needed to silence a runtime warning with QSettings
    Backend w; 
    //qDebug() << "Read old values";
    w.LoadInternalValues();
    QStringList out;
    for(int i=1; i<argc; i++){ //skip the first arg (current binary name)
      //qDebug() << "Run Request:" << argv[i];
      out << w.runRequest(QString::fromLocal8Bit(argv[i]).split(" "), user, lang);
    }
    w.SaveInternalValues();
    //Now print out any outputs to the standard output
    fprintf(stdout, "%s\n", qPrintable(out.join("\n")) );
    return 0;
}
