/***************************************************************************
 *   Copyright (C) 2007 by Kris Moore   *
 *   kris@pcbsd.com   *
 *                                                                         *
 *   Permission is hereby granted, free of charge, to any person obtaining *
 *   a copy of this software and associated documentation files (the       *
 *   "Software"), to deal in the Software without restriction, including   *
 *   without limitation the rights to use, copy, modify, merge, publish,   *
 *   distribute, sublicense, and/or sell copies of the Software, and to    *
 *   permit persons to whom the Software is furnished to do so, subject to *
 *   the following conditions:                                             *
 *                                                                         *
 *   The above copyright notice and this permission notice shall be        *
 *   included in all copies or substantial portions of the Software.       *
 *                                                                         *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. *
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/

#include "ethernetconfig.h"
#include <QLocale>
#include <QDebug>
#include <QFile>
#include <qtranslator.h>
#include <qdesktopwidget.h>
#include <qtextcodec.h>
#include <QApplication>
#include "../../../config.h"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/ethernetconfig_" + langCode + ".qm" ) )
      langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("ethernetconfig_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;
    QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") ); //Force Utf-8 compliance
    
    ethernetconfig *mainWin = 0;

    // no session.. just start up normally
    mainWin = new ethernetconfig();
    if ( argc == 2)
    {
	QString Device = argv[1];
	mainWin->setDevice( Device );
	mainWin->setWindowTitle(Device + " " + "configuration");

        mainWin->programInit();
        mainWin->show();
     } else if ( argc == 3  ) {
	 QString val = argv[1];
	 if ( val != "info" ) {
	   exit(1);  
	 }
	 
        QString Device = argv[2];
	mainWin->setDevice( Device );
	mainWin->setWindowTitle(Device + " " + "configuration");
	mainWin->showInfo();
        mainWin->programInit();
	mainWin->show();
     } else {
	exit(1);
     }

    // mainWin has WDestructiveClose flag by default, so it will delete itself.
    return a.exec();
}

