#include "LPMessages.h"
#include "ui_LPMessages.h"

LPMessages::LPMessages(QWidget *parent) : QDialog(parent), ui(new Ui::LPMessages){
  ui->setupUi(this);
  logsAvailable=false;
  connect(ui->push_close, SIGNAL(clicked()), this, SLOT(closeDialog()) );
  connect(ui->combo_logs, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChangeLog()) );
}

LPMessages::~LPMessages(){
  //no special cleanup
}

// ------------------------------
// PUBLIC FUNCTIONS
// ------------------------------
void LPMessages::setMessages(QString msg, QString err){
  //standard messages
  if(msg.isEmpty()){ ui->label_messages->setText(tr("No recent messages")); }
  else{ ui->label_messages->setText(msg); }
  //error messages
  ui->label_errors->setText(err);
  if(msg.isEmpty() && !err.isEmpty()){
    ui->label_messages->setVisible(false);
  }else{
    ui->label_messages->setVisible(true);
  }
  ui->label_errors->setVisible( !err.isEmpty() );
  //Make sure that the messages tab is visible
  ui->tabWidget->setCurrentWidget(ui->tab_messages);
}

void LPMessages::setLogFiles(QStringList logs){
  ui->combo_logs->clear();
  logsAvailable = !logs.isEmpty();
  if(!logsAvailable){ logs << tr("No Log Files Available"); }
  ui->combo_logs->addItems(logs);
  ui->text_log->setVisible(logsAvailable);
  ui->combo_logs->setCurrentIndex(0);
}

// -------------------------------
// PRIVATE FUNCTIONS
// -------------------------------
void LPMessages::displayLogFile(QString logFile){
  QString msg = tr("(Life Preserver) No file contents to display");
  //Read the log file if it exists
  QStringList contents;
  QFile file(logFile);
  if(!file.exists()){ contents << QString(tr("(Life Preserver) File does not exist: %1")).arg(logFile); }
  else{
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){ 
      QTextStream in(&file);
      while(!in.atEnd()){
        contents << QString( in.readLine() );
      }
      file.close();
    }else{
      contents << QString(tr("(Life Preserver) File could not be opened: %1")).arg(logFile);
    }
  }
  //Now put the contents into a single message
  if(!contents.isEmpty()){ msg = contents.join("\n"); }
  //display the contents of the file
  ui->text_log->setText(msg);
}

// -----------------------
// PRIVATE SLOTS
// -----------------------
void LPMessages::slotChangeLog(){
  qDebug() << "Change Visible Log File";
  if(!logsAvailable){ return; }
  //Get the logfile path from the text
  QString logfile = ui->combo_logs->currentText().section("<",1,1,QString::SectionSkipEmpty).section(">",0,0);
  //Now run the function to display the file's contents
  displayLogFile(logfile);
}

void LPMessages::closeDialog(){
  qDebug() << "Closing down the message dialog";
  //no cleanup necessary for this dialog
  this->close();
}

// ------------------
// PROTECTED
// ------------------
void LPMessages::closeEvent(QCloseEvent *event){
  //Make sure this window only gets hidden rather than closed
  // this prevents the entire tray application from closing down as well
  event->ignore();
  this->hide();	
}
