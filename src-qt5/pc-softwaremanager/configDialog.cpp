/***************************************************************************
 *   Copyright (C) 2011 - iXsystems				 	  *
 *   kris@pcbsd.org  *
 *   tim@pcbsd.org   *
 *   ken@pcbsd.org   *
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
#include "configDialog.h"
#include "ui_configDialog.h" //Qt-designer file


ConfigDialog::ConfigDialog(QWidget* parent) : QDialog(parent), ui(new Ui::ConfigDialog){
  ui->setupUi(this); //load the Qt-Designer file
  madeChanges = false;
  settings = new QSettings("PCBSD-root","AppCafe-repos");
  readSavedRepos();
  loadPbiConf();
  customChanged(); //Make sure everything is enabled/disabled appropriately
  //Setup the signals/slots
  connect(ui->push_apply, SIGNAL(clicked()), this, SLOT(applyClicked()) );
  connect(ui->push_cancel, SIGNAL(clicked()), this, SLOT(cancelClicked()) );
  connect(ui->tool_addRepo, SIGNAL(clicked()), this, SLOT(addCustom()) );
  connect(ui->tool_remRepo, SIGNAL(clicked()), this, SLOT(removeCustom()) );
  connect(ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(customChanged()) );
  connect(ui->radio_edge, SIGNAL(toggled(bool)), this, SLOT(checkSettings()) );
  connect(ui->radio_production, SIGNAL(toggled(bool)), this, SLOT(checkSettings()) );
  connect(ui->radio_custom, SIGNAL(toggled(bool)), this, SLOT(checkSettings()) );
}

ConfigDialog::~ConfigDialog(){
  delete ui;	
}

//=============
//    PRIVATE
//=============

//Main slots
void ConfigDialog::loadPbiConf(){ //fill the UI with the current settings
  QStringList contents = Extras::readFile("/usr/local/etc/pcbsd.conf");
  for(int i=0; i<contents.length(); i++){
    if(contents[i].startsWith("#") || contents[i].isEmpty() ){ continue; } //skip comment
    if(contents[i].startsWith("PACKAGE_SET:")){
      QString val = contents[i].section(":",1,50).simplified();
      if(val=="EDGE"){ ui->radio_edge->setChecked(true); }
      else if(val=="PRODUCTION"){ ui->radio_production->setChecked(true); }
      else if(val=="CUSTOM"){ ui->radio_custom->setChecked(true); }
      else{ ui->radio_production->setChecked(true); } //default to PRODUCTION
      
    }else if(contents[i].startsWith("PACKAGE_URL:")){
      QString cURL = contents[i].section(":",1,50).simplified();
      //Now make sure that custom repo is selected
      bool found = false;
      for(int i=0; i<ui->listWidget->count() && !found; i++){
        if( ui->listWidget->item(i)->whatsThis() == cURL ){
	  found = true;
	  ui->listWidget->setCurrentRow(i);
	}
      }
      qDebug() << "Custom URL detected:" << cURL;
      if(!found){
        //Add this repo as UNKNOWN
	QListWidgetItem *item = new QListWidgetItem( cURL.left(30).append("..."), 0);
	  item->setWhatsThis(cURL);
	  item->setToolTip(cURL);
	ui->listWidget->addItem(item);
	ui->listWidget->setCurrentItem(item);
      }
    }
    
  }
}

void ConfigDialog::savePbiConf(){ //save the current settings to file
  //Assemble the file contents
  QStringList contents;
  contents << "# PC-BSD Configuration Defaults";
  contents << "";
  QString pkgset = "PRODUCTION"; //default value (just in case)
  if(ui->radio_edge->isChecked()){ pkgset = "EDGE"; }
  else if(ui->radio_production->isChecked()){ pkgset = "PRODUCTION"; }
  else if(ui->radio_custom->isChecked()){ pkgset = "CUSTOM"; }
  contents << "PACKAGE_SET: "+pkgset;
  if(pkgset.toLower()=="custom"){
    //Also set the custom url
    QString cURL = ui->listWidget->currentItem()->whatsThis();
    contents << "PACKAGE_URL: "+cURL;
  }
  Extras::writeFile("/usr/local/etc/pcbsd.conf", contents);
}

void ConfigDialog::cancelClicked(){ //quit without saving
  madeChanges = false;
  this->close();
}

void ConfigDialog::applyClicked(){ //quit after saving
  savePbiConf(); //save the current settings
  Extras::getCmdOutput("pc-extractoverlay ports");
  madeChanges = true;
  this->close();
}

void ConfigDialog::checkSettings(){
  if(ui->radio_custom->isChecked()){
      ui->push_apply->setEnabled( (ui->listWidget->currentItem() != 0) );
      ui->group_custom->setVisible(true);	  
  }else{
    ui->push_apply->setEnabled(true);
    ui->group_custom->setVisible(false);
  }
}

//Custom Repo Management
void ConfigDialog::readSavedRepos(){
  QStringList keys = settings->allKeys();
  for(int i=0; i<keys.length(); i++){
    QListWidgetItem *item = new QListWidgetItem(keys[i], 0);
	  QString cURL = settings->value(keys[i]).toString();
	  item->setWhatsThis( cURL );
	  item->setToolTip( cURL );
    ui->listWidget->addItem(item);
    if(i==0){ ui->listWidget->setCurrentItem(item); }
  }	  
}

void ConfigDialog::customChanged(){
  if(ui->listWidget->currentItem() == 0){
    ui->tool_remRepo->setEnabled(false);
  }else{
    ui->tool_remRepo->setEnabled(true);
  }
  checkSettings(); //also update the apply button
}

void ConfigDialog::addCustom(){
  //Get the name/URL from the user
  QString cURL = QInputDialog::getText(this, tr("New Repo URL"), tr("URL:") );
  if(cURL.isEmpty()){ return; } //cancelled
  QString key = QInputDialog::getText(this, tr("New Repo Name"), tr("Name:") );
  while( key.isEmpty() || settings->contains(key) ){
    key = QInputDialog::getText(this, tr("Invalid Name: Try Again"), tr("Name:") );
  }
  settings->setValue(key, cURL);
  QListWidgetItem *item = new QListWidgetItem(key, 0);
	item->setWhatsThis(cURL);
  ui->listWidget->addItem(item);
  ui->listWidget->setCurrentItem(item);
  customChanged();
}

void ConfigDialog::removeCustom(){

  if(ui->listWidget->currentRow()<0){ return; }
  QListWidgetItem *item = ui->listWidget->takeItem(ui->listWidget->currentRow());  
  QString key = item->text();
  settings->remove(key);
  //ui->listWidget->removeItemWidget(item);
  customChanged();
}

