#include <qapplication.h>
#include <qtranslator.h>
#include <qtextcodec.h>
#include <qstylefactory.h>
#include <qdesktopwidget.h>
#include <QDebug>
#include <QObject>
#include "xgui.h"
#include "dialogconfirm.h"
#include "../config.h"

int main( int argc, char ** argv )
{
   QApplication a(argc, argv);

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists(PREFIX + "/share/pcbsd/i18n/XGUISource_" + langCode + ".qm" ) )
      langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("XGUISource_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;

    QString Tmp = QString(argv[1]);
  
    if ( argc == 2)
    {
        QString flag = argv[1];
	if ( flag == "-test" )
	{
	  dialogConfirm d;
          QDesktopWidget *desk = a.desktop();
          QRect dimensions = desk->screenGeometry();
          int wid = dimensions.width();     // returns desktop width
          int hig = dimensions.height();    // returns desktop height
          d.setGeometry((wid/2) - (355/2), (hig/2) - (151/2), 355, 151);
          d.ProgramInit(QObject::tr("Keep these display settings?"), true, true);
          d.show();
          a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
          return a.exec();
        }
	if ( flag == "-failed" )
	{
	  dialogConfirm d;
          QDesktopWidget *desk = a.desktop();
          QRect dimensions = desk->screenGeometry();
          int wid = dimensions.width();     // returns desktop width
          int hig = dimensions.height();    // returns desktop height
          d.setGeometry((wid/2) - (355/2), (hig/2) - (160/2), 355, 160);
          d.ProgramInit(QObject::tr("The previous attempt failed, please check your settings and try again."), false, false);
          d.show();
          a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
          return a.exec();
        }
	if ( flag == "-firstprompt" )
	{
	  dialogConfirm d;
          QDesktopWidget *desk = a.desktop();
          QRect dimensions = desk->screenGeometry();
          int wid = dimensions.width();     // returns desktop width
          int hig = dimensions.height();    // returns desktop height
          d.setGeometry((wid/2) - (385/2), (hig/2) - (190/2), 385, 190);
          d.ProgramInit(QObject::tr("Your display has been configured.\nKeep these settings?"), false, true);
          d.show();
          a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
          return a.exec();
        }
    } 

    XGUI w;

    QDesktopWidget *desk = a.desktop();
    QRect dimensions = desk->screenGeometry();
    int wid = dimensions.width();     // returns desktop width
    int hig = dimensions.height();    // returns desktop height
    w.setGeometry((wid/2) - (564/2), (hig/2) - (461/2), 564, 461);

    w.ProgramInit();

    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
