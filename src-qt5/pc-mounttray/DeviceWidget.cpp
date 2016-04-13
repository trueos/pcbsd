#include "DeviceWidget.h"
#include "ui_DeviceWidget.h"

#include <QProcess>
#include <QStringList>
#include <QIcon>
#include <QInputDialog>
#include <QMessageBox>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QPainter>
#include <QPixmap>
#include <QIcon>

#include <pcbsd-utils.h>

DeviceWidget::DeviceWidget(QWidget *parent, QString devnode) : QWidget(parent), ui(new Ui::DeviceWidget){
  ui->setupUi(this); //load the designer file
  isMounted = false; //default value
  autoplay = true; //default value
  WAct = new QWidgetAction(this);
    WAct->setDefaultWidget(this);
  ui->label_dev->setWhatsThis(devnode);
  connect(ui->tool_run, SIGNAL(clicked()), this, SLOT(runButtonClicked()) );
  connect(ui->tool_mount, SIGNAL(clicked()), this, SLOT(mountButtonClicked()) );
  connect(ui->check_auto, SIGNAL(clicked(bool)), this, SLOT(changeAutoMount(bool)) );
  connect(ui->tool_tray, SIGNAL(clicked()), this, SLOT(OpenTrayClicked()) );
	
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

QString DeviceWidget::mountpoint(){
  return ui->tool_run->whatsThis();	
}

QWidgetAction* DeviceWidget::action(){
  return WAct;
}

// == Widget UpdateRoutines
void DeviceWidget::UpdateDevice(bool ismounted){
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

void DeviceWidget::setAutoPlay(bool ok){
  autoplay = ok;
}

// == PRIVATE FUNCTIONS ==
void DeviceWidget::doUpdate(){
  bool firstrun = type().isEmpty();
  //qDebug() << "Update Item:" << firstrun << quickupdates << node();
  if(firstrun || !quickupdates){
    QStringList info = pcbsd::Utils::runShellCommand("pc-sysconfig \"devinfo "+node()+"\"").join("").split(", ");
    if(info.length() < 3){ emit RefreshDeviceList(); return; } //invalid device - will probably get removed here in a moment
    //Info Output: <filesystem>, <label>, <type> [, <mountpoint>]
    //qDebug() << " - info:" << info;
    //Save this into the internal variables
    ui->label_icon->setWhatsThis(info[2]); //type
    ui->label_dev->setText(info[1]); //label
    ui->tool_mount->setWhatsThis(info[0]); //filesystem
    if(info.length()>=4){ui->tool_run->setWhatsThis(info[3]); } //mountpoint
    
    if(info[0].toLower()=="zfs" || info[0].toLower()=="mtpfs"){ ui->label_icon->setToolTip(node()+" ("+info[0]+")"); }
    else{ ui->label_icon->setToolTip("/dev/"+node()+" ("+info[0]+")"); }
    //Now go through and set all the various icons and such
    QString icon = ":icons/%1.png";
    if(type()=="SATA"){ icon = icon.arg("harddrive"); }
    else if(type()=="USB"){ icon = icon.arg("usb"); }
    else if(type()=="SD"){ icon = icon.arg("sdcard"); }
    else if(type()=="CD-AUDIO"){ icon = icon.arg("musiccd"); }
    else if(type()=="CD-VIDEO"){ icon = icon.arg("cd-video"); }
    else if(type().startsWith("CD")){ icon = icon.arg("cd-generic"); }
    else if(type()=="ISO"){ icon = icon.arg("dvd"); }
    else{ icon = icon.arg("CDdevices"); }
    if(filesystem()=="NONE" && !type().startsWith("CD")){
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
    if(type()=="CD-AUDIO" || type()=="CD-VIDEO"){ 
      ui->tool_run->setIcon(QIcon(":icons/play.png")); 
      ui->tool_run->setText(tr("Play"));
    }else{ 
      ui->tool_run->setIcon(QIcon(":icons/folder.png") ); 
      ui->tool_run->setText(tr("Browse"));
    }
    ui->tool_tray->setVisible(type().startsWith("CD")); //This is a CD tray
    canmount = filesystem()!="NONE" || !type().startsWith("CD"); //has a detected filesystem or is not a CD
    ui->tool_mount->setVisible(canmount);
    ui->check_auto->setVisible(canmount && !type().startsWith("CD"));
  }
  
  //Update the status of the mount button (TO DO - special ISO handling)
  if(isMounted){
    ui->tool_mount->setText(tr("Unmount"));
    ui->tool_mount->setIcon(QIcon(":icons/eject.png"));	
    QString devsize = pcbsd::Utils::runShellCommand("pc-sysconfig \"devsize "+node()+"\"").join("");
    if(devsize.contains("??")){ 
      ui->progressBar->setRange(0,0);
    }else{
       ui->progressBar->setRange(0,100);
       ui->progressBar->setValue(devsize.section("(",1,1).section("%",0,0).toInt());
       ui->progressBar->setToolTip(devsize);
    }
  }else{
    ui->tool_mount->setText(tr("Mount"));
    ui->tool_mount->setIcon(QIcon(":icons/mount.png"));
  }
  ui->label_icon->setEnabled(isMounted || !canmount);
  ui->tool_run->setVisible( (isMounted && !mountpoint().isEmpty()) || type()=="CD-AUDIO" || type()=="CD-VIDEO" ); //if it is mounted, it can also be run
  ui->progressBar->setVisible(isMounted && ui->progressBar->maximum()==100);
  
  
  if(firstrun){
    if(canmount && !type().startsWith("CD") ){
      //Load auto-mount database and act appropriately
      QString AMFILE = QDir::homePath() + "/.pc-automounttray";
      if(QFile::exists(AMFILE)){
        QString cmd = "cat "+AMFILE;
        QString search = label() +" "+ type()+" "+ filesystem();
        bool amount = !pcbsd::Utils::runShellCommandSearch(cmd, search).isEmpty();
	ui->check_auto->setChecked(amount);
        if(amount){
	  mountButtonClicked();
	  if(isMounted){
	    emit ShowMessage(tr("Device Mounted"), QString(tr("%1 has been automatically mounted on %2")).arg(label(), mountpoint()) );
	    runButtonClicked(); //also open the directory
	  }
	}
      }
    }else if(canmount){
      //This is some kind of optical disk that can also be mounted (Blueray/DVD, or data disk for instance)
      if(type()!="CD-DATA" && autoplay){ runButtonClicked(); } //not a pure data disk - go ahead and prompt to run it
    }else if(autoplay){
      //Always try to "play" a non-mountable device when it is first connected (AUDIO/VIDEO CD usually)
      runButtonClicked();
    }
  }
  
}

void DeviceWidget::changeAutoMount(bool checked){
  //auto-mount option changed
  QString  AMFILE= QDir::homePath() + "/.pc-automounttray";
  qDebug() << "Auto-mount toggled for:" << node() << checked;
  QString entry = label()+":::"+type()+":::"+filesystem();
  if(checked){
    //Add this entry to the auto-mount file
    QString cmd = "echo \""+entry+"\" >> "+AMFILE;
    system( cmd.toUtf8() );
  }else{
    //Remove this entry from the automount file
    QString tmpFile = AMFILE+".tmp";
    QString cmd = "cat "+AMFILE+" | grep -v "+entry+" > "+tmpFile+"; mv "+tmpFile+" "+AMFILE;
    system( cmd.toUtf8() );
  }
}

void DeviceWidget::mountButtonClicked(){
  //mount/unmount the device (based on current status)
  if(isMounted){
    QString res = pcbsd::Utils::runShellCommand("pc-sysconfig \"unmount "+node()+"\"").filter("[SUCCESS]").join("");
    if(res.isEmpty()){
      //Can add additional types of error parsing later (TO-DO)
      //See if the user wants to try and force the unmount
      if(QMessageBox::Yes == QMessageBox::question(0, tr("Device Busy"), tr("The device appears to be busy. Do you want to forcibly unmount the device?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No)){
        res = pcbsd::Utils::runShellCommand("pc-sysconfig \"unmount "+node()+" force\"").join("");
      }else{
	return; //don't show the result message if the action was cancelled
      }	      
    }
    //Now parse the output and emit the proper signals
    if(res=="[SUCCESS]"){
      QuickUpdate(false); //quickly update the UI with the new mount status
    }else{
      QMessageBox::warning(this, tr("Unmount Error"), tr("The device could not be unmounted. Please try again later") );
    }
    
  }else{
    //Device unmounted, need to mount it
    QString fs = filesystem();
    if(fs=="NONE"){
      //prompt for what filesystem to try and use  to mount the device
      QStringList fslist = pcbsd::Utils::runShellCommand("pc-sysconfig supportedfilesystems").join("").split(", ");
      bool ok = false;
      fs = QInputDialog::getItem(0,tr("No Filesystem Detected"), tr("Select a filesystem to try:"), fslist, 0, false, &ok);
      if(!ok){ return; } //cancelled
    }
    //Now try to mount the device
    QString res = pcbsd::Utils::runShellCommand("pc-sysconfig \"mount "+node()+" "+fs+"\"").filter("[SUCCESS]").join("");
    if(!res.isEmpty()){
      //Save the mountpoint for use later (return format: "[SUCCESS] <mountpoint>"
      ui->tool_run->setWhatsThis( res.section("]",1,20).simplified() );
      QuickUpdate(true);
    }else{
      qDebug() << node()+":" << res;
      QString err = tr("The device could not be mounted. Please try again later.");
      QMessageBox::warning(0,tr("Device Mounting Error"), err);
    }
    
  } //end of mount detection
  emit RefreshDeviceList();
}

void DeviceWidget::runButtonClicked(){
  //Run the device (audio/video CD, browse filesystem)
  if(isMounted){ 
    //Open the mountpoint directory
    QProcess::startDetached("xdg-open \""+mountpoint()+"\"");
  }else if(type()=="CD-AUDIO"){
    if(QFile::exists("/usr/local/bin/vlc")){ QProcess::startDetached("vlc cdda://1");}
    else if(QFile::exists("/usr/local/bin/smplayer")){ QProcess::startDetached("smplayer cdda://1"); }
    else{ QProcess::startDetached("xdg-open cdda://1"); }
  }else if(type()=="CD-VIDEO"){
    if(QFile::exists("/usr/local/bin/vlc")){ QProcess::startDetached("vlc dvd://1");}
    else if(QFile::exists("/usr/local/bin/smplayer")){ QProcess::startDetached("smplayer dvd://1"); }
    else{ QProcess::startDetached("xdg-open dvd://1"); }
  }
  emit CloseMenu();
}

void DeviceWidget::OpenTrayClicked(){  
  //Open the CD tray
  if(isMounted){ mountButtonClicked(); } //unmount the device before opening the tray
  if(isMounted){ return; } //Could not unmount device: don't open the tray
  QProcess::startDetached("cdcontrol eject /dev/"+node());
  emit CloseMenu();
  emit RefreshDeviceList();
}
