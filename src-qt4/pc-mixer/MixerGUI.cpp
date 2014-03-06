#include "MixerGUI.h"
#include "ui_MixerGUI.h"

MixerGUI::MixerGUI(QSettings *set) : QMainWindow(), ui(new Ui::MixerGUI){
  ui->setupUi(this); //load the designer file
  settings = set; //save this settings access for later
  closing = false;
  //connect the signals/slots
  connect(ui->actionClose_Mixer, SIGNAL(triggered()), this, SLOT(hideGUI()) );
  connect(ui->actionClose_Mixer_and_Tray, SIGNAL(triggered()), this, SLOT(closeApplication()) );
}

MixerGUI::~MixerGUI(){
	
}

void MixerGUI::updateGUI(){
  //Load the list of available devices
  QStringList devList = Mixer::getDevices();
  devList.sort();
  //Clear the UI
  ui->combo_default->disconnect();
  ui->combo_default->clear();
  
  //Now Fill the UI with the devices
  QString cdefault = settings->value("tray-device", "vol").toString();
  for(int i=0; i<devList.length(); i++){
    //Get the individual pieces
    QString dev = devList[i].section(":",0,0);
    int Lval = devList[i].section(":",1,1).toInt();
    int Rval = devList[i].section(":",2,2).toInt();
    //Now create the device widget
	  
    //Now add the device to the default List
    ui->combo_default->addItem(dev);
    if(dev == cdefault){
      ui->combo_default->setCurrentIndex(i);
    }
  }
  //re-connect combobox signal
  connect(ui->combo_default, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeDefaultTrayDevice(QString)) );
}

void MixerGUI::changeDefaultTrayDevice(QString device){
  settings->setValue("tray-device", device);
  emit updateTray();
}

void MixerGUI::itemChanged(QString device){
  if(device == ui->combo_default->currentText()){
    emit updateTray();
  }
}