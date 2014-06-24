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
  isRM = isCP = isRESTORE = isMV = stopped = overwrite = noerrors = false;
}

FODialog::~FODialog(){
  stopped = true; //just in case it might still be running when closed
}

void FODialog::setOverwrite(bool ovw){
  overwrite = ovw;
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
  isCP=true;
  QTimer::singleShot(10,this, SLOT(slotStartOperations()));
}

void FODialog::RestoreFiles(QStringList oldPaths, QStringList newPaths){
  //user/group rw permissions
  if(oldPaths.length() == newPaths.length()){
    ofiles = oldPaths; nfiles = newPaths;
  }
  isRESTORE = true;
  QTimer::singleShot(10,this, SLOT(slotStartOperations()));
}

void FODialog::MoveFiles(QStringList oldPaths, QStringList newPaths){
  //no change in permissions
  if(oldPaths.length() == newPaths.length()){
    ofiles = oldPaths; nfiles = newPaths;
  }
  isMV=true;
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
    for(int i=0; i<subdirs.length(); i++){ out << subfiles(dir.absoluteFilePath(subdirs[i])); }
  }
  out << dirpath; //always put the parent directory after all the contents
  return out;
}

QString FODialog::newFileName(QString path){
  int num=1;
  QString extension = path.section(".",-1);
  if(!extension.isEmpty()){ 
    extension.prepend("."); 
    path.chop(extension.length());
  }
  while( QFile::exists(path+"-"+QString::number(num)+extension) ){ num++; }
  return QString(path+"-"+QString::number(num)+extension);
}

QStringList FODialog::removeItem(QString path){
  QStringList items = subfiles(path);
  QStringList err;	
  for(int i=0; i<items.length(); i++){
    if(items[i]==path){
      QDir dir;
      if( !dir.rmdir(items[i]) ){ err << items[i]; }	    
    }else if(QFileInfo(items[i]).isDir()){
      err << removeItem(items[i]);
    }else{
      if( !QFile::remove(items[i]) ){ err << items[i]; }
    }
  }
  return err;
}

QStringList FODialog::copyItem(QString oldpath, QString newpath){
  QStringList err;
  if(QFileInfo(oldpath).isDir()){
    QDir dir;
    if( !dir.mkpath(newpath) ){ err << oldpath; }
    else{
      dir.cd(oldpath);
      QStringList subs = dir.entryList(QDir::Files | QDir::Files | QDir::NoDotAndDotDot, QDir::Name | QDir::DirsFirst);
      for(int i=0; i<subs.length(); i++){ err << copyItem(oldpath+"/"+subs[i], newpath+"/"+subs[i]); }
    }
  }else{
    if( !QFile::copy(oldpath, newpath) ){ err << oldpath; }
    else{
      if(isCP){
	QFile::setPermissions(newpath, QFile::permissions(oldpath));
      }else if(isRESTORE){
	QFile::setPermissions(newpath, QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup | QFile::WriteGroup);
      }
    }
  }
  return err;
}

// ==== PRIVATE SLOTS ====
void FODialog::slotStartOperations(){
  ui->label->setText(tr("Calculating..."));
  //Now setup the UI
  ui->progressBar->setRange(0,ofiles.length());
  ui->progressBar->setValue(0);
  ui->progressBar->setVisible(true);
  //Now start iterating over the operations
  QStringList errlist;
  for(int i=0; i<ofiles.length() && !stopped; i++){
    if(isRM){
      ui->label->setText( QString(tr("Removing: %1")).arg(ofiles[i].section("/",-1)) );
      errlist << removeItem(ofiles[i]);
    }else if(isCP || isRESTORE){
      ui->label->setText( QString(tr("Copying: %1 to %2")).arg(ofiles[i].section("/",-1), nfiles[i].section("/",-1)) );
      if(QFile::exists(nfiles[i])){
	if(overwrite){
	  errlist << removeItem(nfiles[i]);
	}else{
	  nfiles[i] = newFileName(nfiles[i]);
	}
      }
      errlist << copyItem(ofiles[i], nfiles[i]);
    }else if(isMV){
      ui->label->setText( QString(tr("Moving: %1 to %2")).arg(ofiles[i].section("/",-1), nfiles[i].section("/",-1)) );
      if( !QFile::rename(ofiles[i], nfiles[i]) ){
        errlist << ofiles[i];
      }	
    }
    ui->progressBar->setValue(i+1);
  }
  //All finished, so close the dialog if successful
  errlist.removeAll(""); //make sure to clear any empty items
  if(!errlist.isEmpty()){
    QString msg;
    if(isRM){ msg = tr("Could not remove these files:"); }
    else if(isCP){ msg = tr("Could not copy these files:"); }
    else if(isRESTORE){ msg = tr("Could not restore these files:"); }
    else if(isMV){ msg = tr("Could not move these files:"); }
    QMessageBox::warning(this, tr("File Errors"), msg+"\n\n"+errlist.join("\n"));
  }
  noerrors = errlist.isEmpty();
  this->close();
}

void FODialog::on_push_stop_clicked(){
  stopped = true;
}