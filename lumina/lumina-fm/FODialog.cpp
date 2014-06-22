//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "FODialog.h"
#include "ui_FODialog.h"

FODialog::FODialog(QWidget *parent) : QDialog(parent), ui(new Ui::FODialog){
  ui->setupUi(this); //load the designer file
  ui->label->setText("");
  ui->progressBar->setVisible(false);
  ui->push_stop->setIcon( LXDG::findIcon("edit-delete","") );
  //Now set the internal defaults
  isRM = isCP = isRESTORE = isMV = stopped = false;
}

FODialog::~FODialog(){
  stopped = true; //just in case it might still be running when closed
}

//Public "start" functions
void FODialog::RemoveFiles(QStringList paths){
  ofiles = paths;
  isRM = true;
  QTimer::singleShot(10,this, SLOT(slotStartOperations()));
}

void FODialog::CopyFiles(QStringList oldPaths, QStringList newPaths){ 	 
  //same permissions as old files
  if(oldPaths.length() == newPaths.length()){
    ofiles = oldPaths; nfiles = newPaths;
  }
  QTimer::singleShot(10,this, SLOT(slotStartOperations()));
}

void FODialog::RestoreFiles(QStringList oldPaths, QStringList newPaths){
  //user/group rw permissions
  if(oldPaths.length() == newPaths.length()){
    ofiles = oldPaths; nfiles = newPaths;
  }
  QTimer::singleShot(10,this, SLOT(slotStartOperations()));
}

void FODialog::MoveFiles(QStringList oldPaths, QStringList newPaths){
  //no change in permissions
  if(oldPaths.length() == newPaths.length()){
    ofiles = oldPaths; nfiles = newPaths;
  }
  QTimer::singleShot(10,this, SLOT(slotStartOperations()));
}

// ==== PRIVATE ====
QStringList FODialog::subfiles(QString dirpath){
  QStringList out;
  if( QFileInfo(dirpath).isDir() ){
    QDir dir(dirpath);
    //First list the files
    QStringList files = dir.entryList(QDir::Files | QDir::NoDotAndDotDot, QDir::NoSort);
    for(int i=0; i<files.length(); i++){ out << dir.absoluteFilePath(files[i]); }
    //Now recursively add any subdirectories and their contents
    QStringList subdirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::NoSort);
    for(int i=0; i<subdirs.length(); i++){ out << subfiles(subdirs[i]); }
  }
  out << dirpath; //always put the parent directory after all the contents
  return out;
}

// ==== PRIVATE SLOTS ====
void FODialog::slotStartOperations(){
  ui->label->setText(tr("Calculating..."));
  if(isRM){
    //Need to remove directory contents before the directory: find them
    nfiles.clear();
    for(int i=0; i<ofiles.length(); i++){
      nfiles << subfiles(ofiles[i]);
    }
    nfiles.removeAll("");
    ofiles = nfiles; //use the new list of files to remove (put them in the order to remove)
  }
  //Now setup the UI
  ui->progressBar->setRange(0,ofiles.length());
  ui->progressBar->setValue(0);
  ui->progressBar->setVisible(true);
  //Now start iterating over the operations
  QStringList errlist;
  for(int i=0; i<ofiles.length() && !stopped; i++){
    if(isRM){
      ui->label->setText( QString(tr("Removing: %1")).arg(ofiles[i].section("/",-1)) );
      if(QFileInfo(ofiles[i]).isDir()){
	QDir dir;
	if( !dir.rmdir(ofiles[i]) ){ errlist << ofiles[i]; }
      }else{
        if( !QFile::remove(ofiles[i]) ){ errlist << ofiles[i]; }
      }
    }else if(isCP){
      ui->label->setText( QString(tr("Copying: %1 to %2")).arg(ofiles[i].section("/",-1), nfiles[i].section("/",-1)) );
      if( !QFile::copy(ofiles[i], nfiles[i]) ){
        errlist << ofiles[i];
      }else{
	// for "copies" use the same permissions as the original file
	QFile::setPermissions(nfiles[i], QFile::permissions(ofiles[i]));
      }
    }else if(isRESTORE){
      ui->label->setText( QString(tr("Copying: %1 to %2")).arg(ofiles[i].section("/",-1), nfiles[i].section("/",-1)) );
      if( !QFile::copy(ofiles[i], nfiles[i]) ){
        errlist << ofiles[i];
      }else{
	// for "restores" set user/group read/write
	QFile::setPermissions(nfiles[i], QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup | QFile::WriteGroup);
      }
    }else if(isMV){
      ui->label->setText( QString(tr("Moving: %1 to %2")).arg(ofiles[i].section("/",-1), nfiles[i].section("/",-1)) );
      if( !QFile::rename(ofiles[i], nfiles[i]) ){
        errlist << ofiles[i];
      }	
    }
    ui->progressBar->setValue(i+1);
  }
  //All finished, so close the dialog if successful
  if(!errlist.isEmpty()){
    QString msg;
    if(isRM){ msg = tr("Could not remove these files:"); }
    else if(isCP){ msg = tr("Could not copy these files:"); }
    else if(isRESTORE){ msg = tr("Could not restore these files:"); }
    else if(isMV){ msg = tr("Could not move these files:"); }
    QMessageBox::warning(this, tr("File Errors"), msg+"\n\n"+errlist.join("\n"));
  }
  this->close();
}

void FODialog::on_push_stop_clicked(){
  stopped = true;
}