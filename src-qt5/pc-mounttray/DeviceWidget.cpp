#include "DeviceWidget.h"
#include "ui_DeviceWidget.h"

#include <QProcess>
#include <QStringList>
#include <QIcon>

DeviceWidget::DeviceWidget(QString devnode) : QWidget(), ui(new Ui::DeviceWidget){
  ui->setupUi(this); //load the designer file
  isMounted = false; //default value
  WAct = new QWidgetAction(this);
    WAct->setDefaultWidget(this);
  ui->label_dev->setWhatsThis(devnode);
}

DeviceWidget::~DeviceWidget(){
	
}

//Simplification functions for returning the (hidden) data in the widget regarding the device
QString DeviceWidget::node(){
  return ui->label_dev->whatsThis();
}

QString DeviceWidget::type(){
  return ui->label_icon->whatsThis();
}

QString DeviceWidget::label(){
  return ui->label_dev->text();	
}

QString DeviceWidget::filesystem(){
  return ui->tool_mount->whatsThis();	
}

QWidgetAction* DeviceWidget::action(){
  return WAct;
}

// == Widget UpdateRoutines
void DeviceWidget::updateDevice(bool ismounted){
  //This is the full device update routine - usually only needs to be run on init (except for CD devices)
  isMounted = ismounted; //save this for later
  quickupdates = false;
  QTimer::singleShot(0,this, SLOT(doUpdate())); //allow the main process to continue on
  
}

void DeviceWidget::QuickUpdate(bool ismounted){
  //This is the quick status update routine (can be run on tray click for instance)
  isMounted = ismounted; //save this for later
  quickupdates = true;
  QTimer::singleShot(0,this, SLOT(doUpdate())); //allow the main process to continue on
}

// == PRIVATE FUNCTIONS ==
void DeviceWidget::doUpdates(){
  bool firstrun = type().isEmpty();
  if(firstrun || !quickupdates){
    QStringList info = pcbsd::Utils::runShellCommand("pc-sysconfig \"devinfo "+node()+"\"").join("").split(", ");
    //Info Output: <filesystem>, <label>, <type>
    //Save this into the internal variables
    ui->label_icon->setWhatsThis(info[2]); //type
    ui->label_dev->setText(info[1]); //label
    ui->tool_mount->setWhatsThis(info[0]); //filesystem
    ui->label_icon->setToolTip("/dev/"+node()+" ("+info[0]+")");
    //Now go through and set all the various icons and such
    QString icon = ":icons/%1.png";
    if(type()=="SATA"){ icon = icon.arg("harddrive"); }
    else if(type()=="USB"){ icon = icon.arg("usb"); }
    else if(type()=="SD"){ icon = icon.arg("sdcard"); }
    else if(type()=="CD-AUDIO"){ icon = icon.arg("musiccd"); }
    else if(type()=="CD-VIDEO"){ icon = icon.arg("cd-video"); }
    else if(type().startsWith("CD"){ icon = icon.arg("cd-generic"); }
    else if(type()=="ISO"){ icon = icon.arg("dvd"); }
    else{ icon = icon.arg("CDdevices"); }
    if(filesystem=="NONE"){
      //Add the question-mark overlay to the icon, signifying that it is an unknown filesystem
      QPixmap tmp(icon);
      QPixmap overlay(":icons/question-overlay.png");
      QPainter paint(&tmp);
	    paint.drawPixmap(ui->label_icon->width()-20, ui->label_icon->height()-20, overlay ); //put it in the bottom-right corner
      ui->label_icon->setPixmap(tmp);
    }else{
      //Just the normal icon
      ui->label_icon->setPixmap(QPixmap(icon));
    }
    if(type().startsWith("CD") && type()!="CD-DATA" ){ 
      ui->tool_run->setIcon(QIcon(":icons/play.png")); 
    }else{ 
      ui->tool_run->setIcon(QIcon(":icons/folder.png") ); 
    }
    ui->tool_tray->setVisible(type().startsWith("CD")); //This is a CD tray
    canmount = !filesystem().isEmpty() || !type().startsWith("CD"); //has a detected filesystem or is not a CD
    ui->tool_mount->setVisible(canmount);
    ui->check_auto->setVisible(canmount && !type().startsWith("CD"));
  }
  
  //Update the status of the mount button (TO DO - special ISO handling)
  if(isMounted){
    ui->tool_mount->setText(tr("Mount"));
    ui->tool_mount->setIcon(QIcon(":icons/mount.png"));	
    QString devsize = pcbsd::Utils::runShellCommand("pc-sysconfig \"devsize "+node()+"\"").join("");
    if(devsize.contains("??")){ 
      ui->progressBar->setRange(0,0);
    else{
       ui->progressBar->setRange(0,100);
       ui->progressBar->setValue(devsize.section("(",1,1).section("%",0,0).toInt());
       ui->progressBar->setToolTip(devSize);
    }
  }else{
    ui->tool_mount->setText(tr("Unmount"));
    ui->tool_mount->setIcon(QIcon(":icons/eject.png"));
  }
  ui->label_icon->setEnabled(isMounted || !canmount);
  ui->tool_run->setVisible( !type().startsWith("CD") ); //if it is mounted, it can also be run
  ui->progressBar->setVisible(isMounted && ui->progressBar->maximum()==100);
  
  
  if(firstrun){
    if(canmount && !type.startsWith("CD") ){
      //Load auto-mount database and act appropriately
	    
    }else if(canmount){
      //This is some kind of optical disk that can also be mounted (Blueray/DVD, or data disk for instance)
      if(type()!="CD-DATA"){ runButtonClicked(); } //not a pure data disk - go ahead and prompt to run it
    }else{
      //Always try to "play" a non-mountable device when it is first connected (AUDIO/VIDEO CD usually)
      runButtonClicked();
    }
  }
  
}

void DeviceWidget::changeAutoMount(); //auto-mount option changed
void DeviceWidget::mountButtonClicked(); //mount/unmount the device (based on current status)
void DeviceWidget::runButtonClicked(); //Run the device (audio/video CD, browse filesystem)
void DeviceWidget::OpenTrayClicked(){  
  //Open the CD tray
  QProcess::startDetached("cdcontrol eject /dev/"+node());
  emit 
}
