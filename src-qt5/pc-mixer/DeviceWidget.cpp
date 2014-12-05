#include "DeviceWidget.h"
#include "ui_DeviceWidget.h"

DeviceWidget::DeviceWidget(QWidget *parent) : QWidget(parent), ui(new Ui::DeviceWidget){
  ui->setupUi(this); //Load the designer file
  //Initialize the rest of the items
  islinked = true;
  ismuted = false;
  changing = false; //if true, slots will be de-activated
  CRV = 0; CLV = 0;
  //connect the signals/slots
  connect(ui->slider_L, SIGNAL(valueChanged(int)), this, SLOT(LSliderChanged(int)) );
  connect(ui->slider_R, SIGNAL(valueChanged(int)), this, SLOT(RSliderChanged(int)) );
  connect(ui->push_mute, SIGNAL(clicked()), this, SLOT(muteClicked()) );
  connect(ui->tool_chain, SIGNAL(clicked()), this, SLOT(linkClicked()) );
}

DeviceWidget::~DeviceWidget(){
	
}

//===============
//            PUBLIC
//===============
void DeviceWidget::setupDevice(QString device, int Lvol, int Rvol){
  changing = true; //going to change the sliders
  ui->label_device->setText(device);
  ui->slider_L->setValue(Lvol);
  ui->slider_R->setValue(Rvol);
  islinked = (Lvol == Rvol);
  ismuted = (islinked && (Lvol == 0) );
  changing = false; //done making changes
  updateButtons();
}
	
QString DeviceWidget::device(){
  return ui->label_device->text();	
}

int DeviceWidget::LVolume(){
  return ui->slider_L->value();
}

int DeviceWidget::RVolume(){
  return ui->slider_R->value();
}

//===============
//            PRIVATE
//===============
void DeviceWidget::updateButtons(){
  //Update the button icons/text as necessary
  if(islinked){
    ui->tool_chain->setIcon(QIcon(":icons/link.png"));
    ui->tool_chain->setAutoRaise(false);
  }else{
    ui->tool_chain->setIcon(QIcon(":icons/link_break.png"));
    ui->tool_chain->setAutoRaise(true);
  }
  if(ismuted){
    ui->push_mute->setIcon( QIcon(":icons/audio-volume-high.png") );
    ui->push_mute->setText( tr("Unmute") );
  }else{
    ui->push_mute->setIcon( QIcon(":icons/audio-volume-muted.png") );
    ui->push_mute->setText( tr("Mute") );	  
  }
}

void DeviceWidget::updateVolumes(int Lvol, int Rvol){
  changing=true; //going to change the sliders
  //Update the sliders
  if(ui->slider_L->value() != Lvol){ ui->slider_L->setValue(Lvol); }
  if(ui->slider_R->value() != Rvol){ ui->slider_R->setValue(Rvol); }
  changing = false; //done making changes to sliders
  ismuted = (Lvol==Rvol) && (Lvol ==0);
  updateButtons(); //make sure to update the buttons now
  //Now adjust the backend mixer to reflect the changes
  Mixer::setValues(ui->label_device->text(), Lvol, Rvol);
  emit deviceChanged( ui->label_device->text() );
}

//===============
//      PRIVATE SLOTS
//===============
void DeviceWidget::muteClicked(){
  if(ismuted){
    if(CLV==0  && CRV==0){
      updateVolumes(50,50);
    }else{
      updateVolumes(CLV, CRV);
    }
  }else{
    CLV = ui->slider_L->value(); CRV = ui->slider_R->value(); //save for later
    updateVolumes(0,0);
  }
}

void DeviceWidget::linkClicked(){
  islinked = !islinked; //flip to the other
  updateButtons();
}

void DeviceWidget::LSliderChanged(int Lvol){
  if(changing){ return; } //no double-taps while class is making adjustments
  if(islinked){
    updateVolumes(Lvol, Lvol);
  }else{
    updateVolumes(Lvol, ui->slider_R->value() );
  }
  
}

void DeviceWidget::RSliderChanged(int Rvol){
  if(changing){ return; } //no double-taps while class is making adjustments
  if(islinked){
    updateVolumes(Rvol, Rvol);
  }else{
    updateVolumes(ui->slider_L->value(), Rvol);
  }  
}

