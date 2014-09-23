#include "SConfigDialog.h"
#include "ui_SConfigDialog.h"

SConfigDialog::SConfigDialog(QWidget *parent, ServiceOption *opt, bool newopt) : QDialog(parent), ui(new Ui::SConfigDialog()){
  ui->setupUi(this);
  cancelled = true; //in case the user just closes the window
  //Figure out the inputs/defaults
  if(opt!=0){
    if(newopt){
      //only copy a couple values from the given option
      optOut = new ServiceOption();
	optOut->cfgfile = opt->cfgfile;
	optOut->delim = opt->delim;
	optOut->quotes = opt->quotes;
    }else{
      //Use the given option completely (editing)
      optOut = opt;
    }
  }else{
    //new option (no previous one given)
    optOut = new ServiceOption();
  }
  //Define the values in the combobox
  ui->combo_type->clear();
  ui->combo_type->addItem(tr("ComboBox (pre-defined option list)"));
  ui->combo_type->addItem(tr("NumberBox (integers)"));
  ui->combo_type->addItem(tr("TextBox (user-supplied text)"));
  //Make sure we always start on the file tab
  ui->tabWidget->setCurrentWidget(ui->tab_file);
  //Now populate the GUI
  loadOption();
  //Now enable/disable the GUI appropriately
  checkGUI();
  //Now connect the signals/slots
  connect(ui->line_file, SIGNAL(textChanged(QString)), this, SLOT(checkGUI()) );
  connect(ui->line_key, SIGNAL(textChanged(QString)), this, SLOT(generateSample()) );
  connect(ui->line_delim, SIGNAL(textChanged(QString)), this, SLOT(generateSample()) );
  connect(ui->line_default, SIGNAL(textChanged(QString)), this, SLOT(generateSample()) );
  connect(ui->radio_noquotes, SIGNAL(toggled(bool)), this, SLOT(generateSample(bool)) );
  connect(ui->radio_doublequotes, SIGNAL(toggled(bool)), this, SLOT(generateSample(bool)) );
  connect(ui->radio_singlequotes, SIGNAL(toggled(bool)), this, SLOT(generateSample(bool)) );
  connect(ui->combo_type, SIGNAL(currentIndexChanged(int)), this, SLOT(checkGUI()) );
  connect(ui->line_name, SIGNAL(textChanged(QString)), this, SLOT(checkGUI()) );
  connect(ui->text_description, SIGNAL(textChanged()), this, SLOT(checkGUI()) );
  connect(ui->push_cancel, SIGNAL(clicked()), this, SLOT(cancelClicked()) );
  connect(ui->push_apply, SIGNAL(clicked()), this, SLOT(applyClicked()) );
}

SConfigDialog::~SConfigDialog(){
	
}

void SConfigDialog::loadOption(){
	
}

void SConfigDialog::saveOption(){
	
}

void SConfigDialog::checkGUI(){
  //Check visiblity of optional components
  int ctype = ui->combo_type->currentIndex();
  ui->group_options->setVisible(ctype==0); //COMBOBOX
  ui->group_numbers->setVisible(ctype==1); //NUMBERBOX
  ui->group_text->setVisible(ctype==2); //[STRING/PASSWORD]BOX
  //Check whether the option is filled out (can be applied)
  bool hasEmpty = ui->line_file->text().isEmpty() || ui->line_key->text().isEmpty() || ui->line_delim->text().isEmpty()  \
		|| ui->line_default->text().isEmpty() || ui->line_name->text().isEmpty() || ui->text_description->toPlainText().isEmpty();
  if(!hasEmpty){
    //Also check the optional components for the current type
    if(ctype==0){ //COMBOBOX
      hasEmpty = (ui->list_opts->count() < 2);
    }
    //Number/text box options always have values in the  UI
  }
  ui->push_apply->setEnabled(!hasEmpty);
}

void SConfigDialog::generateSample(bool go){
  if(!go){ return; } //catch for multiple signals from the radio buttons
  QString key = ui->line_key->text();
  QString delim = ui->line_delim->text();
  QString def = ui->line_default->text();
  //Check for empty/default values
  if(key.isEmpty()){ key = tr("KEY"); }
  if(delim.isEmpty()){ delim = "="; }
  if(def.isEmpty()){ def = tr("VALUE"); }
  //Check for encasing in quotes
  if(ui->radio_doublequotes->isChecked()){ def = "\""+def+"\""; }
  else if(ui->radio_singlequotes->isChecked()){ def = "\'"+def+"\'"; }
  //Show the sample
  ui->label_sample->setText(key+delim+def);
  //Now check the GUI for completion
  checkGUI();
}

void SConfigDialog::cancelClicked(){
  cancelled = true;
  this->close();
}

void SConfigDialog::applyClicked(){
  saveOption();
  cancelled = false;
  this->close();
}

