#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDesktopWidget>
#include <QFile>
#include <QSplashScreen>
#include <QTextCodec>

#include "firstboot.h"
#include "backend.h"

int main(int argc, char *argv[])
{
    QString changeLang;

    QApplication a(argc, argv);

    // Check what directory our app is in
    QString appDir;
    if ( QFile::exists("/usr/local/bin/pc-firstboot") )
      appDir = "/usr/local/share/pcbsd";
    else
      appDir = QCoreApplication::applicationDirPath();

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();

    if ( ! changeLang.isEmpty() )       
       langCode = changeLang;

    if ( QFile::exists(appDir + "/i18n/FirstBoot_" + langCode + ".qm" ) ) {
      translator.load( QString("FirstBoot_") + langCode, appDir + "/i18n/" );
      a.installTranslator(&translator);
      qDebug() << "Loaded Translation:" << appDir + "/i18n/FirstBoot_" + langCode + ".qm";
    } else if ( QFile::exists(appDir + "/i18n/FirstBoot_" + langCode.section("_", 0, 0) + ".qm" ) ) {
      translator.load( QString("FirstBoot_") + langCode.section("_", 0, 0), appDir + "/i18n/" );
      a.installTranslator(&translator);
      qDebug() << "Loaded Translation:" << appDir + "/i18n/FirstBoot_" + langCode.section("_", 0, 0) + ".qm";
    } else {
      qDebug() << "Could not find: " << appDir + "/i18n/FirstBoot_" + langCode + ".qm";
      langCode = "";
    }
    QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") ); //Force Utf-8 compliance
    // Show our splash screen, so the user doesn't freak that that it takes a few seconds to show up
    QPixmap pixmap(":/PCBSD/images/pcbsdheader.png");
    QSplashScreen splash(pixmap);
    splash.show();


    Installer w;

    // Center the installer
    QRect dimensions = QApplication::desktop()->screenGeometry();
    int wid = dimensions.width();     // returns desktop width
    int hig = dimensions.height();    // returns desktop height
    QRect wizDimensions = w.geometry();
    int wizWid = wizDimensions.width(); // Wizard width
    int wizHig = wizDimensions.height(); // Wizard height
    qDebug() << "WizWid" << wizWid;
    qDebug() << "WizHig" << wizHig;
    w.setGeometry((wid/2) - (wizWid/2), (hig/2) - (wizHig/2), wizWid, wizHig);

    // Start the init
    w.initInstall();

    w.show();
    splash.finish(&w);
    
    return a.exec();
}
