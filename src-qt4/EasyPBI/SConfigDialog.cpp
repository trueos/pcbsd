#include "SConfigDialog.h"
#include "ui_SConfigDialog.h"

SConfigDialog::SConfigDialog(QWidget *parent, ServiceOption *opt, bool newopt) : QDialog(parent), ui(new Ui::SConfigDialog()){
  ui->setupUi(this);
  cancelled = true; //in case the user just closes the window
  //Figure out the inputs/defaults
  if(opt!=0){
    if(newopt){
      //only copy a couple values from the given option
      optOut = ServiceOption();
	optOut.cfgfile = opt->cfgfile;
	optOut.delim = opt->delim;
	optOut.quotes = opt->quotes;
    }else{
      //Use the given option completely (editing)
      optOut = *opt;
    }
  }else{
    //new option (no previous one given)
    optOut =ServiceOption();
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
  generateSample(); //this will also run checkGUI();
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
  //File format page
  ui->line_file->setText( optOut.cfgfile );
  ui->line_key->setText( optOut.key );
  ui->line_delim->setText( optOut.delim );
  ui->line_default->setText( optOut.defaultv );
  if(optOut.quotes=="single"){ ui->radio_singlequotes->setChecked(true); }
  if(optOut.quotes=="double"){ ui->radio_doublequotes->setChecked(true); }
  else{ ui->radio_noquotes->setChecked(true); }
  //Appearance page
  if(optOut.type=="PASSWORDBOX"){ ui->combo_type->setCurrentIndex(2); ui->check_hidetext->setChecked(true); }
  else if(optOut.type=="STRINGBOX"){ ui->combo_type->setCurrentIndex(2); ui->check_hidetext->setChecked(false); }
  else if(optOut.type=="NUMBERBOX"){ ui->combo_type->setCurrentIndex(1); }
  else{ ui->combo_type->setCurrentIndex(0); } //COMBOBOX
  ui->line_name->setText( optOut.desc );
  ui->text_description->setPlainText( optOut.longdesc.replace("<br>","\n") );
  ui->spin_max->setValue( optOut.max.toInt() );
  ui->spin_min->setValue( optOut.min.toInt() );
  ui->spin_maxlen->setValue( optOut.maxlen.toInt() );
  ui->list_opts->clear();
  for(int i=0; i<optOut.options.length(); i++){
    QListWidgetItem *it = new QListWidgetItem( QString(tr("%1 (saved as: %2)")).arg(optOut.options[i].section("::::",1,50), optOut.options[i].section("::::",0,0)) );
      it->setWhatsThis(optOut.options[i]);
    ui->list_opts->addItem(it);	  
  }
}

void SConfigDialog::saveOption(){
  //File format page
  optOut.cfgfile = ui->line_file->text();
  optOut.key = ui->line_key->text();
  optOut.delim = ui->line_delim->text();
  optOut.defaultv = ui->line_default->text();
  if(ui->radio_singlequotes->isChecked()){ optOut.quotes = "single"; }
  else if(ui->radio_doublequotes->isChecked()){ optOut.quotes = "double"; }
  else{ optOut.quotes.clear(); }
  //Appearance page
  switch (ui->combo_type->currentIndex()){
    case 0:
	optOut.type = "COMBOBOX";
        break;
    case 1:
	optOut.type = "NUMBERBOX";
        break;
    case 2:
	optOut.type = ui->check_hidetext->isChecked() ? "PASSWORDBOX": "STRINGBOX";
        break;
    default:
        optOut.type = "COMBOBOX";
  }
  optOut.desc = ui->line_name->text();
  optOut.longdesc = ui->text_description->toPlainText().replace("\n","<br>");
  optOut.max = ui->spin_max->cleanText();
  optOut.min = ui->spin_min->cleanText();
  optOut.maxlen = ui->spin_maxlen->cleanText();
  optOut.options.clear();
  for(int i=0; i<ui->list_opts->count(); i++){
    optOut.options << ui->list_opts->item(i)->whatsThis();
  }
  
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

void SConfigDialog::on_tool_optadd_clicked(){
  //Get the info from the user
  bool ok = false;
  QString val = QInputDialog::getText(this, tr("New Value"), tr("Invisible File Value:"), QLineEdit::Normal, "", &ok);
  if(!ok || val.isEmpty()){ return; }
  QString name = QInputDialog::getText(this, tr("New Text"), tr("Visible Option Name:"), QLineEdit::Normal, val, &ok);
  if(!ok || name.isEmpty()){ return; }
  //Add it to the list
  QListWidgetItem *it = new QListWidgetItem( QString(tr("%1 (saved as: %2)")).arg(name, val) );
    it->setWhatsThis(val+"::::"+name); //format for saving to the backend
  ui->list_opts->addItem(it);
  ui->list_opts->setCurrentItem(it);
  checkGUI();	
}

void SConfigDialog::on_tool_optrem_clicked(){
  if(ui->list_opts->currentRow() < 0){ return; }
  delete ui->list_opts->takeItem( ui->list_opts->currentRow() );
  checkGUI();
}

void SConfigDialog::on_tool_optup_clicked(){
  int row = ui->list_opts->currentRow();
  if(row < 1){ return; }
  ui->list_opts->insertItem( row-1, ui->list_opts->takeItem(row) );
  ui->list_opts->setCurrentRow(row-1);
}

void SConfigDialog::on_tool_optdown_clicked(){
  int row = ui->list_opts->currentRow();
  if(row >= (ui->list_opts->count()-1) ){ return; }
  ui->list_opts->insertItem( row+1, ui->list_opts->takeItem(row) );
  ui->list_opts->setCurrentRow(row+1);	
}
