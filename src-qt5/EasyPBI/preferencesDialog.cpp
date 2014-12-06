#include "preferencesDialog.h"
#include "ui_preferencesDialog.h"
#include <QDebug>

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
	
       //Setup Initial state of GUI objects and connect signal/slots
       ui->setupUi(this);  //load the main UI from designer
	//Setup the dialog icons
	//ui->push_pbioutdir->setIcon(Backend::icon("load"));
	//ui->push_sigfile->setIcon(Backend::icon("file"));
	ui->push_moduledir->setIcon(Backend::icon("load"));
	ui->push_icondir->setIcon(Backend::icon("load"));
	ui->push_defaulticon->setIcon(Backend::icon("file"));
	ui->push_suutility->setIcon(Backend::icon("file"));
	//ui->push_makeport->setIcon(Backend::icon("file"));
	//ui->push_pbicreate->setIcon(Backend::icon("file"));
	ui->push_resetutils->setIcon(Backend::icon("refresh"));
	
	//Setup the internal settings structure
	settings = new Config();
	settings->scanForExternalUtilities();	
	settings->loadSettingsFile();
	
	//Put the current settings into the GUI
	resetGUI();
}

PreferencesDialog::~PreferencesDialog(){
}

//Setup the UI
void PreferencesDialog::resetGUI(){
  //Put the current settings into all the dialog widgets
	//pbi tab
	//ui->line_pbioutdir->setText(settings->value("pbidir"));
	//ui->check_signpbi->setChecked(settings->check("usesignature"));
	//ui->line_sigfile->setText(settings->value("sigfile"));
	//paths tab
	ui->line_moduledir->setText(settings->value("moduledir"));
	ui->line_icondir->setText(settings->value("icondir"));
	ui->line_defaulticon->setText(settings->value("defaulticon"));
	ui->line_suutility->setText(settings->value("su_cmd"));
	//ui->line_pbicreate->setText(settings->value("pbi_create"));
	//ui->line_makeport->setText(settings->value("pbi_makeport"));
	updateGUI();
}

void PreferencesDialog::updateGUI(){
	//Now adjust the visibility of the necessary elements
	//pbi tab
	//if( ui->check_signpbi->isChecked() ){ ui->groupBox_sig->setVisible(TRUE); }
	//else{ ui->groupBox_sig->setVisible(FALSE); }
}

//Save/Cancel buttons
void PreferencesDialog::on_buttonBox_accepted(){
    //Save the settings in the GUI to the stucture
    //settings->setValue("pbidir", ui->line_pbioutdir->text() );
    //settings->setCheck("usesignature", ui->check_signpbi->isChecked() );
    //settings->setValue("sigfile", ui->line_sigfile->text() );
    settings->setValue("moduledir", ui->line_moduledir->text() );
    settings->setValue("icondir", ui->line_icondir->text() );
    settings->setValue("defaulticon", ui->line_defaulticon->text() );
    settings->setValue("su_cmd", ui->line_suutility->text() );
    //settings->setValue("pbi_create", ui->line_pbicreate->text() );
    //settings->setValue("pbi_makeport", ui->line_makeport->text() );
    //Now save the settings to disk
    settings->saveSettingsFile();
    //Now close the dialog
    this->close();
}

void PreferencesDialog::on_buttonBox_rejected(){
    //Don't save any of the settings and just close the dialog
    this->close();
}

// ===== PBI Settings Tab =====
/*void PreferencesDialog::on_push_pbioutdir_clicked(){
  //Get the desired directory
  QString seldir = QFileDialog::getExistingDirectory(this, tr("Select PBI Output Directory"), QDir::homePath() );
  if(seldir.isEmpty()){ return; } //action cancelled
  //Save the selection to the GUI
  ui->line_pbioutdir->setText(seldir); 
}*/

/*void PreferencesDialog::on_push_sigfile_clicked(){
  //Get the desired directory
  QString seldir = QFileDialog::getOpenFileName(this, tr("Select Digital Signature File"), QDir::homePath(), tr("Signature Files (*)") );
  if(seldir.isEmpty()){ return; } //action cancelled
  //Save the selection to the GUI
  ui->line_sigfile->setText(seldir); 
}*/

/*void PreferencesDialog::on_check_signpbi_stateChanged(int state){
  if(state == Qt::Checked){ ui->groupBox_sig->setVisible(TRUE); }
  else{ ui->groupBox_sig->setVisible(FALSE); }
}*/

// ===== Paths Tab =====  
void PreferencesDialog::on_push_moduledir_clicked(){
  //Get the desired directory
  QString seldir = QFileDialog::getExistingDirectory(this, tr("Select Default Module Directory"), QDir::homePath() );
  if(seldir.isEmpty()){ return; } //action cancelled
  //Save the selection to the GUI
  ui->line_moduledir->setText(seldir); 
}

void PreferencesDialog::on_push_icondir_clicked(){
  //Get the desired directory
  QString seldir = QFileDialog::getExistingDirectory(this, tr("Select Default Directory To Find Icon Files"), QDir::homePath() );
  if(seldir.isEmpty()){ return; } //action cancelled
  //Save the selection to the GUI
  ui->line_icondir->setText(seldir); 
}
void PreferencesDialog::on_push_defaulticon_clicked(){
  //Get the desired directory
  QString seldir = QFileDialog::getOpenFileName(this, tr("Select Default Icon FIle"), QDir::homePath(), tr("PNG Images (*.png)") );
  if(seldir.isEmpty()){ return; } //action cancelled
  //Save the selection to the GUI
  ui->line_defaulticon->setText(seldir); 
}

// ===== Utilities Tab =====
void PreferencesDialog::on_push_suutility_clicked(){
  //Get the desired directory
  QString seldir = QFileDialog::getOpenFileName(this, tr("Select Graphical Switch-User Utility"), QDir::homePath() );
  if(seldir.isEmpty()){ return; } //action cancelled
  //Save the selection to the GUI
  ui->line_suutility->setText(seldir); 
}

/*void PreferencesDialog::on_push_pbicreate_clicked(){
  //Get the desired directory
  QString seldir = QFileDialog::getOpenFileName(this, tr("Select 'pbi_create' File Location"), QDir::homePath(), "(pbi_create)" );
  if(seldir.isEmpty()){ return; } //action cancelled
  //Save the selection to the GUI
  ui->line_pbicreate->setText(seldir); 
}

void PreferencesDialog::on_push_makeport_clicked(){
  //Get the desired directory
  QString seldir = QFileDialog::getOpenFileName(this, tr("Select 'pbi_makeport' File Location"), QDir::homePath(), "(pbi_makeport)" );
  if(seldir.isEmpty()){ return; } //action cancelled
  //Save the selection to the GUI
  ui->line_makeport->setText(seldir); 
}*/

void PreferencesDialog::on_push_resetutils_clicked(){
  //Verify that this is what the user really wants to do
  QMessageBox verify(this);
  verify.setText(tr("Are you sure you wish to return to the automatically detected utility paths?"));
  verify.setInformativeText(tr("This will remove any customized path settings for the external utilities."));
  verify.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  verify.setDefaultButton(QMessageBox::Yes);
  int ret = verify.exec();
  if(ret != QMessageBox::Yes){return;}
  //Rescan for external utilities
  settings->scanForExternalUtilities();
  //Reset the values in the structure (will revert to detected values if any)
  settings->setValue("pbi_makeport","");
  //settings->setValue("pbi_create","");
  //settings->setValue("su_cmd","");
  //Reset the utilities on the GUI	
  ui->line_suutility->setText(settings->value("su_cmd"));
  //ui->line_pbicreate->setText(settings->value("pbi_create"));
  //ui->line_makeport->setText(settings->value("pbi_makeport"));
}
