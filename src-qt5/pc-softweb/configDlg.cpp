#include "configDlg.h"
#include "ui_configDlg.h"

#include <QUrl>

ConfigDlg::ConfigDlg(QWidget *parent) : QDialog(parent), ui(new Ui::ConfigDlg()){
  ui->setupUi(this);
  savedChanges = repoChanged = remoteChanged = false;
  //Migrate over the old root settings file if it exists
  QString tmp = QDir::homePath()+"/.config/";
  if(!QFile::exists(tmp+"PCBSD/AppCafe-savedrepos.conf") && QFile::exists(tmp+"PCBSD-root/AppCafe-repos.conf") ){
    QFile::copy( tmp+"PCBSD-root/AppCafe-repos.conf", tmp+"PCBSD/AppCafe-savedrepos.conf");
    QFile::setPermissions( tmp+"PCBSD/AppCafe-savedrepos.conf", QFile::ReadOwner | QFile::WriteOwner | QFile::ReadUser | QFile::WriteUser | QFile::ReadOther | QFile::ReadGroup);
  }
  settings = new QSettings("PCBSD","AppCafe-savedrepos");
  qDebug() << "Settings file:" << settings->fileName();
  settings->sync();
  LoadConfig();
  readSavedRepos();
  loadPbiConf();
  customChanged(); //start out de-activated
  //Connect the signals/slots
  connect(ui->push_cancel, SIGNAL(clicked()), this, SLOT(CancelClicked()) );
  connect(ui->push_apply, SIGNAL(clicked()), this, SLOT(ApplyClicked()) );
  //Check for changes signals
  connect(ui->lineUser, SIGNAL(textChanged(const QString&)), this, SLOT(checkOptions()) );
  connect(ui->linePass, SIGNAL(textChanged(const QString&)), this, SLOT(checkOptions()) );
  connect(ui->linePass2, SIGNAL(textChanged(const QString&)), this, SLOT(checkOptions()) );
  connect(ui->spinPort, SIGNAL(valueChanged(int)), this, SLOT(checkOptions()) );
  connect(ui->groupAppCafe, SIGNAL(clicked(bool)), this, SLOT(checkOptions()) );
  connect(ui->tool_addRepo, SIGNAL(clicked()), this, SLOT(addCustom()) );
  connect(ui->tool_remRepo, SIGNAL(clicked()), this, SLOT(removeCustom()) );
  connect(ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(customChanged()) );
  connect(ui->radio_edge, SIGNAL(toggled(bool)), this, SLOT(customChanged()) );
  connect(ui->radio_production, SIGNAL(toggled(bool)), this, SLOT(customChanged()) );
  connect(ui->radio_custom, SIGNAL(toggled(bool)), this, SLOT(customChanged()) );
}

ConfigDlg::~ConfigDlg(){
	
}

void ConfigDlg::saveFile(QString path, QStringList contents){
  QFile tmp(path);
    if( tmp.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Truncate) ){
      QTextStream out(&tmp);
      out << contents.join("\n");
      tmp.close();
    }	
}

QStringList ConfigDlg::readFile( QString filePath ){
   QFile file(filePath);
   QStringList output;
   if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
     QTextStream in(&file);
     while(!in.atEnd()){
       QString line = in.readLine();
       if(!line.isEmpty()){ output << line; }
     }
     file.close();
   }
   return output;
 }
 
 // ===================
 
void ConfigDlg::LoadConfig(){
  cPort = 8885; //default
  cEnable = false; //not enabled by default
  QStringList info = readFile(CFGFILE);
  for(int i=0; i<info.length(); i++){
    if(info[i].startsWith(";")){ continue; }
    if(info[i].startsWith("port =")){
      cPort = info[i].section("=",1,1).simplified().toInt();
    }else if(info[i].startsWith("remote =")){
      cEnable = info[i].section("=",1,1).simplified()=="true";
    }	  
  }
  
  //Now set those values in the UI
  ui->spinPort->setValue(cPort);
  ui->groupAppCafe->setChecked(cEnable);
}

void ConfigDlg::SaveConfig(){
  QStringList cmds;
  if(remoteChanged){
    bool cfgchange = false;
    //Save the config file
    if(ui->groupAppCafe->isChecked()){
      //Check for changes to the conf file
      if( cPort!=ui->spinPort->value() || !cEnable){ cfgchange=true; }
      //Save the username/password
      if( !ui->linePass->text().isEmpty() && !ui->lineUser->text().isEmpty() ){
        cmds << "appcafe-setpass \""+ui->lineUser->text()+"\" \""+ui->linePass->text()+"\"";
      }
    }else{
      //remote disabled
      cfgchange=true;
    }
    //Now rewrite the config file (temporarily)
    if(cfgchange){
      QStringList contents;
      if(!ui->groupAppCafe->isChecked()){
        contents << "remote = false";
        contents << "port = 8885";
        contents << "ssl = false";
      }else{
        contents << "remote = true";
        contents << "port = "+QString::number(ui->spinPort->value());
        contents << "ssl = true"; //always require user/pass if enabled with this utility
        contents << "mode = desktop"; //must be desktop since using this utility
      }
      // Save the contents to a temporary file
      saveFile("/tmp/appcafe.conf", contents);
      cmds << "mv -f /tmp/appcafe.conf "+CFGFILE;
      cmds << "chmod 744 "+CFGFILE;
      cmds << "chown root:wheel "+CFGFILE;
      cmds << "service appcafe restart"; //restart the web service
    }
  }
  if(repoChanged){
    savePbiConf();
    cmds << "mv -f /tmp/pcbsd.conf /usr/local/etc/pcbsd.conf";
    cmds << "chmod 744 /usr/local/etc/pcbsd.conf";
    cmds << "chown root:wheel /usr/local/etc/pcbsd.conf";
    cmds << "syscache startsync"; //resync the syscache info now
  }
  
  if(!cmds.isEmpty()){
    cmds.prepend("#/bin/sh");
    saveFile("/tmp/.appscriptrun.sh", cmds);
    savedChanges = (0 == QProcess::execute("pc-su /bin/sh /tmp/.appscriptrun.sh") );
    if(QFile::exists("/tmp/appcafe.conf")){ QFile::remove("/tmp/appcafe.conf"); }
    if(QFile::exists("/tmp/pcbsd.conf")){ QFile::remove("/tmp/pcbsd.conf"); }
    QFile::remove("/tmp/.appscriptrun.sh");
  }
}

void ConfigDlg::loadPbiConf(){ //fill the UI with the current settings
  QStringList contents = readFile("/usr/local/etc/pcbsd.conf");
  for(int i=0; i<contents.length(); i++){
    if(contents[i].startsWith("#") || contents[i].isEmpty() ){ continue; } //skip comment
    if(contents[i].startsWith("PACKAGE_SET:")){
      QString val = contents[i].section(":",1,50).simplified();
      if(val=="EDGE"){ ui->radio_edge->setChecked(true); cRepo = val;}
      else if(val=="PRODUCTION"){ ui->radio_production->setChecked(true); cRepo = val;}
      else if(val=="CUSTOM"){ ui->radio_custom->setChecked(true); cRepo = val;}
      else{ ui->radio_production->setChecked(true); cRepo = "PRODUCTION"; } //default to PRODUCTION
      
    }else if(contents[i].startsWith("PACKAGE_URL:")){
      QString cURL = contents[i].section(":",1,50).simplified();
      if(cRepo=="CUSTOM"){ cRepo.append("::::"+cURL); }
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

void ConfigDlg::savePbiConf(){ //save the current settings to file
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
  saveFile("/tmp/pcbsd.conf", contents);

}

void ConfigDlg::checkOptions(){
  remoteChanged = repoChanged = false;
  //Check for changes to the Repository config
  if(cRepo=="PRODUCTION" && !ui->radio_production->isChecked()){ repoChanged = true; }
  else if(cRepo=="EDGE" && !ui->radio_edge->isChecked()){ repoChanged = true; }
  else if(cRepo.startsWith("CUSTOM::::") && !ui->radio_custom->isChecked()){ repoChanged = true; }
  else if(cRepo.startsWith("CUSTOM::::")){
    QListWidgetItem *it = ui->listWidget->currentItem();
    if(it!=0 && !cRepo.endsWith("::::"+it->whatsThis()) ){ repoChanged=true; }
  }
  if( ui->radio_custom->isChecked() && (ui->listWidget->currentRow() < 0) ){
    repoChanged = false; //Custom chosen, but no repo selected
  }
  
  //Check for changes to the remote access config
  if(ui->groupAppCafe->isChecked()){
    if(cEnable){
      //Currently Enabled (can leave current username/password alone)
      if(ui->spinPort->value()!=cPort){ remoteChanged = true; }
      else if( !ui->linePass->text().isEmpty() && !ui->lineUser->text().isEmpty() && (ui->linePass->text()==ui->linePass2->text()) ){ remoteChanged = true; }
    }else{
      //New Enable (make sure all the options are filled out)
      if( !ui->linePass->text().isEmpty() && !ui->lineUser->text().isEmpty() && (ui->linePass->text()==ui->linePass2->text()) ){ remoteChanged = true; }
    }
  }else if(cEnable){ remoteChanged = true; }
  //Now activate/de-activate the apply button
  ui->push_apply->setEnabled(remoteChanged || repoChanged);
}

void ConfigDlg::ApplyClicked(){
  SaveConfig();
  this->close();
}

void ConfigDlg::CancelClicked(){
  savedChanges = false;
  this->close();	
}

//Custom Repo Management
void ConfigDlg::readSavedRepos(){
  ui->listWidget->clear();
  QStringList keys = settings->allKeys();
  for(int i=0; i<keys.length(); i++){
    if(keys[i].contains("/")){ continue; } //invalid key
    QListWidgetItem *item = new QListWidgetItem(keys[i], 0);
	  QString cURL = settings->value(keys[i]).toString();
	  item->setWhatsThis( cURL );
	  item->setToolTip( cURL );
    ui->listWidget->addItem(item);
    if(i==0){ ui->listWidget->setCurrentItem(item); }
  }	  
}

void ConfigDlg::customChanged(){
  bool enabled = ui->radio_custom->isChecked();
  ui->group_custom->setEnabled(enabled);
  ui->tool_remRepo->setEnabled(enabled && ui->listWidget->currentRow() >= 0);
  checkOptions(); //also update the apply button
}

void ConfigDlg::addCustom(){
  //Get the name/URL from the user
  QString cURL = QInputDialog::getText(this, tr("New Repo URL"), tr("URL:") );
  if(cURL.simplified().isEmpty()){ return; } //cancelled
  //Make sure this is a valid URL format
  while( !QUrl(cURL).isValid() || QUrl(cURL).isRelative()  ){
    cURL = QInputDialog::getText(this, tr("Invalid Repo URL: Try Again"), tr("URL:"), QLineEdit::Normal, cURL);
    if(cURL.simplified().isEmpty()){ return; }
  }
  //Now get a name for this repo
  QString key = QInputDialog::getText(this, tr("New Repo Name"), tr("Name:") );
  if(key.simplified().isEmpty()){ return; } //cancelled
  while( key.isEmpty() || settings->contains(key) ){
    key = QInputDialog::getText(this, tr("Invalid Name: Try Again"), tr("Name:") );
    if(key.simplified().isEmpty()){ return; } //cancelled
  }
  settings->setValue(key, cURL);
  QListWidgetItem *item = new QListWidgetItem(key, 0);
	item->setWhatsThis(cURL);
  ui->listWidget->addItem(item);
  ui->listWidget->setCurrentItem(item);
  customChanged();
}

void ConfigDlg::removeCustom(){

  if(ui->listWidget->currentRow()<0){ return; }
  QListWidgetItem *item = ui->listWidget->takeItem(ui->listWidget->currentRow());  
  QString key = item->text();
  settings->remove(key);
  customChanged();
}