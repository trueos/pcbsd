#include "pkgSelect.h"
#include "ui_pkgSelect.h"

pkgSelect::pkgSelect(QWidget *parent, bool single) : QDialog(parent), ui(new Ui::pkgSelect) {
  ui->setupUi(this); //load the pkgSelect.ui file
  selected = false;
  singleSelection = single;
  if(single){ ui->treeWidget->setSelectionMode(QAbstractItemView::SingleSelection); }
  else{ ui->treeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection); }
  portSelected.clear();
  timer = new QTimer();
	timer->setSingleShot(true);
	timer->setInterval(500); //0.5 seconds
	
  //connect the signals/slots
  connect(timer, SIGNAL(timeout()), this, SLOT(slotSearch()) );
  connect(ui->tool_search, SIGNAL(clicked()), this, SLOT(slotSearch()) );
  connect(ui->line_search, SIGNAL(returnPressed()), this, SLOT(slotSearch()) );
  connect(ui->line_search, SIGNAL(textChanged(QString)), timer, SLOT(start()) );
  connect(ui->treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(slotCheckPkg()) );
  connect(ui->push_ok, SIGNAL(clicked()), this, SLOT(slotAccept()) );
  connect(ui->push_cancel, SIGNAL(clicked()), this, SLOT(slotCancel()) );
	
  //Now fill the GUI
  loadPackageList();
  //Now make sure the ok button has focus
  //QTimer::singleShot(0, ui->line_search, SLOT(setFocus()));
}

pkgSelect::~pkgSelect(){
	
}

void pkgSelect::loadPackageList(){
  QStringList PL = Backend::getPkgList(); //in <cat>/<pkg> format
  ui->treeWidget->clear();
  QTreeWidgetItem* cCat = new QTreeWidgetItem(QStringList() << "gobbledegook");
  for(int i=0; i<PL.length(); i++){
    QString cat = PL[i].section("/",0,0);
    QString pkg = PL[i].section("/",-1);
    if(cat.isEmpty()){ cat = "uncategorized";  }
    if(cat != cCat->text(0)){
      //Create a new parent item for this category
      cCat = new QTreeWidgetItem(QStringList() << cat);
      ui->treeWidget->addTopLevelItem(cCat);
    }
    //Add the item to the current category
    QTreeWidgetItem *item = new QTreeWidgetItem(QStringList() << pkg);
      item->setWhatsThis(0,PL[i]);
    cCat->addChild(item);
  }
  if(PL.length() < 1){
    QTreeWidgetItem *item = new QTreeWidgetItem(QStringList() << tr("No Packages Available"));
    ui->treeWidget->addTopLevelItem(item);
    ui->line_search->setEnabled(false);
    ui->tool_search->setEnabled(false);
  }
  ui->push_ok->setEnabled(false);
}

void pkgSelect::slotCheckPkg(){
  bool ok = !ui->treeWidget->currentItem()->whatsThis(0).isEmpty();
  ui->push_ok->setEnabled(ok);
}

void pkgSelect::slotSearch(){
  if(ui->line_search->text().isEmpty()){ return; }
  if(timer->isActive()){ timer->stop(); } //return pressed instead of the auto-timer
  //Get the currently selected item
  QTreeWidgetItem *CI = ui->treeWidget->currentItem();
  if( (CI==0) && (ui->treeWidget->topLevelItemCount() > 0) ){
    CI = ui->treeWidget->topLevelItem(0); //get the first item available	  
  }
  bool found = performSearch(ui->line_search->text(), ui->treeWidget, CI);
  if( !found ){
    //Start over at the top
    CI = ui->treeWidget->topLevelItem(0);
    found = performSearch(ui->line_search->text(), ui->treeWidget, CI);
  }
  if(!found){
    QMessageBox::information(this,tr("Search Finished"), tr("No package found with that term"));	  
  }
}

void pkgSelect::slotAccept(){

  if(singleSelection){
    portSelected = ui->treeWidget->currentItem()->whatsThis(0);
  }else{
    QList<QTreeWidgetItem*> selList = ui->treeWidget->selectedItems();
    portsSelected.clear();
    for(int i=0; i<selList.length(); i++){
      QString port = selList[i]->whatsThis(0);
      if( !port.isEmpty() ){ portsSelected << port;}
    }
  }
  selected = true;
  this->close();
}

void pkgSelect::slotCancel(){
  selected = false;
  portSelected.clear();
  portsSelected.clear();
  this->close();
}

// ========================
bool pkgSelect::performSearch(QString pkgSearch, QTreeWidget *TW, QTreeWidgetItem *SI){
  //Start Iterating over the tree
  bool found=false;
  bool started = false;
  //if(SI==0){ started = true; }
  for(int p=0; (p<TW->topLevelItemCount()) && !found; p++){
    //Check the actual item itself
    QTreeWidgetItem *CI = TW->topLevelItem(p);
    if(started && CI->text(0).contains(pkgSearch, Qt::CaseInsensitive)){
      TW->setCurrentItem(CI);
      TW->scrollToItem(CI);
      found=true;	  
    }else{    
      found = searchChildren(pkgSearch, TW, CI, started, SI);
    }
  }
  return found;
}

bool pkgSelect::searchChildren(QString srch, QTreeWidget *TW, QTreeWidgetItem *CI, bool &started, QTreeWidgetItem *SI){
  //This is a recursive function for searching all the children of a particular item
  // TW - TreeWidget pointer
  // CI - Current TreeWidget Item (to search the children of)
  // SI - Start Item (Try to start searching right after this item - optional)
  // bool started - Start Item found and search has been started (optional input/output)
	
  //qDebug() << "Search Children of:" << CI->text(0) << srch << started;
  //Check for the start position
  int start = -1;
  if(SI == 0 || SI == CI){
    //No search item to start at
    start = 0;
    started = true;
  }else if( !started){
    QTreeWidgetItem *PI = SI;
      while( (start == -1) && (PI!=0) ){
        start = CI->indexOfChild(PI);
        PI = PI->parent(); //look up one more layer to make sure it is not a child of one of these items
      }
  }else{ start = 0; } //start with the first child
  //Now quit if the start item is not found here
  if(start == -1){ started = false; return false; }
  
  //Now start searching
  bool found = false;
  for(int i=start; (i<CI->childCount()) && !found ; i++){
    if(started){
      //Check this item 
      if(CI->child(i)->text(0).contains(srch, Qt::CaseInsensitive)){
        TW->setCurrentItem(CI->child(i));
	TW->scrollToItem(CI->child(i));
	found=true;
	break;
      }
    }else if( SI == CI->child(i) ){
      started = true; //but don't look at this item, continue on to the next one (or children)
    }
    if(found){ break; }
    else if(CI->child(i)->childCount() > 0){
      //recursively search this items children
      found = searchChildren(srch, TW, CI->child(i), started, SI); 
    }
  }
  return found;
}
