#include "settingsDialog.h"

SettingsDialog::SettingsDialog() : QDialog(){
  //Window settings
  this->setWindowTitle( tr("Mount Tray Settings") );
  this->setWindowIcon( QIcon(":icons/config.png"));
  //Now Create the UI
  QVBoxLayout *vlayout = new QVBoxLayout();
    //AutoPlay setting
    checkAutoPlay = new QCheckBox(tr("Auto-play optical disks"));
    vlayout->addWidget(checkAutoPlay);
	
    //Disk space watcher settings
    QFormLayout *flayout = new QFormLayout;
    groupDiskWatch = new QGroupBox(tr("Monitor disk storage space"));
      groupDiskWatch->setCheckable(true);
      groupDiskWatch->setFlat(false); //make sure the frame is drawn
    checkDiskAutoTimer = new QCheckBox(tr("Perform check on device notifications"));
    QLabel *spinLabel = new QLabel(tr("Maximum time between checks (minutes)"));
    spinDiskRefreshMin = new QSpinBox(); //(in Minutes)
      spinDiskRefreshMin->setMinimum(1); // 1 minute minimum
      spinDiskRefreshMin->setMaximum(1440); //24 hours maximum
      flayout->addRow(spinDiskRefreshMin, spinLabel);
      flayout->addRow(checkDiskAutoTimer);
    groupDiskWatch->setLayout(flayout);
    vlayout->addWidget(groupDiskWatch);
    
    //Now add the apply/close buttons to the bottom
    QHBoxLayout *hb = new QHBoxLayout();
    hb->addStretch();
    closeButton = new QPushButton(QIcon(":icons/application-exit.png"),tr("Cancel"));
    applyButton = new QPushButton(QIcon(":icons/checkmark.png"),tr("Apply"));
      applyButton->setEnabled(false);
    hb->addWidget(applyButton);
    hb->addWidget(closeButton);
    vlayout->addLayout(hb);
    connect(closeButton,SIGNAL(clicked()),this,SLOT(slotCloseDialog()));
    connect(applyButton,SIGNAL(clicked()),this,SLOT(slotApply()));
  //Now set the layout for the dialog
  this->setLayout(vlayout);
  //Now resize the dialog to fit the widget appropriately
  this->adjustSize(); //still not working right -- need research on this
}

SettingsDialog::~SettingsDialog(){
}

void SettingsDialog::showDialog(){
  //apply the values to the UI
  groupDiskWatch->setChecked(useDiskWatcher);
  checkDiskAutoTimer->setChecked(useDiskAutoTimer);
  checkAutoPlay->setChecked(useAutoPlay);
  int minutes = diskRefreshMS/60000;
  //qDebug() << "Refresh Time:"<< QString::number(diskRefreshMS)+" ms, "+QString::number(minutes)+" min";
  spinDiskRefreshMin->setValue(minutes);
    //Apply signals/slots AFTER setting the values
    connect(groupDiskWatch, SIGNAL(clicked(bool)),this,SLOT(slotUpdateUI()) );
    connect(checkDiskAutoTimer, SIGNAL(clicked(bool)),this,SLOT(slotUpdateUI()) );
    connect(spinDiskRefreshMin, SIGNAL(valueChanged(int)),this,SLOT(slotUpdateUI()) );
    connect(checkAutoPlay,  SIGNAL(clicked()),this, SLOT(slotUpdateUI()) );
  this->exec();
	
}

void SettingsDialog::slotApply(){
  //get the values from the UI
  useDiskWatcher = groupDiskWatch->isChecked();
  useDiskAutoTimer = checkDiskAutoTimer->isChecked();
  diskRefreshMS = spinDiskRefreshMin->value() * 60000;
  useAutoPlay = checkAutoPlay->isChecked();
  //Flag that there are saved values and quit
  SettingsChanged=true;
  this->close();
}

void SettingsDialog::slotCloseDialog(){
  SettingsChanged=false;
  this->close();
}

void SettingsDialog::slotUpdateUI(){
  applyButton->setEnabled(true);
  //Could do check for visible/invisible options here as well
}



