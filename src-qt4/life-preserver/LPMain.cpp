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
    POOLDATA = LPGUtils::loadPoolData(ui->combo_pools->currentText());
    //Now list the status information
	  
    //Now list the data restore options
    QStringList
  }else{
    //No Pool selected
    ui->menuDisks->setVisible(false); //make sure this is always invisible if nothing selected
    ui->menuSnapshots->setVisible(false); //make sure this is always invisible if nothing selected
  }

}
