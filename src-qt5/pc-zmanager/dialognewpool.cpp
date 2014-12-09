#include "dialognewpool.h"
#include "ui_dialognewpool.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTreeWidgetItemIterator>


DialogNewPool::DialogNewPool(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewPool)
{
    ui->setupUi(this);

    ui->vdevList->setIconSize(QSize(48,48));
    ui->vdevList->header()->setStretchLastSection(false);
    ui->vdevList->header()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->vdevList->header()->setSectionResizeMode(1,QHeaderView::ResizeToContents);

    ui->vdevList->clear();

    ui->striped->setEnabled(false);
    ui->mirror->setEnabled(false);
    ui->raidz->setEnabled(false);
    ui->raidz2->setEnabled(false);
    ui->raidz3->setEnabled(false);

    ui->cache->setEnabled(false);
    ui->cache->setVisible(false);
    ui->log->setEnabled(false);
    ui->log->setVisible(false);
    ui->spare->setEnabled(false);
    ui->spare->setVisible(false);

    ui->frameErrors->setVisible(false);

    Vdevcount=0;
    Fixednumdisks=0;
    Fixedtype=false;
    ui->striped->setChecked(true);

    ui->poolName->setFocus();

}

DialogNewPool::~DialogNewPool()
{
    delete ui;
}


// INITIALIZE THE DEVICE LIST WITH THE DISKS AVAILABLE FOR USE
void DialogNewPool::setDevices(QList<vdev_t> *disks)
{

    QList<vdev_t>::const_iterator it=disks->constBegin();


    while(it!=disks->constEnd()) {

        // DETERMINE IF THE DEVICE IS AVAILABLE
        if((*it).Partitions.count()!=0) {
            // DISK IS NOT AVAILABLE, CHECK PARTITIONS RECURSIVELY
            setDevices((QList<vdev_t> *)&((*it).Partitions));
        }
        else {
        if((*it).PartType=="freebsd-zfs" && (*it).InPool.isEmpty()) {
            // THIS IS AN EMPTY ZFS PARTITION
            QTreeWidgetItem *item=new QTreeWidgetItem(ui->vdevList);
            QString sz;
            sz=" ("+printBytes((*it).Size)+")";

            item->setText(0,(*it).Name+sz+"\n[freebsd-zfs]");
            item->setText(1," ");
            item->setData(0,Qt::UserRole,(*it).Name);
            item->setIcon(0,QIcon(":/icons/kdf.png"));
            item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            item->setCheckState(1,Qt::Unchecked);

        }
        else {
            if((*it).PartType.isEmpty()  && (*it).InPool.isEmpty() && (*it).Size!=0) {
            // THIS IS AN EMPTY DISK
            QTreeWidgetItem *item=new QTreeWidgetItem(ui->vdevList);

            QString sz;
            sz=" ("+printBytes((*it).Size)+")";
            if(!(*it).PartType.isEmpty()) { sz+=" [" + (*it).PartType + "]"; }
            sz+="\n";
            item->setText(0,(*it).Name + sz + (*it).Description);
            item->setText(1," ");
            item->setData(0,Qt::UserRole,(*it).Name);
            item->setIcon(0,QIcon(":/icons/drive-harddisk.png"));
            item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            item->setCheckState(1,Qt::Unchecked);

        }
        }
        }

        ++it;
    }


    Vdevcount=0;

}


void DialogNewPool::on_vdevList_itemClicked(QTreeWidgetItem *item, int column)
{
    if(column==0) {
    if(item->checkState(1)==Qt::Checked) {
        item->setCheckState(1,Qt::Unchecked);
    }
    else {
        item->setCheckState(1,Qt::Checked);
    }
    }

}

void DialogNewPool::on_vdevList_itemChanged(QTreeWidgetItem *item, int column)
{
    if(column==1) {
    if(item->checkState(1)==Qt::Checked) {
        ++Vdevcount;
    }
    else {
        --Vdevcount;
    }


    if(!Fixedtype) {


    if(Vdevcount>4) ui->raidz3->setEnabled(true);
    else {
        ui->raidz3->setEnabled(false);
        if(ui->raidz3->isChecked()) ui->raidz2->setChecked(true);
    }
    if(Vdevcount>3) ui->raidz2->setEnabled(true);
    else {
        ui->raidz2->setEnabled(false);
        if(ui->raidz2->isChecked()) ui->raidz->setChecked(true);
    }
    if(Vdevcount>2) ui->raidz->setEnabled(true);
    else {
        ui->raidz->setEnabled(false);
        if(ui->raidz->isChecked()) ui->mirror->setChecked(true);
    }


    if(Vdevcount>1) {
        ui->striped->setEnabled(true);
        ui->mirror->setEnabled(true);
    }
    else {
        ui->striped->setEnabled(false);
        ui->mirror->setEnabled(false);
        if(ui->mirror->isChecked()) ui->striped->setChecked(true);
    }

    }

    }
}


QStringList DialogNewPool::getVdevList()
{
    QStringList result;

    QTreeWidgetItemIterator it(ui->vdevList);

    while(*it)
    {
        if( (*it)->checkState(1)==Qt::Checked ) result.append((*it)->data(0,Qt::UserRole).toString());
        ++it;
    }

    return result;
}

QString DialogNewPool::getRaidType()
{
    if(ui->raidz3->isChecked()) return "raidz3";
    if(ui->raidz2->isChecked()) return "raidz2";
    if(ui->raidz->isChecked()) return "raidz";
    if(ui->mirror->isChecked()) return "mirror";
    if(ui->cache->isChecked()) return "cache";
    if(ui->log->isChecked()) return "log";
    if(ui->spare->isChecked()) return "spare";

    return "";

}

QString DialogNewPool::getName()
{
    return ui->poolName->text();
}


void DialogNewPool::setName(QString name)
{
    ui->poolName->setText(name);
    ui->poolName->setReadOnly(true);
}

void DialogNewPool::setTitle(QString name)
{

    this->setWindowTitle(name);
}




void DialogNewPool::setType(int disktype)
{

    switch(disktype)
    {
        case DISK_STRIPE:
            Fixedtype=true;
            ui->striped->setChecked(true);

            ui->striped->setEnabled(true);
            ui->mirror->setEnabled(false);
            ui->raidz->setEnabled(false);
            ui->raidz2->setEnabled(false);
            ui->raidz3->setEnabled(false);
            ui->cache->setEnabled(false);
            ui->log->setEnabled(false);
            ui->spare->setEnabled(false);
            ui->cache->setVisible(false);
            ui->log->setVisible(false);
            ui->spare->setVisible(false);
            break;
        case DISK_MIRROR:
            Fixedtype=true;
            ui->mirror->setChecked(true);

            ui->striped->setEnabled(false);
            ui->mirror->setEnabled(true);
            ui->raidz->setEnabled(false);
            ui->raidz2->setEnabled(false);
            ui->raidz3->setEnabled(false);
            ui->cache->setEnabled(false);
            ui->log->setEnabled(false);
            ui->spare->setEnabled(false);
            ui->cache->setVisible(false);
            ui->log->setVisible(false);
            ui->spare->setVisible(false);
            break;
        case DISK_RAIDZ:
            Fixedtype=true;
            ui->raidz->setChecked(true);

            ui->striped->setEnabled(false);
            ui->mirror->setEnabled(false);
            ui->raidz->setEnabled(true);
            ui->raidz2->setEnabled(false);
            ui->raidz3->setEnabled(false);
            ui->cache->setEnabled(false);
            ui->log->setEnabled(false);
            ui->spare->setEnabled(false);
            ui->cache->setVisible(false);
            ui->log->setVisible(false);
            ui->spare->setVisible(false);
            break;
        case DISK_RAIDZ2:
            Fixedtype=true;
            ui->raidz2->setChecked(true);

            ui->striped->setEnabled(false);
            ui->mirror->setEnabled(false);
            ui->raidz->setEnabled(false);
            ui->raidz2->setEnabled(true);
            ui->raidz3->setEnabled(false);
            ui->cache->setEnabled(false);
            ui->log->setEnabled(false);
            ui->spare->setEnabled(false);
            ui->cache->setVisible(false);
            ui->log->setVisible(false);
            ui->spare->setVisible(false);
            break;
        case DISK_RAIDZ3:
            Fixedtype=true;
            ui->raidz3->setChecked(true);

            ui->striped->setEnabled(false);
            ui->mirror->setEnabled(false);
            ui->raidz->setEnabled(false);
            ui->raidz2->setEnabled(false);
            ui->raidz3->setEnabled(true);
            ui->cache->setEnabled(false);
            ui->log->setEnabled(false);
            ui->spare->setEnabled(false);
            ui->cache->setVisible(false);
            ui->log->setVisible(false);
            ui->spare->setVisible(false);
            break;

        case DISK_CACHE:
            Fixedtype=true;
            ui->cache->setVisible(true);
            ui->log->setVisible(true);
            ui->spare->setVisible(true);

            ui->cache->setChecked(true);

            ui->striped->setEnabled(false);
            ui->mirror->setEnabled(false);
            ui->raidz->setEnabled(false);
            ui->raidz2->setEnabled(false);
            ui->raidz3->setEnabled(false);
            ui->cache->setEnabled(true);
            ui->log->setEnabled(false);
            ui->spare->setEnabled(false);
            break;
        case DISK_LOG:
            Fixedtype=true;
            ui->cache->setVisible(true);
            ui->log->setVisible(true);
            ui->spare->setVisible(true);

            ui->log->setChecked(true);

            ui->striped->setEnabled(false);
            ui->mirror->setEnabled(false);
            ui->raidz->setEnabled(false);
            ui->raidz2->setEnabled(false);
            ui->raidz3->setEnabled(false);
            ui->cache->setEnabled(false);
            ui->log->setEnabled(true);
            ui->spare->setEnabled(false);
            break;
        case DISK_SPARE:
            Fixedtype=true;
            ui->cache->setVisible(true);
            ui->log->setVisible(true);
            ui->spare->setVisible(true);

            ui->spare->setChecked(true);

            ui->striped->setEnabled(false);
            ui->mirror->setEnabled(false);
            ui->raidz->setEnabled(false);
            ui->raidz2->setEnabled(false);
            ui->raidz3->setEnabled(false);
            ui->cache->setEnabled(false);
            ui->log->setEnabled(false);
            ui->spare->setEnabled(true);
            break;
    }
}

void DialogNewPool::setNumDisks(int ndisks)
{
    Fixednumdisks=ndisks;
}

void DialogNewPool::done(int r)
{
    // VALIDATE DATA, SHOW ERRORS AND/OR EXIT

    if(r==QDialog::Accepted) {
        // CHECK FOR EMPTY POOL NAME
    if(ui->poolName->text().isEmpty()) {
        ui->labelError->setText(tr("Must provide a valid pool name"));
        ui->frameErrors->setVisible(true);
        return;
    }

        // CHECK FOR BAD DISK COUNT

        QStringList list=getVdevList();

        if(Fixednumdisks) {
            if(list.count()!=Fixednumdisks) {
            ui->labelError->setText(tr("Must select exactly %1 devices").arg(Fixednumdisks));
            ui->frameErrors->setVisible(true);
            return;
            }
        }
        else {
            if(list.count()==0) {
                ui->labelError->setText(tr("Must select at least one device"));
                ui->frameErrors->setVisible(true);
                return;
            }

        }


    }


    QDialog::done(r);
}
