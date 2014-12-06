/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
#include <QFileInfo>
#include <QProcess>
#include <QTextStream>
#include "dialogconfirm.h"


void dialogConfirm::ProgramInit(QString txt, bool ac, bool yesno) 
{
   labelText->setText(txt);
  
   if ( ! yesno ) {
     pushNo->setEnabled(false);
     pushNo->setVisible(false);
     pushYes->setText(tr("&Ok"));
   } else {

     // If we are not auto-closing, Display the X resolution
     if ( ! ac ) {
       QString tmp, res, driver;
       QProcess d;
       d.start(QString("xdpyinfo"), QStringList());
       while(d.state() == QProcess::Starting || d.state() == QProcess::Running) {
          d.waitForFinished(200);
          QCoreApplication::processEvents();
       }

       while (d.canReadLine()) {
         tmp = d.readLine().simplified();
	 if ( tmp.indexOf("dimensions:") != -1 ) {
	   res = tmp.section(" ", 1, 1);
           break;
         }
       }

       // Figure out the driver
       QFile file("/var/log/Xorg.0.log");
       if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
         QTextStream in(&file);
         while (!in.atEnd()) {
            QString line = in.readLine();
	    if ( line.indexOf("Loading /usr/local/lib/xorg/modules/drivers/") != -1 ) {
	       QFileInfo fi(line);
               driver = fi.fileName();
	       driver = driver.section("_", 0, 0);
	       break;
	    } 
         }
         file.close();
       }

       textAutoClose->setText(tr("Resolution:") + " " + res + "\n" + tr("Driver:") + " " + driver  );
     }
   }

   if ( ac ) {
     timeLeft = 10;
     textAutoClose->setText(tr("Auto-close in 10 seconds...") );
     QTimer::singleShot( 2000, this, SLOT(checkTime()) );
   }
}


void dialogConfirm::slotYes()
{
    system("rm /var/.runxsetup 2>/dev/null");
    system("sync");
    exit(0);
}


void dialogConfirm::slotNo()
{
    exit(1);
}


void dialogConfirm::checkTime()
{
    timeLeft--;
    if ( timeLeft <= 0 )
    {
	exit(1);
    }
    QString tmp;
    tmp.setNum(timeLeft);
    textAutoClose->setText(tr("Auto-close in ") + tmp + tr(" seconds...") );
     QTimer::singleShot( 1000, this, SLOT(checkTime()) );
}
