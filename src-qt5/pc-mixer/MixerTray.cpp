#include "MixerTray.h"

#include "pcbsd-utils.h"

MixerTray::MixerTray() : QSystemTrayIcon(){
  starting = true;
  //Initialize the settings backend
  settings = new QSettings("PCBSD", "pc-mixer");
  //Initialize all the widgets
  isMuted = false;
  slider = new QSlider(Qt::Vertical, 0);
    slider->setRange(0,100);
    slider->setTickPosition(QSlider::NoTicks);
  slideA  = new QWidgetAction(0);
	slideA->setDefaultWidget(slider);
  mute = new QToolButton(0);
  muteA = new QWidgetAction(0);
	muteA->setDefaultWidget(mute);
  mixer = new QToolButton(0);
	mixer->setText(tr("Mixer"));
	mixer->setIcon(QIcon(":icons/configure.png"));
	mixer->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  mixerA = new QWidgetAction(0);
	mixerA->setDefaultWidget(mixer);

  //Get output devices


  actionMenu = new QMenu(0);
	actionMenu->addAction(slideA);
	actionMenu->addAction(muteA);
	actionMenu->addSeparator();
    FillOutputDevices(actionMenu->addMenu(tr("Output")));
	actionMenu->addAction(mixerA);
  //Now initialize the GUI
  GUI = new MixerGUI(settings);
  
  //Connect the signals/slots
  connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayActivated()) );
  connect(mixer, SIGNAL(clicked()), this, SLOT(openMixerGUI()) );
  connect(mute, SIGNAL(clicked()), this, SLOT(muteClicked()) );
  connect(slider, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)) );
  connect(GUI, SIGNAL(updateTray()), this, SLOT(loadVol()) );
  
  //Show a quick icon to prevent a warning message
  this->setIcon(QIcon(":icons/audio-volume-high.png"));
  CVOL = 101; //quick default to maximize it
  CDIFF = 0;
  //Make sure no single-instance events for 30 seconds
  QTimer::singleShot(30000, this, SLOT(doneStarting()) );
  QTimer::singleShot(10, this, SLOT(loadVol()) ); //Update the UI
}

MixerTray::~MixerTray(){

}

void MixerTray::FillOutputDevices(QMenu *menu)
{
    QStringList outdevs = pcbsd::Utils::runShellCommand("pc-sysconfig list-audiodev").join("").split(", ");
      for(int i=0; i<outdevs.length(); i++){
        if(outdevs[i].startsWith("pcm")){

          QAction* action = new QAction(menu);
          action->setCheckable(true);
          action->setChecked(outdevs[i].contains(" default"));
          action->setText(/*outdevs[i].section(" default",0,0),*/ outdevs[i].section(":",1,1).replace(" default",""));
          menu->addAction(action);
          /*ui->combo_outdevice->addItem(outdevs[i].section(" default",0,0), outdevs[i].section(":",0,0) );
          if(outdevs[i].contains(" default")){ ui->combo_outdevice->setCurrentIndex( ui->combo_outdevice->count()-1); }*/
        }
      }
    //QMenu
}

//==============
//        PRIVATE
//==============
void MixerTray::loadVol(){
  //Generally only used to update the tray to match the backend setting (or if tray device changed)
  QString val = Mixer::getValues(settings->value("tray-device","vol").toString());
  if(val.isEmpty()){
    //invalid device - return to default "vol"
    settings->setValue("tray-device","vol");
    val = Mixer::getValues("vol");
  }
  int L = val.section(":",0,0).toInt();
  int R = val.section(":",1,1).toInt();
  //Record the different between L/R channels (if any)
  CDIFF = L - R; // - for L lower, + for R lower
  //Just use the largest value for the moment
  if(L > R){ R = L; }
  else if(L < R){ L = R; }
  //Now just run the changeVol function to update everything (better than duplication)
  changeVol(R, false);
}

void MixerTray::changeVol(int percent, bool modify){
  //Determine the percentage to actually use and adjust internal value
  if(percent < 0 && isMuted){ percent = CVOL; }	  //Return to CVOL value
  else if(percent < 0){ percent = 0; } //Don't overwrite CVOL if mute clicked
  else if(percent > 100){ percent = 100; CVOL = 100;}
  else{ CVOL = percent; }
  //Now update the backend mixer to the proper value
  if(modify){
    if(CDIFF==0){
      Mixer::setValues(settings->value("tray-device","vol").toString(), percent);
    }else if(CDIFF < 0){
      //L channel lower
      Mixer::setValues(settings->value("tray-device","vol").toString(), percent + CDIFF, percent);
    }else{
      //R channel lower
      Mixer::setValues(settings->value("tray-device","vol").toString(), percent, percent - CDIFF);
    }
    if(GUI->isVisible()){
      //also update the main mixer GUI if it is visible
      GUI->updateGUI();
    }
  }
  //Now update the display appropriately
  if(percent == 0){
    isMuted = true;
    mute->setIcon( QIcon::fromTheme("audio-volume-high", QIcon(":icons/audio-volume-high.png")) );
    this->setIcon( QIcon::fromTheme("audio-volume-muted",QIcon(":icons/audio-volume-muted.png")) );
  }else{
    isMuted = false;
    if(percent < 33){
      this->setIcon( QIcon::fromTheme("audio-volume-low", QIcon(":icons/audio-volume-low.png")) );
    }else if(percent < 66){
      this->setIcon( QIcon::fromTheme("audio-volume-medium", QIcon(":icons/audio-volume-medium.png")) );
    }else{
      this->setIcon( QIcon::fromTheme("audio-volume-high", QIcon(":icons/audio-volume-high.png")) );
    }
    mute->setIcon( QIcon::fromTheme("audio-volume-muted", QIcon(":icons/audio-volume-muted.png")) );
  }
  
  slider->disconnect(); //Make sure this slider change does not trigger the signal/slot
  slider->setValue(percent);
  connect(slider, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)) ); //reconnect it
  
  this->setToolTip(QString::number(percent)+"%");
}

