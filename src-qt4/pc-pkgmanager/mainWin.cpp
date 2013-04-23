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
#include <fcntl.h>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QProgressDialog>
#include <QSocketNotifier>
#include <QString>
#include <QTextStream>
#include <pcbsd-utils.h>
#include "mainWin.h"
#include "../config.h"

void mainWin::ProgramInit(QString ch)
{
  // Set any warden directories
  doingUpdate=false;
  lastError="";
  wDir = ch;

  //Grab the username
  //username = QString::fromLocal8Bit(getenv("LOGNAME"));
  connect(pushUpdatePkgs, SIGNAL(clicked()), this, SLOT(slotUpdatePkgsClicked()));
  connect(pushClose, SIGNAL(clicked()), this, SLOT(slotCloseClicked()));
  connect(buttonRescanPkgs, SIGNAL(clicked()), this, SLOT(slotRescanPkgsClicked()));
  progressUpdate->setHidden(true);

  QTimer::singleShot(200, this, SLOT(slotRescanPkgsClicked() ) );
}

void mainWin::slotRescanPkgsClicked()
{
  // Check for pkg updates
  checkMPKGUpdates();
}

void mainWin::checkMPKGUpdates() {
  if ( doingUpdate )
     return;

  QString line, tmp, name, pkgname, pkgover, pkgnver;
  QStringList up, listPkgs;
  bool haveUpdates = false;
  int totPkgs=0;
  buttonRescanPkgs->setEnabled(false);
  pushUpdatePkgs->setEnabled(false);
  listViewUpdatesPkgs->clear();
  groupUpdatesPkgs->setTitle(tr("Checking for updates"));

  QProcess p;
  if ( wDir.isEmpty() )
     p.start(QString("pc-updatemanager"), QStringList() << "pkgcheck");
  else
     p.start(QString("chroot"), QStringList() << wDir << "pc-updatemanager" << "pkgcheck");
  while(p.state() == QProcess::Starting || p.state() == QProcess::Running)
     QCoreApplication::processEvents();

  while (p.canReadLine()) {
    line = p.readLine().simplified();
    qDebug() << line;
    if ( line.indexOf("Upgrading") != 0) {
       continue;
    }
    tmp = line;
    pkgname = tmp.section(" ", 1, 1);
    pkgname.replace(":", "");
    pkgover = tmp.section(" ", 2, 2);
    pkgnver = tmp.section(" ", 4, 4);
    QTreeWidgetItem *myItem = new QTreeWidgetItem(QStringList() << pkgname << pkgover << pkgnver);
    listViewUpdatesPkgs->addTopLevelItem(myItem);
    haveUpdates = true;
    totPkgs++;
  }

  buttonRescanPkgs->setEnabled(true);
  pushUpdatePkgs->setEnabled(haveUpdates);
  if ( totPkgs > 0 ) {
    if ( listUpdates.isEmpty() )
      tabUpdates->setCurrentIndex(1);
    tabUpdates->setTabText(1, tr("Package Updates (%1)").arg(totPkgs));
    groupUpdatesPkgs->setTitle(tr("Available updates"));
  } else {
    tabUpdates->setTabText(1, tr("Package Updates"));
    groupUpdatesPkgs->setTitle(tr("No available updates"));
  }
 
  slotDisplayUpdates();
}

void mainWin::slotSingleInstance() {
   this->hide();
   this->showNormal();
   this->activateWindow();
   this->raise();
}

void mainWin::slotCloseClicked() {
   close();
}

void mainWin::slotUpdatePkgsClicked() {
  // Set our UI elements
  progressUpdate->setHidden(false);
  tabUpdates->setEnabled(false);

  dPackages = false;
  uPackages = false;
  doingUpdate=true;
  curUpdate = 0;

  // Setup the upgrade process
  uProc = new QProcess();
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("PCFETCHGUI", "YES");
  uProc->setProcessEnvironment(env);
  uProc->setProcessChannelMode(QProcess::MergedChannels);

  // Connect the slots
  connect( uProc, SIGNAL(readyReadStandardOutput()), this, SLOT(slotReadPkgUpdateOutput()) );
  connect( uProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotUpdatePkgDone()) );

  if ( wDir.isEmpty() )
    uProc->start("pc-updatemanager", QStringList() << "pkgupdate");
  else
    uProc->start("chroot", QStringList() << wDir << "pc-updatemanager" << "pkgupdate");

  textLabel->setText(tr("Starting package updates..."));

  progressUpdate->setRange(0, listViewUpdatesPkgs->topLevelItemCount() );
  progressUpdate->setValue(0);
}

void mainWin::slotReadPkgUpdateOutput() {
   QString line, tmp, cur, tot, fname;

   while (uProc->canReadLine()) {
     line = uProc->readLine().simplified();
     qDebug() << "Normal Line:" << line;
     tmp = line;
     tmp.truncate(50);
     if ( line.indexOf("to be downloaded") != -1 ) {
       textLabel->setText(tr("Downloading packages..."));
       curUpdate = 0;
       progressUpdate->setValue(0);
       continue;
     }
     if ( line.indexOf("Checking integrity") == 0 ) {
       textLabel->setText(line);
       uPackages = true;
       dPackages = false;
       curUpdate = 0;
       progressUpdate->setValue(0);
     }
     if ( line.indexOf("FETCH: ") == 0 ) { 
	progressUpdate->setValue(progressUpdate->value() + 1); 
	tmp = line; 
	tmp = tmp.remove(0, tmp.lastIndexOf("/") + 1); 
	textLabel->setText(tr("Downloading: %1").arg(tmp)); 
        continue;
     } 
     
     if ( line.indexOf("SIZE: ") == 0 ) {
          bool ok, ok2;
     	  line.replace("SIZE: ", "");
          line.replace("DOWNLOADED: ", "");
          line.replace("SPEED: ", "");
          line.section(" ", 0, 0).toInt(&ok);
          line.section(" ", 1, 1).toInt(&ok2);
   
          if ( ok && ok2 ) {
	    QString unit;
            int tot = line.section(" ", 0, 0).toInt(&ok);
            int cur = line.section(" ", 1, 1).toInt(&ok2);
            QString percent = QString::number( (float)(cur * 100)/tot );
            QString speed = line.section(" ", 2, 3);

  	    // Get the MB downloaded / total
	    if ( tot > 2048 ) {
	      unit="MB";
	      tot = tot / 1024;
       	      cur = cur / 1024;
	    } else {
	      unit="KB";
            }

            QString ProgressString=QString("%1" + unit + " of %2" + unit + " at %3").arg(cur).arg(tot).arg(speed);
            progressUpdate->setRange(0, tot);
            progressUpdate->setValue(cur);
         }
     }

     if ( uPackages ) {
       if ( line.indexOf("Upgrading") == 0 ) {
         textLabel->setText(line);
         curUpdate++;
         progressUpdate->setValue(curUpdate);
       }
       continue;
     }

   } // end of while
}

// Function to read output of pipefile
void mainWin::slotReadEventPipe(int fd) {
  QString tmp, fname, cur, tot;
  bool ok, ok2;
  char buff[4028];
  int totread = read(fd, buff, 4020);
  buff[totread]='\0';
  QString line = buff;
  line = line.simplified();
  //qDebug() << "Found line:" << line;
  
  if ( line.indexOf("INFO_FETCH") != -1  && dPackages ) {
     tmp = line;
     fname = tmp.section(":", 4, 4);
     fname.remove(0, fname.lastIndexOf('/') + 1);
     fname  = fname.section('"', 0, 0);
     cur = tmp.section(":", 5, 5);
     cur = cur.remove(',');
     cur = cur.section(" ", 1, 1);
     cur = cur.simplified();
     tot = tmp.section(":", 6, 6);
     tot = tot.simplified();
     tot = tot.remove(',');
     tot = tot.section("}", 0, 0);

     textLabel->setText(tr("Downloading %1").arg(fname));
     tot.toInt(&ok);
     cur.toInt(&ok2);
     if ( ok && ok2 )
     { 
       progressUpdate->setRange(0, tot.toInt(&ok2));
       progressUpdate->setValue(cur.toInt(&ok2));
     }
     
     //qDebug() << "File:" << fname << "cur" << cur << "tot" << tot;
  }
}

void mainWin::slotUpdatePkgDone() {
  progressUpdate->setHidden(true);
  QFile sysTrig( SYSTRIGGER );
  if ( sysTrig.open( QIODevice::WriteOnly ) ) {
    QTextStream streamTrig( &sysTrig );
     streamTrig << "INSTALLFINISHED: ";
  }

  if ( uProc->exitCode() != 0 )
    QMessageBox::warning(this, tr("Failed package update"), tr("The package update failed! If this persists, you may need to manually run: pc-updatemanager pkgupdate"));

  doingUpdate=false;
  checkMPKGUpdates();
}
