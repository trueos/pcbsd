#include "fsDialog.h"

FSDialog::FSDialog() : QDialog(){
  generateUI();
}

FSDialog::~FSDialog(){
}

void FSDialog::generateUI(){
  //Auto-generate the UI
  QVBoxLayout *vlayout = new QVBoxLayout();
  //Filesystem display widgets
  QScrollArea *scroll = new QScrollArea(this);
  QVBoxLayout *sclLayout = new QVBoxLayout();
  QWidget *wgt = new QWidget();
    //Auto fill the scroll area
    QStringList fsList = FSWatcher::getFSmountpoints();
    //Format: mountpoint::filesystem::totalspace(K)::usedspace(K)::percentUsed
    for(int i=0; i<fsList.length(); i++){
      //Get the data for this item
      QString name = fsList[i].section("::",0,0);
      QString filesystem = fsList[i].section("::",1,1);
      int totalK = fsList[i].section("::",2,2).toInt();
      int usedK = fsList[i].section("::",3,3).toInt();
      int percent = fsList[i].section("::",4,4).toInt();
      //qDebug() << "Dialog Item:" << name << filesystem << totalK << usedK << percent;
      //Set the font/color for the name based upon status
      if(percent < 75){ name.prepend("<b><FONT COLOR='#00FF00'>"); name.append("</b>"); } //green
      else if(percent < 90){ name.prepend("<b><FONT COLOR='#FFFF00'>"); name.append("</b>"); } //yellow
      else{ name.prepend("<b><FONT COLOR='#FF0000'>"); name.append("</b>"); } //red
      //Create items for the data
      QLabel *mntpnt = new QLabel(name);
      QLabel *fstype = new QLabel(filesystem.toUpper());
      QProgressBar *prgbar = new QProgressBar();
        prgbar->setMaximum(totalK); prgbar->setMinimum(0);
        prgbar->setValue(usedK);
      QLabel *info = new QLabel( FSWatcher::intToDisplay(usedK)+"/"+FSWatcher::intToDisplay(totalK) );
      
      //Now add the items to a layout
      QHBoxLayout *tmp= new QHBoxLayout();
      tmp->addWidget(mntpnt);
      tmp->addWidget(fstype);
      tmp->addWidget(prgbar);
      tmp->addWidget(info);
      //Add the tmp layout to the main scroll area layout
      sclLayout->addLayout(tmp);
    }
    wgt->setLayout(sclLayout);
    scroll->setWidget(wgt);
  vlayout->addWidget(scroll);
  //Now add the close button to the bottom
  QPushButton *closeButton = new QPushButton("Close");
  vlayout->addWidget(closeButton);
  connect(closeButton,SIGNAL(clicked()),this,SLOT(closeDialog()));
  //Now set the layout for the dialog
  this->setLayout(vlayout);
}

void FSDialog::closeDialog(){
  this->close();
}
