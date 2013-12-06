#include "newModuleDialog.h"
#include "ui_newModuleDialog.h"
#include <QDir>
#include <QDebug>

NewModuleDialog::NewModuleDialog(QWidget *parent, QString portsDir) :
    QDialog(parent),
    ui(new Ui::NewModuleDialog)
{
	
       //Setup Initial state of GUI objects and connect signal/slots
       ui->setupUi(this);  //load the main UI from designer
       isAccepted = FALSE;
       PORTS_DIR = portsDir;
	//Setup the dialog icons
	ui->push_port->setIcon(Backend::icon("load"));
	ui->push_sources->setIcon(Backend::icon("load"));
	ui->push_icon->setIcon(Backend::icon("file"));
	//Connect up the type changed slot
       connect(ui->radio_port, SIGNAL(clicked()), this, SLOT(slotTypeChanged()) );
       connect(ui->radio_local, SIGNAL(clicked()), this, SLOT(slotTypeChanged()) );
	//now run the slot to properly setup the UI elements
	slotTypeChanged();
}

NewModuleDialog::~NewModuleDialog(){
}


void NewModuleDialog::on_buttonBox_accepted(){
  isAccepted=TRUE;
  //Now set the public variables
  if(ui->radio_port->isChecked()){
    moduleType = "port";
    moduleData = ui->line_port->text();
  }else if(ui->radio_local->isChecked() ){
    moduleType="local";
    moduleData = ui->line_sources->text().replace("~",QDir::homePath());
  }else{
    moduleType.clear(); moduleData.clear();
  }
  moduleIcon = ui->line_icon->text().replace("~",QDir::homePath());
  if(moduleIcon.isEmpty()){ moduleIcon = ui->line_icon->placeholderText().replace("~",QDir::homePath()); }
  //Now close the UI
  this->close();
}

void NewModuleDialog::on_buttonBox_rejected(){
  isAccepted=FALSE;
  moduleType.clear();
  moduleData.clear();
  moduleIcon.clear();
  this->close();
}

void NewModuleDialog::slotTypeChanged(){
  if(ui->radio_port->isChecked()){
    ui->label_port->setVisible(TRUE); ui->line_port->setVisible(TRUE); ui->push_port->setVisible(TRUE);
    ui->label_sources->setVisible(FALSE); ui->line_sources->setVisible(FALSE); ui->push_sources->setVisible(FALSE);
  }else if(ui->radio_local->isChecked()){
    ui->label_port->setVisible(FALSE); ui->line_port->setVisible(FALSE); ui->push_port->setVisible(FALSE);
    ui->label_sources->setVisible(TRUE); ui->line_sources->setVisible(TRUE); ui->push_sources->setVisible(TRUE);
  }else{
    ui->label_port->setVisible(FALSE); ui->line_port->setVisible(FALSE); ui->push_port->setVisible(FALSE);
    ui->label_sources->setVisible(FALSE); ui->line_sources->setVisible(FALSE); ui->push_sources->setVisible(FALSE);
  }
  if(PORTS_DIR.isEmpty()){ ui->push_port->setEnabled(FALSE); }
}

void NewModuleDialog::on_push_port_clicked(){
  if( PORTS_DIR.isEmpty() ){
    //No ports tree available
    QMessageBox::warning(this,tr("EasyPBI: No FreeBSD Ports"), tr("The FreeBSD Ports tree could not be found on your system. You may fetch the ports tree through the EasyPBI menu or manually set the path to the port tree in the EasyPBI preferences if it is installed in a non-standard location."));
    return;
  }
  //Prompt for a new port
  QString portSel = QFileDialog::getExistingDirectory(this, tr("Select Port"), PORTS_DIR);
  if(portSel.isEmpty()){return;} //action cancelled or closed	
  else{
    ui->line_port->setText(portSel.remove(PORTS_DIR+"/"));
  }
}

void NewModuleDialog::on_push_sources_clicked(){
  //Prompt for a new selection
  QString Sel = QFileDialog::getExistingDirectory(this, tr("Select Package Directory"), QDir::homePath());
  if(Sel.isEmpty()){return;} //action cancelled or closed	
  else{
    ui->line_sources->setText(Sel.replace(QDir::homePath(),"~"));
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
