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
#include <pcbsd-ui.h>
#include <QSettings>
#include "mainWin.h"
#include "../config.h"

void mainWin::ProgramInit(QString ch)
{
  // Set any warden directories
  lastError="";
  wDir = ch;
	
  //Grab the username
  //username = QString::fromLocal8Bit(getenv("LOGNAME"));
  connect(pushUpdatePkgs, SIGNAL(clicked()), this, SLOT(slotUpdatePkgsClicked()));
  connect(pushClose, SIGNAL(clicked()), this, SLOT(slotCloseClicked()));
  connect(pushCloseAdv, SIGNAL(clicked()), this, SLOT(slotCloseAdvClicked()));
  connect(buttonRescanPkgs, SIGNAL(clicked()), this, SLOT(slotRescanPkgsClicked()));
  connect(pushPkgApply, SIGNAL( clicked() ), this, SLOT( slotApplyClicked() ) );
  connect(action_Quit, SIGNAL( triggered(bool) ), this, SLOT( slotCloseClicked() ) );
  connect(action_Configuration, SIGNAL( triggered(bool) ), this, SLOT( slotConfigClicked() ) );
  connect(tool_search, SIGNAL( clicked() ), this, SLOT( slotSearchPackages() ) );
  connect(line_search, SIGNAL( returnPressed()), this, SLOT( slotSearchPackages()) );
	
  // Setup the action group
  viewGroup = new QActionGroup(this);
  viewGroup->addAction(action_Basic);
  viewGroup->addAction(action_Advanced);

  treeMetaPkgs->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(treeMetaPkgs, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(slotMetaRightClick()) );

  QSettings settings("PC-BSD", "PackageManager");
  QString curMode = settings.value("view/mode").toString();
  if ( curMode == "Advanced" )
  {
     stackedPkgView->setCurrentIndex(1);
     action_Basic->setChecked(false);
     action_Advanced->setChecked(true);
  }

  // If we are running on a chroot, only do advanced mode
  if ( !wDir.isEmpty() )
  {
     stackedPkgView->setCurrentIndex(1);
     action_Basic->setChecked(false);
     action_Basic->setEnabled(false);
     action_Advanced->setChecked(true);
     action_Advanced->setEnabled(false);
  } else {
    // Connect our view modes only when not running via in a chroot
    connect(action_Basic, SIGNAL( triggered(bool) ), this, SLOT( slotViewChanged() ) );
    connect(action_Advanced, SIGNAL( triggered(bool) ), this, SLOT( slotViewChanged() ) );
  }

  initMetaWidget();

  //Change output palette (white on black)
  QPalette palette = textDisplayOut->palette();
  palette.setColor(QPalette::Base, Qt::black);
  palette.setColor(QPalette::Text, Qt::white);
  textDisplayOut->setPalette(palette);
}

void mainWin::slotViewChanged()
{
  QString mode;
  if ( action_Basic->isChecked() ) {
    stackedPkgView->setCurrentIndex(0);
    mode="Basic";
  } else {
    mode="Advanced";
    stackedPkgView->setCurrentIndex(1);
  }

  // Save the mode as the default at next open
  QSettings settings("PC-BSD", "PackageManager");
  settings.setValue("view/mode", mode);

  // Changed view, lets refresh
  initMetaWidget();
}

void mainWin::slotRescanPkgsClicked()
{
  // Check for pkg updates
  checkMPKGUpdates();
}

void mainWin::slotApplyClicked() {
  // Running in basic mode
  if ( stackedPkgView->currentIndex() == 0 )
  {
     saveMetaPkgs();   
  } else {
     // Running in advanced mode
     applyNGChanges();
  }

}

void mainWin::slotSearchPackages(){
  QString pkgSearch = line_search->text();
  if(pkgSearch.isEmpty()){ return; }
  qDebug() << "Search for package:" <<pkgSearch;
  //Get the pointer to the proper treewidget
  QTreeWidget *TW = treeNGPkgs;
  if( stackedPkgView->currentIndex() == 0 ){ TW = treeMetaPkgs; }
  //Make sure the tree widget is not empty
  if(TW->topLevelItemCount() < 2){ return; }
  //Get the currently selected item
  QTreeWidgetItem *CI = TW->currentItem();
  bool found=false; bool atTop=false;
  if(CI == 0){ atTop=true; }
  //Now search the tree, starting at that item
  found = performSearch(pkgSearch, TW, CI);
  if(!found && !atTop){
    //Ask whether to restart the search at the top 
    if(QMessageBox::Yes == QMessageBox::question(this,tr("No Search Results"),tr("Do you want to continue the search from the top?"),QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) ){
      //Restart the search from the top
      found = performSearch(pkgSearch, TW, 0);
    }
  }
  if(!found){
    QMessageBox::information(this, tr("No Search Results"), tr("No packages could be found with that search term. Please adjust it and try again.") );
  }
  qDebug() << " - Search Finished";
}

bool mainWin::performSearch(QString pkgSearch, QTreeWidget *TW, QTreeWidgetItem *SI){
  //Start Iterating over the tree
  bool found=false;
  bool started = false;
  //if(SI==0){ started = true; }
  for(int p=0; (p<TW->topLevelItemCount()) && !found; p++){
    //Check the actual item itself
    QTreeWidgetItem *CI = TW->topLevelItem(p);
    if(started && CI->text(0).contains(pkgSearch, Qt::CaseInsensitive)){
      TW->setCurrentItem(CI);
      TW->scrollToItem(CI);
      found=true;	  
    }else{    
      found = searchChildren(pkgSearch, TW, CI, started, SI);
    }
  }
  return found;
}

bool mainWin::searchChildren(QString srch, QTreeWidget *TW, QTreeWidgetItem *CI, bool &started, QTreeWidgetItem *SI){
  //This is a recursive function for searching all the children of a particular item
  // TW - TreeWidget pointer
  // CI - Current TreeWidget Item (to search the children of)
  // SI - Start Item (Try to start searching right after this item - optional)
  // bool started - Start Item found and search has been started (optional input/output)
	
  //qDebug() << "Search Children of:" << CI->text(0) << srch << started;
  //Check for the start position
  int start = -1;
  if(SI == 0 || SI == CI){
    //No search item to start at
    start = 0;
    started = true;
  }else if( !started){
    QTreeWidgetItem *PI = SI;
      while( (start == -1) && (PI!=0) ){
        start = CI->indexOfChild(PI);
        PI = PI->parent(); //look up one more layer to make sure it is not a child of one of these items
      }
  }else{ start = 0; } //start with the first child
  //Now quit if the start item is not found here
  if(start == -1){ started = false; return false; }
  
  //Now start searching
  bool found = false;
  for(int i=start; (i<CI->childCount()) && !found ; i++){
    if(started){
      //Check this item 
      if(CI->child(i)->text(0).contains(srch, Qt::CaseInsensitive)){
        TW->setCurrentItem(CI->child(i));
	TW->scrollToItem(CI->child(i));
	found=true;
	break;
      }
    }else if( SI == CI->child(i) ){
      started = true; //but don't look at this item, continue on to the next one (or children)
    }
    if(found){ break; }
    else if(CI->child(i)->childCount() > 0){
      //recursively search this items children
      found = searchChildren(srch, TW, CI->child(i), started, SI); 
    }
  }
  return found;
}

void mainWin::checkMPKGUpdates() {

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

  if ( p.exitCode() != 0 )
    QMessageBox::warning(this, tr("Package Check"), tr("Unable to check for package updates!"));

  while (p.canReadLine()) {
    line = p.readLine().simplified();
    qDebug() << line;
    if ( line.indexOf("Upgrading") == 0 ) {
      tmp = line;
      pkgname = tmp.section(" ", 1, 1);
      pkgname.replace(":", "");
      pkgover = tmp.section(" ", 2, 2);
      pkgnver = tmp.section(" ", 4, 4);
      QTreeWidgetItem *myItem = new QTreeWidgetItem(QStringList() << pkgname << pkgover << pkgnver);
      listViewUpdatesPkgs->addTopLevelItem(myItem);
      haveUpdates = true;
      totPkgs++;
    } // End of upgrading section
    if ( line.indexOf("Reinstalling") == 0 ) {
      tmp = line;
      pkgname = tmp.section(" ", 1, 1);
      pkgover = pkgname.section("-", -1);
      pkgname.truncate(pkgname.lastIndexOf("-"));
      pkgnver = tmp.section(" ", 2);
      QTreeWidgetItem *myItem = new QTreeWidgetItem(QStringList() << pkgname << pkgover << pkgnver);
      listViewUpdatesPkgs->addTopLevelItem(myItem);
      haveUpdates = true;
      totPkgs++;
    }
  }

  buttonRescanPkgs->setEnabled(true);
  pushUpdatePkgs->setEnabled(haveUpdates);
  if ( totPkgs > 0 ) {
    tabUpdates->setTabText(1, tr("Package Updates (%1)").arg(totPkgs));
    groupUpdatesPkgs->setTitle(tr("Available updates"));
  } else {
    tabUpdates->setTabText(1, tr("Package Updates"));
    groupUpdatesPkgs->setTitle(tr("No available updates"));
  }
 
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
  dPackages = false;
  uPackages = false;

  // Set the type of pkg command
  pkgProcessType="update";

  // Init the pkg process
  prepPkgProcess();

  // Create our runlist of package commands
  QStringList pCmds;

  if ( wDir.isEmpty() )
    pCmds << "pc-updatemanager" << "pkgupdate";
  else
    pCmds << "chroot" << wDir << "pc-updatemanager" << "pkgupdate";

  // Setup our runList
  pkgCmdList << pCmds;

  // Start the updating now
  startPkgProcess();

  textStatus->setText(tr("Starting package updates..."));

}

QString mainWin::getConflictDetailText() {

  QStringList ConList = ConflictList.split(" ");
  QStringList tmpDeps;
  QString retText;

  for (int i = 0; i < ConList.size(); ++i) {
    QProcess p;
    tmpDeps.clear();

    if ( wDir.isEmpty() )
      p.start("pkg", QStringList() << "rquery" << "%rn-%rv" << ConList.at(i));
    else
      p.start("chroot", QStringList() << wDir << "pkg" "rquery" << "%rn-%rv" << ConList.at(i) );

    if(p.waitForFinished()) {
      while (p.canReadLine()) {
        tmpDeps << p.readLine().simplified();
      }
    }
    retText+= ConList.at(i) + " " + tr("required by:") + "\n" + tmpDeps.join(" ");
  }

  return retText;
}

void mainWin::prepPkgProcess() {
  pkgCmdList.clear();
  textDisplayOut->clear();
  pkgHasFailed=false;
}

void mainWin::startPkgProcess() {

  if ( pkgCmdList.isEmpty() )
    return;
  if ( pkgCmdList.at(0).at(0).isEmpty() )
     return; 

  // Get the command name
  QString cmd;
  cmd = pkgCmdList.at(0).at(0);

  // Get any optional flags
  QStringList flags;
  for (int i = 0; i < pkgCmdList.at(0).size(); ++i) {
     if ( i == 0 )
       continue;
     flags << pkgCmdList.at(0).at(i);
  } 

  qDebug() << cmd + " " + flags.join(" ");

  system("rm /tmp/pkg-fifo 2>/dev/null");

  // Create the EVENT_PIPE
  if ( wDir.isEmpty() )
    system("mkfifo /tmp/pkg-fifo ; sleep 1");
  else
    system("mkfifo " + wDir.toLatin1() + "/tmp/pkg-fifo ; sleep 1");

  // Open and connect the EVENT_PIPE
  eP = new QProcess();
  eP->setProcessChannelMode(QProcess::MergedChannels);
  connect( eP, SIGNAL(readyRead()), this, SLOT(slotReadEventPipe()) );
  connect( eP, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotReadEventPipe()) );
  eP->start(QString("cat"), QStringList() << "-u" << wDir + "/tmp/pkg-fifo");
  qDebug() << "Starting EVENT_PIPE";
  eP->waitForStarted();

  // Setup the first process
  uProc = new QProcess();
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("PCFETCHGUI", "YES");
  env.insert("EVENT_PIPE", "/tmp/pkg-fifo");
  uProc->setProcessEnvironment(env);
  uProc->setProcessChannelMode(QProcess::MergedChannels);

  // Connect the slots
  connect( uProc, SIGNAL(readyReadStandardOutput()), this, SLOT(slotReadPkgOutput()) );
  connect( uProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotPkgDone()) );

  uProc->start(cmd, flags);

  stackedTop->setCurrentIndex(1);

  progressUpdate->setRange(0, 0 );
  progressUpdate->setValue(0);

}

void mainWin::slotReadPkgOutput() {
   QString line, tmp, cur, tot, fname;
   int curItem, totItem;
   bool ok;

   while (uProc->canReadLine()) {
     line = uProc->readLine().simplified();
     qDebug() << line;

     // Empty line? We can skip it
     if ( line.isEmpty() )
	continue;

     tmp = line;
     tmp.truncate(50);

     // Flags we can parse out and not show the user

     // Check if we have crashed into a conflict and ask the user what to do
     if ( line.indexOf("PKGCONFLICTS: ") == 0 ) {
	tmp = line; 
     	tmp.replace("PKGCONFLICTS: ", "");
        ConflictList = tmp;
        continue;
     }
     if ( line.indexOf("PKGREPLY: ") == 0 ) {
	QString ans;
	tmp = line; 
     	tmp.replace("PKGREPLY: ", "");
        QMessageBox msgBox;
 	msgBox.setText(tr("The following packages are causing conflicts with the selected changes and can be automatically removed. Continue?") + "\n" + ConflictList);
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
   	msgBox.setDetailedText(getConflictDetailText());
        msgBox.setDefaultButton(QMessageBox::No);
        if ( msgBox.exec() == QMessageBox::Yes) {
	  // We will try to fix conflicts
	  ans="yes";
        } else {
	  // We will fail :(
          QMessageBox::warning(this, tr("Package Conflicts"),
          tr("You may need to manually fix the conflicts before trying again."),
          QMessageBox::Ok,
          QMessageBox::Ok);
	  ans="no";
        }

        QFile pkgTrig( tmp );
        if ( pkgTrig.open( QIODevice::WriteOnly ) ) {
           QTextStream streamTrig( &pkgTrig );
           streamTrig << ans;
	   pkgTrig.close();
	}
	continue;
     }

     if ( line.indexOf("FETCH: ") == 0 ) { 
	progressUpdate->setValue(progressUpdate->value() + 1); 
	tmp = line; 
	tmp = tmp.remove(0, tmp.lastIndexOf("/") + 1); 
        progressUpdate->setRange(0, 0);
        progressUpdate->setValue(0);
	curFileText = tr("Downloading: %1").arg(tmp); 
	textStatus->setText(tr("Downloading: %1").arg(tmp)); 
        continue;
     } 
     if ( line.indexOf("FETCHDONE") == 0 )
        continue;

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

            QString ProgressString=QString("(%1" + unit + " of %2" + unit + " at %3)").arg(cur).arg(tot).arg(speed);
            progressUpdate->setRange(0, tot);
            progressUpdate->setValue(cur);
	    textStatus->setText(curFileText + " " + ProgressString); 
         }
         continue;
     }


     // Now show output on GUI
     textDisplayOut->insertPlainText(line + "\n");
     textDisplayOut->moveCursor(QTextCursor::End);


     // Any other flags to look for?
     /////////////////////////////////////////////////////
     if ( line.indexOf("to be downloaded") != -1 ) {
       textStatus->setText(tr("Downloading packages..."));
       curUpdate = 0;
       progressUpdate->setValue(0);
       continue;
     }
     if ( line.indexOf("Checking integrity") == 0 ) {
       textStatus->setText(line);
       uPackages = true;
       dPackages = false;
       curUpdate = 0;
       progressUpdate->setValue(0);
       progressUpdate->setRange(0, 0);
       progressUpdate->setValue(0);
       continue;
     }
     
     if ( uPackages ) {
       if ( line.indexOf("[") == 0 ) {
	 tmp=line.section("]", 1, 1);
         textStatus->setText(tmp);
         tmp=line.section("/", 0, 0).replace("[", "");
	 tmp.toInt(&ok);
	 if (ok)  {
	   curItem=tmp.toInt(&ok);
           tmp=line.section("/", 1, 1).section("]", 0, 0);
	   tmp.toInt(&ok);
	   if (ok)  {
	     totItem=tmp.toInt(&ok);
             progressUpdate->setRange(0, totItem);
             progressUpdate->setValue(curItem);
	   }

         }
       }
       continue;
     }

   } // end of while
}

void mainWin::slotPkgDone() {

  if ( uProc->exitCode() != 0 )
    pkgHasFailed=true;

  // Close the event pipe
  eP->kill();
  qDebug() << "Stopping EVENT_PIPE";
  system("rm " + wDir.toLatin1() + "/tmp/pkg-fifo");

  // Run the next command on the stack if necessary
  if (  pkgCmdList.size() > 1 ) {
	pkgCmdList.removeAt(0);	
        startPkgProcess();	
	return;
  }

  // Eventually we will have more stuff to do after running a package command
  //if ( pkgProcessType == "update" )
  //{
  //}

  // Nothing left to run! Lets wrap up
  QFile sysTrig( SYSTRIGGER );
  if ( sysTrig.open( QIODevice::WriteOnly ) ) {
    QTextStream streamTrig( &sysTrig );
     streamTrig << "INSTALLFINISHED: ";
  }

  if ( pkgHasFailed ) {
    QFile file( "/tmp/pkg-output.log" );
    if ( file.open( QIODevice::WriteOnly ) ) {
       QTextStream stream( &file );
       stream << textDisplayOut->toPlainText();
       file.close();
    }
    QMessageBox::warning(this, tr("Failed!"), tr("The package commands failed. A copy of the output was saved to /tmp/pkg-output.log"));
  } else
    QMessageBox::warning(this, tr("Finished!"), tr("Package changes complete!" ));

  // Clear out the old commands
  pkgCmdList.clear();

  // Switch back to our main display
  stackedTop->setCurrentIndex(0);
 
  // Re-init the meta-widget
  initMetaWidget();

}

/*****************************************
Code for package stuff
******************************************/

void mainWin::initMetaWidget()
{
  qDebug() << "Starting metaWidget...";
  groupInfo->setVisible(false);
  //Make sure the search box is disabled at startup
  tool_search->setEnabled(false);

  // We will refresh the update tab after, clear it out for now
  buttonRescanPkgs->setEnabled(false);
  pushUpdatePkgs->setEnabled(false);
  listViewUpdatesPkgs->clear();
  groupUpdatesPkgs->setTitle(tr("Reading package database..."));

  // Running in basic mode
  if ( stackedPkgView->currentIndex() == 0 )
  {
    populateMetaPkgs();
    // Connect our slots
  } else {
    // Running in advanced mode
    populateNGPkgs();
  }
}

void mainWin::populateNGPkgs()
{
  pushPkgApply->setEnabled(false);
  treeNGPkgs->clear();
  tmpPkgList.clear();
  new QTreeWidgetItem(treeNGPkgs, QStringList() << tr("Loading... Please wait...") );

  if ( ! pkgList.isEmpty() ) {
  	disconnect(treeNGPkgs, SIGNAL(itemChanged(QTreeWidgetItem *, int)), 0, 0);
  	disconnect(treeNGPkgs, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), 0, 0);
  }
  pkgList.clear();
  selPkgList.clear();

  // Start the process to get meta-pkg info
  getNGProc = new QProcess();
  qDebug() << "Searching for pkgs...";
  connect( getNGProc, SIGNAL(readyReadStandardOutput()), this, SLOT(slotGetNGPackageDataOutput()) );
  connect( getNGProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotGetNGInstalledPkgs()) );
  getNGProc->setProcessChannelMode(QProcess::MergedChannels);
  if ( wDir.isEmpty() )
    getNGProc->start(QString("pkg"), QStringList() << "rquery" << "-a" << "%o:::%n-%v:::%c:::%sh:::%m:::%w");
  else
    getNGProc->start(QString("chroot"), QStringList() << wDir << "pkg" << "rquery" << "-a" << "%o:::%n-%v:::%c:::%sh:::%m:::%w");

}

void mainWin::slotGetNGPackageDataOutput()
{
   while (getNGProc->canReadLine())
     tmpPkgList << getNGProc->readLine().simplified();
}

void mainWin::slotGetNGInstalledDataOutput()
{
   while (getNGProc->canReadLine())
     pkgList << getNGProc->readLine().simplified();
}

void mainWin::slotGetNGInstalledPkgs() {

  qDebug() << "Building dependancy lists...";
  QProcess p;
  pkgDepList.clear();
  if ( wDir.isEmpty() )
    p.start("pkg", QStringList() << "rquery" << "-a" << "%n-%v:::%dn-%dv");
  else
    p.start("chroot", QStringList() << wDir << "pkg" << "rquery" << "-a" << "%n-%v:::%dn-%dv" );
  while(p.state() == QProcess::Starting || p.state() == QProcess::Running) {
      p.waitForFinished(200);
      QCoreApplication::processEvents();
  }
  while (p.canReadLine()) {
    pkgDepList << p.readLine().simplified();
  }

  getNGProc = new QProcess();
  qDebug() << "Searching for installed pkgs...";
  connect( getNGProc, SIGNAL(readyReadStandardOutput()), this, SLOT(slotGetNGInstalledDataOutput()) );
  connect( getNGProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotFinishLoadingNGPkgs()) );
  getNGProc->setProcessChannelMode(QProcess::MergedChannels);
  if ( wDir.isEmpty() )
    getNGProc->start(QString("pkg"), QStringList() << "info" << "-aq" );
  else
    getNGProc->start(QString("chroot"), QStringList() << wDir << "pkg" << "info" << "-aq");

}

void mainWin::slotFinishLoadingNGPkgs()
{
  treeNGPkgs->clear();

  addNGItems();

  pushPkgApply->setEnabled(false);

  connect(treeNGPkgs, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(slotEnableApply()));
  connect(treeNGPkgs, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this, SLOT(slotNGItemChanged()));
  //Enable the search option
  tool_search->setEnabled(true);
  // Now we can look for updates safely
  slotRescanPkgsClicked();
}

void mainWin::slotNGItemChanged()
{
  if ( ! treeNGPkgs->currentItem() ) {
     groupInfo->setVisible(false);
     return;
  }
  QString desc, size, maint, weburl;

  QTreeWidgetItem *cItem = treeNGPkgs->currentItem();
  QString pName = cItem->text(0).section("(", 1, 1).section(")", 0, 0);
  if ( pName.isEmpty() ) {
     groupInfo->setVisible(false);
     return;
  }
  qDebug() << "Checking: " + pName;

  QRegExp rx("*:::" + pName + ":::*");
  rx.setPatternSyntax(QRegExp::Wildcard);
  int pAt = tmpPkgList.indexOf(rx);
  if (pAt == -1 ) {
     qDebug() << "Unable to find package: " + pName;
     groupInfo->setVisible(false);
     return;
  }

  desc = tmpPkgList.at(pAt).section(":::", 2,2);
  size = tmpPkgList.at(pAt).section(":::", 3,3);
  maint = tmpPkgList.at(pAt).section(":::", 4,4);
  weburl = tmpPkgList.at(pAt).section(":::", 5,5);
  labelPkgNameVer->setText(pName);
  labelSize->setText(size);
  labelWeb->setText(weburl);
  textDesc->setText(desc);
  textOptions->clear();

  QCoreApplication::processEvents();

  // Display the depends
  QString depTxt;
  QRegExp rxd( pName + ":::*");
  rxd.setPatternSyntax(QRegExp::Wildcard);
  QStringList aDeps = pkgDepList.filter(rxd);
  for ( int r=0; r < aDeps.size(); ++r) {
      QString dName = aDeps.at(r).section(":::", 1, 1);
      // Is this package installed?
      if ( pkgList.indexOf(dName) != -1 )
         depTxt+= dName + " (Installed)\n";
      else
         depTxt+= dName + "\n";
  }

  textDeps->setText(depTxt);

  groupInfo->setVisible(true);

  getNGInfo = new QProcess();
  qDebug() << "Getting Info for " + pName;
  connect( getNGInfo, SIGNAL(readyReadStandardOutput()), this, SLOT(slotNGReadInfo()) );
  getNGInfo->setProcessChannelMode(QProcess::MergedChannels);
  if ( wDir.isEmpty() )
    getNGInfo->start(QString("pkg"), QStringList() << "rquery" << "%Ok=%Ov" << pName );
  else
    getNGInfo->start(QString("chroot"), QStringList() << wDir << "pkg" << "rquery" << "%Ok=%Ov" << pName);
}

void mainWin::slotNGReadInfo()
{
  while (getNGInfo->canReadLine())
     textOptions->append(getNGInfo->readLine().simplified() );

  textOptions->moveCursor(QTextCursor::Start);
}

void mainWin::slotEnableApply()
{
  pushPkgApply->setEnabled(true);
}

void mainWin::addNGItems()
{
   QString curCat, cat, name, pkgname, desc, size, maint, weburl;

   // We like to add alphabetically
   tmpPkgList.sort();

   QTreeWidgetItem *catItem = new QTreeWidgetItem;

   // Lets start adding packages to the tree widget
   for (int i = 0; i < tmpPkgList.size(); ++i) {
	name = cat = tmpPkgList.at(i).section(":::", 0,0);
	cat=cat.section("/", 0, 0);
	name=name.section("/", 1, 1);
	pkgname = tmpPkgList.at(i).section(":::", 1,1);
	desc = tmpPkgList.at(i).section(":::", 2,2);
	size = tmpPkgList.at(i).section(":::", 3,3);
	maint = tmpPkgList.at(i).section(":::", 4,4);
	weburl = tmpPkgList.at(i).section(":::", 5,5);

	// Check if we need to add a top-level category
	if ( cat != curCat )
        {
	   qDebug() << "Adding cat: " + cat;
           catItem = new QTreeWidgetItem(QStringList() << cat);
           treeNGPkgs->addTopLevelItem(catItem);
           curCat = cat;
        }
 
	// Now lets create the item and attach to the category
        QTreeWidgetItem *pkgItem = new QTreeWidgetItem();
        pkgItem->setText(0, name + " (" + pkgname + ") - " + size );
        pkgItem->setToolTip(0, desc);

        if ( pkgList.indexOf(pkgname) != -1 ) {
          pkgItem->setCheckState(0, Qt::Checked);
	  selPkgList << pkgname;
        } else
          pkgItem->setCheckState(0, Qt::Unchecked);
  
        catItem->addChild(pkgItem);
   }
}

// Lets prompt user, and do it!
void mainWin::applyNGChanges()
{
   QString tmp;
   QStringList curPkgChecked;
   QStringList newPkgs;
   QStringList rmPkgs;

   QTreeWidgetItemIterator it(treeNGPkgs);
   while (*it) {
         if ((*it)->checkState(0) == Qt::Checked) {
	   tmp = (*it)->text(0).section("(", 1, 1).section(")", 0, 0);
	   curPkgChecked << tmp;
	   if (pkgList.indexOf(tmp) == -1 ) 
	      newPkgs << tmp;
	 }
         ++it;
   }

   for ( int i=0; i < pkgList.size(); ++i)
      // Has this package been unchecked?
      if (curPkgChecked.indexOf(pkgList.at(i)) == -1 )  {
	 // Make sure this is a package in the repo
	 // This filters out any custom packages the user may have loaded which may not exist in our repo
	 QRegExp rx("*" + pkgList.at(i) + "*");
         rx.setPatternSyntax(QRegExp::Wildcard);
	 if ( tmpPkgList.indexOf(rx) != -1 )
	   rmPkgs << pkgList.at(i);
      }

   if ( rmPkgs.isEmpty() && newPkgs.isEmpty() ) {
      QMessageBox::warning(this, tr("No changes"),
        tr("No changes to make!"),
        QMessageBox::Ok,
        QMessageBox::Ok);
      return;
   }

   qDebug() << "Added packages" << newPkgs;
   qDebug() << "Removed packages" << rmPkgs;
   pkgRemoveList = rmPkgs;
   pkgAddList = newPkgs;

   QString confirmText;

   // Lets start creating our confirmation text
   if ( ! rmPkgs.isEmpty() ) {
      confirmText+=tr("The following packages will be removed:") + "\n"; 
      confirmText+= "------------------------------------------\n";
      confirmText+=rmPkgs.join("\n"); 
      confirmText+= "\n\n" + tr("The following packages that require the above packages will also removed:") + "\n"; 
      confirmText+= "------------------------------------------\n";
      for ( int i=0; i < rmPkgs.size(); ++i) {
	
	 // Get rdeps for this pkg
         qDebug() << "Building reverse dependancy lists...";
         pkgRDepList.clear();
	 QProcess p;
         if ( wDir.isEmpty() )
           p.start("pkg", QStringList() << "rquery" << "%n-%v:::%rn-%rv" << rmPkgs.at(i));
         else
           p.start("chroot", QStringList() << wDir << "pkg" << "rquery" << "%n-%v:::%rn-%rv" << rmPkgs.at(i) );
         while(p.state() == QProcess::Starting || p.state() == QProcess::Running) {
           p.waitForFinished(200);
           QCoreApplication::processEvents();
         }
         while (p.canReadLine()) {
           pkgRDepList << p.readLine().simplified();
         }

	 QRegExp rx(rmPkgs.at(i) + ":::*");
         rx.setPatternSyntax(QRegExp::Wildcard);
         QStringList rDeps = pkgRDepList.filter(rx);
         for ( int r=0; r < rDeps.size(); ++r) {
             QString pName = rDeps.at(r).section(":::", 1, 1); 
	     // Is this package installed?
	     if ( pkgList.indexOf(pName) != -1 )
               confirmText+= pName + " ";
         }
      }
   }

   if ( ! newPkgs.isEmpty() ) {
      if ( ! rmPkgs.isEmpty() )
        confirmText+= "\n\n";
      confirmText+=tr("The following packages will be installed:") + "\n"; 
      confirmText+= "------------------------------------------\n";
      confirmText+=newPkgs.join("\n"); 
      confirmText+= "\n\n" + tr("The following dependances will also be installed:") + "\n"; 
      confirmText+= "------------------------------------------\n";
      for ( int i=0; i < newPkgs.size(); ++i) {
	 QRegExp rx(newPkgs.at(i) + ":::*");
         rx.setPatternSyntax(QRegExp::Wildcard);
         QStringList aDeps = pkgDepList.filter(rx);
         for ( int r=0; r < aDeps.size(); ++r) {
             QString pName = aDeps.at(r).section(":::", 1, 1); 
	     // Is this package installed?
	     if ( pkgList.indexOf(pName) == -1 )
               confirmText+= pName + " ";
         }
      }
   }

   // Launch our AddPartitionDialog to add a new device
   askUserConfirm = new dialogConfirm();
   connect(askUserConfirm, SIGNAL(ok()),this, SLOT(slotStartNGChanges()) );
   askUserConfirm->programInit(tr("Confirm package changes"));
   askUserConfirm->setInfoText(QString(confirmText));
   askUserConfirm->exec();

}


// Time to start doing our NG changes!
void mainWin::slotStartNGChanges()
{
  // Init the pkg process
  prepPkgProcess();

  // Create our runlist of package commands
  QStringList pCmds;
  
  if ( ! pkgRemoveList.isEmpty() ) {
    pkgProcessType="delete";
    if ( wDir.isEmpty() )
      pCmds << "pkg" << "delete" << "-R" << "-y" << pkgRemoveList.join(" ");
    else
      pCmds << "chroot" << wDir << "pkg" << "delete" << "-R" << "-y" << pkgRemoveList.join(" ");
    pkgCmdList << pCmds;
  }
         
  pCmds.clear();

  // Adding packages
  if ( ! pkgAddList.isEmpty() ) {
    pkgProcessType="add";

    // Look for conflicts first
    if ( wDir.isEmpty() )
      pCmds << "pc-pkg" << "check-conflict" << pkgAddList.join(" ");
    else
      pCmds << "chroot" << wDir << "pc-pkg" << "check-conflict" << pkgAddList.join(" ");
    pkgCmdList << pCmds;
    pCmds.clear();

    // Now spin up the install process
    if ( wDir.isEmpty() )
      pCmds << "pkg" << "install" << "-y" << pkgAddList.join(" ");
    else
      pCmds << "chroot" << wDir << "pkg" << "install" << "-y" << pkgAddList.join(" ");

    pkgCmdList << pCmds;
  }

  // Lets kick it off now
  startPkgProcess();
}

// Display found meta-pkg data
void mainWin::populateMetaPkgs()
{
  pushPkgApply->setEnabled(false);
  treeMetaPkgs->clear();
  tmpMetaPkgList.clear();
  new QTreeWidgetItem(treeMetaPkgs, QStringList() << tr("Loading... Please wait...") );

  if ( ! metaPkgList.isEmpty() )
  	disconnect(treeMetaPkgs, SIGNAL(itemChanged(QTreeWidgetItem *, int)), 0, 0);
  metaPkgList.clear();

  // Start the process to get meta-pkg info
  getMetaProc = new QProcess();
  qDebug() << "Searching for meta-pkgs...";
  connect( getMetaProc, SIGNAL(readyReadStandardOutput()), this, SLOT(slotGetPackageDataOutput()) );
  connect( getMetaProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotFinishLoadingMetaPkgs()) );
  getMetaProc->setProcessChannelMode(QProcess::MergedChannels);
  getMetaProc->start(QString("pc-metapkgmanager"), QStringList() << "list");

}

// Display found meta-pkg data
void mainWin::slotFinishLoadingMetaPkgs()
{

  // Populate the metaPkgList
  parseTmpMetaList();

  treeMetaPkgs->clear();

  addTreeItems(QString()); 

  pushPkgApply->setEnabled(false);

  connect(treeMetaPkgs, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(slotDeskPkgsChanged(QTreeWidgetItem *, int)));
  //Enable the search option
  tool_search->setEnabled(true);
  // Now we can look for updates safely
  slotRescanPkgsClicked();
}

void mainWin::addTreeItems(QString parent)
{
  for (int z=0; z < metaPkgList.count(); ++z) {
    if ( metaPkgList.at(z).at(3) != parent )
      continue;

    // Hide the "base-system" package, since we are installing it anyway
    if (metaPkgList.at(z).at(0) == "base-system" )
      return;

    QTreeWidgetItem *deskItem = new QTreeWidgetItem(QStringList() << metaPkgList.at(z).at(0) );
    deskItem->setIcon(0, QIcon(metaPkgList.at(z).at(2)));
    deskItem->setToolTip(0, metaPkgList.at(z).at(1));
    deskItem->setCheckState(0, Qt::Unchecked);

    if ( metaPkgList.at(z).at(5) == "YES" )
      deskItem->setCheckState(0, Qt::Checked);

    if ( parent.isEmpty() ) {
      treeMetaPkgs->addTopLevelItem(deskItem);
    } else {
      // Locate the parent to attach to
      QTreeWidgetItemIterator it(treeMetaPkgs);
      while (*it) {
        if ((*it)->text(0) == parent ) {
	  (*it)->addChild(deskItem);
          if ( metaPkgList.at(z).at(5) == "YES" && (*it)->checkState(0) == Qt::Unchecked)
	    (*it)->setCheckState(0, Qt::PartiallyChecked);
          if ( metaPkgList.at(z).at(5) == "NO" && (*it)->checkState(0) == Qt::Checked)
	    (*it)->setCheckState(0, Qt::PartiallyChecked);
	  break;
	}
        it++;
      }
    }

    // Now look for any possible children
    addTreeItems(metaPkgList.at(z).at(0));    
  }

}

// Check if a meta-pkg is installed
bool mainWin::isMetaPkgInstalled(QString mPkg)
{
  QString tmp;
  QProcess pcmp;
  pcmp.start(QString("pc-metapkgmanager"), QStringList() << chrootArg1 << chrootArg2 << "status" << mPkg);
  while ( pcmp.state() != QProcess::NotRunning ) {
     pcmp.waitForFinished(50);
     QCoreApplication::processEvents();
  }

  while (pcmp.canReadLine()) {
     tmp = pcmp.readLine().simplified();
     if ( tmp.indexOf("is installed") != -1 )
     return true;
  }

  return false;
}

// Function which checks for our GUI package schema data
void mainWin::slotGetPackageDataOutput()
{
  while (getMetaProc->canReadLine())
	tmpMetaPkgList << getMetaProc->readLine().simplified();
}

// Parse the pc-metapkg saved output
void mainWin::parseTmpMetaList()
{
  QString tmp, mName, mDesc, mIcon, mParent, mDesktop, mInstalled, mPkgFileList;
  QStringList package;

  for ( int i = 0 ; i < tmpMetaPkgList.size(); i++ )
  {
	QApplication::processEvents();

        tmp = tmpMetaPkgList.at(i);

	if ( tmp.indexOf("Meta Package: ") == 0) {
		mName = tmp.replace("Meta Package: ", "");
		continue;
	}
	if ( tmp.indexOf("Description: ") == 0) {
		mDesc = tmp.replace("Description: ", "");
		continue;
	}
	if ( tmp.indexOf("Icon: ") == 0) {
		mIcon = tmp.replace("Icon: ", "");
		mPkgFileList = mIcon;
		mPkgFileList.replace("pkg-icon.png", "pkg-list");
		continue;
	}
	if ( tmp.indexOf("Parent: ") == 0) {
		mParent = tmp.replace("Parent: ", "");
		continue;
	}
	if ( tmp.indexOf("Desktop: ") == 0) {
		mDesktop = tmp.replace("Desktop: ", "");
		continue;
	}

	// This is an empty category
	if ( tmp.indexOf("Category Entry") == 0) {
		// Now add this category to the string list
		package.clear();
		qDebug() << "Found Package" << mName << mDesc << mIcon << mParent << mDesktop;
		mInstalled = "CATEGORY";
		package << mName << mDesc << mIcon << mParent << mDesktop << mInstalled;
		metaPkgList.append(package);
		mName=""; mDesc=""; mIcon=""; mParent=""; mDesktop=""; mInstalled=""; mPkgFileList="";
	}

	// We have a Meta-Pkg
	if ( tmp.indexOf("Required Packages:") == 0) {
		// Now add this meta-pkg to the string list
		package.clear();
		qDebug() << "Found Package" << mName << mDesc << mIcon << mParent << mDesktop << mPkgFileList;

		if ( isMetaPkgInstalled(mName) )
			mInstalled = "YES";
		else
			mInstalled = "NO";

		package << mName << mDesc << mIcon << mParent << mDesktop << mInstalled << mPkgFileList;
		metaPkgList.append(package);
		mName=""; mDesc=""; mIcon=""; mParent=""; mDesktop=""; mInstalled=""; mPkgFileList="";
	}

    }

}

void mainWin::saveMetaPkgs()
{
	if ( ! haveMetaPkgChanges() )
		return;

	addPkgs = getAddPkgs();
	delPkgs = getDelPkgs();	

	startMetaChanges();

}

void mainWin::startMetaChanges()
{

  // Init the pkg process
  prepPkgProcess();
  // Create our runlist of package commands
  QStringList pCmds;

  if ( ! delPkgs.isEmpty() ) {
    pkgProcessType="deletemeta";
    if ( wDir.isEmpty() )
      pCmds << "pc-metapkgmanager" << "del" << delPkgs;
    else  
      pCmds << "chroot" << wDir << "pc-metapkgmanager" << "del" << delPkgs;
    pkgCmdList << pCmds;
  }
  
  pCmds.clear();

  if ( ! addPkgs.isEmpty() ) {
    pkgProcessType="addmeta";
    if ( wDir.isEmpty() )
      pCmds << "pc-metapkgmanager" << "add" << addPkgs;
    else  
      pCmds << "chroot" << wDir << "pc-metapkgmanager" << "add" << addPkgs;
    pkgCmdList << pCmds;
  }

  // Lets kick it off now
  startPkgProcess();
}

bool mainWin::haveAMetaDesktop()
{
	// If running in a chroot we can skip this check
	if ( ! chrootArg1.isEmpty() )
	  return true;
        
        QTreeWidgetItemIterator it(treeMetaPkgs);
        while (*it) {
         if ( ((*it)->checkState(0) == Qt::Checked) || ((*it)->checkState(0) == Qt::PartiallyChecked) )
	   for (int z=0; z < metaPkgList.count(); ++z)
	     if ( (*it)->text(0) == metaPkgList.at(z).at(0) && metaPkgList.at(z).at(4) == "YES" )
		return true;
         ++it;
        }

        QMessageBox::warning(this, tr("No Desktop"),
          tr("No desktops have been selected! Please choose at least one desktop before saving."),
          QMessageBox::Ok,
          QMessageBox::Ok);

	return false;
}

bool mainWin::haveMetaPkgChanges()
{
        QTreeWidgetItemIterator it(treeMetaPkgs);
        while (*it) {
	  for (int z=0; z < metaPkgList.count(); ++z)
	    // See if any packages status have changed
	    if ( ( (*it)->text(0) == metaPkgList.at(z).at(0) && metaPkgList.at(z).at(5) == "YES" && (*it)->checkState(0) == Qt::Unchecked ) \
	      || ( (*it)->text(0) == metaPkgList.at(z).at(0) && metaPkgList.at(z).at(5) == "YES" && (*it)->checkState(0) == Qt::PartiallyChecked ) \
	      || ( (*it)->text(0) == metaPkgList.at(z).at(0) && metaPkgList.at(z).at(5) == "NO" && (*it)->checkState(0) == Qt::Checked ) )
		return true;
         ++it;
        }

	return false;
}

QString mainWin::getAddPkgs()
{
	QString tmp;
        QTreeWidgetItemIterator it(treeMetaPkgs);
        while (*it) {
	  for (int z=0; z < metaPkgList.count(); ++z)
	    // See if any packages status have changed
	    if ( ( (*it)->text(0) == metaPkgList.at(z).at(0) && metaPkgList.at(z).at(5) == "NO" && (*it)->checkState(0) == Qt::Checked ) || \
	         ( (*it)->text(0) == metaPkgList.at(z).at(0) && metaPkgList.at(z).at(5) == "NO" && (*it)->checkState(0) == Qt::PartiallyChecked ) ){
		if ( tmp.isEmpty() ){
			tmp = (*it)->text(0);
		}else{
			tmp = tmp + "," + (*it)->text(0);
		}
	    }
         ++it;
        }

	return tmp;
}

QString mainWin::getDelPkgs()
{
	QString tmp;
        QTreeWidgetItemIterator it(treeMetaPkgs);
        while (*it) {
	  for (int z=0; z < metaPkgList.count(); ++z)
	    // See if any packages status have changed
	    if ( (*it)->text(0) == metaPkgList.at(z).at(0) && metaPkgList.at(z).at(5) == "YES" && (*it)->checkState(0) == Qt::Unchecked ) {
		if ( tmp.isEmpty() ){
			tmp = (*it)->text(0);
		}else{
			tmp = tmp + "," + (*it)->text(0);
		}
	    }
         ++it;
        }

	return tmp;
}


// Time to save meta-pkgs
void mainWin::slotApplyMetaChanges() {
    saveMetaPkgs();
}



// The User changed the tree widget checked / unchecked stuff sanity check
void mainWin::slotDeskPkgsChanged(QTreeWidgetItem *aItem, int __unused)
{
	if (!aItem)
  	  return;

        disconnect(treeMetaPkgs, SIGNAL(itemChanged(QTreeWidgetItem *, int)), 0, 0);

	if (aItem->childCount() == 0) {
		if (aItem->checkState(0) == Qt::Checked && aItem->parent() ){
			if ( allChildrenPkgsChecked(aItem->parent()->text(0))){
				aItem->parent()->setCheckState(0, Qt::Checked);	
			}else{
				aItem->parent()->setCheckState(0, Qt::PartiallyChecked);	
			}
		}
		if (aItem->checkState(0) == Qt::Unchecked && aItem->parent() ){
			if ( ! allChildrenPkgsUnchecked(aItem->parent()->text(0)))
				aItem->parent()->setCheckState(0, Qt::PartiallyChecked);	
		}

	} else {
		if (aItem->checkState(0) == Qt::Checked ){
			checkAllChildrenPkgs(aItem->text(0));
		}else{
			uncheckAllChildrenPkgs(aItem->text(0));
		}
	}
	

    connect(treeMetaPkgs, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(slotDeskPkgsChanged(QTreeWidgetItem *, int)));

	if ( haveMetaPkgChanges() )
		pushPkgApply->setEnabled(true);
	else
		pushPkgApply->setEnabled(false);
}

// Check the "parent" app to see if all its children are checked or not
bool mainWin::allChildrenPkgsChecked(QString parent)
{
        QTreeWidgetItemIterator it(treeMetaPkgs);
        while (*it) {
         if ((*it)->text(0) == parent ) {
	   if ( (*it)->childCount() <= 0)
	     return true;

	   for ( int i = 0; i < (*it)->childCount() ; ++i) {
	     if ( ! allChildrenPkgsChecked((*it)->child(i)->text(0)))
	       return false;

             if ((*it)->child(i)->checkState(0) != Qt::Checked ) 
	       return false;
	   }
         }
         ++it;
        }
	return true;
}

// Check the "parent" app to see if all its children are unchecked or not
bool mainWin::allChildrenPkgsUnchecked(QString parent)
{
        QTreeWidgetItemIterator it(treeMetaPkgs);
        while (*it) {
         if ((*it)->text(0) == parent ) {
	   if ( (*it)->childCount() <= 0)
	     return true;

	   for ( int i = 0; i < (*it)->childCount() ; ++i) {
	     if ( ! allChildrenPkgsUnchecked((*it)->child(i)->text(0)))
	       return false;

             if ((*it)->child(i)->checkState(0) != Qt::Unchecked ) 
	       return false;
	   }
         }
         ++it;
        }
	return true;
}

// Check all children of parent
void mainWin::checkAllChildrenPkgs(QString parent)
{
        QTreeWidgetItemIterator it(treeMetaPkgs);
        while (*it) {
         if (! (*it)->parent()) {
           ++it;
	   continue;
	 } 

         // Lets walk the tree see what pops up
	 bool pFound=false;
         QTreeWidgetItem *itP = (*it)->parent();
	 do {
	   pFound=false;
	   if (itP->text(0) == parent) {
	     (*it)->setCheckState(0, Qt::Checked);
	     break;
	   }
	   if ( itP->parent() ) {
	     itP = itP->parent();
             pFound=true;
           }
         } while (pFound);

         ++it;
       }
}

// UnCheck all children of parent
void mainWin::uncheckAllChildrenPkgs(QString parent)
{
        QTreeWidgetItemIterator it(treeMetaPkgs);
        while (*it) {
         if (! (*it)->parent()) {
           ++it;
	   continue;
	 } 

         // Lets walk the tree see what pops up
	 bool pFound=false;
         QTreeWidgetItem *itP = (*it)->parent();
	 do {
	   pFound=false;
	   if (itP->text(0) == parent) {
	     (*it)->setCheckState(0, Qt::Unchecked);
	     break;
	   }
	   if ( itP->parent() ) {
	     itP = itP->parent();
             pFound=true;
           }
         } while (pFound);

         ++it;
       }
}

void mainWin::slotMetaRightClick()
{
        QTreeWidgetItemIterator it(treeMetaPkgs);
        while (*it) {
          for (int z=0; z < metaPkgList.count(); ++z) {
            if ( (*it)->isSelected() && (*it)->text(0) == metaPkgList.at(z).at(0) ) {
	      if (metaPkgList.at(z).at(5) == "CATEGORY")
	        return;
	      popup = new QMenu;
	      popup->setTitle((*it)->text(0));
	      popup->addAction(tr("View Packages"), this, SLOT(slotMetaViewPkgs()));
	      popup->exec( QCursor::pos() );
	    }
	  }
         ++it;
        }
}

void mainWin::slotMetaViewPkgs()
{
	QStringList packageList;
        QTreeWidgetItemIterator it(treeMetaPkgs);
        while (*it) {
          for (int z=0; z < metaPkgList.count(); ++z) {
            if ( (*it)->isSelected() && (*it)->text(0) == metaPkgList.at(z).at(0) ) {
 
		QFile pList(metaPkgList.at(z).at(6));
		if ( ! pList.exists() )
		  return;
		
 	        if ( ! pList.open(QIODevice::ReadOnly | QIODevice::Text))
                  return;

                while ( !pList.atEnd() )
     		  packageList << pList.readLine().simplified();

		pList.close();
	        packageList.sort();
			
		dIB = new dialogInfo();
    		dIB->programInit(tr("Package Listing for:") + " " + (*it)->text(0));
		dIB->setInfoText(packageList.join("\n"));
                dIB->show();
	    }
	  }
         ++it;
        }
}

QString mainWin::getLineFromCommandOutput( QString cmd )
{
        FILE *file = popen(cmd.toLatin1(),"r");
 
        char buffer[100];
 
        QString line = "";
        char firstChar;

        if ((firstChar = fgetc(file)) != -1){
                line += firstChar;
                line += fgets(buffer,100,file);
        }
        pclose(file);
        return line.simplified();
}

void mainWin::slotCloseAdvClicked()
{
   groupInfo->setVisible(false);
}

void mainWin::closeEvent(QCloseEvent *event) {

  if ( pkgCmdList.isEmpty() ) {
    this->close();
    return;   
  }
  if ( pkgCmdList.at(0).at(0).isEmpty() ) {
    this->close();
    return;  
  }

  //Verify that they want to continue
  QMessageBox::StandardButton button = QMessageBox::warning(this, tr("Processes Running"), tr("Packages are currently being changed. Are you sure you want to quit?"), QMessageBox::Yes | QMessageBox::Cancel,QMessageBox::Cancel);
  if(button == QMessageBox::Yes) {
    this->close();
  } else {
    event->ignore();
    return;
  }
}


void mainWin::slotReadEventPipe()
{
   QString line, tmp, file, dl, tot;
   bool ok, ok2;

   while (eP->canReadLine()) {
     line = eP->readLine().simplified();
     //qDebug() << line;

     // KPM!!
     // TODO 12-12-2013
     // No JSON in Qt4, once we move to Qt5, replace this hack
     // with the new JSON parser

     // Look for any "msg" lines
     if ( line.indexOf("\"msg") != -1 ) {
          line.remove(0, line.indexOf("\"msg") + 8);
          line.truncate(line.lastIndexOf("\""));
	  qDebug() << line;
	  textStatus->setText(line);
	  continue;
     }

     // Look for a download status update
     if ( line.indexOf("\"INFO_FETCH") != -1 && line.indexOf("\"url\"") != -1 ) {
          line.remove(0, line.indexOf("\"url") + 8);
          line.truncate(line.lastIndexOf("}"));

          // Get the file basename
          file = line;
          file.truncate(line.indexOf("\""));
          QFileInfo tFile;
          tFile.setFile(file);
          file = tFile.baseName();

          // Get the download / total
          dl = line.section(":", 2, 2).section(",", 0, 0);
          tot = line.section(":", 3, 3).section("}", 0, 0);
          dl = dl.simplified();
          tot = tot.simplified();

          dl.toLongLong(&ok);
          tot.toLongLong(&ok2);
          if ( ok && ok2) {
            progressUpdate->setRange(0, tot.toLongLong(&ok) / 1024);
            progressUpdate->setValue(dl.toLongLong(&ok) / 1024 );
          }

          // Set the status update
	  textStatus->setText(tr("Downloading") + " " + file + " (" + dl + " / " + tot + ")" );
     }

   } // End of while canReadLine()

}

void mainWin::slotConfigClicked()
{
   configD = new dialogConfig();
   connect(configD, SIGNAL(ok()),this, SLOT(slotConfigFinished()) );
   configD->programInit();
   configD->show();
}

void mainWin::slotConfigFinished()
{
   // Changed view, lets refresh
   initMetaWidget();
}
