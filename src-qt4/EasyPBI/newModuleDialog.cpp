#include "newModuleDialog.h"
#include "ui_newModuleDialog.h"
#include <QDir>
#include <QDebug>

NewModuleDialog::NewModuleDialog(QWidget *parent, QString portsdir) :
    QDialog(parent),
    ui(new Ui::NewModuleDialog)
{
	
       //Setup Initial state of GUI objects and connect signal/slots
       ui->setupUi(this);  //load the main UI from designer
       isAccepted = false;
       isPort=false;
       quickMake=false;
	portsDir=portsdir;
	if(!portsDir.isEmpty() && !portsDir.endsWith("/")){ portsDir.append("/"); }
	//Setup the dialog icons
	//ui->push_port->setIcon(Backend::icon("load"));
	ui->push_icon->setIcon(Backend::icon("file"));
	//if(portsDir.isEmpty()){ ui->push_port->setEnabled(false); }
}

NewModuleDialog::~NewModuleDialog(){
}


void NewModuleDialog::on_buttonBox_accepted(){
  if(ui->line_port->text().isEmpty()){ return; }
  isAccepted=TRUE;
  quickMake = ui->check_quick->isChecked();
  //Now set the public variables
  moduleData = ui->line_port->text();
  moduleIcon = ui->line_icon->text().replace("~",QDir::homePath());
  if(moduleIcon.isEmpty()){ moduleIcon = ui->line_icon->placeholderText().replace("~",QDir::homePath()); }
  //Now close the UI
  this->close();
}

void NewModuleDialog::on_buttonBox_rejected(){
  isAccepted=FALSE;
  moduleData.clear();
  moduleIcon.clear();
  this->close();
}

/*void NewModuleDialog::on_push_port_clicked(){
  QString portSel = QFileDialog::getExistingDirectory(this, tr("Select Port"), portsDir);
  if(!portSel.isEmpty()){
    ui->line_port->setText(portSel.remove(portsDir));
    isPort=true;
  }	  
}*/

void NewModuleDialog::on_push_package_clicked(){
  pkgSelect dlg(this);
  dlg.exec();
  if(dlg.selected){
    ui->line_port->setText(dlg.portSelected);
    isPort=false;
  }	
}

void NewModuleDialog::on_push_icon_clicked(){
//Prompt for a new selection
  QString Sel = QFileDialog::getOpenFileName(this, tr("Select PNG Icon"), QDir::homePath(), tr("PNG icon (*.png)") );
  if(Sel.isEmpty()){return;} //action cancelled or closed	
  else{
    ui->line_icon->setText(Sel);
  }
}

void NewModuleDialog::setDefaultIconFile(QString icon){
  ui->line_icon->setPlaceholderText(icon.replace(QDir::homePath(),"~"));
}
