#include "dialogpartition.h"
#include "ui_dialogpartition.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QGraphicsWidget>
#include <QGraphicsRectItem>
#include <QTransform>
#include <QPen>
#include <QBrush>


DialogPartition::DialogPartition(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPartition)
{
    ui->setupUi(this);
    device=NULL;
    dontupdate=0;

    ui->devLayout->header()->setStretchLastSection(false);
    ui->devLayout->header()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->devLayout->header()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    ui->devLayout->header()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    ui->devLayout->header()->setSectionResizeMode(3,QHeaderView::ResizeToContents);
    ui->devLayout->header()->setSectionResizeMode(4,QHeaderView::ResizeToContents);
    ui->devLayout->setIconSize(QSize(22,22));
    ui->devLayout->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->GraphicPart->setScene(new QGraphicsScene());

}

DialogPartition::~DialogPartition()
{
    delete ui;
}


void DialogPartition::setDevice(vdev_t * _device)
{
    QString tmp;
    device=_device;

    ui->devName->setText(device->Name);
    ui->devTotalSize->setText(printBytes(device->Size));


    /* This is to overcome an implementation limitation in Qt */
    /* The slider widget can only count up to 2^32 in a 32 bit system */
    /* So we will use coarser granularity to (larger sectors) to make it work */
    granularity=0;
    unsigned long long temp=device->SectorsCount;
    while(temp > 1ULL<<31) {
        granularity++;
        temp>>=1;
    }

    // SHOW ONLY PARTITIONS THAT ARE AVAILABLE DEPENDING ON TYPE OF TABLE

    ui->PartType->clear();
    if(device->PartType=="MBR") {
        // TODO: ADD MORE PARTITION TYPES FROM GPART SOURCE CODE
        ui->PartType->addItem("freebsd",QVariant(1));
        ui->PartType->addItem("appe-boot",QVariant(1));
        ui->PartType->addItem("ebr",QVariant(1));
        ui->PartType->addItem("fat16",QVariant(1));
        ui->PartType->addItem("fat32",QVariant(1));
        ui->PartType->addItem("ntfs",QVariant(1));
        ui->PartType->addItem("linux-data",QVariant(1));
        ui->PartType->addItem("linux-lvm",QVariant(1));
        ui->PartType->addItem("linux-raid",QVariant(1));
        ui->PartType->addItem("linux-swap",QVariant(1));
        ui->PartType->addItem("vmware-vmfs",QVariant(1));
        ui->PartType->addItem("vmware-vmkdiag",QVariant(1));



        ui->PartType->setCurrentIndex(0);
    }
    if(device->PartType=="GPT") {
        // TODO: ADD MORE PARTITION TYPES FROM GPART SOURCE CODE
        ui->PartType->addItem("freebsd-ufs",QVariant(1));
        ui->PartType->addItem("freebsd-zfs",QVariant(1));
        ui->PartType->addItem("freebsd-boot",QVariant(1));
        ui->PartType->addItem("freebsd-swap",QVariant(1));
        ui->PartType->addItem("freebsd-vinum",QVariant(1));
        ui->PartType->addItem("bios-boot",QVariant(1));
        ui->PartType->addItem("efi",QVariant(1));
        ui->PartType->addItem("apple-boot",QVariant(1));
        ui->PartType->addItem("apple-hfs",QVariant(1));
        ui->PartType->addItem("apple-label",QVariant(1));
        ui->PartType->addItem("apple-raid",QVariant(1));
        ui->PartType->addItem("apple-raid-offline",QVariant(1));
        ui->PartType->addItem("apple-tv-recovery",QVariant(1));
        ui->PartType->addItem("apple-ufs",QVariant(1));
        ui->PartType->addItem("linux-data",QVariant(1));
        ui->PartType->addItem("linux-lvm",QVariant(1));
        ui->PartType->addItem("linux-raid",QVariant(1));
        ui->PartType->addItem("linux-swap",QVariant(1));
        ui->PartType->addItem("ms-basic-data",QVariant(1));
        ui->PartType->addItem("ms-ldm-data",QVariant(1));
        ui->PartType->addItem("ms-ldm-metadata",QVariant(1));
        ui->PartType->addItem("vmware-vmfs",QVariant(1));
        ui->PartType->addItem("vmware-vmkdiag",QVariant(1));





        ui->PartType->setCurrentIndex(0);
    }
    if(device->PartType=="BSD") {
        // BSD PARTITION ONLY SUPPORTS FreeBSD types
        ui->PartType->addItem("freebsd-ufs",QVariant(1));
        ui->PartType->addItem("freebsd-zfs",QVariant(1));
        ui->PartType->addItem("freebsd-swap",QVariant(1));
        ui->PartType->setCurrentIndex(0);
    }



    QList<vdev_t>::const_iterator idx=device->Partitions.constBegin();
    unsigned long long sector=((device->Level==0)? device->SectorStart:0),maxsectorcount=0;
    int maxspaceidx=-1,numidx=0;
    QTreeWidgetItem *maxspaceitem=NULL;

    int coloridx=0;
    double scalex=(double)1000.0/(double)device->SectorsCount;



    ui->GraphicPart->scene()->clear();

    while(idx!=device->Partitions.constEnd())
    {
        if((*idx).SectorStart>sector) {
            QTreeWidgetItem *item=new QTreeWidgetItem(ui->devLayout);
            item->setIcon(0,QIcon(":/icons/task-complete.png"));
            item->setText(0,tr("** FREE **"));
            item->setText(1,tmp.sprintf("%llu",sector));
            item->setText(2,tmp.sprintf("%llu",(*idx).SectorStart-sector));
            item->setText(3,printBytes(((*idx).SectorStart-sector)*device->SectorSize));
            //item->setDisabled(false);
            item->setBackgroundColor(4,QColor::fromHsv(coloridx,0,192));

            ui->GraphicPart->scene()->addRect(sector*scalex,0,((*idx).SectorStart-sector)*scalex,100,QPen(QColor::fromHsv(coloridx,0,192)),QBrush(QColor::fromHsv(coloridx,0,192)));
            coloridx+=30;

            if((*idx).SectorStart-sector>maxsectorcount) { maxspaceitem=item; maxsectorcount=(*idx).SectorStart-sector; maxspaceidx=numidx; }
            sector=(*idx).SectorStart;
        }

        QTreeWidgetItem *item=new QTreeWidgetItem(ui->devLayout);
        item->setIcon(0,QIcon(":/icons/kdf.png"));
        item->setText(0,(*idx).Name + " [" + (*idx).PartType + "]");
        item->setText(1,tmp.sprintf("%llu",(*idx).SectorStart));
        item->setText(2,tmp.sprintf("%llu",(*idx).SectorsCount));
        item->setText(3,printBytes(((*idx).SectorsCount)*device->SectorSize));
        //item->setDisabled(true);
        item->setBackgroundColor(4,QColor::fromHsv(coloridx,128,255));
        sector=(*idx).SectorStart+(*idx).SectorsCount;

        ui->GraphicPart->scene()->addRect((*idx).SectorStart*scalex,0,(*idx).SectorsCount*scalex,100,QPen(QColor::fromHsv(coloridx,128,255)),QBrush(QColor::fromHsv(coloridx,128,255)));
        coloridx+=30;

        ++idx;
        ++numidx;



    }




    if( ((device->Level==0)? device->SectorStart:0)+device->SectorsCount>sector) {
        QTreeWidgetItem *item=new QTreeWidgetItem(ui->devLayout);
        item->setIcon(0,QIcon(":/icons/task-complete.png"));
        item->setText(0,tr("** FREE **"));
        item->setText(1,tmp.sprintf("%llu",sector));
        item->setText(2,tmp.sprintf("%llu",((device->Level==0)? device->SectorStart:0)+device->SectorsCount-sector));
        item->setText(3,printBytes((((device->Level==0)? device->SectorStart:0)+device->SectorsCount-sector)*device->SectorSize));
        //item->setDisabled(false);
        item->setBackgroundColor(4,QColor::fromHsv(coloridx,0,192));
        if(((device->Level==0)? device->SectorStart:0)+device->SectorsCount-sector>maxsectorcount) { maxspaceitem=item; maxsectorcount=((device->Level==0)? device->SectorStart:0)+device->SectorsCount-sector; maxspaceidx=numidx; }

        ui->GraphicPart->scene()->addRect(sector*scalex,0,(((device->Level==0)? device->SectorStart:0)+device->SectorsCount-sector)*scalex,100,QPen(QColor::fromHsv(coloridx,0,192)),QBrush(QColor::fromHsv(coloridx,0,192)));
        coloridx+=30;


    }

    // ALWAYS RECOMMEND 1 MB ALIGNMENT FOR ALL NEW PARTITIONS
    ui->partAlign->setChecked(true);

    if(!maxspaceitem) {
        // THIS DISK DOESN'T HAVE ANY FREE SPACE!
        ui->devLargestFree->setText(tr("No free space!"));
        ui->SizeSelect->setDisabled(true);
        ui->sizeSlider->setRange(0,0);
        ui->sizeSlider->setDisabled(true);
        ui->SizeText->setDisabled(true);
        ui->SizeText->clear();
        ui->PartType->clear();
        ui->PartType->setEnabled(false);
        ui->newfsCheck->setEnabled(false);
        ui->newfsType->setEnabled(false);
    }
    else {

    ui->devLayout->setCurrentItem(maxspaceitem);

    ui->devLargestFree->setText(printBytes(maxsectorcount*device->SectorSize));
    }

}


void DialogPartition::on_SizeSelect_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    ui->SizeText->setText(printBytes(((unsigned long long)ui->sizeSlider->sliderPosition())*device->SectorSize,ui->SizeSelect->currentIndex()));
}

void DialogPartition::on_sizeSlider_valueChanged(int value)
{
    Q_UNUSED(value);
    if(!dontupdate) {
    ui->SizeText->setStyleSheet("");
    ui->SizeText->setText(printBytes(( ((unsigned long long)ui->sizeSlider->sliderPosition())<<granularity)*device->SectorSize,ui->SizeSelect->currentIndex()));
    }
    dontupdate=0;

}


void DialogPartition::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);

    QSize sz=ui->GraphicPart->size();
    QTransform matrix;

    matrix.scale((qreal)(sz.width()-10)/1000.0,(qreal)(sz.height()-10)/100.0);
    ui->GraphicPart->setTransform(matrix);
}


void DialogPartition::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);

    QSize sz=ui->GraphicPart->size();
    QTransform matrix;

    matrix.scale((qreal)(sz.width()-10)/1000.0,(qreal)(sz.height()-10)/100.0);
    ui->GraphicPart->setTransform(matrix);

}

long long DialogPartition::getStartSector()
{
    if(ui->devLayout->currentItem()) {
        if(ui->devLayout->currentItem()->text(0)==tr("** FREE **")) {
        long long sector=ui->devLayout->currentItem()->text(1).toLongLong();
        // CORRECT THE START SECTOR IF PARTITIONS NEED TO BE ALIGNED
        if(ui->partAlign->checkState()) {
            sector=(sector+(1024*1024)/device->SectorSize-1)/((1024*1024)/device->SectorSize);
            sector*=(1024*1024)/device->SectorSize;
        }
        return sector;
        }

    }
    return -1;
}

unsigned long long DialogPartition::getSectorCount()
{
    long long sectorcount=((unsigned long long)ui->sizeSlider->sliderPosition())<<granularity;

    if(ui->partAlign->checkState()) {
        // ALIGN PARTITION TO 1 MB BOUNDARY
        sectorcount/=(1024*1024)/device->SectorSize;
        sectorcount*=(1024*1024)/device->SectorSize;
    }

    return sectorcount;
}

QString DialogPartition::getPartType()
{
    return ui->PartType->currentText();
}


void DialogPartition::on_devLayout_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);

    if(current->text(0)!=tr("** FREE **")) {
        ui->PartType->setEnabled(false);
        ui->newfsCheck->setEnabled(false);
        ui->newfsType->setEnabled(false);
        ui->SizeSelect->setEnabled(false);
        ui->SizeText->clear();
        ui->SizeText->setEnabled(false);
        ui->sizeSlider->setRange(0,0);
        ui->sizeSlider->setEnabled(false);
        return;
    }

    ui->PartType->setEnabled(true);
    ui->SizeSelect->setEnabled(true);
    ui->SizeSelect->setEnabled(true);
    ui->SizeSelect->setCurrentIndex(printUnits(current->text(2).toULongLong()*device->SectorSize));
    long long startsector=current->text(1).toULongLong();
    long long maxsectors=current->text(2).toULongLong();
    long long endsector=startsector+maxsectors;

    // CORRECT THE START SECTOR IF PARTITIONS NEED TO BE ALIGNED
    if(ui->partAlign->checkState()) {
        startsector=(startsector+(1024*1024)/device->SectorSize-1)/((1024*1024)/device->SectorSize);
        startsector*=(1024*1024)/device->SectorSize;
        endsector/=((1024*1024)/device->SectorSize);
        endsector*=((1024*1024)/device->SectorSize);
        maxsectors=endsector-startsector;
    }


    ui->sizeSlider->setRange(0,maxsectors>>granularity);
    ui->sizeSlider->setSliderPosition(maxsectors>>granularity);
    ui->sizeSlider->setEnabled(true);
    ui->SizeText->setText(printBytes(maxsectors*device->SectorSize,ui->SizeSelect->currentIndex()));
    ui->SizeText->setEnabled(true);
    on_PartType_currentIndexChanged(0);
    return;

}

void DialogPartition::on_PartType_currentIndexChanged(int index)
{
    Q_UNUSED(index);

    if((ui->PartType->currentText()=="freebsd-ufs")
      || (ui->PartType->currentText()=="freebsd-boot") ) {
        ui->newfsCheck->setEnabled(true);
        ui->newfsType->setEnabled(true);
        ui->newfsType->clear();
        ui->newfsType->addItem("ufs");
        return;
    }
    if(ui->PartType->currentText()=="ms-basic-data") {
        ui->newfsCheck->setEnabled(true);
        ui->newfsType->setEnabled(true);
        ui->newfsType->clear();
        ui->newfsType->addItem("fat16");
        ui->newfsType->addItem("fat32");
        ui->newfsType->addItem("ntfs");
        return;
    }
    if(ui->PartType->currentText()=="fat16") {
        ui->newfsCheck->setEnabled(true);
        ui->newfsType->setEnabled(true);
        ui->newfsType->clear();
        ui->newfsType->addItem("fat16");
        return;
    }
    if(ui->PartType->currentText()=="fat32") {
        ui->newfsCheck->setEnabled(true);
        ui->newfsType->setEnabled(true);
        ui->newfsType->clear();
        ui->newfsType->addItem("fat32");
        return;
    }
    if(ui->PartType->currentText()=="ntfs") {
        ui->newfsCheck->setEnabled(true);
        ui->newfsType->setEnabled(true);
        ui->newfsType->clear();
        ui->newfsType->addItem("ntfs");
        return;
    }


    if(ui->PartType->currentText()=="linux-data") {
        ui->newfsCheck->setEnabled(true);
        ui->newfsType->setEnabled(true);
        ui->newfsType->clear();
        ui->newfsType->addItem("ext2");
        ui->newfsType->addItem("ext3");
        ui->newfsType->addItem("ext4");
        return;
    }



    ui->newfsCheck->setEnabled(false);
    ui->newfsType->clear();
    ui->newfsType->setEnabled(false);
    ui->newfsCheck->setChecked(false);
}


bool DialogPartition::isnewfsChecked()
{
 return ui->newfsCheck->isChecked();
}

void DialogPartition::on_SizeText_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1);

    long long sizenumber;
    switch(ui->SizeSelect->currentIndex())
    {
    case 1:
        // kbytes
        sizenumber=1024LL;
        break;
    case 2:
        // Mbytes
        sizenumber=1024LL*1024LL;
        break;
    case 3:
        // Gbytes
        sizenumber=1024LL*1024LL*1024LL;
        break;
    case 4:
        // Tbytes
        sizenumber=1024LL*1024LL*1024LL*1024LL;
        break;
    default:
    case 0:
        // bytes
        sizenumber=1;
    }

    sizenumber=sizenumber*(ui->SizeText->text().toDouble()/(double)device->SectorSize);
    if((sizenumber>>granularity)>ui->sizeSlider->maximum()) {
        ui->SizeText->setStyleSheet("background-color: rgb(255, 188, 183);");
    } else {
        ui->SizeText->setStyleSheet("");
        dontupdate=1;
        ui->sizeSlider->setSliderPosition(sizenumber>>granularity);
    }
}


bool DialogPartition::needAlign()
{
    return ui->partAlign->checkState();
}

QString DialogPartition::getnewFSType()
{
    return ui->newfsType->currentText();
}
