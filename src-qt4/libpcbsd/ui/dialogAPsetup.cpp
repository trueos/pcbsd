#include "pcbsd-dialogAPsetup.h"
#include "ui_dialogAPsetup.h"

#include "pcbsd-netif.h"
APSetupDialog::APSetupDialog(QString devicenode, QWidget *parent) : QDialog(parent), ui(new Ui::APSetupDialog){
  ui->setupUi(this);
  wdev = devicenode;
  cancelled = true; //Make sure this is always set by default
  if(wdev.startsWith("/dev/")){ wdev = wdev.section("/",-1); } //just need the node
  ui->label_device->setText( QString(tr("Wireless Device: %1")).arg(wdev) );
  connect(ui->push_cancel, SIGNAL(clicked()), this, SLOT(cancelDialog()) );
  connect(ui->push_apply, SIGNAL(clicked()), this, SLOT(applyDialog()) );
}

APSetupDialog::~APSetupDialog(){
	
}

void APSetupDialog::applyDialog(){
  if(ui->line_name->text().isEmpty()){ return; } //Can't continue without a name
  if(!ui->line_pass->text().isEmpty() && ui->line_pass->text().length() < 8){
    QMessageBox::warning(this, tr("Invalid Password Length"), tr("WPA password must be a minimum of 8 characters long."));
    return;
  }
  
  int ret = NetworkInterface::enableWirelessAccessPoint(wdev, ui->line_name->text(), ui->line_pass->text(), true, \
	ui->line_ip->text(), ui->combo_mode->currentText(), ui->spin_channel->value(), \
	ui->line_netmask->text(), ui->line_country->text() );
  QString msg;
  if(ret < 0){
    msg = tr("Wireless driver does not support hosting a wireless access point.");
  }else if(ret > 0){
    msg = tr("Could not create access point (is wlan0 already active?)");
  }
  if(!msg.isEmpty()){
    QMessageBox::warning(this, tr("Failure"), msg);
  }
  cancelled=false;
  this->close();	
}