#include <qtranslator.h>
#include <qlocale.h>
#include <qtsingleapplication.h>
#include <QDebug>
#include <QSettings>
#include "preserver.h"
#include "externals.h"
#include "lifePreserverMain.h"
#include "lifePreserverWelcome.h"
#include "../config.h"

bool havePreservers;
lifePreserver *m;
lifePreserverWelcome *w;

int main( int argc, char ** argv )
{
    QtSingleApplication a(argc, argv);
    if (a.isRunning())
      return !(a.sendMessage("show"));

    havePreservers = false;

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/lifePreserver/i18n/LifePreserver_" + langCode + ".qm" ) )
      langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("LifePreserver_") + langCode, PREFIX + "/share/lifePreserver/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;

    // Check if we should not open
    if ( (a.arguments().size() > 2) && a.arguments().at(2) == "-autostart" ) {
       QSettings settings("PCBSD", "LifePreserver");
       if ( ! settings.value("autostart", true).toBool() ) {
	  qDebug() << "Life-Preserver autostart is diabled! Run without -autostart flag!";
	  exit(1);
       }
    }

    m = new lifePreserver(); 
    w = new lifePreserverWelcome(); 
	
    m->ProgramInit();
    w->ProgramInit();

    if ( havePreservers ) {
      if ( (a.arguments().size() > 1) && a.arguments().at(1) == "-tray" )
        m->hide();
      else
        m->show();
    } else {
      if ( (a.arguments().size() > 1) && a.arguments().at(1) == "-tray" )
        w->hide();
      else
        w->show();
    }

    QObject::connect( &a, SIGNAL( messageReceived(const QString &) ), m, SLOT( slotSingleInstance() ) );
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
