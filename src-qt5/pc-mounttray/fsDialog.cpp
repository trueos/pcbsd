#include "fsDialog.h"

FSDialog::FSDialog() : QDialog(){
  this->setWindowTitle( tr("System Disk Capacity") );
  this->setWindowIcon( QIcon(":icons/harddrive.png"));
  generateUI();
}

FSDialog::~FSDialog(){
}

void FSDialog::generateUI(){
  //Auto-generate the UI
  QVBoxLayout *vlayout = new QVBoxLayout();
  //Filesystem display widget
  QTreeWidget *tree = new QTreeWidget(this);
    tree->setIndentation(0);
    //tree->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Preferred );
    //setup the column headers
    tree->setColumnCount(4);
    QStringList header;
    header << tr("Mount Point") << tr("Filesystem") << tr("Used/Total") << tr("Percent %");
    tree->setHeaderLabels(header);
    
    //Auto fill the scroll area
    QStringList fsList = FSWatcher::getFSmountpoints();
    //Format: mountpoint::filesystem::totalspace(K)::usedspace(K)::percentUsed
    for(int i=0; i<fsList.length(); i++){
      //Get the data for this item
      QString name = fsList[i].section("::",0,0);
      QString filesystem = fsList[i].section("::",1,1);
      //Don't show NULLFS filesystem (prevent showing all the PBI mountpoints)
      //if(filesystem.toLower()=="nullfs"){ continue; }
      double totalK = fsList[i].section("::",2,2).toDouble();
      double usedK = fsList[i].section("::",3,3).toDouble();
      int percent = fsList[i].section("::",4,4).toInt();
      //Create item for the data
      QStringList text; //column text
      text << name << filesystem.toUpper() << FSWatcher::doubleToDisplay(usedK)+"/"+FSWatcher::doubleToDisplay(totalK) << QString::number(percent)+"%";
      QTreeWidgetItem *tmp = new QTreeWidgetItem(text);
      
      //set the item background based upon urgency
      if(percent < 75){} //do nothing
      else if(percent < 90){ // yellow
      	QBrush color(QColor(237,233,12));
      	tmp->setBackground(0,color); tmp->setBackground(1,color);
      	tmp->setBackground(2,color); tmp->setBackground(3,color);
      }
      else{  //red
        QBrush color(QColor(237,12,53));
      	tmp->setBackground(0,color); tmp->setBackground(1,color);
      	tmp->setBackground(2,color); tmp->setBackground(3,color);
      }   
      //Now add the item to the widget
      tree->addTopLevelItem(tmp);
    }
    
  vlayout->addWidget(tree);
  //Make sure the widget is the smallest size possible
  tree->resizeColumnToContents(0); tree->resizeColumnToContents(1);
  tree->resizeColumnToContents(2); tree->resizeColumnToContents(3);
  //Now add the close button to the bottom
  QHBoxLayout *hb = new QHBoxLayout();
  hb->addStretch();
  QPushButton *closeButton = new QPushButton(QIcon(":icons/application-exit.png"),tr("Close"));
  hb->addWidget(closeButton);
  hb->addStretch();
  vlayout->addLayout(hb);
  connect(closeButton,SIGNAL(clicked()),this,SLOT(closeDialog()));
  //Now set the layout for the dialog
  this->setLayout(vlayout);
  //Now resize the dialog to fit the widget appropriately
  this->adjustSize(); //still not working right -- need research on this
}

void FSDialog::closeDialog(){
  this->close();
}
