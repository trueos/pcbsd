#include "PartitionSelect.h"
#include "ui_PartitionSelect.h"

#include <QDebug>
#include <QPoint>
#include <pcbsd-utils.h>

PartitionSelect::PartitionSelect(QWidget *parent) : QDialog(parent), ui(new Ui::PartitionSelect){
  ui->setupUi(this);
  if(parent!=0){
    //Center the window on the parent
    QPoint ctr = parent->geometry().center();
    this->move(ctr.x() - (this->width()/2), ctr.y()-(this->height()/2) );
  }
  //Initial output values
  selected = false;
  HD.clear(); PART.clear();
  //Load the disk information 
  GetDiskInfo();
  //Now load the available hard disks into the UI
  UpdateDisks();
  //Now load the available partitions on the current disk into the UI
  UpdateParts();
  //Now setup the signals/slots
  connect(ui->combo_hd, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateParts()) );
  connect(ui->list_part, SIGNAL(currentRowChanged(int)), this, SLOT(CheckValidity()) );
}

PartitionSelect::~PartitionSelect(){

}

void PartitionSelect::setupGui(QString name){
  ui->label_name->setText( ui->label_name->text().arg(name) );
}

void PartitionSelect::GetDiskInfo(){
  //Fill the DISKINFO hash with the gpart information
  QStringList info = pcbsd::Utils::runShellCommand("gpart list");
  //Do the consumers (disks) first
  bool insection = false;
  QString name, size, num;
  for(int i=info.indexOf("Consumers:"); i<info.length(); i++){
    if(i<0){ continue; } //just in case
    if(info[i]=="Consumers:"){ insection = true; }
    else if(info[i]=="Providers:" && insection){ insection = false; } //done with consumers
    else if(info[i].contains(". Name:") && insection){
      if(!num.isEmpty()){
        //Save the current entry into the database
	DEVINFO.insert(name+" ("+size+")::::"+num, QStringList() ); //empty list at the moment
      }
      //Now clear the saved variables and load the new one
      size.clear(); 
      name = info[i].section("Name:",1,1).simplified();
      num = name.right(1); //ada0 -> 0
    }else if(!num.isEmpty() && insection){
	if(info[i].contains("Mediasize:")){ size = info[i].section("(",1,1).section(")",0,0); }
    }
  }
  //Final save of current HD  info
  if(!num.isEmpty()){
    //Save the current entry into the database
    DEVINFO.insert(name+" ("+size+")::::"+num, QStringList() ); //empty list at the moment
  }
  
  //Now do the providers (partitions)
  insection = false;
  name.clear(); size.clear(); num.clear(); //re-use variables from before
  QString type, disk; //also need this new info
  QStringList disks = DEVINFO.keys();
  for(int i=info.indexOf("Providers:"); i<info.length(); i++){
    if(i<0){ continue; } //just in case
    //qDebug() << "Part Line:" << info[i];
    //qDebug() << " - " << insection << name << size << num << type << disk;
    if(info[i]=="Providers:"){ insection = true; }
    else if(info[i]=="Consumers:" && insection){insection = false; } //done with consumers
    else if(info[i].contains(". Name:") && insection){
      if(!num.isEmpty() && !disk.isEmpty()){
	//qDebug() << "Found Part Info:" << disk << name << type << size << num;
        //Save the current entry into the database
	DEVINFO[disk].append(name+" ("+type+", "+size+")::::"+num);
      }
      //Now clear the saved variables and load the new one
      size.clear(); num.clear(); disk.clear(); type.clear();
      name = info[i].section("Name:",1,1).simplified();
      QString shortname;
      if(name.contains("s")){ num = name.section("s",1,1); shortname = name.section("s",0,0); } //ada0s1 -> 1
      else if(name.contains("p")){ num = name.section("p",1,1); shortname = name.section("p",0,0);} //ada0p1 -> 1
      //Also fetch the corresponding disk index in the hash
      //qDebug() << "Disk Find:" << disks << shortname;
      if(!disks.filter(shortname+" (").isEmpty()){
	disk = disks.filter(shortname+" (").first();
      }
    }else if(!num.isEmpty() && insection){
	if(info[i].contains("Mediasize:")){ size = info[i].section("(",1,1).section(")",0,0); }
	else if(info[i].contains("type:")){ type = info[i].section("type:",1,1).simplified(); }
    }
  }
  //Final save of current partition  info
  if(!num.isEmpty() && !disk.isEmpty()){
    //Save the current entry into the database
    //qDebug() << "Found Part Info:" << disk << name << type << size << num;
    DEVINFO[disk].append(name+" ("+type+", "+size+")::::"+num);
  }
}

void PartitionSelect::on_push_cancel_clicked(){
  selected = false;
  this->close();
}

void PartitionSelect::on_push_ok_clicked(){
  selected = true;
  HD = ui->combo_hd->currentData().toString().section("::::",1,1);
  PART = ui->list_part->currentItem()->whatsThis();
  if(HD.isEmpty()){ HD = "0"; }
  if(PART.isEmpty()){ PART = "1"; }
  this->close();
}


void PartitionSelect::UpdateDisks(){
  QStringList disks = DEVINFO.keys();
  //qDebug() << "Disks:" << disks;
  ui->combo_hd->clear();
  for(int i=0; i<disks.length(); i++){
    ui->combo_hd->addItem(disks[i].section("::::",0,0), disks[i]);
  }
}

void PartitionSelect::UpdateParts(){
  ui->list_part->clear();
  QStringList parts = DEVINFO.value(ui->combo_hd->currentData().toString());
  //qDebug() << "Parts:" << parts;
  for(int i=0; i<parts.length(); i++){
    QListWidgetItem *it = new QListWidgetItem(parts[i].section("::::",0,0));
      it->setWhatsThis(parts[i].section("::::",1,1) );
    ui->list_part->addItem(it);
  }
}

void PartitionSelect::CheckValidity(){
  ui->push_ok->setEnabled( ui->list_part->currentItem()!=0 && !ui->combo_hd->currentData().toString().isEmpty());
}