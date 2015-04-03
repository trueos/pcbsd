#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include <QMessageBox>
#include <QProcess>
#include <QFile>
#include <QTextCodec>
#include <QDir>

#include "pdfUI.h"

#include <unistd.h>
#include <sys/types.h>

//#include "../config.h"
#ifndef PREFIX
#define PREFIX QString("/usr/local")
#endif

void showUsage(){
  qDebug() << "Usage: pc-pdfviewer [--debug] [ [--iconfile | -i] <icon file>] [ [--titletext | -t] <title>] <File path>";
}

int main( int argc, char ** argv )
{
    //Check for root permissions
    if( getuid() == 0){
      qDebug() << "pc-pdfviewer must not be started as root!";
      return 1;
    }
    qDebug() << "Starting up the generic pdfviewer";
    QApplication a(argc, argv);   

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/pdfviewer_" + langCode + ".qm" ) )
      langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("pdfviewer_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;
    QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") ); //Force Utf-8 compliance
    
    bool debug = false;
    QString fileURL;
    //Load the inputs
      for(int i=1; i<argc; i++){
	QString arg = QString(argv[i]);
	if(arg=="help" || arg=="-h" || arg=="--help"){ showUsage(); return 0; }
        if(arg=="--debug"){ 
	  debug = true; 
	  continue;
        }
        fileURL = QString(arg);
      }
      //Add a check for relative file paths
      if(!fileURL.startsWith("/") && !fileURL.isEmpty() ){
        if(QFile::exists(QDir::currentPath()+"/"+fileURL)){
	  //Save and use the full path instead
	  fileURL = QDir::currentPath()+"/"+fileURL;
	}
      }
      //Launch the UI
      //qDebug() << "Opening:" << fileURL;
      //qDebug() << " - Inputs:" << debug << title << iconpath;
      pdfUI w(debug, fileURL); 
      w.show();

      a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
      return a.exec();

}
