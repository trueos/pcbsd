#include <QTimer>
#include "zmanagerwindow.h"
#include "ui_zmanagerwindow.h"
#include "dialogpartition.h"
#include "dialogmount.h"
#include "dialognewpool.h"
#include "dialogname.h"
#include "dialogprop.h"
#include "dialogfsprop.h"
#include "dialogfscreate.h"
#include <pcbsd-utils.h>
#include <QDebug>
#include <QIcon>
#include <QString>
#include <QStringList>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QPushButton>
#include <QTreeWidgetItemIterator>
#include <QToolButton>

ZManagerWindow::ZManagerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ZManagerWindow)
{

    lastSelectedPool=NULL;
    lastSelectedVdev=NULL;
    lastSelectedFileSystem=NULL;

    ui->setupUi(this);

    ui->zpoolList->setIconSize(QSize(48,48));
    ui->zpoolList->header()->setStretchLastSection(true);
    ui->zpoolList->header()->setSectionResizeMode(QHeaderView::Interactive);
    ui->zpoolList->header()->resizeSection(0,ui->deviceList->width()*2/3);

    ui->deviceList->setIconSize(QSize(48,48));
    ui->deviceList->header()->setStretchLastSection(true);
    ui->deviceList->header()->setSectionResizeMode(QHeaderView::Interactive);
    ui->deviceList->header()->resizeSection(0,ui->deviceList->width()*2/3);


    connect(ui->zpoolList,SIGNAL(customContextMenuRequested(QPoint)),SLOT(zpoolContextMenu(QPoint)));
    connect(ui->deviceList,SIGNAL(customContextMenuRequested(QPoint)),SLOT(deviceContextMenu(QPoint)));
    connect(ui->fsList,SIGNAL(customContextMenuRequested(QPoint)),SLOT(filesystemContextMenu(QPoint)));

    ui->frameStatus->setVisible(false);


    ui->fspoolList->setIconSize(QSize(48,48));
    ui->tabZFS->layout()->setAlignment(ui->dropDownButton,Qt::AlignTop);

    ui->fsList->setIconSize(QSize(48,48));
    ui->fsList->header()->setStretchLastSection(true);
    ui->fsList->header()->setSectionResizeMode(QHeaderView::Interactive);
    ui->fsList->header()->resizeSection(0,ui->deviceList->width()*2/3);


}

ZManagerWindow::~ZManagerWindow()
{
    delete ui;
}


void ZManagerWindow::ProgramInit()
{
    // Perform initialization
    QTimer::singleShot(300,this,SLOT(refreshState()));
}

void ZManagerWindow::slotSingleInstance()
{
    activateWindow();
}


// THIS IS THE MAIN FUNCTION THAT GATHERS ALL INFORMATION FROM THE BACKEND

void ZManagerWindow::GetCurrentTopology()
{
    // RUN ALL REQUIRED PROCESSES AND GET THE RESULTS

    QStringList a=pcbsd::Utils::runShellCommand("zpool status");    // GET ALL ACTIVE POOLS
    QStringList i=pcbsd::Utils::runShellCommand("zpool import");    // GET ALL EXPORTED POOLS AVAILABLE
    QStringList d=pcbsd::Utils::runShellCommand("zpool import -D");    // GET ALL DESTROYED POOLS READY TO RECOVER
    QStringList g=pcbsd::Utils::runShellCommand("geom disk list");
    QStringList h=pcbsd::Utils::runShellCommand("gpart list");
    QStringList h2=pcbsd::Utils::runShellCommand("gpart show -p");
    QStringList lbl=pcbsd::Utils::runShellCommand("glabel status");
    QStringList fsid=pcbsd::Utils::runShellCommand("sh -c \"file -s /dev/da* /dev/ada*\"");
    QStringList m=pcbsd::Utils::runShellCommand("mount");
    QStringList ps=pcbsd::Utils::runShellCommand("sh -c \"ps -A -w -w | grep 'ntfs\\|ext4'\"");
    QStringList prop;   // GET PROPERTIES FOR ALL POOLS ONCE WE HAVE A LIST OF POOLS
    QStringList zfsl=pcbsd::Utils::runShellCommand("zfs list -H -t all");
    QStringList zfspr=pcbsd::Utils::runShellCommand("zfs get -H all");


    // CLEAR ALL EXISTING TOPOLOGY
    this->Pools.clear();
    this->Disks.clear();
    this->Errors.clear();
    this->FileSystems.clear();

    QStringList::const_iterator idx;
    int state;



    // BEGIN PROCESSING DISKS FROM GEOM

    idx=g.constBegin();
    vdev_t dsk,part;
    state=0;

    while(idx!=g.constEnd()) {

        QString str=*idx;


        if(str.startsWith("Geom name: ")) {

            if(state) {
                this->Disks.append(dsk);
                state=0;
            }
            dsk.Name=str.remove(0,11);
            dsk.SectorSize=0;
            dsk.SectorStart=0;
            dsk.SectorsCount=0;
            dsk.Description.clear();
            dsk.Level=0;
            dsk.Status=0;
            dsk.InPool.clear();
            dsk.Index=0;
            dsk.Size=0LL;
            dsk.PartType.clear();
            dsk.FSType.clear();
            dsk.Partitions.clear();
            ++state;
        }

        if(str.startsWith("   Mediasize: ") && (state==1)) {

            QStringList l=str.remove(0,14).split(" ");
            QStringList::const_iterator lit=l.constBegin();
            while( (*lit).isEmpty() && lit!=l.constEnd()) ++lit;
            if(lit!=l.constEnd()) dsk.Size=(*lit).toULongLong();
            ++state;
        }

        if(str.startsWith("   Sectorsize: ") && (state==2)) {

            dsk.SectorSize=str.remove(0,15).toInt();
            ++state;
        }


        if(str.startsWith("   descr: ") && (state==3)) {

            dsk.Description=str.remove(0,10);
            ++state;
        }


        ++idx;
    }

    if(state==4) {
        this->Disks.append(dsk);
    }




    // BEGIN PROCESSING PARTITIONS FROM GPART

    idx=h.constBegin();
    state=0;
    QString ParentGeom;

    while(idx!=h.constEnd()) {

        QString str=*idx;


        if(str.startsWith("Geom name: ")) {

            if(state) {
                // APPEND SLICES AND PARTITIONS
                QList<vdev_t>::iterator dskidx=this->Disks.begin();

                while(dskidx!=this->Disks.end()) {
                    if((*dskidx).Name==dsk.Name) {
                        // ADD SLICES
                        (*dskidx).PartType=dsk.PartType;
                        (*dskidx).Partitions=dsk.Partitions;

                        break;
                    }
                    if(dsk.Name.startsWith((*dskidx).Name)) {
                        // THE GEOM IS ACTUALLY A SLICE
                        QList<vdev_t>::iterator partidx=(*dskidx).Partitions.begin();

                        while(partidx!=(*dskidx).Partitions.end()) {
                            if((*partidx).Name==dsk.Name) {
                                // ADD PARTITIONS
                                (*partidx).PartType=dsk.PartType;
                                (*partidx).Partitions=dsk.Partitions;

                                break;
                            }

                            ++partidx;

                        }
                    }

                    ++dskidx;

                }
                if(dskidx==this->Disks.constEnd()) {
                    // THIS SHOULDN'T HAPPEN, DISK HAD TO BE IN THE GEOM LIST!!!
                    // SIMPLY DISCARD


                }

                state=0;
            }


            // DETERMINE IF THE GEOM IS A DISK OR A SLICE
            QList<vdev_t>::iterator dskidx=this->Disks.begin();



            dsk.Name=str.remove(0,11);
            dsk.Level=0;
            while(dskidx!=this->Disks.end()) {
                if((*dskidx).Name==dsk.Name) {
                    // GEOM IS A DISK
                    break;
                }
                if(dsk.Name.startsWith((*dskidx).Name)) {
                    // GEOM IS A SLICE
                    dsk.Level=1;
                }
                ++dskidx;
            }

            dsk.Description.clear();
            dsk.PartType.clear();
            dsk.Partitions.clear();
            dsk.SectorSize=0;
            dsk.SectorStart=0;
            dsk.SectorsCount=0;
            dsk.Index=0;
            dsk.Status=0;
            dsk.Size=0LL;
            ++state;
        }

        if(str.startsWith("scheme: ") && (state==1)) {

            dsk.PartType=str.remove(0,8);
            ++state;
        }


        if(str.startsWith("Providers:") &&(state==2)) {
            ++state;
        }

        if(str.contains(". Name: ") && (state>=3)) {
            if(state>3) {
                // ADD PREVIOUS PARTITION TO MAIN GEOM
                // WE MUST INSERT IN THE LIST, SORTED BY SECTOR START
               QList<vdev_t>::iterator partit=dsk.Partitions.begin();

                while(partit!=dsk.Partitions.end())
                {
                    if( (*partit).SectorStart>part.SectorStart) break;
                    ++partit;
                }

                if(partit==dsk.Partitions.end()) dsk.Partitions.append(part);
                else dsk.Partitions.insert(partit,part);
                state=3;
            }
            part.Name=str.remove(0,9);
            part.Description.clear();
            part.Partitions.clear();
            part.PartType.clear();
            part.Level=dsk.Level+1;
            part.SectorSize=0;
            part.SectorStart=0;
            part.SectorsCount=0;
            part.Status=0;
            part.InPool.clear();
            part.Index=0;
            part.Size=0LL;
            ++state;
        }

        if(str.startsWith("   Mediasize: ") &&(state==4)) {
            QStringList l=str.remove(0,14).split(" ");
            QStringList::const_iterator lit=l.constBegin();
            while( (*lit).isEmpty() && lit!=l.constEnd()) ++lit;
            if(lit!=l.constEnd()) part.Size=(*lit).toULongLong();
            ++state;
        }

        if(str.startsWith("   Sectorsize: ") && (state==5)) {

            part.SectorSize=str.remove(0,15).toInt();
            ++state;
        }


        if(str.startsWith("   type: ") &&(state==6)) {
            part.PartType=str.remove(0,9);
            ++state;
        }

        if(str.startsWith("   index: ") && (state==7)) {

            part.Index=str.remove(0,10).toInt();
            ++state;
        }

        if(str.startsWith("   end: ") &&(state==8)) {
            part.SectorsCount=str.remove(0,8).toULongLong();
            ++state;
        }

        if(str.startsWith("   start: ") &&(state==9)) {
            part.SectorStart=str.remove(0,10).toULongLong();
            part.SectorsCount-=(part.SectorStart-1);
            ++state;
        }

        if(str.startsWith("Consumers:") &&(state==10)) {
            // WE MUST INSERT IN THE LIST, SORTED BY SECTOR START
           QList<vdev_t>::iterator partit=dsk.Partitions.begin();

            while(partit!=dsk.Partitions.end())
            {
                if( (*partit).SectorStart>part.SectorStart) break;
                ++partit;
            }

            if(partit==dsk.Partitions.end()) dsk.Partitions.append(part);
            else dsk.Partitions.insert(partit,part);
            state=-1;
        }


        ++idx;
    }

    if(state==3 || state==-1) {
            // APPEND SLICES AND PARTITIONS
            QList<vdev_t>::iterator dskidx=this->Disks.begin();

            while(dskidx!=this->Disks.end()) {
                if((*dskidx).Name==dsk.Name) {
                    // ADD SLICES
                    (*dskidx).PartType=dsk.PartType;
                    (*dskidx).Partitions=dsk.Partitions;

                    break;
                }
                if(dsk.Name.startsWith((*dskidx).Name)) {
                    // THE GEOM IS ACTUALLY A SLICE
                    QList<vdev_t>::iterator partidx=(*dskidx).Partitions.begin();
                    while(partidx!=(*dskidx).Partitions.end()) {
                        if((*partidx).Name==dsk.Name) {
                            // ADD PARTITIONS
                            (*partidx).PartType=dsk.PartType;
                            (*partidx).Partitions=dsk.Partitions;

                            break;
                        }

                        ++partidx;

                    }
                }

                ++dskidx;

            }
            if(dskidx==this->Disks.constEnd()) {
                // THIS SHOULDN'T HAPPEN, DISK HAD TO BE IN THE GEOM LIST!!!
                // SIMPLY DISCARD


            }

            state=0;
        }

    // GPART SHOW IS ONLY NEEDED BECAUSE GPART LIST
    // DOESN'T SHOW IF A DISK HAS A PARTITION TABLE
    // BUT NO PARTITIONS IN IT
    // IN ANY CASE, WE READ THE SECTORS AGAIN FOR EACH PARTITION

    vdev_t *exist_disk;

    bool mainitem;
    idx=h2.constBegin();

    while(idx!=h2.constEnd()) {
        QStringList tokens=(*idx).split(" ", QString::SkipEmptyParts);

        if(tokens.count()>=4)
        {

            if(tokens.at(0)=="=>") { mainitem=true; tokens.removeAt(0); }
            else mainitem=false;

            exist_disk=getDiskbyName(tokens.at(2));
                if(exist_disk!=NULL) {
                    if(!mainitem || !exist_disk->SectorStart) exist_disk->SectorStart=tokens.at(0).toULongLong();
                    exist_disk->SectorsCount=tokens.at(1).toULongLong();
                    exist_disk->PartType=tokens.at(3);
                }

        }


        ++idx;
    }








    // GET LABELS FROM GLABEL

    idx=lbl.constBegin();

    while(idx!=lbl.constEnd()) {
        QStringList tokens=(*idx).split(" ", QString::SkipEmptyParts);

        if(tokens.count()>=2) {
        QList<vdev_t>::iterator dskit=this->Disks.begin();

        while(dskit!=this->Disks.end())
        {
            QList<vdev_t>::iterator sliceit=(*dskit).Partitions.begin();
            while(sliceit!=(*dskit).Partitions.end()) {
                QList<vdev_t>::Iterator partit=(*sliceit).Partitions.begin();
                while(partit!=(*sliceit).Partitions.end()) {
                    if(tokens.at(2)==(*partit).Name) { (*partit).Alias=tokens.at(0); break; }
                    ++partit;
                }
                    if(partit!=(*sliceit).Partitions.end()) break;
                    if(tokens.at(2)==(*sliceit).Name) { (*sliceit).Alias=tokens.at(0); break; }
                    ++sliceit;
            }

            if(sliceit!=(*dskit).Partitions.end()) break;

            if(tokens.at(2)==(*dskit).Name) { (*dskit).Alias=tokens.at(0); break; }

            ++dskit;
        }
        }

        ++idx;
    }

    // GET MOUNT LOCATIONS FROM MOUNT

    idx=m.constBegin();

    while(idx!=m.constEnd()) {
        QString MountString=(*idx);
        QStringList tokens=(*idx).split(" ",QString::SkipEmptyParts);
        int startpos=MountString.indexOf(" "+tokens.at(2)+" ");
        int endpos=MountString.lastIndexOf(" (");

        MountString=MountString.mid(startpos+1,endpos-startpos);

        if(tokens.count()<3) { ++ idx; continue; }
        // FIND DISK OR PARTITION WITH GIVEN NAME


        QList<vdev_t>::iterator dskit=this->Disks.begin();

        while(dskit!=this->Disks.end())
        {
            QList<vdev_t>::iterator sliceit=(*dskit).Partitions.begin();
            while(sliceit!=(*dskit).Partitions.end()) {
                QList<vdev_t>::Iterator partit=(*sliceit).Partitions.begin();
                while(partit!=(*sliceit).Partitions.end()) {
                    if(tokens.at(0)=="/dev/"+(*partit).Name) { (*partit).MountPoint=MountString; break; }
                    if(!(*partit).Alias.isEmpty()) {
                        if(tokens.at(0)=="/dev/"+(*partit).Alias) { (*partit).MountPoint=MountString; break; }
                    }
                    ++partit;
                }
                    if(partit!=(*sliceit).Partitions.end()) break;
                    if(tokens.at(0)=="/dev/"+(*sliceit).Name) { (*sliceit).MountPoint=MountString; break; }
                    if(!(*sliceit).Alias.isEmpty()) {
                        if(tokens.at(0)=="/dev/"+(*sliceit).Alias) { (*sliceit).MountPoint=MountString; break; }
                    }
                    ++sliceit;
            }

            if(sliceit!=(*dskit).Partitions.end()) break;

            if(tokens.at(0)=="/dev/"+(*dskit).Name) { (*dskit).MountPoint=MountString; break; }
            if(!(*dskit).Alias.isEmpty()) {
                if(tokens.at(0)=="/dev/"+(*dskit).Alias) { (*dskit).MountPoint=MountString; break; }
            }

            ++dskit;
        }




        ++idx;
    }


    // GET FILE SYSTEM TYPES FROM FILES


    idx=fsid.constBegin();

    while(idx!=fsid.constEnd()) {
        QString FileSystemID;
        int f;
        QStringList tokens=(*idx).split(" ",QString::SkipEmptyParts);

        if(tokens.count()<2) { ++ idx; continue; }
        // FIND DISK OR PARTITION WITH GIVEN NAME

        FileSystemID.clear();

        for(f=1;f<tokens.count();++f) {
            if(tokens.at(f).startsWith("FAT")) {
                if(f+1<tokens.count()) {
                    if(tokens.at(f+1).startsWith("(16")) { FileSystemID="fat16"; break; }
                    if(tokens.at(f+1).startsWith("(32")) {FileSystemID="fat32"; break; }
                }
            }
            if(tokens.at(f).startsWith("ext2")) {
                if(tokens.at(f+1).startsWith("filesystem"))
                { FileSystemID="ext2"; break; }
            }
            if(tokens.at(f).startsWith("ext3")) {
                if(tokens.at(f+1).startsWith("filesystem"))
                { FileSystemID="ext3"; break; }
            }
            if(tokens.at(f).startsWith("ext4")) {
                if(tokens.at(f+1).startsWith("filesystem"))
                { FileSystemID="ext4"; break; }
            }

            if(tokens.at(f).startsWith("NTFS")) {
                FileSystemID="ntfs";
                break;
            }

            if(tokens.at(f).startsWith("Unix")) {
                if(f+4<tokens.count()) {
                    if(!tokens.at(f+1).startsWith("Fast")) break;
                    if(!tokens.at(f+2).startsWith("File")) break;
                    FileSystemID="ufs";
                    break;

                }
            }



        }


        QList<vdev_t>::iterator dskit=this->Disks.begin();

        while(dskit!=this->Disks.end())
        {
            QList<vdev_t>::iterator sliceit=(*dskit).Partitions.begin();
            while(sliceit!=(*dskit).Partitions.end()) {
                QList<vdev_t>::Iterator partit=(*sliceit).Partitions.begin();
                while(partit!=(*sliceit).Partitions.end()) {
                    if(tokens.at(0)=="/dev/"+(*partit).Name+":") {

                        (*partit).FSType=FileSystemID;

                        // CHECK IF PARTITION TYPE MAKES SENSE WITH FILE SYSTEM
                        if((FileSystemID=="fat16")||(FileSystemID=="fat32")||(FileSystemID=="ntfs")) {
                            if(((*partit).PartType!=FileSystemID) && (!(*partit).PartType.startsWith("ms-"))) (*partit).FSType.clear();
                        }
                        if(FileSystemID.startsWith("ext")) {
                            if(!(*partit).PartType.startsWith("linux-")) (*partit).FSType.clear();
                        }
                        if(FileSystemID.startsWith("ufs")) {
                            if(!(*partit).PartType.startsWith("freebsd-ufs")) (*partit).FSType.clear();
                        }


                        break;

                    }
                    if(!(*partit).Alias.isEmpty()) {
                        if(tokens.at(0)=="/dev/"+(*partit).Alias+":") {
                            (*partit).FSType=FileSystemID;


                            // CHECK IF PARTITION TYPE MAKES SENSE WITH FILE SYSTEM
                            if((FileSystemID=="fat16")||(FileSystemID=="fat32")||(FileSystemID=="ntfs")) {
                                if(((*partit).PartType!=FileSystemID) && (!(*partit).PartType.startsWith("ms-"))) (*partit).FSType.clear();
                            }
                            if(FileSystemID.startsWith("ext")) {
                                if(!(*partit).PartType.startsWith("linux-")) (*partit).FSType.clear();
                            }
                            if(FileSystemID.startsWith("ufs")) {
                                if(!(*partit).PartType.startsWith("freebsd-ufs")) (*partit).FSType.clear();
                            }





                            break;
                        }
                    }
                    ++partit;
                }
                    if(partit!=(*sliceit).Partitions.end()) break;
                    if(tokens.at(0)=="/dev/"+(*sliceit).Name+":") {
                        (*sliceit).FSType=FileSystemID;


                        // CHECK IF PARTITION TYPE MAKES SENSE WITH FILE SYSTEM
                        if((FileSystemID=="fat16")||(FileSystemID=="fat32")||(FileSystemID=="ntfs")) {
                            if(((*sliceit).PartType!=FileSystemID) && (!(*sliceit).PartType.startsWith("ms-"))) (*sliceit).FSType.clear();
                        }
                        if(FileSystemID.startsWith("ext")) {
                            if(!(*sliceit).PartType.startsWith("linux-")) (*sliceit).FSType.clear();
                        }
                        if(FileSystemID.startsWith("ufs")) {
                            if(!(*sliceit).PartType.startsWith("freebsd-ufs")) (*sliceit).FSType.clear();
                        }




                        break;
                    }
                    if(!(*sliceit).Alias.isEmpty()) {
                        if(tokens.at(0)=="/dev/"+(*sliceit).Alias+":") {

                            (*sliceit).FSType=FileSystemID;


                            // CHECK IF PARTITION TYPE MAKES SENSE WITH FILE SYSTEM
                            if((FileSystemID=="fat16")||(FileSystemID=="fat32")||(FileSystemID=="ntfs")) {
                                if(((*sliceit).PartType!=FileSystemID) && (!(*sliceit).PartType.startsWith("ms-"))) (*sliceit).FSType.clear();
                            }
                            if(FileSystemID.startsWith("ext")) {
                                if(!(*sliceit).PartType.startsWith("linux-")) (*sliceit).FSType.clear();
                            }
                            if(FileSystemID.startsWith("ufs")) {
                                if(!(*sliceit).PartType.startsWith("freebsd-ufs")) (*sliceit).FSType.clear();
                            }



                            break;
                        }
                    }
                    ++sliceit;
            }

            if(sliceit!=(*dskit).Partitions.end()) break;

            if(tokens.at(0)=="/dev/"+(*dskit).Name+":") {
                (*dskit).FSType=FileSystemID;


                // CHECK IF PARTITION TYPE MAKES SENSE WITH FILE SYSTEM
                if((FileSystemID=="fat16")||(FileSystemID=="fat32")||(FileSystemID=="ntfs")) {
                    if(((*dskit).PartType!=FileSystemID) && (!(*dskit).PartType.startsWith("ms-"))) (*dskit).FSType.clear();
                }
                if(FileSystemID.startsWith("ext")) {
                    if(!(*dskit).PartType.startsWith("linux-")) (*dskit).FSType.clear();
                }
                if(FileSystemID.startsWith("ufs")) {
                    if(!(*dskit).PartType.startsWith("freebsd-ufs")) (*dskit).FSType.clear();
                }




                break;
            }
            if(!(*dskit).Alias.isEmpty()) {
                if(tokens.at(0)=="/dev/"+(*dskit).Alias+":") {
                    (*dskit).FSType=FileSystemID;

                    // CHECK IF PARTITION TYPE MAKES SENSE WITH FILE SYSTEM
                    if((FileSystemID=="fat16")||(FileSystemID=="fat32")||(FileSystemID=="ntfs")) {
                        if(((*dskit).PartType!=FileSystemID) && (!(*dskit).PartType.startsWith("ms-"))) (*dskit).FSType.clear();
                    }
                    if(FileSystemID.startsWith("ext")) {
                        if(!(*dskit).PartType.startsWith("linux-")) (*dskit).FSType.clear();
                    }
                    if(FileSystemID.startsWith("ufs")) {
                        if(!(*dskit).PartType.startsWith("freebsd-ufs")) (*dskit).FSType.clear();
                    }



                    break;
                }
            }

            ++dskit;
        }




        ++idx;
    }



    // GET MOUNT LOCATIONS FROM ps, FOR FUSE FILESYSTEMS ONLY

    idx=ps.constBegin();

    while(idx!=ps.constEnd()) {
        QString MountString=(*idx);
        QStringList tokens=(*idx).split(" ",QString::SkipEmptyParts);


        if(tokens.count()<7) { ++ idx; continue; }

        int devpos;
        for(devpos=0;devpos<tokens.count();++devpos) {
            if(tokens.at(devpos).startsWith("/dev/")) break;
        }
        if(devpos==tokens.count()) { ++ idx; continue; }
        if(!tokens.at(devpos).startsWith("/dev/")) { ++ idx; continue; }


        // FIND DISK OR PARTITION WITH GIVEN NAME
        int posdir=MountString.indexOf(tokens.at(devpos+1));
        MountString=MountString.right(MountString.length()-posdir);

        QList<vdev_t>::iterator dskit=this->Disks.begin();

        while(dskit!=this->Disks.end())
        {
            QList<vdev_t>::iterator sliceit=(*dskit).Partitions.begin();
            while(sliceit!=(*dskit).Partitions.end()) {
                QList<vdev_t>::Iterator partit=(*sliceit).Partitions.begin();
                while(partit!=(*sliceit).Partitions.end()) {
                    if(tokens.at(devpos)=="/dev/"+(*partit).Name) { (*partit).MountPoint=MountString; break; }
                    if(!(*partit).Alias.isEmpty()) {
                        if(tokens.at(devpos)=="/dev/"+(*partit).Alias) { (*partit).MountPoint=MountString; break; }
                    }
                    ++partit;
                }
                    if(partit!=(*sliceit).Partitions.end()) break;
                    if(tokens.at(devpos)=="/dev/"+(*sliceit).Name) { (*sliceit).MountPoint=MountString; break; }
                    if(!(*sliceit).Alias.isEmpty()) {
                        if(tokens.at(devpos)=="/dev/"+(*sliceit).Alias) { (*sliceit).MountPoint=MountString; break; }
                    }
                    ++sliceit;
            }

            if(sliceit!=(*dskit).Partitions.end()) break;

            if(tokens.at(devpos)=="/dev/"+(*dskit).Name) { (*dskit).MountPoint=MountString; break; }
            if(!(*dskit).Alias.isEmpty()) {
                if(tokens.at(devpos)=="/dev/"+(*dskit).Alias) { (*dskit).MountPoint=MountString; break; }
            }

            ++dskit;
        }




        ++idx;
    }


    // FINISHED PROCESSING DEVICES

    // PROCESS THE POOL LIST

    idx=a.constBegin();
    zpool_t pool;
    zerror_t err;

    state=0;

    while(idx!=a.constEnd()) {

        QString str=*idx;

        if(str.startsWith("  pool: ")) {

            if(state) {
                // PREVIOUS POOL FINISHED, STORE
                this->Pools.append(pool);
                state=0;
            }
            pool.Name=str.remove(0,8);
            pool.Size=0;
            pool.Free=0;
            pool.Status=0;
            pool.VDevs.clear();
            pool.Type.clear();
            ++state;
        }

        if(str.startsWith(" state: ")&& (state==1)) {
            QString tmp=*idx;
            tmp=tmp.remove(0,8);
            pool.Status=STATE_UNKNOWN;
            if(tmp.startsWith("ONLINE")) pool.Status=STATE_ONLINE;
            if(tmp.startsWith("OFFLINE")) pool.Status=STATE_OFFLINE;
            if(tmp.startsWith("DEGRADED")) pool.Status=STATE_DEGRADED;
            if(tmp.startsWith("FAULTED")) pool.Status=STATE_FAULTED;
            if(tmp.startsWith("REMOVED")) pool.Status=STATE_REMOVED;
            if(tmp.startsWith("UNAVAIL")) pool.Status=STATE_UNAVAIL;
            ++state;
        }

        if(str.startsWith("status: ")&& (state==2)) {
            QString tmp=*idx;
            tmp.remove(0,8);
            err.PoolName=pool.Name;
            err.Error=tmp;
            ++state;
        }
        else {
        if(state==3) {
            if(str.startsWith(QString("\t"))) {
                // ERROR MESSAGE CONTINUES
                QString tmp=*idx;
                tmp.remove(0,1);
                err.Error+=" "+tmp;
            }
            else {
                // ERROR MESSAGE FINISHED
                this->Errors.append(err);
                ++state;
            }
        }
        }



        if(str.startsWith(QString("\t")+pool.Name) && ((state==2)||(state==4)) ) {
        // START DETAILED LIST OF THE POOL
            state=5;
        }

        if(str.startsWith("\tlogs") && (state==5)) {
            vdev_t vdev;

            vdev.Name=vdev.PartType="logs";
            vdev.Level=0;
            vdev.Status=STATE_NOTAPPLICABLE;
            vdev.InPool=pool.Name;
            pool.VDevs.append(vdev);

        }

        if(str.startsWith("\tspares") && (state==5)) {
            vdev_t vdev;

            vdev.Name=vdev.PartType="spares";
            vdev.Level=0;
            vdev.Status=STATE_NOTAPPLICABLE;
            vdev.InPool=pool.Name;
            pool.VDevs.append(vdev);

        }

        if(str.startsWith("\tcache") && (state==5)) {
            vdev_t vdev;

            vdev.Name=vdev.PartType="cache";
            vdev.Level=0;
            vdev.Status=STATE_NOTAPPLICABLE;
            vdev.InPool=pool.Name;
            pool.VDevs.append(vdev);

        }


        if(str.startsWith("\t  ")&& (state==5)) {
            QString tmp=*idx;
            int level=0;
            tmp=tmp.remove(0,3);
            if(tmp.startsWith("  ")) {
                // THIS IS A DISK INSIDE A MIRROR/RAID, ETC.
                ++level;
            }
            else {
                if( (pool.VDevs.count()>0) && ((pool.VDevs.last().PartType=="logs")||(pool.VDevs.last().PartType=="cache")||(pool.VDevs.last().PartType=="spares")))
                {
                    // THIS DEVICE IS PART OF A SPECIAL GROUP
                    ++level;
                }
            }
            QStringList tokens=tmp.split(" ",QString::SkipEmptyParts);

            QStringList::const_iterator it=tokens.constBegin();
            vdev_t vdev;

            if(tokens.count()>0)
            {
                QString tok=*it;

                vdev.Name=tok;
                vdev.Level=level;
                vdev.Status=STATE_UNKNOWN;
                ++it;
                if(it!=tokens.constEnd()) {


                QString tmp=*it;


                if(tmp=="ONLINE") vdev.Status=STATE_ONLINE;
                if(tmp=="OFFLINE") vdev.Status=STATE_OFFLINE;
                if(tmp=="DEGRADED") vdev.Status=STATE_DEGRADED;
                if(tmp=="FAULTED") vdev.Status=STATE_FAULTED;
                if(tmp=="REMOVED") vdev.Status=STATE_REMOVED;
                if(tmp=="UNAVAIL") vdev.Status=STATE_UNAVAIL;
                if(tmp=="AVAIL") vdev.Status=STATE_AVAIL;

                }

                if(vdev.Status==STATE_UNKNOWN) vdev.Name.clear();

                // CHECK IF VDEV IS A MIRROR OR RAID ARRAY

                if(vdev.Name.startsWith("mirror")) vdev.PartType="mirror";
                if(vdev.Name.startsWith("raidz3")) vdev.PartType="raidz3";
                else {
                if(vdev.Name.startsWith("raidz2")) vdev.PartType="raidz2";
                else {
                if(vdev.Name.startsWith("raidz")) vdev.PartType="raidz";
                }
                }

                vdev.Partitions.clear();


            }
            if(!vdev.Name.isEmpty()) {
                vdev.InPool=pool.Name;
                if(vdev.Level) pool.VDevs.last().Partitions.append(vdev);
                else pool.VDevs.append(vdev);

                vdev_t *dsk=getDiskbyName(vdev.Name);
                if(dsk!=NULL) dsk->InPool = pool.Name;
                if(dsk!=NULL && level==0) {
                 // THE POOL IS STRIPED ONLY
                    pool.Type="striped";
                }
                if(pool.Type.isEmpty() && !vdev.PartType.isEmpty()) pool.Type=vdev.PartType;

            }
        }

        if(str.startsWith("errors: ")&& (state==5)) ++state;


        idx++;
    }
    if(state) {
        // PREVIOUS POOL FINISHED, STORE
        this->Pools.append(pool);
        state=0;
    }

    // NOW THAT WE HAVE A LIST OF POOLS, GET PROPERTIES FOR ALL ACTIVE ZPOOLS

    QString cmdline="zpool get all";
    zpool_t n;

    foreach(n,Pools) {
        cmdline+= " \""+n.Name+"\"";
    }

    prop=pcbsd::Utils::runShellCommand(cmdline);






    // PROCESS THE EXPORTED POOL LIST

    idx=i.constBegin();

    state=0;

    while(idx!=i.constEnd()) {

        QString str=*idx;

        if(str.startsWith("   pool: ")) {

            if(state) {
                // PREVIOUS POOL FINISHED, STORE
                this->Pools.append(pool);
                state=0;
            }
            pool.Name=str.remove(0,9);
            pool.Size=0;
            pool.Free=0;
            pool.Status=0;
            pool.VDevs.clear();
            pool.Type.clear();
            ++state;
        }

        if(str.startsWith("     id: ")&& (state==1)) {
            QString tmp=*idx;
            tmp=tmp.remove(0,9);
            pool.Type=tmp;
        }


        if(str.startsWith("  state: ")&& (state==1)) {
            QString tmp=*idx;
            tmp=tmp.remove(0,9);
            pool.Status=STATE_UNKNOWN;
            if(tmp.startsWith("ONLINE")) pool.Status=STATE_ONLINE;
            if(tmp.startsWith("OFFLINE")) pool.Status=STATE_OFFLINE;
            if(tmp.startsWith("DEGRADED")) pool.Status=STATE_DEGRADED;
            if(tmp.startsWith("FAULTED")) pool.Status=STATE_FAULTED;
            if(tmp.startsWith("REMOVED")) pool.Status=STATE_REMOVED;
            if(tmp.startsWith("UNAVAIL")) pool.Status=STATE_UNAVAIL;
            pool.Status|=STATE_EXPORTED;
            ++state;
        }

        /*
        if(str.startsWith(" status: ")&& (state==2)) {
            QString tmp=*idx;
            tmp.remove(0,9);
            err.PoolName=pool.Name;
            err.Error=tmp;
            ++state;
        }
        else {
        if(state==3) {
            if(str.startsWith(QString("\t"))) {
                // ERROR MESSAGE CONTINUES
                QString tmp=*idx;
                tmp.remove(0,1);
                err.Error+=" "+tmp;
            }
            else {
                // ERROR MESSAGE FINISHED
                this->Errors.append(err);
                ++state;
            }
        }
        }
        */


        if(str.startsWith(QString("\t")+pool.Name) && ((state==2)||(state==4)) ) {
        // START DETAILED LIST OF THE POOL
            state=5;
        }

        if(str.startsWith("\tlogs") && (state==5)) {
            vdev_t vdev;

            vdev.Name=vdev.PartType="logs";
            vdev.Level=0;
            vdev.Status=STATE_NOTAPPLICABLE;
            vdev.InPool=pool.Name;
            pool.VDevs.append(vdev);

        }

        if(str.startsWith("\tspares") && (state==5)) {
            vdev_t vdev;

            vdev.Name=vdev.PartType="spares";
            vdev.Level=0;
            vdev.Status=STATE_NOTAPPLICABLE;
            vdev.InPool=pool.Name;
            pool.VDevs.append(vdev);

        }

        if(str.startsWith("\tcache") && (state==5)) {
            vdev_t vdev;

            vdev.Name=vdev.PartType="cache";
            vdev.Level=0;
            vdev.Status=STATE_NOTAPPLICABLE;
            vdev.InPool=pool.Name;
            pool.VDevs.append(vdev);

        }


        if(str.startsWith("\t  ")&& (state==5)) {
            QString tmp=*idx;
            int level=0;
            tmp=tmp.remove(0,3);
            if(tmp.startsWith("  ")) {
                // THIS IS A DISK INSIDE A MIRROR/RAID, ETC.
                ++level;
            }
            else {
                if( (pool.VDevs.count()>0) && ((pool.VDevs.last().PartType=="logs")||(pool.VDevs.last().PartType=="cache")||(pool.VDevs.last().PartType=="spares")))
                {
                    // THIS DEVICE IS PART OF A SPECIAL GROUP
                    ++level;
                }
            }
            QStringList tokens=tmp.split(" ",QString::SkipEmptyParts);

            QStringList::const_iterator it=tokens.constBegin();
            vdev_t vdev;

            if(tokens.count()>0)
            {
                QString tok=*it;

                vdev.Name=tok;
                vdev.Level=level;
                vdev.Status=STATE_UNKNOWN;
                ++it;
                if(it!=tokens.constEnd()) {

                QString tmp=*it;


                if(tmp=="ONLINE") vdev.Status=STATE_ONLINE;
                if(tmp=="OFFLINE") vdev.Status=STATE_OFFLINE;
                if(tmp=="DEGRADED") vdev.Status=STATE_DEGRADED;
                if(tmp=="FAULTED") vdev.Status=STATE_FAULTED;
                if(tmp=="REMOVED") vdev.Status=STATE_REMOVED;
                if(tmp=="UNAVAIL") vdev.Status=STATE_UNAVAIL;
                if(tmp=="AVAIL") vdev.Status=STATE_AVAIL;
                }

                if(vdev.Status==STATE_UNKNOWN) vdev.Name.clear();

                // CHECK IF VDEV IS A MIRROR OR RAID ARRAY

                if(vdev.Name.startsWith("mirror")) vdev.PartType="mirror";
                if(vdev.Name.startsWith("raidz3")) vdev.PartType="raidz3";
                else {
                if(vdev.Name.startsWith("raidz2")) vdev.PartType="raidz2";
                else {
                if(vdev.Name.startsWith("raidz")) vdev.PartType="raidz";
                }
                }

                vdev.Partitions.clear();


            }
            if(!vdev.Name.isEmpty()) {
                vdev.InPool=pool.Name;
                if(vdev.Level) pool.VDevs.last().Partitions.append(vdev);
                else pool.VDevs.append(vdev);

                vdev_t *dsk=getDiskbyName(vdev.Name);
                if(dsk!=NULL) dsk->InPool = pool.Name;
                //if(dsk!=NULL && level==0) {
                 // THE POOL IS STRIPED ONLY
                //    pool.Type="striped";
                //}
                //if(pool.Type.isEmpty() && !vdev.PartType.isEmpty()) pool.Type=vdev.PartType;

            }
        }

        if(str.startsWith("errors: ")&& (state==5)) ++state;


        idx++;
    }
    if(state) {
        // PREVIOUS POOL FINISHED, STORE
        this->Pools.append(pool);
        state=0;
    }



    // PROCESS THE DESTROYED POOL LIST

    idx=d.constBegin();

    state=0;

    while(idx!=d.constEnd()) {

        QString str=*idx;

        if(str.startsWith("   pool: ")) {

            if(state) {
                // PREVIOUS POOL FINISHED, STORE
                if(pool.Status!=(STATE_FAULTED|STATE_DESTROYED)) {
                    // IF POOL IS DAMAGED, CANNOT BE RECOVERED. DON'T SHOW IN THE LIST
                this->Pools.append(pool);
                }
                state=0;

            }
            pool.Name=str.remove(0,9);
            pool.Size=0;
            pool.Free=0;
            pool.Status=0;
            pool.VDevs.clear();
            pool.Type.clear();
            ++state;
        }

        if(str.startsWith("     id: ")&& (state==1)) {
            QString tmp=*idx;
            tmp=tmp.remove(0,9);
            pool.Type=tmp;
        }


        if(str.startsWith("  state: ")&& (state==1)) {
            QString tmp=*idx;
            tmp=tmp.remove(0,9);
            pool.Status=STATE_UNKNOWN;
            if(tmp.startsWith("ONLINE")) pool.Status=STATE_ONLINE;
            if(tmp.startsWith("OFFLINE")) pool.Status=STATE_OFFLINE;
            if(tmp.startsWith("DEGRADED")) pool.Status=STATE_DEGRADED;
            if(tmp.startsWith("FAULTED")) pool.Status=STATE_FAULTED;
            if(tmp.startsWith("REMOVED")) pool.Status=STATE_REMOVED;
            if(tmp.startsWith("UNAVAIL")) pool.Status=STATE_UNAVAIL;
            pool.Status|=STATE_DESTROYED;
            ++state;
        }




        if(str.startsWith(QString("\t")+pool.Name) && ((state==2)||(state==4)) ) {
        // START DETAILED LIST OF THE POOL
            state=5;
        }

        if(str.startsWith("\tlogs") && (state==5)) {
            vdev_t vdev;

            vdev.Name=vdev.PartType="logs";
            vdev.Level=0;
            vdev.Status=STATE_NOTAPPLICABLE;
            //vdev.InPool=pool.Name;
            pool.VDevs.append(vdev);

        }

        if(str.startsWith("\tspares") && (state==5)) {
            vdev_t vdev;

            vdev.Name=vdev.PartType="spares";
            vdev.Level=0;
            vdev.Status=STATE_NOTAPPLICABLE;
            //vdev.InPool=pool.Name;
            pool.VDevs.append(vdev);

        }

        if(str.startsWith("\tcache") && (state==5)) {
            vdev_t vdev;

            vdev.Name=vdev.PartType="cache";
            vdev.Level=0;
            vdev.Status=STATE_NOTAPPLICABLE;
            //vdev.InPool=pool.Name;
            pool.VDevs.append(vdev);

        }


        if(str.startsWith("\t  ")&& (state==5)) {
            QString tmp=*idx;
            int level=0;
            tmp=tmp.remove(0,3);
            if(tmp.startsWith("  ")) {
                // THIS IS A DISK INSIDE A MIRROR/RAID, ETC.
                ++level;
            }
            else {
                if( (pool.VDevs.count()>0) && ((pool.VDevs.last().PartType=="logs")||(pool.VDevs.last().PartType=="cache")||(pool.VDevs.last().PartType=="spares")))
                {
                    // THIS DEVICE IS PART OF A SPECIAL GROUP
                    ++level;
                }
            }
            QStringList tokens=tmp.split(" ",QString::SkipEmptyParts);

            QStringList::const_iterator it=tokens.constBegin();
            vdev_t vdev;

            if(tokens.count()>0)
            {
                QString tok=*it;

                vdev.Name=tok;
                vdev.Level=level;
                vdev.Status=STATE_UNKNOWN;

                ++it;
                if(it!=tokens.constEnd()) {

                QString tmp=*it;


                if(tmp=="ONLINE") vdev.Status=STATE_ONLINE;
                if(tmp=="OFFLINE") vdev.Status=STATE_OFFLINE;
                if(tmp=="DEGRADED") vdev.Status=STATE_DEGRADED;
                if(tmp=="FAULTED") vdev.Status=STATE_FAULTED;
                if(tmp=="REMOVED") vdev.Status=STATE_REMOVED;
                if(tmp=="UNAVAIL") vdev.Status=STATE_UNAVAIL;
                if(tmp=="AVAIL") vdev.Status=STATE_AVAIL;

                }
                if(vdev.Status==STATE_UNKNOWN) vdev.Name.clear();

                // CHECK IF VDEV IS A MIRROR OR RAID ARRAY

                if(vdev.Name.startsWith("mirror")) vdev.PartType="mirror";
                if(vdev.Name.startsWith("raidz3")) vdev.PartType="raidz3";
                else {
                if(vdev.Name.startsWith("raidz2")) vdev.PartType="raidz2";
                else {
                if(vdev.Name.startsWith("raidz")) vdev.PartType="raidz";
                }
                }

                vdev.Partitions.clear();


            }
            if(!vdev.Name.isEmpty()) {
                //vdev.InPool=pool.Name;
                if(vdev.Level) pool.VDevs.last().Partitions.append(vdev);
                else pool.VDevs.append(vdev);

                //vdev_t *dsk=getDiskbyName(vdev.Name);
                //if(dsk!=NULL) dsk->InPool = pool.Name;
                //if(dsk!=NULL && level==0) {
                 // THE POOL IS STRIPED ONLY
                //    pool.Type="striped";
                //}
                //if(pool.Type.isEmpty() && !vdev.PartType.isEmpty()) pool.Type=vdev.PartType;

            }
        }

        if(str.startsWith("errors: ")&& (state==5)) ++state;


        idx++;
    }
    if(state) {
        // PREVIOUS POOL FINISHED, STORE
        if(pool.Status!=(STATE_FAULTED|STATE_DESTROYED)) {
            // IF POOL IS DAMAGED, CANNOT BE RECOVERED. DON'T SHOW IN THE LIST
        this->Pools.append(pool);
        state=0;
        }
    }


// EXTRACT PROPERTIES

QStringList::const_iterator pit=prop.constBegin();

while(pit!=prop.constEnd())
{
    // SEARCH THROUGH ALL CURRENT POOLS TO SEE IF NAMES MATCH
    QList<zpool_t>::const_iterator poolit=Pools.constBegin();
    QList<zprop_t> *proplist=NULL;
    int matchinglength=0;
    while(poolit!=Pools.constEnd())
    {
        if(!((*poolit).Status&(STATE_DESTROYED|STATE_EXPORTED)) )
        {
            // THE POOL IS ACTIVE
            if((*pit).startsWith((*poolit).Name)) {
                if(matchinglength<(*poolit).Name.length()) { proplist=(QList<zprop_t> *)&(*poolit).Properties; matchinglength=(*poolit).Name.length(); }
            }
        }

     ++poolit;
    }
    if(proplist!=NULL)
    {
        QString tmpline=(*pit);
        QStringList a=tmpline.remove(0,matchinglength).split(" ",QString::SkipEmptyParts);
        if(a.count()>=2) {
        zprop_t proptmp;
        proptmp.Name=a[0];
        QList<QString> val=a.mid(1,a.count()-2);
        proptmp.Value.clear();
        while(val.count()) { proptmp.Value+=val.first(); val.removeFirst(); if(val.count()) proptmp.Value+=" "; }

        proplist->append(proptmp);
        }
    }



    ++pit;
}



// BEGIN PROCESSING FILESYSTEMS


QStringList::const_iterator fsit=zfsl.constBegin();

while(fsit!=zfsl.constEnd())
{

    QString tmpline=(*fsit);

    QStringList line=tmpline.split("\t",QString::SkipEmptyParts);

    if(line.count()>=4) {
     zfs_t tmp;
     tmp.FullPath=line[0];
     tmp.Properties.clear();
     FileSystems.append(tmp);
    }

    ++fsit;
}


// GET ALL PROPERTIES FOR FILESYSTEMS


QStringList::const_iterator fspr=zfspr.constBegin();

while(fspr!=zfspr.constEnd())
{

    QStringList line=(*fspr).split("\t",QString::SkipEmptyParts);

    if(line.count()>=4) {
     zprop_t tmp;
     zfs_t *zptr=getFileSystembyPath(line[0]);
     if(zptr) {

         tmp.Name=line[1];
         tmp.Value=line[2];
         if(tmp.Value=="-") tmp.Value.clear();
         tmp.From=line[3];
         if(tmp.From=="-") tmp.Source=ZFS_SRCNONE;
         if(tmp.From=="default") tmp.Source=ZFS_SRCDEFAULT;
         if(tmp.From=="local")  tmp.Source=ZFS_SRCLOCAL;
         if(tmp.From.startsWith("inherited")) tmp.Source=ZFS_SRCINHERIT;

         zptr->Properties.append(tmp);
    }
    }

    ++fspr;
}



}

void ZManagerWindow::refreshState()
{
    QToolButton splash;
    splash.setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    splash.setMinimumHeight(100);
    splash.setMinimumWidth(100);
    splash.setMaximumHeight(100);
    splash.setMaximumWidth(100);

    splash.setText(tr("Refreshing..."));
    splash.setIcon(QIcon(":/icons/server-database.png"));
    splash.setIconSize(QSize(48,48));
    splash.setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    splash.setWindowFlags(Qt::SplashScreen);

    splash.setParent(this);

    int x;
    int y;
    x=this->width()/2;
    y=this->height()/2;

    x-=50;
    y-=50;

    splash.move(x,y);

    splash.show();

//    ui->zpoolList->header()->setStretchLastSection(false);
//    ui->zpoolList->header()->setResizeMode(0,QHeaderView::Stretch);
//    ui->zpoolList->header()->setResizeMode(1,QHeaderView::ResizeToContents);

//    ui->deviceList->header()->setStretchLastSection(false);
//    ui->deviceList->header()->setResizeMode(0,QHeaderView::Stretch);
//    ui->deviceList->header()->setResizeMode(1,QHeaderView::ResizeToContents);



    ui->zpoolList->clear();
    ui->deviceList->clear();
    ui->fspoolList->clear();
    ui->fsList->clear();


    GetCurrentTopology();


    // SHOW ERRORS
    if(Errors.count()>0) {
	//Do not show the warning about unused feature flags on a pool
	if( Errors.at(0).Error.contains("supported features are not enabled", Qt::CaseInsensitive) ){
	  ui->frameStatus->setVisible(false);
	}else{
	  //Valid Warning
          ui->statusLabel->setText(Errors.at(0).Error);
          ui->poolLabel->setText(Errors.at(0).PoolName);
          ui->frameStatus->setVisible(true);
	}
    }
    else     ui->frameStatus->setVisible(false);



    // ADD ALL POOLS

    if(Pools.count()!=0) {

    int itindex=0;

    QList<zpool_t>::iterator it=Pools.begin();

    while(it!=Pools.end()) {
        // ADD POOL TO THE POOL LIST
    QTreeWidgetItem *item=new QTreeWidgetItem(ui->zpoolList);
    item->setText(0,(*it).Name);
    item->setIcon(0,QIcon(":/icons/server-database.png"));
    item->setText(1,getStatusString((*it).Status));
    item->setData(0,Qt::UserRole,QVariant(itindex));
    if((*it).Status&(STATE_DESTROYED|STATE_EXPORTED)) { item->setIcon(1,QIcon(":/icons/task-reject.png")); item->setDisabled(true); }
    else {
    if((*it).Status==STATE_ONLINE) item->setIcon(1,QIcon(":/icons/task-complete.png"));
    else if( ((*it).Status==STATE_FAULTED)|| ((*it).Status==STATE_UNAVAIL)|| ((*it).Status==STATE_REMOVED)) item->setIcon(1,QIcon(":/icons/task-reject.png"));
        else item->setIcon(1,QIcon(":/icons/task-attention.png"));

    // ALSO ADD IT TO THE FILESYSTEMS TAB, BUT ONLY IF IT'S A VALID POOL (NOT DESTROYED OR EXPORTED)
    QTreeWidgetItem *fsitem=new QTreeWidgetItem(ui->fspoolList);
    fsitem->setText(0,(*it).Name);
    fsitem->setIcon(0,QIcon(":/icons/server-database.png"));
    fsitem->setData(0,Qt::UserRole,QVariant(itindex));
    }





    QList<vdev_t>::const_iterator devit=(*it).VDevs.constBegin();

    while(devit!=(*it).VDevs.constEnd())
    {
        QTreeWidgetItem *subitem=new QTreeWidgetItem(item);

        // TODO: ADD MORE DESCRIPTION HERE FROM THE GEOM INFO OF THE DEVICE
        subitem->setText(0,(*devit).Name);
        // TODO: USE APPROPRIATE ICONS FOR DISKS, SLICES, FILES, ETC.
        subitem->setIcon(0,QIcon(":/icons/drive-harddisk.png"));
        subitem->setData(0,Qt::UserRole,QVariant(-1));
        subitem->setText(1,getStatusString((*devit).Status));
        if((*devit).Status==STATE_ONLINE) subitem->setIcon(1,QIcon(":/icons/task-complete.png"));
        else if( ((*devit).Status==STATE_FAULTED)|| ((*devit).Status==STATE_UNAVAIL)|| ((*devit).Status==STATE_REMOVED)) subitem->setIcon(1,QIcon(":/icons/task-reject.png"));
            else if((*devit).Status!=STATE_NOTAPPLICABLE) subitem->setIcon(1,QIcon(":/icons/task-attention.png"));

        // ADD SUB-VDEVS IN CASE THIS IS A MIRROR OR RAIDZ VIRTUAL DEVICE

        QList<vdev_t>::const_iterator level2it=(*devit).Partitions.constBegin();

        while(level2it!=(*devit).Partitions.constEnd())
        {
            QTreeWidgetItem *subsubitem=new QTreeWidgetItem(subitem);

            // TODO: ADD MORE DESCRIPTION HERE FROM THE GEOM INFO OF THE DEVICE
            subsubitem->setText(0,(*level2it).Name);
            // TODO: USE APPROPRIATE ICONS FOR DISKS, SLICES, FILES, ETC.
            subsubitem->setIcon(0,QIcon(":/icons/drive-harddisk.png"));
            subsubitem->setData(0,Qt::UserRole,QVariant(-1));
            subsubitem->setText(1,getStatusString((*level2it).Status));
            if((*level2it).Status==STATE_ONLINE || (*level2it).Status==STATE_AVAIL) subsubitem->setIcon(1,QIcon(":/icons/task-complete.png"));
            else if( ((*level2it).Status==STATE_FAULTED)|| ((*level2it).Status==STATE_UNAVAIL)|| ((*level2it).Status==STATE_REMOVED)) subsubitem->setIcon(1,QIcon(":/icons/task-reject.png"));
                else subsubitem->setIcon(1,QIcon(":/icons/task-attention.png"));


            ++level2it;

        }


        ++devit;
    }

    ++it;
    ++itindex;
    }

    }

    else {
        QTreeWidgetItem *item=new QTreeWidgetItem(ui->zpoolList);
        item->setText(0,tr("No pools available, right click to create a new one..."));
   }



    // ADD ALL DISK DEVICES


    QList<vdev_t>::const_iterator idx=Disks.constBegin();


    while(idx!=Disks.constEnd()) {
        QTreeWidgetItem *item=new QTreeWidgetItem(ui->deviceList);

        QString sz;
        if((*idx).Size!=0) sz=" ("+printBytes((*idx).Size)+")";
        else sz=tr(" (No media in drive)");
        if(!(*idx).PartType.isEmpty()) { sz+=" [" + (*idx).PartType + "]"; }
        sz+="\n";
        item->setText(0,(*idx).Name + sz + (*idx).Description);
        item->setIcon(0,QIcon(":/icons/drive-harddisk.png"));
        if((*idx).MountPoint.isEmpty()) {
            // IS NOT MOUNTED, CHECK IF IT HAS ANY PARTITIONS
            if((*idx).Partitions.count()==0) {
                // NO PARTITIONS, IT'S EITHER UNUSED OR PART OF A POOL (DEDICATED)
                if((*idx).InPool.isEmpty())  {
                    if((*idx).FSType.isEmpty()) {
                    if((*idx).Size!=0) item->setText(1,tr("Avaliable"));
                    else item->setText(1,tr("No disk"));
                    }
                    else {
                    // CONTAINS A FILE SYSTEM - NO PARTITION TABLE!
                    item->setText(1,tr("Unmounted"));
                    }
                }
                else item->setText(1,tr("ZPool: ")+(*idx).InPool);
            } else item->setText(1,tr("Sliced")); }
        else item->setText(1,tr("Mounted: ")+(*idx).MountPoint);

        // ADD SLICES HERE AS SUBITEMS
        if((*idx).Partitions.count()>0) {
            QList<vdev_t>::const_iterator partidx=(*idx).Partitions.constBegin();


            while(partidx!=(*idx).Partitions.constEnd()) {

            QTreeWidgetItem *subitem=new QTreeWidgetItem(item);

            QString sz;

            sz=" ("+printBytes((*partidx).Size)+")\n";
            subitem->setText(0,(*partidx).Name + sz + " ["+(*partidx).PartType+(((*partidx).FSType.isEmpty())? "":" | ")+(*partidx).FSType+"]");
            subitem->setIcon(0,QIcon(":/icons/partitionmanager.png"));

            if((*partidx).MountPoint.isEmpty()) {
                if((*partidx).Partitions.count()==0) {
                    if( (*partidx).InPool.isEmpty()) {
                        if((*partidx).PartType.isEmpty() || ((*partidx).PartType=="BSD")) subitem->setText(1,tr("Available")); else
                        {
                            if((*partidx).FSType.isEmpty()) subitem->setText(1,tr("Blank/Unknown"));
                            else subitem->setText(1,tr("Unmounted"));
                        }
                    } else subitem->setText(1,tr("ZPool: ") + (*partidx).InPool);
                } else subitem->setText(1,tr("Partitioned")); }
            else subitem->setText(1,tr("Mounted: ")+(*partidx).MountPoint);

            if((*partidx).Partitions.count()>0) {
                QList<vdev_t>::const_iterator part2idx=(*partidx).Partitions.constBegin();


                while(part2idx!=(*partidx).Partitions.constEnd()) {

                QTreeWidgetItem *subsubitem=new QTreeWidgetItem(subitem);

                QString sz;

                sz=" ("+printBytes((*part2idx).Size)+")\n";
                subsubitem->setText(0,(*part2idx).Name + sz + " ["+(*part2idx).PartType+"]");
                subsubitem->setIcon(0,QIcon(":/icons/kdf.png"));

                if((*part2idx).MountPoint.isEmpty()) {

                    if((*part2idx).InPool.isEmpty()) {
                        if( (*part2idx).PartType.isEmpty()) subsubitem->setText(1,tr("Available")); else
                        {
                            if((*part2idx).FSType.isEmpty()) subsubitem->setText(1,tr("Blank/Unknown"));
                            else subsubitem->setText(1,tr("Unmounted"));
                        }
                    } else subsubitem->setText(1,tr("ZPool: ") + (*part2idx).InPool);
                }
                else subsubitem->setText(1,tr("Mounted: ")+(*part2idx).MountPoint);


                ++part2idx;
                }


            }




            ++partidx;
            }


        }

    ++idx;
    }

ui->zpoolList->expandAll();
ui->deviceList->expandAll();
if(ui->fspoolList->topLevelItem(0)) ui->fspoolList->setCurrentItem(ui->fspoolList->topLevelItem(0));


// CHECK SECTION SIZES FOR ALL LISTS AND ADJUST ACCORDINGLY

/*
if(ui->deviceList->header()->sectionSize(1)>ui->deviceList->width()/2) {
    ui->deviceList->header()->setResizeMode(1,QHeaderView::Interactive);
    ui->deviceList->header()->resizeSection(1,ui->deviceList->width()/3);

}
else     {
    ui->deviceList->header()->setResizeMode(1,QHeaderView::Interactive);

}
*/
/*
if(ui->zpoolList->header()->sectionSize(1)>ui->zpoolList->width()/2) {
    ui->zpoolList->header()->setResizeMode(1,QHeaderView::Interactive);
    ui->zpoolList->header()->resizeSection(1,ui->zpoolList->width()/3);

}
else     {
    ui->zpoolList->header()->setResizeMode(1,QHeaderView::Interactive);
}
*/

}


bool ZManagerWindow::close()
{
    return QMainWindow::close();
}


const QString ZManagerWindow::getStatusString(int status)
{
    QString result;

    if(status&STATE_EXPORTED) {
        result=tr("(Exported)\n");
        status&=~STATE_EXPORTED;
    }

    if(status&STATE_DESTROYED) {
        result=tr("(Destroyed)\n");
        status&=~STATE_DESTROYED;
    }

    switch(status)
    {
    case STATE_NOTAPPLICABLE:
        return result;
    case STATE_OFFLINE:
        return result+tr("Offline");
    case STATE_ONLINE:
        return result+tr("Online");
    case STATE_DEGRADED:
        return result+tr("Degraded");
    case STATE_FAULTED:
        return result+tr("Faulted");
    case STATE_REMOVED:
        return result+tr("Removed");
    case STATE_UNAVAIL:
        return result+tr("Unavailable");
    case STATE_AVAIL:
        return result+tr("Available");
    default:
    case STATE_UNKNOWN:
        return result+tr("Unknown");

    }
}




void ZManagerWindow::zpoolContextMenu(QPoint p)
{
    struct zactions zpoolmenu[]={
        /*  QString menutext        ,   int triggermask, triggerflags    ,  action_func slot */
        { QString(tr("Create new pool"))    ,     ~ITEM_ISPOOL, ITEM_NONE         ,  SLOT(zpoolCreate(bool)) },
        { QString(tr("Rename pool"))    ,     ITEM_TYPE|ITEM_ISEXPORTED|ITEM_ISDESTROYED, ITEM_ISPOOL         ,  SLOT(zpoolRename(bool)) },
        { QString(tr("Destroy pool"))       ,    ITEM_TYPE|ITEM_ISEXPORTED|ITEM_ISDESTROYED, ITEM_ISPOOL         ,  SLOT(zpoolDestroy(bool)) },
        { QString(tr("Add devices..."))       ,    ITEM_TYPE|ITEM_ISEXPORTED|ITEM_ISDESTROYED, ITEM_ISPOOL         ,  SLOT(zpoolAdd(bool)) },
        { QString(tr("Add log devices..."))       ,    ITEM_TYPE|ITEM_ISEXPORTED|ITEM_ISDESTROYED, ITEM_ISPOOL         ,  SLOT(zpoolAddLog(bool)) },
        { QString(tr("Add cache devices..."))       ,    ITEM_TYPE|ITEM_ISEXPORTED|ITEM_ISDESTROYED, ITEM_ISPOOL         ,  SLOT(zpoolAddCache(bool)) },
        { QString(tr("Add spare devices..."))       ,    ITEM_TYPE|ITEM_ISEXPORTED|ITEM_ISDESTROYED, ITEM_ISPOOL         ,  SLOT(zpoolAddSpare(bool)) },
        { QString(tr("Scrub"))      ,   ITEM_TYPE|ITEM_ISEXPORTED|ITEM_ISDESTROYED, ITEM_ISPOOL         ,  SLOT(zpoolScrub(bool)) },
        { QString(tr("Export pool"))      ,   ITEM_TYPE|ITEM_ISEXPORTED|ITEM_ISDESTROYED, ITEM_ISPOOL         ,  SLOT(zpoolExport(bool)) },
        { QString(tr("Import pool"))    ,     ITEM_ALL, ITEM_ISPOOL|ITEM_ISEXPORTED         ,  SLOT(zpoolImport(bool)) },
        { QString(tr("Recover destroyed pool"))    ,     ITEM_ALL, ITEM_ISPOOL|ITEM_ISDESTROYED         ,  SLOT(zpoolImport(bool)) },
        { QString(tr("Properties..."))      ,   ITEM_TYPE|ITEM_ISEXPORTED|ITEM_ISDESTROYED, ITEM_ISPOOL         ,  SLOT(zpoolEditProperties(bool)) },

        { QString(tr("Attach (mirror) device..."))      ,   ITEM_TYPE|PARENT(ITEM_TYPE&~(ITEM_ISMIRROR|ITEM_ISPOOL)), ITEM_ISDISK    ,  SLOT(zpoolAttachDevice(bool)) },
        { QString(tr("Detach from mirror"))      ,   ITEM_TYPE|PARENT(ITEM_TYPE), ITEM_ISDISK|PARENT(ITEM_ISMIRROR)    ,  SLOT(zpoolDetachDevice(bool)) },
        { QString(tr("Take offline"))      ,   ITEM_ALL|PARENT(ITEM_TYPE&~(ITEM_ISMIRROR|ITEM_ISPOOL|ITEM_ISRAIDZ)), ITEM_ISDISK   ,  SLOT(zpoolOfflineDevice(bool)) },
        { QString(tr("Bring online"))      ,   ITEM_STATE, ITEM_ISOFFLINE   ,  SLOT(zpoolOnlineDevice(bool)) },

        { QString(tr("Remove"))      ,   ITEM_TYPE|PARENT(ITEM_TYPE&~(ITEM_ISLOG|ITEM_ISCACHE|ITEM_ISSPARE)), ITEM_ISDISK    ,  SLOT(zpoolRemoveDevice(bool)) },




        { QString()       ,   0, 0      ,  NULL }

    };

    int flags=0;
    int idx;

    QMenu m(tr("zpool Menu"),this);

    QTreeWidgetItem *item=ui->zpoolList->itemAt(p);

    if(item!=NULL) {
        // FIRST DETERMINE IF THE ITEM IS A POOL OR A DEVICE
//        qDebug() << item->text(0);
        flags=0;
        if( (lastSelectedPool=getZpoolbyName(item->text(0),item->data(0,Qt::UserRole).toInt()) )) {
            flags=ITEM_ISPOOL;
            lastSelectedVdev=NULL;

            if(lastSelectedPool->Status&STATE_EXPORTED) flags|=ITEM_ISEXPORTED;
            if(lastSelectedPool->Status&STATE_DESTROYED) flags|=ITEM_ISDESTROYED;

        }
        else {
        // IT MUST BE A DEVICE, GET MORE INFORMATION
            if(!item->isDisabled()) {
            if( (lastSelectedVdev=getVDevbyName(item->text(0)))) flags=zpoolGetVDEVType(lastSelectedVdev);
            else flags=ITEM_NONE;   // NO ITEM WAS SELECTED, USER MUST'VE CLICKED IN THE BLANK SPACE
            }
            else flags=ITEM_NONE;
        }
    }

    for(idx=0;zpoolmenu[idx].slot!=NULL;++idx)
    {
        if( (flags & zpoolmenu[idx].triggermask) == zpoolmenu[idx].triggerflags) {

            QAction *act=m.addAction(zpoolmenu[idx].menutext);
            connect(act,SIGNAL(triggered(bool)),this,zpoolmenu[idx].slot);

        }
    }

    p.setX(p.x()+5);
    p.setY(p.y()+5);

    needRefresh=false;
    m.exec(ui->zpoolList->viewport()->mapToGlobal(p));

    if(needRefresh) refreshState();
}

void ZManagerWindow::deviceContextMenu(QPoint p)
{
    QMenu m(tr("Device Menu"),this);
    QTreeWidgetItem *item=ui->deviceList->itemAt(p);

    vdev_t *dev=(item)? getDiskbyName(item->text(0).split(" ",QString::SkipEmptyParts).at(0)):NULL;

    if(dev==NULL) return;

    if(dev->Name.startsWith("cd")) return;  // NOTHING TO DO ON A CD/DVD DRIVE

    if(!dev->InPool.isEmpty()) {
        // DISK IS IN A POOL, NOTHING TO DO UNTIL IT'S REMOVED
        m.addAction(tr("Manage zpool ")+dev->InPool)->setData(QVariant(9));
    }
    else {
    if(!dev->MountPoint.isEmpty()) m.addAction(tr("Unmount"))->setData(QVariant(1));  // OFFER TO UNMOUNT IF PARTITION IS MOUNTED
    else {
        if( (!dev->PartType.isEmpty()) && (dev->Partitions.count()==0) && (dev->PartType!="MBR") && (dev->PartType!="GPT") && (dev->PartType!="BSD") && (dev->PartType!="freebsd") )
        {
            if(!dev->FSType.isEmpty()) {
                // THIS IS A PARTITION WITH A FILE SYSTEM BUT NOT MOUNTED
                m.addAction(tr("Mount"))->setData(QVariant(2));
            }
        }

        if(dev->PartType.isEmpty() && dev->Level<2) {
            // THIS IS NOT PARTITIONED, OFFER TO CREATE A PARTITION TABLE
            m.addAction(tr("Create MBR partition table"))->setData(QVariant(3));
            m.addAction(tr("Create GPT partition table"))->setData(QVariant(4));
        }
        if(dev->PartType=="freebsd" && dev->Level<2) {
            // THIS IS NOT PARTITIONED, OFFER TO CREATE A PARTITION TABLE
            m.addAction(tr("Create BSD partition table"))->setData(QVariant(5));
        }

        else if( (dev->Level<2) && ((dev->PartType=="MBR") || (dev->PartType=="GPT") || (dev->PartType=="BSD"))) {
            // THIS DISK HAS PARTITIONS, OFFER TO ADD A NEW ONE
            if(dev->Partitions.count()==0) m.addAction(tr("Delete Partition Table"))->setData(QVariant(6));
            if(dev->Level==0) m.addAction(tr("Add new slice"))->setData(QVariant(7));
            else m.addAction(tr("Add new partition"))->setData(QVariant(7));
        }

        if( (dev->Partitions.count()==0) && (dev->Level>0)) {
            // OFFER TO DELETE THE PARTITION
            if(dev->Level==1) m.addAction(tr("Destroy this slice"))->setData(QVariant(8));
            else m.addAction(tr("Destroy this partition"))->setData(QVariant(8));

        }


    }
    }
    p.setX(p.x()+5);
    p.setY(p.y()+5);

    QAction *result=m.exec(ui->deviceList->viewport()->mapToGlobal(p));

    if(result!=NULL) {
        int selected=result->data().toInt();
        bool result;
        switch(selected)
        {
        case 1:
            result=deviceUnmount(dev);
            break;
        case 2:
            result=deviceMount(dev);
            break;
        case 3:
        case 4:
        case 5:
            result=deviceCreatePartitionTable(dev,selected-3);
            break;
        case 6:
            result=deviceDestroyPartitionTable(dev);
            break;
        case 7:
            result=deviceAddPartition(dev);
            break;
        case 8:
            result=deviceDestroyPartition(dev);
            break;
        case 9:
            result=0;
            ui->tabContainer->setCurrentIndex(1);
            break;
        default:
            result=false;
            break;
        }


        if(result) this->refreshState();


    }


}



vdev_t *ZManagerWindow::getDiskbyName(QString name)
{
    QList<vdev_t>::const_iterator it=this->Disks.constBegin();


    while(it!=this->Disks.constEnd())
    {
        if((*it).Name==name) return (vdev_t *)&(*it);
        if((*it).Partitions.count()!=0) {
            // PROCESS ALL SLICES
            QList<vdev_t>::const_iterator sliceit=(*it).Partitions.constBegin();

            while(sliceit!=(*it).Partitions.constEnd())
            {
                if((*sliceit).Name==name) return (vdev_t *)&(*sliceit);
                if((*it).Partitions.count()!=0) {
                    // PROCESS ALL BSD LABEL PARTITIONS
                    QList<vdev_t>::const_iterator partit=(*sliceit).Partitions.constBegin();

                    while(partit!=(*sliceit).Partitions.constEnd())
                    {
                        if((*partit).Name==name) return (vdev_t *)&(*partit);

                    ++partit;
                    }

                }

            ++sliceit;
            }

        }

        ++it;
    }

    return NULL;

}


vdev_t *ZManagerWindow::getContainerDisk(vdev_t* device)
{

    QList<vdev_t>::const_iterator it=this->Disks.constBegin();


    while(it!=this->Disks.constEnd())
    {
        if((*it).Partitions.count()!=0) {
            // PROCESS ALL SLICES



            if((*it).Partitions.contains(*device)) return (vdev_t *)&(*it);

            QList<vdev_t>::const_iterator sliceit=(*it).Partitions.constBegin();

            while(sliceit!=(*it).Partitions.constEnd())
            {

                if((*sliceit).Partitions.contains(*device)) return (vdev_t *)&(*sliceit);

            ++sliceit;
            }

        }

        ++it;
    }

    return NULL;



}


vdev_t *ZManagerWindow::getContainerGroup(vdev_t* device)
{

    QList<zpool_t>::const_iterator it=this->Pools.constBegin();


    while(it!=this->Pools.constEnd())
    {
        if((*it).VDevs.count()!=0) {
            // PROCESS ALL SLICES



            if((*it).VDevs.contains(*device)) return NULL;  // THERE IS NO GROUP, THIS DEVICE WAS DIRECTLY IN THE POOL

            QList<vdev_t>::const_iterator sliceit=(*it).VDevs.constBegin();

            while(sliceit!=(*it).VDevs.constEnd())
            {

                if((*sliceit).Partitions.contains(*device)) return (vdev_t *)&(*sliceit);

            ++sliceit;
            }

        }

        ++it;
    }

    return NULL;



}





zpool_t *ZManagerWindow::getZpoolbyName(QString name,int index)
{

    if(index>=0) return (zpool_t *)&(this->Pools.at(index));

QList<zpool_t>::const_iterator it=this->Pools.constBegin();

while(it!=this->Pools.constEnd())
{
    if((*it).Name==name) {
        return (zpool_t *)&(*it);
    }
    ++it;
}

return NULL;

}
vdev_t *ZManagerWindow::getVDevbyName(QString name)
{
QList<zpool_t>::const_iterator it=this->Pools.constBegin();

while(it!=this->Pools.constEnd())
{
    QList<vdev_t>::const_iterator vdit=(*it).VDevs.constBegin();
    while(vdit!=(*it).VDevs.constEnd()) {
        vdev_t *ptr=(vdev_t *)&(*vdit);
        if((*vdit).Name==name) return ptr;
        if((*vdit).Partitions.count()!=0) {
            QList<vdev_t>::const_iterator vdev2=(*vdit).Partitions.constBegin();
            while(vdev2!=(*vdit).Partitions.constEnd()) {
                if((*vdev2).Name==name) return (vdev_t *)&(*vdev2);
            ++vdev2;
            }
        }
        ++vdit;
    }

    ++it;
}

return NULL;

}




bool ZManagerWindow::deviceCreatePartitionTable(vdev_t *device,int type)
{
    QString cmd;
    if(type==0) cmd="gpart create -s mbr ";
    if(type==1) cmd="gpart create -s gpt ";
    if(type==2) cmd="gpart create -s bsd ";
    cmd+=device->Name;

    QStringList a=pcbsd::Utils::runShellCommand(cmd);

    if(processErrors(a,"gpart")) return false;

    return true;
}

bool ZManagerWindow::deviceDestroyPartitionTable(vdev_t *device)
{
    QString cmd;
    cmd="gpart destroy ";
    cmd+=device->Name;

    QStringList a=pcbsd::Utils::runShellCommand(cmd);

    if(processErrors(a,"gpart")) return false;

    return true;
}
bool ZManagerWindow::deviceUnmount(vdev_t * device)
{
    QString cmdline="umount ";

    if( (device->FSType=="ext4") || (device->FSType=="ntfs")) {
     // DEVICES MOUNTED WITH FUSE NEED TO BE UNMOUNTED BY MOUNT POINT
        cmdline+= "\""+device->MountPoint + "\"";
    }
    else {
    if(device->Alias.isEmpty())  cmdline += "/dev/"+device->Name;
    else cmdline+="/dev/"+device->Alias;
    }

    QStringList a=pcbsd::Utils::runShellCommand(cmdline);


    if(processErrors(a,"umount")) return false;


    return true;
}
bool ZManagerWindow::deviceMount(vdev_t * device)
{

DialogMount mnt;

mnt.setDevice(device);

int result=mnt.exec();

if(result) {

    QString cmdline="mount ";

    if(device->FSType.startsWith("fat")) {
        cmdline+="-t msdosfs ";
    }

    if((device->FSType=="ext2") || (device->FSType=="ext3")) {
        cmdline+=" -t ext2fs ";
    }


    if(device->FSType=="ext4") {
        cmdline="ext4fuse ";
    }

    if(device->FSType=="ntfs") {
        cmdline="ntfs-3g ";
    }

    if(device->Alias.isEmpty())  cmdline += "/dev/"+device->Name;
    else cmdline+="/dev/"+device->Alias;

    cmdline += " \"" + mnt.getMountLocation()+"\"";

    QStringList a=pcbsd::Utils::runShellCommand(cmdline);


    if(processErrors(a,"mount")) return false;


}

    return true;
}
bool ZManagerWindow::deviceAddPartition(vdev_t *device)
{
    DialogPartition part;

    part.setDevice(device);

    int result=part.exec();

    if(result) {

        long long startsector=part.getStartSector();
        unsigned long long sectorcount=part.getSectorCount();
        bool align=part.needAlign();

        if(!sectorcount) return false;

        QString type=part.getPartType();

        QString cmdline,tmp;
        cmdline="gpart add";
        cmdline += " -t " + type;
        if(align) {
            cmdline += " -a 1M ";
        }
        if(startsector>=0) { tmp.sprintf(" -b %lld",startsector); cmdline += tmp; }
        tmp.sprintf(" -s %llu",sectorcount);
        cmdline += tmp;

        cmdline += " " + device->Name;

        QStringList a=pcbsd::Utils::runShellCommand(cmdline);

        if(processErrors(a,"gpart")) return false;


        if(part.isnewfsChecked()) {
            // WE NEED TO INITIALIZE A FILE SYSTEM IN THE DEVICE

            // SINCE GPART SUCCEEDED, IT MUST'VE OUTPUT THE NAME OF THE NEW DEVICE

            QStringList tmp=a.at(0).split(" ",QString::SkipEmptyParts);

            QString fstype=part.getnewFSType();

            if(fstype=="ufs") {

            cmdline="newfs /dev/"+ tmp.at(0);

            QStringList b=pcbsd::Utils::runShellCommand(cmdline);

            if(processErrors(b,"newfs")) return false;

            }

            if(fstype=="ntfs") {

                cmdline="mkntfs -F -Q  /dev/"+ tmp.at(0);

                QStringList b=pcbsd::Utils::runShellCommand(cmdline);

                // TODO: PROPER ERROR PROCESSING FOR MKNTFS
                //if(processErrors(b,"mkntfs")) return false;

            }

            if(fstype=="ext2" || fstype=="ext3" || fstype=="ext4") {

                cmdline="mke2fs ";
                cmdline+=" -t "+fstype + " ";
                cmdline+=" /dev/"+ tmp.at(0);

                QStringList b=pcbsd::Utils::runShellCommand(cmdline);

                // TODO: PROPER ERROR PROCESSING FOR MKE2FS
                //if(processErrors(b,"mkntfs")) return false;

            }

            if(fstype=="fat16") {

                cmdline="newfs_msdos -F 16 /dev/"+ tmp.at(0);

                QStringList b=pcbsd::Utils::runShellCommand(cmdline);

                // TODO: PROPER ERROR PROCESSING FOR newfs_msdos
                //if(processErrors(b,"mkntfs")) return false;

            }

            if(fstype=="fat32") {

                cmdline="newfs_msdos -F 32 /dev/"+ tmp.at(0);

                QStringList b=pcbsd::Utils::runShellCommand(cmdline);

                // TODO: PROPER ERROR PROCESSING FOR newfs_msdos
                //if(processErrors(b,"mkntfs")) return false;

            }


        }


        return true;

    }

    return false;
}
bool ZManagerWindow::deviceDestroyPartition(vdev_t* device)
{

    QMessageBox msg(QMessageBox::Warning,tr("Warning"),tr("This operation cannot be undone.\nOK to destroy the slice/partition?"),QMessageBox::Yes|QMessageBox::No);
    int result=msg.exec();

    if(result!=QMessageBox::Yes) return false;



   QString cmdline;
   QString tmp;
   cmdline="gpart delete ";
   tmp.sprintf(" -i %d",device->Index);
   cmdline+=tmp;
   cmdline += " " + getContainerDisk(device)->Name;

   QStringList a=pcbsd::Utils::runShellCommand(cmdline);

   if(processErrors(a,"gpart")) return false;

   return true;
}


bool ZManagerWindow::processErrors(QStringList& output,QString command)
{
    QStringList::const_iterator it=output.constBegin();
    QString start;
    QString errormsg;
    bool errorfound=false;

    start=command+":";

    while(it!=output.constEnd())
    {

        if((*it).startsWith(command)) {
            QString tmp=(*it);
            tmp.remove(0,command.length()+2);
            errormsg+= tmp + "\n";
            errorfound=true;
        }
        ++it;
    }

    if(errorfound) {
        QString msg(tr("An error was detected while executing '%1':\n\n"));
        msg=msg.arg(command);
        msg+=errormsg;
        QMessageBox err(QMessageBox::Warning,tr("Error report"),msg,QMessageBox::Ok,this);

        err.exec();

        return true;

    }

    return false;
}

bool ZManagerWindow::processzpoolErrors(QStringList& output)
{
    QStringList::const_iterator it=output.constBegin();
    QString errormsg;
    bool errorfound=false;

    while(it!=output.constEnd())
    {

        if(!(*it).isEmpty()) {
            QString tmp=(*it);
            errormsg+= tmp + "\n";
            errorfound=true;
        }
        ++it;
    }

    if(errorfound) {
        QString msg(tr("An error was detected while executing 'zpool':\n\n"));
        msg+=errormsg;
        QMessageBox err(QMessageBox::Warning,tr("Error report"),msg,QMessageBox::Ok,this);

        err.exec();

        return true;

    }

    return false;

}

bool ZManagerWindow::processzfsErrors(QStringList& output)
{
    QStringList::const_iterator it=output.constBegin();
    QString errormsg;
    bool errorfound=false;

    while(it!=output.constEnd())
    {

        if(!(*it).isEmpty()) {
            QString tmp=(*it);
            errormsg+= tmp + "\n";
            errorfound=true;
        }
        ++it;
    }

    if(errorfound) {
        QString msg(tr("An error was detected while executing 'zfs':\n\n"));
        msg+=errormsg;
        QMessageBox err(QMessageBox::Warning,tr("Error report"),msg,QMessageBox::Ok,this);

        err.exec();

        return true;

    }

    return false;

}


QString ZManagerWindow::getPoolProperty(zpool_t *pool,QString Property)
{
    zprop_t tmp;

    foreach(tmp,pool->Properties) {
        if(tmp.Name==Property) return tmp.Value;
    }

    return "";

}

void    ZManagerWindow::setPoolProperty(zpool_t *pool,QString Property,QString Value)
{
    QString cmdline="zpool set ";

    cmdline+=Property+"=\""+Value+"\"";

    cmdline+=" \""+pool->Name+"\"";

    QStringList a=pcbsd::Utils::runShellCommand(cmdline);

    if(!processzpoolErrors(a)) needRefresh=true;


}


void ZManagerWindow::zpoolCreate(bool b)
{

    Q_UNUSED(b);
    DialogNewPool dlg;

    dlg.setDevices(&Disks);
    dlg.setTitle(tr("Create new zpool"));

    int result=dlg.exec();

    if(result) {
    QString cmdline="zpool create ";

    cmdline+="\""+dlg.getName()+"\" ";

    cmdline+=dlg.getRaidType();

    QStringList vdev=dlg.getVdevList();

    QString arg;

    foreach( arg, vdev) cmdline+=" "+arg;

    QStringList a=pcbsd::Utils::runShellCommand(cmdline);

    if(!processzpoolErrors(a)) needRefresh=true;

    }
}

void ZManagerWindow::zpoolDestroy(bool b)
{
    Q_UNUSED(b);

    if(getPoolProperty(lastSelectedPool,"readonly")=="on") {
        QMessageBox msg(QMessageBox::Information,tr("Important information"),tr("The pool was imported in read-only mode, therefore attempting to destroy the pool will leave the pool in the state it was when imported, not necessarily destroyed."),QMessageBox::Ok);
        msg.exec();
    }

    QString cmdline="zpool destroy ";


    cmdline+="\""+lastSelectedPool->Name+"\"";

    // TODO: ASK USER FOR CONFIRMATION BEFORE DESTROYING ANYTHING!


//    qDebug() << cmdline;

    QStringList a=pcbsd::Utils::runShellCommand(cmdline);

    if(!processzpoolErrors(a)) needRefresh=true;

}

void ZManagerWindow::zpoolClear(bool b)
{
    Q_UNUSED(b);

    zpool_t *ptr=lastSelectedPool;
    QString cmdline;
    cmdline="zpool clear \""+ptr->Name+"\"";

    QStringList a=pcbsd::Utils::runShellCommand(cmdline);

    if(processzpoolErrors(a)) needRefresh=false;
    else needRefresh=true;


}



void ZManagerWindow::on_toolButton_clicked()
{
    lastSelectedPool=getZpoolbyName(ui->poolLabel->text());

    if(lastSelectedPool) {
        needRefresh=false;
        zpoolClear(false);
        if(needRefresh) refreshState();
    }

}




// ANALYZE THE VDEV, AND RETURN A SERIES OF FLAGS (ITEM_XXXXX) TO BE USED
// FOR EASIER HANDLING OF THE DIFFERENT KINDS OF VDEVS

int ZManagerWindow::zpoolGetVDEVType(vdev_t *dev)
{
    int result=0;
    if(getDiskbyName(dev->Name)!=NULL) result|=ITEM_ISDISK;

    if(dev->Name.startsWith("mirror")) result|=ITEM_ISMIRROR;
    if(dev->Name.startsWith("raidz")) result|=ITEM_ISRAIDZ;
    if(dev->Name.startsWith("logs")) result|=ITEM_ISLOG;
    if(dev->Name.startsWith("cache")) result|=ITEM_ISCACHE;
    if(dev->Name.startsWith("spares")) result|=ITEM_ISSPARE;


    if(dev->Status==STATE_OFFLINE) result |=ITEM_ISOFFLINE;
    if(dev->Status==STATE_DEGRADED) result |=ITEM_ISDEGRADED;
    if(dev->Status==STATE_FAULTED) result |=ITEM_ISFAULTED;
    if(dev->Status==STATE_REMOVED) result |=ITEM_ISREMOVED;
    if(dev->Status==STATE_UNAVAIL) result |=ITEM_ISUNAVAIL;



    vdev_t *parent=getContainerGroup(dev);


    // RETURN THE FLAGS FOR THIS DEVICE IN THE LOW BYTE, AND ITS IMMEDIATE PARENT
    if(parent==NULL) { return result | PARENT(ITEM_ISPOOL); }
    else return result | PARENT((zpoolGetVDEVType(parent) & ITEM_ALL) );
}


void ZManagerWindow::zpoolEditProperties(bool)
{
    DialogProp dlg;

    dlg.refreshList(lastSelectedPool);

    int result=dlg.exec();

    if(result==QDialog::Accepted) {
        // TODO: UPDATE ALL MODIFIED PROPERTIES
        QStringList props;
        QStringList vals;

        props=dlg.getAllChangedProps();

        vals=dlg.getAllChangedValues();

        QStringList::const_iterator itp=props.constBegin(),itv=vals.constBegin();

        while(itp!=props.constEnd()&&itv!=vals.constEnd()) {
            setPoolProperty(lastSelectedPool,(*itp),(*itv));

            ++itp;
            ++itv;
        }

        needRefresh=true;
    }
    return;
}

void ZManagerWindow::zpoolRemoveDevice(bool b)
{
    Q_UNUSED(b);

    QString cmdline="zpool remove ";

    cmdline+="\""+lastSelectedVdev->InPool+"\" "+lastSelectedVdev->Name;

    QStringList a=pcbsd::Utils::runShellCommand(cmdline);

    if(!processzpoolErrors(a)) needRefresh=true;


}

void ZManagerWindow::zpoolAttachDevice(bool b)
{
    Q_UNUSED(b);

    DialogNewPool dlg;

    dlg.setDevices(&Disks);

    dlg.setTitle(tr("Attach mirror devices to ")+lastSelectedVdev->Name);

    dlg.setName(lastSelectedVdev->InPool);

    dlg.setType(DialogNewPool::DISK_MIRROR);

    dlg.setNumDisks(1);

    int result=dlg.exec();

    if(result) {
    QString cmdline="zpool attach ";

    cmdline+="\""+dlg.getName()+"\" "+lastSelectedVdev->Name+" ";

    QStringList vdev=dlg.getVdevList();

    QString arg;

    foreach( arg, vdev) cmdline+=" "+arg;

    QStringList a=pcbsd::Utils::runShellCommand(cmdline);

    if(!processzpoolErrors(a)) needRefresh=true;

    }



    return;

}

void ZManagerWindow::zpoolDetachDevice(bool b)
{
    Q_UNUSED(b);

    QString cmdline="zpool detach ";

    cmdline+="\""+lastSelectedVdev->InPool+"\" "+lastSelectedVdev->Name;

    QStringList a=pcbsd::Utils::runShellCommand(cmdline);

    if(!processzpoolErrors(a)) needRefresh=true;


}



void ZManagerWindow::zpoolOfflineDevice(bool b)
{
    Q_UNUSED(b);

    QString cmdline="zpool offline ";

    cmdline+="\""+lastSelectedVdev->InPool+"\" "+lastSelectedVdev->Name;

    QStringList a=pcbsd::Utils::runShellCommand(cmdline);

    if(!processzpoolErrors(a)) needRefresh=true;

}

void ZManagerWindow::zpoolOnlineDevice(bool b)
{
    Q_UNUSED(b);

    QString cmdline="zpool online ";

    cmdline+="\""+lastSelectedVdev->InPool+"\" "+lastSelectedVdev->Name;

    QStringList a=pcbsd::Utils::runShellCommand(cmdline);

    if(!processzpoolErrors(a)) needRefresh=true;
}

void ZManagerWindow::zpoolScrub(bool b)
{
    Q_UNUSED(b);

    QString cmdline="zpool scrub ";

    cmdline+="\""+lastSelectedPool->Name+"\"";

    QStringList a=pcbsd::Utils::runShellCommand(cmdline);

    if(!processzpoolErrors(a)) needRefresh=true;

}

void ZManagerWindow::zpoolExport(bool b)
{
    Q_UNUSED(b);

    if(getPoolProperty(lastSelectedPool,"readonly")=="on") {
        QMessageBox msg(QMessageBox::Information,tr("Important information"),tr("The pool was imported in read-only mode, therefore attempting to export the pool will leave the pool in the state it was when imported, not necessarily exported."),QMessageBox::Ok);
        msg.exec();
    }

    QString cmdline="zpool export ";

    cmdline+="\""+lastSelectedPool->Name+"\"";

//    qDebug() << cmdline;

    QStringList a=pcbsd::Utils::runShellCommand(cmdline);

    if(!processzpoolErrors(a)) needRefresh=true;

}

void ZManagerWindow::zpoolImport(bool b)
{
    Q_UNUSED(b);

    DialogName dlg;

    if(lastSelectedPool->Status&STATE_DESTROYED)    dlg.setTitle(tr("Recover destroyed pool as..."));
        else dlg.setTitle(tr("Import pool as..."));


    QStringList usednames;

    zpool_t tmp;

    foreach(tmp, Pools) {
        if(!(tmp.Status&(STATE_DESTROYED|STATE_EXPORTED))) usednames.append(tmp.Name);
    }

    dlg.setForbiddenList(usednames);
    dlg.setName(lastSelectedPool->Name);

    dlg.showOptions(true);

    int result=dlg.exec();

    if(result==QDialog::Accepted) {

        if(lastSelectedPool->Status&STATE_DESTROYED) {
            // SHOW A WARNING

            QMessageBox msg(QMessageBox::Warning,tr("Warning"),tr("This pool had been destroyed, and its disks may have been reused. Attempting to recover will destroy any new data that could've been stored in the devices that were reused and cannot be recovered.\nProceed with recovery?"),QMessageBox::Yes|QMessageBox::No);
            int result=msg.exec();

            if(result!=QMessageBox::Yes) return;

        }

    QString cmdline="zpool import ";

    if(dlg.importReadOnly()) cmdline+=" -o readonly=on ";

    if(dlg.importSetAltRoot())
        cmdline+=" -R \""+dlg.getAltRoot()+"\" ";
    else if(!dlg.importAutomount()) {
        cmdline+=" -N ";
    }
    if(dlg.importForce()) cmdline+=" -f ";

    if(lastSelectedPool->Status&STATE_DESTROYED) cmdline+="-D ";

    cmdline+=lastSelectedPool->Type; //"\""+lastSelectedPool->Name+"\"";        // Type CONTAINS THE ID OF THE POOL, IT'S BETTER TO USE THE ID TO PREVENT PROBLEMS

    cmdline+=" \""+ dlg.getName()+"\"";

//    qDebug() << cmdline;


    QStringList a=pcbsd::Utils::runShellCommand(cmdline);

    if(!processzpoolErrors(a)) needRefresh=true;

    }

}

void ZManagerWindow::zpoolRename(bool b)
{
    Q_UNUSED(b);

    if(getPoolProperty(lastSelectedPool,"readonly")=="on") {
        QMessageBox msg(QMessageBox::Information,tr("Important information"),tr("The pool was imported in read-only mode, it cannot be renamed."),QMessageBox::Ok);
        msg.exec();
        return;
    }

    DialogName dlg;

    dlg.setTitle(tr("Rename pool"));

    dlg.setName(lastSelectedPool->Name);

    dlg.showOptions(false);

    QStringList usednames;

    zpool_t tmp;

    foreach(tmp, Pools) {
        if(!(tmp.Status&(STATE_DESTROYED|STATE_EXPORTED))) usednames.append(tmp.Name);
    }

    dlg.setForbiddenList(usednames);


    int result=dlg.exec();


    if(result==QDialog::Accepted) {

        QString id=getPoolProperty(lastSelectedPool,"guid");

        QString cmdline="zpool export ";

        cmdline+="\""+lastSelectedPool->Name+"\"";

//        qDebug() << cmdline;
    QStringList a=pcbsd::Utils::runShellCommand(cmdline);

    if(!processzpoolErrors(a)) {
        cmdline="zpool import -N "+id+" \""+dlg.getName()+"\"";

//        qDebug() << cmdline;
        a=pcbsd::Utils::runShellCommand(cmdline);

        if(!processzpoolErrors(a)) needRefresh=true;


    }

    }
}


void ZManagerWindow::zpoolAdd(bool b)
{
    Q_UNUSED(b);

    DialogNewPool dlg;

    dlg.setDevices(&Disks);

    dlg.setTitle(tr("Add more devices to zpool"));
    dlg.setName(lastSelectedPool->Name);

    if(lastSelectedPool->Type=="striped") dlg.setType(DialogNewPool::DISK_STRIPE);
    if(lastSelectedPool->Type=="mirror") {
        dlg.setType(DialogNewPool::DISK_MIRROR);
        dlg.setNumDisks(lastSelectedPool->VDevs[0].Partitions.count());
        }
    if(lastSelectedPool->Type=="raidz") {
        dlg.setType(DialogNewPool::DISK_RAIDZ);
        dlg.setNumDisks(lastSelectedPool->VDevs[0].Partitions.count());

    }
    if(lastSelectedPool->Type=="raidz2") {
        dlg.setType(DialogNewPool::DISK_RAIDZ2);
        dlg.setNumDisks(lastSelectedPool->VDevs[0].Partitions.count());
    }
    if(lastSelectedPool->Type=="raidz3") {
        dlg.setType(DialogNewPool::DISK_RAIDZ3);
        dlg.setNumDisks(lastSelectedPool->VDevs[0].Partitions.count());
    }


    int result=dlg.exec();

    if(result) {
    QString cmdline="zpool add ";

    cmdline+="\""+dlg.getName()+"\" ";

    cmdline+=dlg.getRaidType();

    QStringList vdev=dlg.getVdevList();

    QString arg;

    foreach( arg, vdev) cmdline+=" "+arg;

    QStringList a=pcbsd::Utils::runShellCommand(cmdline);

    if(!processzpoolErrors(a)) needRefresh=true;

    }



    return;
}

void ZManagerWindow::zpoolAddLog(bool b)
{
    Q_UNUSED(b);

    DialogNewPool dlg;

    dlg.setDevices(&Disks);

    dlg.setTitle(tr("Add log devices to zpool"));
    dlg.setName(lastSelectedPool->Name);

    dlg.setType(DialogNewPool::DISK_LOG);

    int result=dlg.exec();

    if(result) {
    QString cmdline="zpool add ";

    cmdline+="\""+dlg.getName()+"\" ";

    cmdline+=dlg.getRaidType();

    QStringList vdev=dlg.getVdevList();

    QString arg;

    foreach( arg, vdev) cmdline+=" "+arg;

    QStringList a=pcbsd::Utils::runShellCommand(cmdline);

    if(!processzpoolErrors(a)) needRefresh=true;

    }



    return;

}
void ZManagerWindow::zpoolAddCache(bool b)
{
    Q_UNUSED(b);

    DialogNewPool dlg;

    dlg.setDevices(&Disks);

    dlg.setTitle(tr("Add cache devices to zpool"));
    dlg.setName(lastSelectedPool->Name);

    dlg.setType(DialogNewPool::DISK_CACHE);

    int result=dlg.exec();

    if(result) {
    QString cmdline="zpool add ";

    cmdline+="\""+dlg.getName()+"\" ";

    cmdline+=dlg.getRaidType();

    QStringList vdev=dlg.getVdevList();

    QString arg;

    foreach( arg, vdev) cmdline+=" "+arg;

    QStringList a=pcbsd::Utils::runShellCommand(cmdline);

    if(!processzpoolErrors(a)) needRefresh=true;

    }



    return;

}
void ZManagerWindow::zpoolAddSpare(bool b)
{
    Q_UNUSED(b);

    DialogNewPool dlg;

    dlg.setDevices(&Disks);

    dlg.setTitle(tr("Add spare devices to zpool"));
    dlg.setName(lastSelectedPool->Name);

    dlg.setType(DialogNewPool::DISK_SPARE);

    int result=dlg.exec();

    if(result) {
    QString cmdline="zpool add ";

    cmdline+="\""+dlg.getName()+"\" ";

    cmdline+=dlg.getRaidType();

    QStringList vdev=dlg.getVdevList();

    QString arg;

    foreach( arg, vdev) cmdline+=" "+arg;

    QStringList a=pcbsd::Utils::runShellCommand(cmdline);

    if(!processzpoolErrors(a)) needRefresh=true;

    }



    return;

}


// PRINT SIZE IN BYTES, KBYTES , GBYTES OR TBYTES
QString printBytes(unsigned long long bytes,int unit)
{
    QString a;

    if(unit>4) unit=-1;

    // UNIT == -1 FOR AUTOMATIC SELECTION OF UNITS
    if(unit==-1) {
    if(bytes<2*1024) return a.sprintf("%llu bytes",bytes);
    if(bytes<2*1024*1024) return a.sprintf("%.2lf kB",((double)bytes)/1024);
    if(bytes<1*1024*1024*1024) return a.sprintf("%.2lf MB",((double)bytes)/(1024*1024));
    if(bytes<(1LL*1024LL*1024LL*1024LL*1024LL)) return a.sprintf("%.2lf GB",((double)bytes)/(1024*1024*1024));
    return a.sprintf("%.2lf TB",((double)bytes)/((double)1024.0*1024.0*1024.0*1024.0));
    }

    switch(unit)
    {
    case 1:
        return a.sprintf("%.2lf",((double)bytes)/1024);
    case 2:
        return a.sprintf("%.2lf",((double)bytes)/(1024*1024));
    case 3:
        return a.sprintf("%.2lf",((double)bytes)/(1024*1024*1024));
    case 4:
        return a.sprintf("%.2lf",((double)bytes)/((double)1024.0*1024.0*1024.0*1024.0));
    default:
        return a.sprintf("%llu",bytes);

    }

}


// GET PREFERRED UNITS FOR PRINTING
int printUnits(unsigned long long bytes)
{
    if(bytes<2*1024) return 0;
    if(bytes<2*1024*1024) return 1;
    if(bytes<1*1024*1024*1024) return 2;
    if(bytes<(1LL*1024LL*1024LL*1024LL*1024LL)) return 3;
    return 4;
}



void ZManagerWindow::on_dropDownButton_clicked()
{
    ui->fspoolList->setMaximumHeight(1<<24);
    ui->fspoolList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->dropDownButton->setEnabled(false);
    updateGeometry();
}

void ZManagerWindow::on_fspoolList_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);

    if(ui->fspoolList->maximumHeight()!=ui->fspoolList->minimumHeight()) {
    ui->fspoolList->setMaximumHeight(ui->fspoolList->minimumHeight());
    ui->fspoolList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    updateGeometry();
    ui->dropDownButton->setEnabled(true);
    }
    else on_dropDownButton_clicked();

}


zfs_t *ZManagerWindow::getFileSystembyPath(QString path, int index)
{

    if(index>=0) return (zfs_t *)&(this->FileSystems.at(index));

    QList<zfs_t>::const_iterator it=this->FileSystems.constBegin();

    while(it!=this->FileSystems.constEnd())
    {
        if((*it).FullPath==path) {
            return (zfs_t *)&(*it);
        }
        ++it;
    }

    return NULL;

}


QTreeWidgetItem *ZManagerWindow::getParentFileSystem(QString path)
{
    QTreeWidgetItemIterator it(ui->fsList);
    int len=-1;
    QTreeWidgetItem *ptr=NULL;

    while(*it) {
        if(path.startsWith((*it)->text(0))) {
            if((*it)->text(0).length()>len) {
                len=(*it)->text(0).length();
                ptr=(*it);
            }
        }
        ++it;
    }

    if(ptr) return ptr;
    return NULL;
}

void ZManagerWindow::on_fspoolList_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);



//    ui->fsList->header()->setStretchLastSection(false);
//    ui->fsList->header()->setResizeMode(0,QHeaderView::Stretch);
//    ui->fsList->header()->setResizeMode(1,QHeaderView::ResizeToContents);



    ui->fsList->clear();
    if(current==NULL) return;
    QString pool=current->text(0);
    QTreeWidgetItem *item,*parent;
    zprop_t *prop;

    QList<zfs_t>::const_iterator it=FileSystems.constBegin();

    while(it!=FileSystems.constEnd())
    {

        if( ((*it).FullPath.startsWith(pool+"/")) || ((*it).FullPath==pool))  {

            zprop_t *origin=getFileSystemProperty((zfs_t *)&(*it),"origin");
            if(origin && !origin->Value.isEmpty()) parent=getParentFileSystem(origin->Value);
            else parent=getParentFileSystem((*it).FullPath);
            if(parent) item=new QTreeWidgetItem(parent);
            else item=new QTreeWidgetItem(ui->fsList);

        item->setText(0,(*it).FullPath);
        if((*it).FullPath==pool) item->setIcon(0,QIcon(":/icons/server-database.png"));
            else {
        prop=getFileSystemProperty((zfs_t *)&(*it),"type");
        if(prop)
        {
            if(prop->Value=="filesystem")  item->setIcon(0,QIcon(":/icons/kexi.png"));
            else if(prop->Value=="snapshot") item->setIcon(0,QIcon(":/icons/camera-photo.png"));
            else item->setIcon(0,QIcon(":/icons/kdf.png"));
        }
        }

        QString state;
        prop=getFileSystemProperty((zfs_t *)&(*it),"mounted");

        if(prop && prop->Value=="yes")  {
            state=tr("[Mounted]");
            prop=getFileSystemProperty((zfs_t *)&(*it),"mountpoint");
            if(prop) state+=" "+prop->Value;
        }
        else state=tr("[Not Mounted]");

        state+="\n";

        prop=getFileSystemProperty((zfs_t *)&(*it),"used");

        if(prop) state+=prop->Value;

        prop=getFileSystemProperty((zfs_t *)&(*it),"available");

        if(prop) state+=tr(" of ")+prop->Value;


        item->setText(1,state);



        // TODO: SHOW OTHER STATE INFO
        }
        ++it;
    }


    // NEED TO DO A SECOND PASS DUE TO ITEMS NOT BEING IN PROPER DEPENDENCY ORDER

    it=FileSystems.constBegin();


    while(it!=FileSystems.constEnd())
    {

        if( ((*it).FullPath.startsWith(pool+"/")) || ((*it).FullPath==pool)) {

            zprop_t *origin=getFileSystemProperty((zfs_t *)&(*it),"origin");
            if(origin && !origin->Value.isEmpty()) parent=getParentFileSystem(origin->Value);
            else parent=NULL;
            if(parent) {
                QTreeWidgetItem *item=getParentFileSystem((*it).FullPath);
                item->parent()->takeChild(item->parent()->indexOfChild(item));
                parent->addChild(item);
            }
        }
        ++it;
    }


    ui->fsList->expandAll();




}


zprop_t *ZManagerWindow::getFileSystemProperty(zfs_t *fs,QString prop)
{
    QList<zprop_t>::iterator it=fs->Properties.begin();

    while(it!=fs->Properties.end())
    {
        if((*it).Name==prop) return &(*it);
        ++it;
    }

    return NULL;
}


void ZManagerWindow::filesystemContextMenu(QPoint p)
{
    Q_UNUSED(p);

    struct zactions zfsmenu[]={
        /*  QString menutext        ,   int triggermask, triggerflags    ,  action_func slot */
        { QString(tr("Mount"))       ,    FSITEM_TYPEFS|FSITEM_ISMOUNTED, FSITEM_TYPEFS       ,  SLOT(fsMount(bool)) },
        { QString(tr("Unmount"))       ,    FSITEM_TYPEFS|FSITEM_ISMOUNTED, FSITEM_TYPEFS|FSITEM_ISMOUNTED       ,  SLOT(fsUnmount(bool)) },
        { QString(tr("Rename dataset"))    ,     FSITEM_NONE | FSITEM_ISROOTFS, 0         ,  SLOT(fsRename(bool)) },
        { QString(tr("Create new dataset"))    ,    FSITEM_TYPESNAP|FSITEM_TYPEVOL, 0         ,  SLOT(fsCreate(bool)) },
        { QString(tr("Create a clone dataset"))       ,    ~(FSITEM_TYPEFS|FSITEM_TYPESNAP|FSITEM_ISROOTFS|FSITEM_ISMOUNTED), 0         ,  SLOT(fsClone(bool)) },
        { QString(tr("Destroy dataset"))       ,    FSITEM_TYPEFS|FSITEM_NONE|FSITEM_ISROOTFS, FSITEM_TYPEFS         ,  SLOT(fsDestroy(bool)) },
        { QString(tr("Promote filesystem"))       ,    FSITEM_ALL, FSITEM_TYPEFS|FSITEM_ISCLONE         ,  SLOT(fsPromote(bool)) },
        { QString(tr("Take a snapshot"))       ,    FSITEM_TYPEFS, FSITEM_TYPEFS         ,  SLOT(fsSnapshot(bool)) },
        { QString(tr("Destroy snapshot"))       ,    FSITEM_TYPESNAP, FSITEM_TYPESNAP         ,  SLOT(fsDestroy(bool)) },
        { QString(tr("Rollback to this snapshot"))       ,    FSITEM_ALL, FSITEM_TYPESNAP       ,  SLOT(fsRollback(bool)) },
        { QString(tr("Edit properties"))       ,    FSITEM_NONE, 0       ,  SLOT(fsEditProps(bool)) },


// TODO: ADD MORE COMMANDS HERE
        { QString()       ,   0, 0      ,  NULL }

    };

    int flags=0;
    int idx;

    QMenu m(tr("zfs Menu"),this);

    QTreeWidgetItem *item=ui->fsList->itemAt(p);

    if(item!=NULL) {
        // FIRST DETERMINE THE FILESYSTEM OF THE ITEM
//        qDebug() << item->text(0);
        flags=0;
        if( (lastSelectedFileSystem=getFileSystembyPath(item->text(0)) )) {
            // YES, IT'S A VALID FILESYSTEM
            flags=getFileSystemFlags(lastSelectedFileSystem);
        }
        else {
            // THIS SHOULD NEVER HAPPEN!
            return;
        }
    } else { flags=FSITEM_NONE; lastSelectedFileSystem=NULL; }

    for(idx=0;zfsmenu[idx].slot!=NULL;++idx)
    {
        if( (flags & zfsmenu[idx].triggermask) == zfsmenu[idx].triggerflags) {

            QAction *act=m.addAction(zfsmenu[idx].menutext);
            connect(act,SIGNAL(triggered(bool)),this,zfsmenu[idx].slot);

        }
    }

    p.setX(p.x()+5);
    p.setY(p.y()+5);

    needRefresh=false;
    m.exec(ui->fsList->viewport()->mapToGlobal(p));

    if(needRefresh) {
        // REFRESH STATE BUT KEEP THE CURRENT POOL SELECTED
        QString currentPool=ui->fspoolList->currentItem()->text(0);
        refreshState();

        QTreeWidgetItemIterator it(ui->fspoolList);

        while(*it) {
            if((*it)->text(0)==currentPool) { ui->fspoolList->setCurrentItem((*it)); break; }
            ++it;
        }

    }


}

int ZManagerWindow::getFileSystemFlags(zfs_t *fs)
{
    int flags=0;
    zprop_t *prop;

    if(fs==NULL) return 0;

    if(getZpoolbyName(fs->FullPath)) flags|=FSITEM_ISROOTFS;

    prop=getFileSystemProperty(fs,"origin");
    if(prop && !prop->Value.isEmpty()) flags|=FSITEM_ISCLONE;

    prop=getFileSystemProperty(fs,"type");
    if(prop && prop->Value=="filesystem") flags|=FSITEM_TYPEFS;
    if(prop && prop->Value=="snapshot") flags|=FSITEM_TYPESNAP;
    if(prop && prop->Value=="volume") flags|=FSITEM_TYPEVOL;

    prop=getFileSystemProperty(fs,"mounted");
    if(prop && prop->Value=="yes") flags|=FSITEM_ISMOUNTED;


    // TODO: ADD MORE CASES HERE


    return flags;

}



void ZManagerWindow::fsCreate(bool b)
{
    Q_UNUSED(b);

    DialogfsCreate dlg;

    if(lastSelectedFileSystem) dlg.setRootPath(lastSelectedFileSystem->FullPath+"/",QString());
    else dlg.setRootPath(ui->fspoolList->currentItem()->text(0)+"/",QString());

    int result=dlg.exec();

    if(result==QDialog::Accepted) {
            // TODO: DO THE ACTUAL FILESYSTEM CREATION

        QString cmdline="zfs create ";


        QStringList optlist=dlg.getOptions();

        QString opt;

        foreach(opt,optlist) {
            cmdline+="-o "+opt+" ";
        }


        cmdline+=" \""+dlg.getPath()+"\"";

        QStringList a=pcbsd::Utils::runShellCommand(cmdline);

        if(!processzfsErrors(a)) needRefresh=true;

        }
        return;


}

void ZManagerWindow::fsDestroy(bool b)
{
    Q_UNUSED(b);

    if(!lastSelectedFileSystem)  return;


    QString cmdline="zfs destroy -n -v -R";

    cmdline+=" \""+lastSelectedFileSystem->FullPath+"\"";

    QStringList a=pcbsd::Utils::runShellCommand(cmdline);

    QString msg(tr("This operation cannot be undone and will cause data loss.\n\nYou are about to perform the following operation(s):\n\n"));

    QString str;

    foreach(str,a) {
        msg+=str.right(str.length()-6)+"\n";
    }

    msg+=tr("\n\nAre you sure you want to proceed?\n\n");

    QMessageBox err(QMessageBox::Warning,tr("Confirmation"),msg,QMessageBox::Yes | QMessageBox::No,this);
    err.setDefaultButton(QMessageBox::No);

    int result=err.exec();

    if(result==QMessageBox::Yes) {
    QString cmdline="zfs destroy -R";

    cmdline+=" \""+lastSelectedFileSystem->FullPath+"\"";

    QStringList b=pcbsd::Utils::runShellCommand(cmdline);


    if(!processzfsErrors(b)) needRefresh=true;
    }
}





void ZManagerWindow::fsSnapshot(bool b)
{
    Q_UNUSED(b);

    DialogfsCreate dlg;

    if(lastSelectedFileSystem) dlg.setRootPath(lastSelectedFileSystem->FullPath+"@",QString());
    else dlg.setRootPath(ui->fspoolList->currentItem()->text(0)+"@",QString());

    dlg.nameOnlyMode();
    dlg.changeTitle(tr("Take a new snapshot"));

    int result=dlg.exec();

    if(result==QDialog::Accepted) {

        QString cmdline="zfs snapshot ";

        cmdline+=" \""+dlg.getPath()+"\"";

        QStringList a=pcbsd::Utils::runShellCommand(cmdline);

        if(!processzfsErrors(a)) needRefresh=true;

        }
        return;



}
void ZManagerWindow::fsRename(bool b)
{
    Q_UNUSED(b);

    if(!lastSelectedFileSystem) return;

    DialogfsCreate dlg;

    bool isSnapshot=lastSelectedFileSystem->FullPath.contains("@");

    if(isSnapshot) {
        int pos=lastSelectedFileSystem->FullPath.lastIndexOf(QChar('@'));
        dlg.setRootPath(lastSelectedFileSystem->FullPath.left(pos+1),lastSelectedFileSystem->FullPath);

    } else {
        int pos=lastSelectedFileSystem->FullPath.lastIndexOf(QChar('/'));
        dlg.setRootPath(lastSelectedFileSystem->FullPath.left(pos+1),lastSelectedFileSystem->FullPath);
    }



    dlg.nameOnlyMode();
    dlg.changeTitle(tr("New name"));

    int result=dlg.exec();

    if(result==QDialog::Accepted) {

        QString cmdline="zfs rename ";

        cmdline+=" \""+lastSelectedFileSystem->FullPath+"\"";
        cmdline+=" \""+dlg.getPath()+"\"";

        QStringList a=pcbsd::Utils::runShellCommand(cmdline);

        if(!processzfsErrors(a)) needRefresh=true;

        }
        return;



}
void ZManagerWindow::fsPromote(bool b)
{
    Q_UNUSED(b);

    if(!lastSelectedFileSystem) return;

    QString cmdline="zfs promote ";

    cmdline+="\""+lastSelectedFileSystem->FullPath+"\"";

    QStringList a=pcbsd::Utils::runShellCommand(cmdline);

    if(!processzfsErrors(a)) needRefresh=true;

}
void ZManagerWindow::fsClone(bool b)
{
    Q_UNUSED(b);

    DialogfsCreate dlg;

    if(!lastSelectedFileSystem) return;

    bool isSnapshot=lastSelectedFileSystem->FullPath.contains("@");

    int pos=lastSelectedFileSystem->FullPath.lastIndexOf(QChar('/'));

    dlg.setRootPath(lastSelectedFileSystem->FullPath.left(pos+1),QString());

    int result=dlg.exec();

    if(result==QDialog::Accepted) {
            // TODO: DO THE ACTUAL FILESYSTEM CREATION

        QString cmdline="zfs clone ";


        QStringList optlist=dlg.getOptions();

        QString opt;

        foreach(opt,optlist) {
            cmdline+="-o "+opt+" ";
        }

        if(isSnapshot) cmdline+=" \""+lastSelectedFileSystem->FullPath+"\"";
        else {
            // IF THIS ISN'T A SNAPSHOT, WE NEED TO TAKE ONE NOW
            QString newname=dlg.getPath();
            QString snapname=lastSelectedFileSystem->FullPath+"@"+newname.right(newname.size()-pos-1)+"_base";
            QString cmd2="zfs snapshot \""+snapname+"\"";
            QStringList a=pcbsd::Utils::runShellCommand(cmd2);
            if(processzfsErrors(a)) return;

            cmdline+=" \""+snapname+"\"";
        }

        cmdline+=" \""+dlg.getPath()+"\"";

        QStringList a=pcbsd::Utils::runShellCommand(cmdline);

        if(!processzfsErrors(a)) needRefresh=true;

        }
        return;



}



void ZManagerWindow::fsRollback(bool b)
{
    Q_UNUSED(b);

    if(!lastSelectedFileSystem)  return;


    QString cmdline="zfs rollback -r -R";

    cmdline+=" \""+lastSelectedFileSystem->FullPath+"\"";

    QString msg(tr("This operation cannot be undone and will cause all data added after the snapshot to be lost.\n"
                   "Any snapshots created after this one will be deleted, along with any clone filesystems that depend on them.\n"));

    msg+=tr("\n\nAre you sure you want to proceed?\n\n");

    QMessageBox err(QMessageBox::Warning,tr("Confirmation"),msg,QMessageBox::Yes | QMessageBox::No,this);
    err.setDefaultButton(QMessageBox::No);

    int result=err.exec();

    if(result==QMessageBox::Yes) {

    QStringList b=pcbsd::Utils::runShellCommand(cmdline);


    if(!processzfsErrors(b)) needRefresh=true;
    }
}


void ZManagerWindow::fsMount(bool b)
{
    Q_UNUSED(b);

    if(!lastSelectedFileSystem)  return;


    QString cmdline="zfs mount";

    cmdline+=" \""+lastSelectedFileSystem->FullPath+"\"";

    QStringList a=pcbsd::Utils::runShellCommand(cmdline);


    if(!processzfsErrors(a)) needRefresh=true;


}

void ZManagerWindow::fsUnmount(bool b)
{
    Q_UNUSED(b);
    if(!lastSelectedFileSystem)  return;


    QString cmdline="zfs unmount";

    cmdline+=" \""+lastSelectedFileSystem->FullPath+"\"";

    QStringList a=pcbsd::Utils::runShellCommand(cmdline);


    if(!processzfsErrors(a)) needRefresh=true;

}


void ZManagerWindow::fsEditProps(bool)
{
    DialogFSProp dlg;

    dlg.setDataset(lastSelectedFileSystem);

    int result=dlg.exec();

    if(result==QDialog::Accepted) {
        // TODO: UPDATE ALL MODIFIED PROPERTIES
        QStringList props;
        QStringList vals;
        QList<int> flags;

        props=dlg.getAllChangedProps();

        flags=dlg.getAllChangedFlags();

        vals=dlg.getAllChangedValues();

        QStringList::const_iterator itp=props.constBegin(),itv=vals.constBegin();
        QList<int>::const_iterator itf=flags.constBegin();

        while(itp!=props.constEnd()&&itv!=vals.constEnd() && itf!=flags.constEnd()) {
            if((*itf)&PROP_INHERIT) inheritFSProperty(lastSelectedFileSystem,(*itp),dlg.applyRecursively());
            else setFSProperty(lastSelectedFileSystem,(*itp),(*itv));

            ++itp;
            ++itv;
            ++itf;
        }

        needRefresh=true;
    }
    return;
}


void    ZManagerWindow::setFSProperty(zfs_t *fs, QString Property, QString Value)
{
    QString cmdline="zfs set ";

    cmdline+=Property+"=\""+Value+"\"";

    cmdline+=" \""+fs->FullPath+"\"";

    QStringList a=pcbsd::Utils::runShellCommand(cmdline);

    if(!processzfsErrors(a)) needRefresh=true;


}


void    ZManagerWindow::inheritFSProperty(zfs_t *fs,QString Property,bool recursive)
{
    QString cmdline="zfs inherit ";

    if(recursive) cmdline+=" -r ";

    cmdline+=Property;

    cmdline+=" \""+fs->FullPath+"\"";

    QStringList a=pcbsd::Utils::runShellCommand(cmdline);

    if(!processzfsErrors(a)) needRefresh=true;


}


void ZManagerWindow::on_refreshButton_clicked()
{
 this->refreshState();
}
