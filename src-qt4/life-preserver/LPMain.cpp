#include "LPMain.h"

LPMain::LPMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::LPMain){
  ui->setupUi(); //load the Qt-designer UI file
  //Create the basic/advanced view options
  viewBasic = new QRadioButton(tr("Basic"), ui->menuView);
  viewAdvanced = new QRadioButton(tr("Advanced"), ui->menuView);
  ui->menuView->addWidget(viewBasic);
  ui->menuView->addWidget(viewAdvanced);
  connect(viewBasic, SIGNAL(toggled()), this, SLOT(viewChanged()) );
  //Now set the default view type
  viewBasic->setChecked(true); //will automatically call the "viewChanged" function
	
  //Connect the UI to all the functions
	
}

LPMain::~LPMain(){
	
}

// ==============
//      PUBLIC SLOTS
// ==============
void LPMain::updateUI(){
  //Get the currently selected pool (if there is one)
  QString cPool;
  if(ui->combo_pools->currentIndex != -1){ cPool = ui->combo_pools_currentText(); }
  //Get the list of managed pools
  QStringList pools = LPBackend::listDatasets();
  //Now put that list into the UI
  ui->combo_pools->clear();
  for(int i=0; i<pools.length(); i++){
	  
  }
  //Now set the currently selected pools
  if(pools.length() > 0){
    int index = pools.indexOf(cPool);
    if(index < 0){ ui->combo_pools->setCurrentIndex(0); }
    else{ ui->combo_pools->setCurrentIndex(index); }
    poolSelected=true;
  }else{
    //No managed pools
    ui->combo_pools->addItem("No Managed Pools!");
    ui->combo_pools->setCurrentIndex(0);
    poolSelected=false;
  }
  //Now update the interface appropriately
  updateInterface();
}

// ==============
//          PRIVATE
// ==============
LPDataset LPMain::loadPoolData(QString pool){
  //Load the current information for the given zpool
  qDebug() << "New Dataset: " << ds;
  LPDataset DSC;
  //List all the mountpoints in this dataset
  QStringList subsets = LPBackend::listDatasetSubsets(ds);
  QStringList lpsnaps = LPBackend::listLPSnapshots(ds);
  //populate the list of snapshots available for each mountpoint
  for(int i=0; i<subsets.length(); i++){
    //qDebug() << "Subset:" << subsets[i];
    QStringList snaps = LPBackend::listSnapshots(subsets[i]);
    //qDebug() << " - Snapshots:" << snaps;
    if(snaps.isEmpty()){
      //invalid subset - remove it from the list
      subsets.removeAt(i);
      i--;
    }else{
      QStringList subsnaps;
      //only list the valid snapshots that life preserver created
      for(int s=0; s<lpsnaps.length(); s++){
	int index = snaps.indexOf(lpsnaps[s]);
        if(index > -1){ subsnaps << lpsnaps[s]; snaps.removeAt(index); }
      }
      //Now list all the other available snapshots (no certain ordering)
      if(!snaps.isEmpty()){
	subsnaps << "--"; //so we know that this is a divider between the sections
	subsnaps << snaps;
      }
      DSC.subsetHash.insert(subsets[i],subsnaps); //add it to the internal container hash
    }
  }
  //Get the time for the latest life-preserver snapshot (and total number)
  //Find the index for the current list
  int ci = 0;
  while(ci < CLIST.length()){
    if(CLIST[ci].startsWith(ds+":::")){ break; }
    else{ ci++; }
  }
  if(CLIST.isEmpty()){ ci = -1; } //catch for empty list
  if(DSC.subsetHash.size() < 1){
    DSC.numberOfSnapshots = "0";
    DSC.latestSnapshot= "";
  }else{
    DSC.numberOfSnapshots = QString::number(lpsnaps.length());
    if(lpsnaps.isEmpty()){ DSC.latestSnapshot=""; }
    else if(ci > -1 && ci < CLIST.length()){ 
      QString sna = CLIST[ci].section(":::",1,1);
      if(sna != "-"){ DSC.latestSnapshot= sna; }
      else{ DSC.latestSnapshot = ""; }      
    }else{ DSC.latestSnapshot=lpsnaps[0]; }
  }
  //List the replication status
  if(RLIST.contains(ds) && (ci > -1)){ 
    QString rep = CLIST[ci].section(":::",2,2);
    if(rep != "-"){ DSC.latestReplication = rep; }
    else{ DSC.latestReplication= tr("Enabled"); }
  }else{ 
    DSC.latestReplication= tr("Disabled");
  }
  //Return the dataset
  return DSC;
}


// ==============
//     PRIVATE SLOTS
// ==============
void LPMain::viewChanged(){
  if(viewBasic->isChecked()){
    ui->menuDisks->setVisible(false);
    ui->menuSnapshots->setVisible(false);
  }else{
    ui->menuDisks->setVisible(true);
    ui->menuSnapshots->setVisible(true);
  }
}

void updateInterface(){
  viewChanged();
  ui->tabWidget->setEnabled(poolSelected);
  ui->menuView->setVisible(poolSelected);	
  ui->tool_configure->setVisible(poolSelected);
  ui->tool_configBackups->setVisible(poolSelected);
  ui->actionUnmanage_Pool->setEnabled(poolSelected);
  ui->action_SaveKeyToUSB->setEnabled(poolSelected);
  if(poolSelected){
    POOLDATA = loadPoolData(ui->combo_pools->currentText());
    //Now list the status information
	  
    //Now list the data restore options
    QStringList
  }else{
    //No Pool selected
    ui->menuDisks->setVisible(false); //make sure this is always invisible if nothing selected
    ui->menuSnapshots->setVisible(false); //make sure this is always invisible if nothing selected
  }

}
