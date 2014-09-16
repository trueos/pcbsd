#include "configDlg.h"
#include "ui_configDlg.h"

ConfigDlg::ConfigDlg(QWidget *parent) : QDialog(parent), ui(new Ui::ConfigDlg()){
  ui->setupUi(this);
  savedChanges = false;
  LoadConfig();
  ui->push_apply->setEnabled(false); //start out de-activated
  //Connect the signals/slots
  connect(ui->push_cancel, SIGNAL(clicked()), this, SLOT(CancelClicked()) );
  connect(ui->push_apply, SIGNAL(clicked()), this, SLOT(ApplyClicked()) );
  //Check for changes signals
  connect(ui->lineUser, SIGNAL(textChanged(const QString&)), this, SLOT(checkOptions()) );
  connect(ui->linePass, SIGNAL(textChanged(const QString&)), this, SLOT(checkOptions()) );
  connect(ui->linePass2, SIGNAL(textChanged(const QString&)), this, SLOT(checkOptions()) );
  connect(ui->spinPort, SIGNAL(valueChanged(int)), this, SLOT(checkOptions()) );
  connect(ui->groupAppCafe, SIGNAL(clicked(bool)), this, SLOT(checkOptions()) );
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

void ConfigDlg::LoadConfig(){
  cPort = 8885; //default
  cEnable = false; //not enabled by default
  QFile cfg(CFGFILE);
  if(cfg.open(QIODevice::ReadOnly | QIODevice::Text)){
    QTextStream in(&cfg);
    while(!in.atEnd()){
      QString line = in.readLine();
      if(line.startsWith(";")){ continue; }
      if(line.startsWith("port =")){
        cPort = line.section("=",1,1).simplified().toInt();
      }else if(line.startsWith("remote =")){
        cEnable = line.section("=",1,1).simplified()=="true";
      }      
    }
    cfg.close();
  }
  //Now set those values in the UI
  ui->spinPort->setValue(cPort);
  ui->groupAppCafe->setChecked(cEnable);
}

void ConfigDlg::SaveConfig(){
  QStringList cmds;
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
  }
  if(!cmds.isEmpty()){
    cmds.prepend("#/bin/sh");
    cmds.append("service appcafe restart");
    saveFile("/tmp/.appscriptrun.sh", cmds);
    savedChanges = (0 == QProcess::execute("pc-su /bin/sh /tmp/.appscriptrun.sh") );
    if(QFile::exists("/tmp/appcafe.conf")){ QFile::remove("/tmp/appcafe.conf"); }
    QFile::remove("/tmp/.appscriptrun.sh");
  }
}

void ConfigDlg::checkOptions(){
  bool change = false;
  if(ui->groupAppCafe->isChecked()){
    if(cEnable){
      //Currently Enabled (can leave current username/password alone)
      if(ui->spinPort->value()!=cPort){ change = true; }
      else if( !ui->linePass->text().isEmpty() && !ui->lineUser->text().isEmpty() && (ui->linePass->text()==ui->linePass2->text()) ){ change = true; }
    }else{
      //New Enable (make sure all the options are filled out)
      if( !ui->linePass->text().isEmpty() && !ui->lineUser->text().isEmpty() && (ui->linePass->text()==ui->linePass2->text()) ){ change = true; }
    }
  }else if(cEnable){ change = true; }
  //Now activate/de-activate the apply button
  ui->push_apply->setEnabled(change);
}

void ConfigDlg::ApplyClicked(){
  SaveConfig();
  this->close();
}

void ConfigDlg::CancelClicked(){
  savedChanges = false;
  this->close();	
}
