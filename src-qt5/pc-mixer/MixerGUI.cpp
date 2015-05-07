#include "MixerGUI.h"
#include "ui_MixerGUI.h"

#include <pcbsd-utils.h>

MixerGUI::MixerGUI(QSettings *set) : QMainWindow(), ui(new Ui::MixerGUI){
  ui->setupUi(this); //load the designer file
  settings = set; //save this settings access for later
  closing = false;
  //connect the signals/slots
  connect(ui->actionClose_Mixer, SIGNAL(triggered()), this, SLOT(hideGUI()) );
  connect(ui->actionClose_Mixer_and_Tray, SIGNAL(triggered()), this, SLOT(closeApplication()) );
  connect(ui->menuConfiguration, SIGNAL(triggered(QAction*)), this, SLOT(startExternalApp(QAction*)) );
  connect(ui->tool_test, SIGNAL(clicked()), this, SLOT(TestSound()) );
  connect(ui->actionRestart_PulseAudio, SIGNAL(triggered()), this, SLOT(RestartPulseAudio()) );
}

MixerGUI::~MixerGUI(){
	
}

void MixerGUI::updateGUI(){
  //Load the list of available devices
  QStringList devList = Mixer::getDevices();
  //devList.sort();
  //Clear the UI
  ui->combo_default->disconnect();
  ui->combo_default->clear();
  ui->combo_record->disconnect();
  ui->combo_record->clear();
  ui->combo_outdevice->disconnect();
  ui->combo_outdevice->clear();
  delete ui->scrollArea->widget(); //delete the widget and all children
  ui->scrollArea->setWidget( new QWidget() ); //create a new widget in the scroll area
  ui->scrollArea->widget()->setContentsMargins(0,0,0,0);
  QHBoxLayout *layout = new QHBoxLayout;
  //Now Fill the UI with the devices
  QString cdefault ="none";
  QString rdefault = Mixer::getRecDevice();
  if(settings!=0){
    cdefault = settings->value("tray-device", "vol").toString();
  }
  for(int i=0; i<devList.length(); i++){
    //Get the individual pieces
    QString dev = devList[i].section(":",0,0);
    int Lval = devList[i].section(":",1,1).toInt();
    int Rval = devList[i].section(":",2,2).toInt();
    //Now create the device widget
    DeviceWidget *device = new DeviceWidget(this);
      device->setupDevice(dev, Lval, Rval);
      layout->addWidget(device);
      connect(device, SIGNAL(deviceChanged(QString)), this, SLOT(itemChanged(QString)) );
    //Now add the device to the default/record lists
    ui->combo_default->addItem(dev);
    ui->combo_record->addItem(dev);
    if(dev == cdefault){
      ui->combo_default->setCurrentIndex(i);
    }
    if(dev == rdefault){
      ui->combo_record->setCurrentIndex(i);
    }
  }
  layout->addStretch(); //add spacer to the end
  layout->setContentsMargins(2,2,2,2);
  layout->setSpacing(4);
  ui->scrollArea->widget()->setLayout(layout);
  ui->scrollArea->setMinimumHeight(ui->scrollArea->widget()->minimumSizeHint().height()+ui->scrollArea->horizontalScrollBar()->height());
  //Now rebuild the output device list
  QStringList outdevs = pcbsd::Utils::runShellCommand("pc-sysconfig list-audiodev").join("").split(", ");
  for(int i=0; i<outdevs.length(); i++){
    if(outdevs[i].startsWith("pcm")){
      ui->combo_outdevice->addItem(outdevs[i].section(" default",0,0), outdevs[i].section(":",0,0) );
      if(outdevs[i].contains(" default")){ ui->combo_outdevice->setCurrentIndex( ui->combo_outdevice->count()-1); }
    }
  }
  ui->combo_outdevice->setEnabled(ui->combo_outdevice->count() > 0);
  //re-connect combobox signal
  connect(ui->combo_default, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeDefaultTrayDevice(QString)) );
  connect(ui->combo_record, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeRecordingDevice(QString)) );
  connect(ui->combo_outdevice, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeOutputDevice()) );
  //Hide the tray icon controls if necessary
  ui->combo_default->setVisible(settings!=0);
  ui->label_tray->setVisible(settings!=0);
  ui->actionClose_Mixer_and_Tray->setVisible(settings!=0);
}

void MixerGUI::changeDefaultTrayDevice(QString device){
  if(settings!=0){ settings->setValue("tray-device", device); }
  emit updateTray();
}

void MixerGUI::changeRecordingDevice(QString device){
  Mixer::setRecDevice(device);
}

void MixerGUI::changeOutputDevice(){
  QString dev = ui->combo_outdevice->currentData().toString();
  if(dev.isEmpty()){ return; }
  QProcess::execute("pc-sysconfig \"setdefaultaudiodevice "+dev+"\"");
  updateGUI();
  emit outChanged();
}

void MixerGUI::itemChanged(QString device){
  if(device == ui->combo_default->currentText()){
    emit updateTray();
  }
}
 
void MixerGUI::TestSound(){
  QProcess::startDetached("mplayer /usr/local/share/sounds/testsound.ogg");
}

void MixerGUI::RestartPulseAudio(){
  QProcess::execute("pulseaudio --kill");
  QProcess::startDetached("start-pulseaudio-x11");
}
