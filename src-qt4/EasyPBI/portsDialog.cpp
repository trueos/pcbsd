#include "portsDialog.h"
#include "ui_portsDialog.h"
#include <QDebug>

portsDialog::portsDialog(QWidget *parent, QString localPortsDir, QString sucmd) :
    QDialog(parent),
    ui(new Ui::portsDialog)
{
	
       //Setup Initial state of GUI objects and connect signal/slots
       ui->setupUi(this);  //load the main UI from designer

	//Setup the local variables
	localPortsTree = localPortsDir;
	systemPortsTree = "/usr/ports";
	suCMD = sucmd;
	localPorts = FALSE;
	systemPorts = FALSE;
	p = new QProcess(this);
	//p->setProcessChannelMode(QProcess::MergedChannels);
	p->setProcessEnvironment( QProcessEnvironment::systemEnvironment() );
	connect(p,SIGNAL(readyReadStandardOutput()),this,SLOT(updateStatusBar()) );
	connect(p,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(processFinished(int,QProcess::ExitStatus)) );
	
	//Now check for the ports trees at those locations
	checkForPorts();
	//Make sure that the progress group is invisible until needed
	ui->groupUpdate->setVisible(FALSE);
	//Setup signal/slots
	connect(ui->push_close,SIGNAL(clicked()), this, SLOT(slotClose()) );
	connect(ui->push_updatelocal,SIGNAL(clicked()), this, SLOT(slotGetLocalPorts()) );
	connect(ui->push_updatesystem,SIGNAL(clicked()), this, SLOT(slotGetSystemPorts()) );
}

portsDialog::~portsDialog(){
}

void portsDialog::checkForPorts(){
  //Check the Local Ports Tree
  QDir ldir(localPortsTree);
  QFileInfoList llist = ldir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs, QDir::Time); //sort by most recently changed
  if(llist.length() > 0){
	localPorts =TRUE;
	ui->push_updatelocal->setText( tr("Update Local Ports") );
	ui->label_localinfo->setText( QString(tr("The local ports tree was last updated on %1")).arg( llist[0].lastModified().toString() ));
  }else{
    //The local ports tree is missing
    localPorts = FALSE;
    ui->push_updatelocal->setText( tr("Fetch Local Ports") );
    ui->label_localinfo->setText( QString(tr("The local ports tree does not exist at %1")).arg(localPortsTree) );
  }
  //Check the System Ports Tree
  if( !suCMD.isEmpty() ){
    QDir dir(systemPortsTree);
    QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs, QDir::Time); //sort by most recently changed
    if(list.length() > 2){ //allow for a couple directories to still be there, and not have the full ports tree
	systemPorts = TRUE; //files and/or directories exist
	ui->push_updatesystem->setText( tr("Update System Ports") );
	ui->label_systeminfo->setText( QString(tr("The system ports tree was last updated on %1")).arg( list[0].lastModified().toString() ));
    }else{
	systemPorts = FALSE;
    }
  }else{
    //Disable the system ports option if no switch-user utility available
    ui->groupSystem->setEnabled(FALSE);
  }
  if(!systemPorts){
    //The system ports tree is missing
    ui->push_updatesystem->setText( tr("Fetch System Ports") );
    ui->label_systeminfo->setText( QString(tr("The system ports tree does not exist at %1")).arg(systemPortsTree) );
  }
}

void portsDialog::slotClose(){
  //Close the UI
  this->close();
}

void portsDialog::slotGetLocalPorts(){
  qDebug() << "Starting local download of FreeBSD Ports tree";
  //Disable the interface buttons
  ui->push_updatesystem->setEnabled(FALSE);
  ui->push_updatelocal->setEnabled(FALSE);
  ui->push_close->setEnabled(FALSE);
  //Now make sure the progress box is visible
  ui->groupUpdate->setVisible(TRUE);
  ui->progressBar->setValue(0);
    //Begin creating the network variables
    QNetworkReply *currentDL;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    //Start the download
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)) );
    currentDL = manager->get(QNetworkRequest(QUrl("ftp://ftp.freebsd.org/pub/FreeBSD/ports/ports/ports.tar.gz")));
    //Update the status
    ui->label_status->setText(tr("Downloading the FreeBSD ports tree..."));
    connect(currentDL, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateProgressBar(qint64,qint64)) );

}

void portsDialog::slotGetSystemPorts(){
  //Disable the interface buttons
  ui->push_updatesystem->setEnabled(FALSE);
  ui->push_updatelocal->setEnabled(FALSE);
  ui->push_close->setEnabled(FALSE);
  //Now make sure the progress box is visible
  ui->groupUpdate->setVisible(TRUE);
  ui->progressBar->setMaximum(0); ui->progressBar->setMinimum(0); //default to a "loading" indicator
  ui->label_status->setText(tr("Requesting Root Access"));
  QString cmd = suCMD + " \""; //use the swutch-user utility
  bool usePortsnap = FALSE;
  if(systemPorts){
    //Update the ports tree	  
    if( QFile::exists("/usr/ports/.svn") ){
      cmd.append("svn update /usr/ports");
    }else{
      cmd.append("portsnap fetch update");
      usePortsnap = TRUE;
    }
  }else{
    //Fresh installation of the ports tree
    cmd.append("portsnap fetch extract");
    usePortsnap = TRUE;
  }
  cmd.append("\""); //close the quotes
  //If using portsnap, make sure the intermediate file is there in the EasyPBI directory
  if( usePortsnap ){
    QString pspath = localPortsTree;
    pspath.chop( pspath.length() - pspath.lastIndexOf("/") ); //remove the ports dir from the path to get the base path
    pspath.append(".portsnapNI.sh");
    if( !QFile::exists(pspath) ){
      QString pscmd ="echo \"#!/bin/sh \n#Non-Interactive portsnap workaround for EasyPBI \ncat /usr/sbin/portsnap | sed 's|! -t 0|-z '1'|g' | /bin/sh -s \\$@\" > "+pspath+"; chmod 755 "+pspath;
      //qDebug() << "system call:" << pscmd;
      system(pscmd.toUtf8());
    }
    cmd.replace("portsnap",pspath);
  }
  //Now run the command
  p->start(cmd);
  qDebug() << "System ports update started: CMD:" << cmd;
}

void portsDialog::updateProgressBar(qint64 bytesReceived, qint64 bytesTotal){
  //Determine the percentage
  int percent = 0;
  if(bytesTotal != 0){
    percent = (bytesReceived*100)/bytesTotal;
  }
  //Update the progress bar
  ui->progressBar->setValue(percent);
  ui->progressBar->update();
}

void portsDialog::downloadFinished(QNetworkReply *reply){
  //Disable the progress bar
  ui->progressBar->setMaximum(0); ui->progressBar->setMinimum(0); //default to a "loading" indicator
  //See if the Download was successful
  qDebug() << "Ports download completion code:" << reply->error();
  if( reply->error() != QNetworkReply::NoError){
    QMessageBox::warning(this, tr("Download Failed"),tr("Downloading the FreeBSD ports tree failed. Please check your internet connection and try again."));
    return;
  }
  //Download Succesful, save it to file
  QFile file(localPortsTree+".tar.gz");
  if( !file.open(QIODevice::WriteOnly)){   //if file could not be opened
    QMessageBox::warning(this, tr("Saving Failed"),tr("Saving the FreeBSD ports tree file failed. Please try again."));
    reply->deleteLater();
    return;
  }
  file.write(reply->readAll());
  file.close();
  //Saving the file successful, delete the downloaded data from memory
  reply->deleteLater();
  QTimer::singleShot(1000, this, SLOT(extractLocalPorts()) ); //wait 1 sec to start process
}

void portsDialog::extractLocalPorts(){
  //Now start the extraction
  QString pFile = localPortsTree.section("/",-1) + ".tar.gz";
  QString pDir = localPortsTree + ".tar.gz";
  pDir.remove("/"+pFile); //cut the file name off the end
  if(pDir.endsWith("/")){ pDir.chop(1); } //remove any extra "/" on the end
  if(localPorts){ //remove the old tree if it exists
    qDebug() << "Removing current local ports tree";
    ui->label_status->setText(tr("Removing the old ports tree from the EasyPBI directory."));
    QString rcmd = "cd "+pDir+"/ports; rm -rf `ls | grep -v distfiles`";
    system(rcmd.toUtf8()); 
  } 
  //extract the new tree and remove the downloaded file
  ui->label_status->setText(tr("Extracting the ports tree into the EasyPBI directory. (This may take a while)"));
  QString cmd1 = "tar xvf "+pFile;
  p->setWorkingDirectory(pDir); //Make sure we start in the proper directory
  p->start(cmd1);
  qDebug() << "Starting ports extraction process: CMD:" << cmd1;
}

void portsDialog::updateStatusBar(){
  QString tmp = p->readAllStandardOutput();
  if(tmp.endsWith("\n")){ tmp.chop(1); }
  tmp = tmp.section("\n",-1);
  tmp = tmp.remove("\n").simplified();
  //qDebug() << "Status Update:" << tmp;
  ui->label_status->clear();
  ui->label_status->setText(tmp);
  //Re-size the dialog to the smallest possible size with this text
  //QTimer::singleShot(1,this,SLOT(resizeDialog()));
}

void portsDialog::resizeDialog(){
  this->resize(0,0); //will automatically get larger to show everything at the current size
}

void portsDialog::processFinished(int exitCode,QProcess::ExitStatus status){
  qDebug() << " - Ports update finished:" << exitCode << status;
  //Enable the interface buttons
  ui->push_updatesystem->setEnabled(TRUE);
  ui->push_updatelocal->setEnabled(TRUE);
  ui->push_close->setEnabled(TRUE);
  //Now make sure the progress box is invisible
  ui->groupUpdate->setVisible(FALSE);	
	
  //If the ports package exists in the EasyPBI directory, remove it
  if(QFile::exists(localPortsTree+".tar.gz") ){ 
    QString cmd = "rm "+localPortsTree+".tar.gz";
    system(cmd.toUtf8());
  }
  //Inform the user of the result
  if(exitCode == 0){
    //Finished successfully
    QMessageBox::information(this, tr("Success"), tr("The FreeBSD ports tree has been successfully updated") );
  }else{
    //Finished with error
    QString msg = p->readAllStandardError();
    QMessageBox::information(this, tr("Failure"), QString(tr("The FreeBSD ports tree has failed to update.")+"\n"+tr("Last Message: %1") ).arg(msg) );
  }
  checkForPorts(); //Update the interface times and such
}
