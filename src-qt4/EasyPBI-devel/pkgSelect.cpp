#include "pkgSelect.h"
#include "ui_pkgSelect.h"

pkgSelect::pkgSelect(QWidget *parent) : QDialog(parent), ui(new Ui::pkgSelect) {
  ui->setupUi(this); //load the pkgSelect.ui file
  timer = new QTimer();
	timer->setSingleShot(true);
	timer->setInterval(500); //0.5 seconds
	
  //setup the icons for the buttons
  ui->tool_search->setIcon(Backend::icon("search"));
  
  //connect the signals/slots
  connect(timer, SIGNAL(timeout()), this, SLOT(slotSearch()) );
  connect(ui->tool_search, SIGNAL(clicked()), this, SLOT(slotSearch()) );
  connect(ui->line_search, SIGNAL(returnPressed()), this, SLOT(slotSearch()) );
  connect(ui->line_search, SIGNAL(textChanged(QString)), timer, SLOT(start()) );
	
  //Now fill the GUI
  loadPackageList();
}

pkgSelect::~pkgSelect(){
	
}

void pkgSelect::loadPackageList(){
  QStringList PL = Backend::getPkgList(); //in <cat>/<pkg> format
  ui->treeWidget->clear();
  QTreeWidgetItem cCat;
  for(int i=0; i<PL.length(); i++){
    QString cat = PL[i].section("/",0,0,QString::SectionSkipEmpty);
    QString pkg = PL[i].section("/",-1);
    if(cat != cCat.text(0)){
      //Create a new parent item for this category
      cCat = new QTreeWidgetItem(QStringList() << cat);
      ui->treeWidget->addTopLevelItem(cCat);
    }
    //Add the item to the current category
    QTreeWidgetItem *item = new QTreeWidgetItem(QStringList() << pkg);
      item->setWhatsThis(PL[i]);
    cCat->addChild(item);
  }
}

void pkgSelect::slotSearch(){

}

void pkgSelect::slotAccept(){
  portSelected = "something";
  selected = true;
  this->close();
}

void pkgSelect::slotCancel(){
  selected = false;
  portSelected.clear();
  this->close();
}