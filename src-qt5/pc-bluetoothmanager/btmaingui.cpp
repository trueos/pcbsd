#include "btmaingui.h"
#include "ui_btmaingui.h"
#include <pcbsd-hardware.h>
#include <pcbsd-utils.h>
#include <pcbsd-ui.h>
#include <QDebug>
#include <QMessageBox>

/* UI handles
 * Comp settings Tab: pushStartTray   pushChangeCompName  pushDisconnectDevice  pushRestart  lineCompName  listConnectedDevices
 * New Device Tab:  pushAddDevice  pushScanNew  listNewDevice  newDeviceInfo
 * Old Device Tab:  pushConfigureOld  pushRemoveOld  listOldDevice oldDeviceInfo pushConnectDevice
 */

QStringList oldSaveBdaddrList, oldSaveKeyList, oldSavePinList, oldSaveNameList, newSaveBdaddrList, currentSaveDeviceList;

btmaingui::btmaingui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::btmaingui)
{
    ui->setupUi(this);
    firstRun();
}

btmaingui::~btmaingui()
{
    delete ui;
}

void btmaingui::firstRun(){
  //Setup Signals/Slots
  connect(ui->pushScanNew,SIGNAL(clicked()),this,SLOT(scanForDevices()));
  connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(tabChanged(int)));
  connect(ui->pushRemoveOld,SIGNAL(clicked()),this,SLOT(removeOldDevice()));
  connect(ui->listOldDevice,SIGNAL(currentRowChanged(int)),this,SLOT(updateOldDeviceInfo(int)));
  connect(ui->listNewDevice,SIGNAL(currentRowChanged(int)),this,SLOT(refreshGUIslot(int)));
  connect(ui->listConnectedDevices,SIGNAL(currentRowChanged(int)),this,SLOT(refreshGUIslot(int)));
  connect(ui->pushAddDevice,SIGNAL(clicked()),this,SLOT(addNewDevice()));
  connect(ui->pushConfigureOld,SIGNAL(clicked()),this,SLOT(configureOldDevice()));
  connect(ui->pushDisconnectDevice,SIGNAL(clicked()),this,SLOT(disconnectDevice()));
  connect(ui->pushConnectDevice,SIGNAL(clicked()),this,SLOT(connectDevice()));
  connect(ui->pushRestart,SIGNAL(clicked()),this,SLOT(restartBluetooth()));
  connect(ui->pushChangeCompName,SIGNAL(clicked()),this,SLOT(changeCompName()));
  connect(ui->pushStartTray,SIGNAL(clicked()),this,SLOT(startTrayApplication()));
  //Start on the system settings tab
  ui->tabWidget->setCurrentIndex(0);
  //Load the information for each Tab as needed
  updateCompInfo(); //Load info for computer settings tab
  refreshOldDeviceList(); //Load info for the saved devices tab (fill needed arrays)
  refreshGUI();
}

void btmaingui::tabChanged(int newTab){

  if(newTab==0){  //Computer Settings
    updateCompInfo();
  }else if(newTab==1){  //New Device Tab
    if(newSaveBdaddrList.isEmpty() ){ refreshGUI(); scanForDevices(); }
  }else if(newTab==2){  //Old Device Tab
    refreshOldDeviceList();
  }else{
    qDebug() << "BlueToothManager: TabChanged Error: Tab #"<< newTab << " does not exist";
  }
  refreshGUI();
}

void btmaingui::refreshGUI(){
  //Get the current Tab
  int tab = ui->tabWidget->currentIndex();
  refreshGUIslot(tab);
}
  
void btmaingui::refreshGUIslot(int tab){
  //Setup the dynamic enables/disables for each tab
  if(tab==0){ //Computer Setting Tab
    if(ui->listConnectedDevices->currentRow() == -1){ui->pushDisconnectDevice->setEnabled(false);}
    else{ui->pushDisconnectDevice->setEnabled(true);}
    ui->pushRestart->setEnabled(true);
  }else if(tab==1){
    if(ui->listNewDevice->currentRow() == -1){ui->pushAddDevice->setEnabled(false);}
    else{ui->pushAddDevice->setEnabled(true);}
    ui->pushScanNew->setEnabled(true);
  }else if(tab==2){
    if(ui->listOldDevice->currentRow() == -1){
      ui->pushConfigureOld->setEnabled(false);
      ui->pushRemoveOld->setEnabled(false);
      ui->pushConnectDevice->setEnabled(false);    
    }else{
      ui->pushConfigureOld->setEnabled(true);
      ui->pushRemoveOld->setEnabled(true);
      ui->pushConnectDevice->setEnabled(true);
    }
    
  }else{
    qDebug() << "Attempted to refresh an unknown tab number:" << tab;
  }
}

void btmaingui::scanForDevices(){
   ui->pushScanNew->setEnabled(false);
   ui->pushAddDevice->setEnabled(false);
   //Clear the GUI for new results
   ui->newDeviceInfo->clear();
   ui->newDeviceInfo->append(tr("Searching for discoverable Bluetooth devices"));
   ui->listNewDevice->clear();
   newSaveBdaddrList.clear();
   //Start scanning and put the results in the listbox
   QStringList bdaddrList = Hardware::findBTdevices();
   QStringList connectionList = pcbsd::Utils::runShellCommand("hccontrol read_connection_list");
   for(int i=0; i<bdaddrList.length(); i++){
     if( !connectionList.contains(bdaddrList[i]) && !oldSaveBdaddrList.contains(bdaddrList[i]) ){ //Check if it is a new device (not connected or saved)
       QString name = Hardware::getBTRemoteName(bdaddrList[i],true);
       newSaveBdaddrList << bdaddrList[i];
       ui->listNewDevice->addItem(name+" ("+bdaddrList[i]+")"); //Add it to the new device list
     }
   }
   //Clear the info box
   ui->newDeviceInfo->clear();
   if(ui->listNewDevice->count()==0){     //No Devices found
     ui->newDeviceInfo->append(tr("No new Bluetooth devices discovered! Please put your device into \"discovery\" mode and rescan."));
   }
   ui->pushScanNew->setEnabled(true);
   refreshGUI();
}

void btmaingui::refreshOldDeviceList(){
  //Clear the list for updating
  ui->listOldDevice->clear(); 
  ui->listOldDevice->setCurrentRow(-1);
  ui->oldDeviceInfo->clear();
  ui->oldDeviceInfo->append(tr("Reading saved Bluetooth devices. Please wait."));
  //Make sure there is root permissions to read/write the settings
  if( !rootPermissions() ){
    ui->oldDeviceInfo->clear();
    ui->oldDeviceInfo->append(tr("This program must be run with root permissions to view/edit saved configurations"));
    return;
  }
    
  //Get all the Old Devices saved in hcsecd.conf and load them into memory
  QStringList oldBdaddrList = Hardware::readAllSavedBTDevices();
  oldSaveKeyList.clear(); oldSavePinList.clear(); oldSaveNameList.clear(); oldSaveBdaddrList.clear(); //reset the saved variables
  
  //Stop earlier if there is no saved devices
  if(oldBdaddrList.isEmpty()){
    ui->oldDeviceInfo->clear();
    ui->oldDeviceInfo->append(tr("No saved configurations found"));
    return;
  }
  for(int i=0; i<oldBdaddrList.length(); i++){
    QStringList temp = Hardware::readSavedBTDevice(oldBdaddrList[i]);
    oldSaveBdaddrList << oldBdaddrList[i];
    oldSaveNameList << temp[1];
    oldSaveKeyList << temp[2];
    oldSavePinList << temp[3];
  }
  
  //Display the name/bdaddr of all the saved devices
  for(int i=0; i<oldSaveBdaddrList.length();i++){
    ui->listOldDevice->addItem(oldSaveNameList[i]+" ("+oldSaveBdaddrList[i]+")");
  }
  ui->oldDeviceInfo->clear();
}

void btmaingui::removeOldDevice(){
  //Get the BDADDR of the selected device
  int row = ui->listOldDevice->currentRow();
  if(row == -1){
   //No device selected
   QMessageBox::warning(this,tr("Warning"),tr("No device has been selected. Please choose a device first."));
   return;
  }
  QString bdaddr = oldSaveBdaddrList[row];
  //Remove the Device from hcsecs.conf
  Hardware::rmBTdevice(bdaddr,true); //restart the network after removing the device
  //Refresh the Device List
  refreshOldDeviceList();
}

void btmaingui::addNewDevice(){
  //Check for root permissions
  if( !rootPermissions() ){
    //No root permissions to save device
    QMessageBox::warning(this,tr("Invalid Permissions"),tr("This program must be run with root permissions to save device settings"));
    return;
  }
  
  //Get the BDADDR of the selected device
  int row = ui->listNewDevice->currentRow();
  if(row == -1){
    //No device selected
    QMessageBox::warning(this,tr("Warning"),tr("No device has been selected. Please choose a device first."));
    return;
  }
  QString bdaddr = newSaveBdaddrList[row];
  
  //Ask for PIN and KEY for configuration
  QStringList requests;
  requests << tr("PIN Code (Examples: 0000, 1234, or none)") << tr("Link Key (Leave blank for automatic)");
  QStringList outputs = pcbsd::UI::quickUserInputBox(tr("Device Configuration"),requests);
  if( outputs.length() != requests.length() ){ //rememberempty strings are valid
    //Error/cancelled operation - do nothing
    qDebug() << "Add new device: Cancelled";
    return;
  }
  QString pin = outputs[0];
  QString key = outputs[1];
  //Quick Check for pin validity
  if( pin.length()!=4 && !pin.isEmpty() ){ 
    QMessageBox::warning(this,tr("Warning"),tr("Invalid Pin length: Defaulting to none"));
    pin = "";
  }
  if(pin == "none"){ pin=""; }
  
  ui->pushAddDevice->setEnabled(false);
  //Add the entry to hcsecd.conf
  Hardware::addBTdevice(bdaddr,"",key,pin);
  //Move to the Saved devices tab
  ui->tabWidget->setCurrentIndex(2);
  refreshOldDeviceList();
  refreshGUI();
  
  //Initiate pairing of device
  QString result = Hardware::pairBTDevice(bdaddr);
  
  //Rescan for devices in the background
  scanForDevices();
  
}

void btmaingui::updateCompInfo(){
  //Clear the display for action
  ui->listConnectedDevices->clear();
  //Get computer host/device names and display it
  QStringList compName = Hardware::getBTdevNames(); //compName[0]=host name; compName[1]=device name
  ui->lineCompName->setText(compName[0]+" ("+compName[1]+")");
  //List all active connections (if any)
  currentSaveDeviceList.clear();
  QStringList connectionList = pcbsd::Utils::runShellCommand("hccontrol read_connection_list");
  if(connectionList.length() > 1){
    for(int i=1; i<connectionList.length(); i++){ //skip the first line (labels)
      QString bdaddr = connectionList[i].section(" ",0,0,QString::SectionSkipEmpty).simplified();
      currentSaveDeviceList << bdaddr;
      QString name = Hardware::getBTRemoteName(bdaddr,true);
      ui->listConnectedDevices->addItem(name+" ("+bdaddr+")");
    }
  }
}

void btmaingui::updateNewDeviceInfo(){
  
}

void btmaingui::updateOldDeviceInfo(int row){
  //qDebug() << row << oldBdaddrList << oldNameList << oldKeyList << oldPinList;
  //Display the hcsecd configuration for the selected device
  ui->oldDeviceInfo->clear();
  if(row != -1){
    ui->oldDeviceInfo->append("BD_ADDR: "+oldSaveBdaddrList[row]);
    ui->oldDeviceInfo->append("Name:    "+oldSaveNameList[row]);
    ui->oldDeviceInfo->append("Key:     "+oldSaveKeyList[row]);
    ui->oldDeviceInfo->append("PIN:     "+oldSavePinList[row]);
  }
  refreshGUI();
}

bool btmaingui::rootPermissions(){
  QString userID = pcbsd::Utils::runShellCommand("id -u").join("");
  if( userID.toInt() == 0){ return true; }
  else{ return false; }
}

void btmaingui::configureOldDevice(){
  //Get the selected device information
  int row = ui->listOldDevice->currentRow();
  if(row == -1){
    //No device selected
    QMessageBox::warning(this,tr("Warning"),tr("No device has been selected. Please choose a device first."));
    return;
  }
  QString bdaddr = oldSaveBdaddrList[row];
  
  //Get the new PIN and KEY
  QStringList requests;
  requests << tr("PIN Code (Examples: 0000, 1234, or none)") << tr("Link Key (Leave blank for automatic)");
  QStringList outputs = pcbsd::UI::quickUserInputBox(tr("Device Configuration"),requests);
  if( outputs.length() != requests.length() ){ //remember empty strings are valid
    //Error/cancelled operation - do nothing
    qDebug() << "Configure old device: Cancelled";
    return;
  }
  QString pin = outputs[0];
  QString key = outputs[1];
  //Quick Check for pin validity
  if(pin == "none"){ pin=""; }
  
  //Add the entry to hcsecd.conf
  Hardware::addBTdevice(bdaddr,"",key,pin);
  
  //Refresh the Tab
  refreshOldDeviceList();
}

void btmaingui::disconnectDevice(){
  //Get the bdaddr of the selected device
  int row = ui->listConnectedDevices->currentRow();
  if(row == -1){
    //No device selected
    QMessageBox::warning(this,tr("Warning"),tr("No device has been selected. Please choose a device first."));
    return;
  }
  QString bdaddr = currentSaveDeviceList[row];
  
  //Call the disconnect routine
  Hardware::disconnectBTDevice(bdaddr);
  
  //Refresh the Box
  updateCompInfo();
}

void btmaingui::connectDevice(){
  //Get the selected device
  int row = ui->listOldDevice->currentRow();
  if(row == -1){
    //No device selected
    QMessageBox::warning(this,tr("Warning"),tr("No device has been selected. Please choose a device first."));
    return;
  }
  QString bdaddr = oldSaveBdaddrList[row];
  
  //Initiate connection (pairing)
  QString result = Hardware::pairBTDevice(bdaddr);
  
  //Determine if the connection was successful
  if(!result.isEmpty()){
    //Error making the connection
    QMessageBox::warning(this,tr("Pairing Error"),tr("Could not pair to this device. Please verify your device configuration before trying again."));
    qDebug() << "Bluetooth Connection: " + result; //display the full error on the command line
    return;
  }
  
  //Connection successful - cleanup GUI
  ui->tabWidget->setCurrentIndex(0); //move to the Computer settings tab
  updateCompInfo();
  refreshGUI();
  
}

void btmaingui::restartBluetooth(){
  ui->pushRestart->setEnabled(false);
  Hardware::restartBT();
  //refresh the GUI
  updateCompInfo();
  refreshGUI();
  
}

void btmaingui::changeCompName(){
  //Get the desired computer name
  QStringList requests;
  requests << tr("New Bluetooth Computer name");
  QStringList outputs = pcbsd::UI::quickUserInputBox(tr("New Bluetooth Name"),requests);
  //Check for invalid names
  if( outputs.join(" ").simplified().isEmpty() ){ 
    //Error/cancelled operation - do nothing
    qDebug() << "Change Computer Name: Cancelled";
    return;
  }else if(outputs.join(" ").contains("(") || outputs.join(" ").contains(")") || outputs.join(" ").contains(":") ){
    QMessageBox::warning(this,tr("Invalid Name"),tr("Device name must not contain symbols"));
    return;
  }
  QString newName = outputs[0];
  
  //Retrieve the current device names
  QStringList oNames = Hardware::getBTdevNames();
  
  //Change the current computer name
  Hardware::setBTdevName(newName,oNames[1]);
  
  //Refresh the GUI
  updateCompInfo();
  
}

void btmaingui::slotSingleInstance()
{
  this->showNormal();
  this->activateWindow();
  this->raise();
  //this->show();
}

void btmaingui::startTrayApplication(){
  //Startup pc-bluetoothtray with sudo
  QString cmd = "sudo pc-bluetoothtray &";
  pcbsd::Utils::runShellCommand(cmd);
}

