#include "aboutDialog.h"
#include "ui_aboutDialog.h"
#include <QDebug>

aboutDialog::aboutDialog(QWidget *parent, QString version) :
    QDialog(parent),
    ui(new Ui::aboutDialog)
{
	
       //Setup Initial state of GUI objects and connect signal/slots
       ui->setupUi(this);  //load the main UI from designer


	//Set the dialog summary information
	QString summary;
	summary = QString(tr("EasyPBI version %1")).arg(version);
	summary.append("\n"+tr("Create PBI's quickly and easily."));
	summary.append("\n"+QString("Wiki Page: http://wiki.pcbsd.org/index.php/EasyPBI2/10.0"));
	summary.append("\n"+QString("Current Developer: Ken Moore <ken@pcbsd.org>"));
	ui->text_summary->setText(summary);
	//Connect up the type changed slot
       connect(ui->radio_history, SIGNAL(clicked()), this, SLOT(slotTypeChanged()) );
       connect(ui->radio_license, SIGNAL(clicked()), this, SLOT(slotTypeChanged()) );
	//now run the slot to properly setup the UI elements
	slotTypeChanged();
}

aboutDialog::~aboutDialog(){
}


void aboutDialog::on_buttonBox_accepted(){
  //Close the UI
  this->close();
}


void aboutDialog::slotTypeChanged(){
  QString filename;
  if(ui->radio_history->isChecked()){
    filename = ":/files/HISTORY.txt";
  }else if(ui->radio_license->isChecked()){
    filename = ":/files/LICENSE";
  }
  //Load the appripriate file contents
  QString contents;
  QFile file(filename);
  if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) ){
    contents = QString(tr("Unable to open resource file: %1")).arg(filename);
    contents.append("\n"+tr("Please check that your EasyPBI compilation was performed correctly") );
  }else{
    QTextStream in(&file);
    contents.append(in.readLine());
    while( !in.atEnd() ){ contents.append("\n" + in.readLine()); }
    file.close();
  }
  //display the contents
  ui->text_info->clear();
  ui->text_info->setText(contents);
}
