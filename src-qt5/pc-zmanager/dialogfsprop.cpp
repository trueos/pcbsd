#include "zmanagerwindow.h"
#include "dialogfsprop.h"
#include "ui_dialogfsprop.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTreeWidgetItemIterator>
#include <QComboBox>
#include <QColor>
#include <QMenu>
#include <QTimer>


DialogFSProp::DialogFSProp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFSProp)
{
    ui->setupUi(this);

    ui->propList->setIconSize(QSize(32,32));
    ui->propList->header()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->propList->header()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->propList->header()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    ui->propList->sortByColumn(0,Qt::AscendingOrder);
    InitAllProperties();
    on_propList_currentItemChanged(NULL,NULL);

}

DialogFSProp::~DialogFSProp()
{
    delete ui;
}


void DialogFSProp::InitAllProperties()
{
    // INITIALIZATION CANNOT BE STATIC DUE TO CALLS TO tr() FOR PROPER TRANSLATION
    // IT MUST BE DONE AT RUNTIME AND AS PART OF A CLASS BASED IN QOBJECT
    zproperty tmp;


    tmp.Name="aclinherit";
    tmp.Description=tr("Controls how ACL entries are inherited when files and directories are created. The values are\n"
                       "discard, noallow, restricted, and passthrough. For a description of these values, see ACL Properties.");
    tmp.Flags=PROP_ISOPTION;
    tmp.ValidOptions.clear();
    tmp.ValidOptions.append("discard");
    tmp.ValidOptions.append("noallow");
    tmp.ValidOptions.append("restricted");
    tmp.ValidOptions.append("passthrough");
    tmp.ValidOptions.append("passthrough-x");
    AllProperties.append(tmp);




//    aclmode

    tmp.Name="aclmode";
    tmp.Description=tr("Controls how an ACL entry is modified during a chmod operation.\n"
                       "The values are discard, groupmask, and passthrough. For a description of\n"
                       "these values, see ACL Properties.");
    tmp.Flags=PROP_ISOPTION;
    tmp.ValidOptions.clear();
    tmp.ValidOptions.append("discard");
    tmp.ValidOptions.append("groupmask");
    tmp.ValidOptions.append("passthrough");
    AllProperties.append(tmp);

//    atime

    tmp.Name="atime";
    tmp.Description=tr("Controls whether the access time for files is updated when they are read.\n"
                       "Turning this property off avoids producing write traffic when reading files and\n"
                       "can result in significant performance gains, though it might confuse mailers and\n"
                       "similar utilities.");
    tmp.Flags=PROP_ISOPTION;
    tmp.ValidOptions.clear();
    tmp.ValidOptions.append("on");
    tmp.ValidOptions.append("off");
    AllProperties.append(tmp);



//    available



    tmp.Name="available";
    tmp.Description=tr("Read-only property that identifies the amount of disk space available to a dataset\n"
                       "and all its children, assuming no other activity in the pool. Because disk space is\n"
                       "shared within a pool, available space can be limited by various factors including\n"
                       "physical pool size, quotas, reservations, and other datasets within the pool.");
    tmp.Flags=PROP_READONLY;
    tmp.ValidOptions.clear();
    AllProperties.append(tmp);




//    canmount


            tmp.Name="canmount";
            tmp.Description=tr("If this property is set to off, the file system cannot be mounted,\n"
                               "and is ignored by 'zfs mount -a'.  Setting this property to off is\n"
                               "similar to setting the mountpoint property to none, except that the\n"
                               "dataset still has a normal mountpoint property, which can be inherited\n"
                               "Setting this property to off allows datasets to be used solely as a\n"
                               "mechanism to inherit properties. One example of setting canmount=off is\n"
                               "to have two datasets with the same mountpoint, so that the children of\n"
                               "both datasets appear in the same directory, but might have different\n"
                               "inherited characteristics.");

            tmp.Flags=PROP_ISOPTION;
            tmp.ValidOptions.clear();
            tmp.ValidOptions.append("on");
            tmp.ValidOptions.append("off");
            tmp.ValidOptions.append("noauto");
            AllProperties.append(tmp);




//    checksum
            tmp.Name="checksum";
            tmp.Description=tr("Controls the checksum used to verify data integrity. The default\n"
                               "value is on, which automatically selects an appropriate algorithm\n"
                               "(currently, fletcher4, but this may change in future releases). The\n"
                               "value off disables integrity checking on user data. Disabling\n"
                               "checksums is NOT a recommended practice.");

            tmp.Flags=PROP_ISOPTION;
            tmp.ValidOptions.clear();
            tmp.ValidOptions.append("on");
            tmp.ValidOptions.append("off");
            tmp.ValidOptions.append("fletcher2");
            tmp.ValidOptions.append("fletcher4");
            AllProperties.append(tmp);


//    compression

            tmp.Name="compression";
            tmp.Description=tr("Enables or disables compression for a dataset. The values are on, off, lz4, lzjb, zle, gzip,\n"
                               "lz4 and gzip-N. Currently, setting this property to lz4, lzjb, gzip, or gzip-N has the same\n"
                               "effect as setting this property to on. Enabling compression on a file system with\n"
                               "existing data only compresses new data. Existing data remains uncompressed.");

            tmp.Flags=PROP_ISOPTION;
            tmp.ValidOptions.clear();
            tmp.ValidOptions.append("on");
            tmp.ValidOptions.append("off");
            tmp.ValidOptions.append("lz4");
            tmp.ValidOptions.append("lzjb");
            tmp.ValidOptions.append("zle");
            tmp.ValidOptions.append("gzip");
            tmp.ValidOptions.append("gzip-1");
            tmp.ValidOptions.append("gzip-2");
            tmp.ValidOptions.append("gzip-3");
            tmp.ValidOptions.append("gzip-4");
            tmp.ValidOptions.append("gzip-5");
            tmp.ValidOptions.append("gzip-6");
            tmp.ValidOptions.append("gzip-7");
            tmp.ValidOptions.append("gzip-8");
            tmp.ValidOptions.append("gzip-9");
            tmp.ValidOptions.append("lz4");

            AllProperties.append(tmp);

//    compressratio
            tmp.Name="compressratio";
            tmp.Description=tr("Read-only property that identifies the compression ratio achieved for a dataset,\n"
                               "expressed as a multiplier. Compression can be enabled by the zfs set compression=on\n"
                               "dataset command. The value is calculated from the logical size of all files and the\n"
                               "amount of referenced physical data. It includes explicit savings through the use of\n"
                               "the compression property.");

            tmp.Flags=PROP_READONLY;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);


//    copies


            tmp.Name="copies";
            tmp.Description=tr("Sets the number of copies of user data per file system. These copies\n"
                               "are in addition to any pool-level redundancy. Disk space used by\n"
                               "multiple copies of user data is charged to the corresponding file and\n"
                               "dataset, and counts against quotas and reservations. In addition, the\n"
                               "used property is updated when multiple copies are enabled. Consider setting\n"
                               "this property when the file system is created because changing this property\n"
                               "on an existing file system only affects newly written data.");

            tmp.Flags=PROP_ISOPTION;
            tmp.ValidOptions.clear();
            tmp.ValidOptions.append("1");
            tmp.ValidOptions.append("2");
            tmp.ValidOptions.append("3");
            AllProperties.append(tmp);




//    creation

           tmp.Name="creation";
           tmp.Description=tr("Read-only property that identifies the date and time that a dataset was created.");
           tmp.Flags=PROP_READONLY;
           tmp.ValidOptions.clear();
           AllProperties.append(tmp);


//    dedup
           tmp.Name="dedup";
           tmp.Description=tr("Configures deduplication for a dataset. The default value is off.\n"
                              "The default deduplication checksum is sha256 (this may change in the\n"
                              "future).  When dedup is enabled, the checksum defined here overrides\n"
                              "the checksum property. Setting the value to verify has the same\n"
                              "effect as the setting sha256,verify.\n"
                              "If set to verify, ZFS will do a byte-to-byte comparsion in case of\n"
                              "two blocks having the same signature to make sure the block contents\n"
                              "are identical.");
           tmp.Flags=PROP_ISOPTION;
           tmp.ValidOptions.clear();
           tmp.ValidOptions.append("on");
           tmp.ValidOptions.append("off");
           tmp.ValidOptions.append("verify");
           tmp.ValidOptions.append("sha256");
           AllProperties.append(tmp);



//    devices
            tmp.Name="devices";
            tmp.Description=tr("The devices property is currently not supported on FreeBSD.");
            tmp.Flags=PROP_ISOPTION;
            tmp.ValidOptions.clear();
            tmp.ValidOptions.append("on");
            tmp.ValidOptions.append("off");
            AllProperties.append(tmp);


//    exec
            tmp.Name="exec";
            tmp.Description=tr("Controls whether processes can be executed from within this file\n"
                               "system. The default value is on.");
            tmp.Flags=PROP_ISOPTION;
            tmp.ValidOptions.clear();
            tmp.ValidOptions.append("on");
            tmp.ValidOptions.append("off");
            AllProperties.append(tmp);



//    mounted

            tmp.Name="mounted";
            tmp.Description=tr("Read-only property that indicates whether a file system, clone, or snapshot\n"
                               "is currently mounted.");
            tmp.Flags=PROP_READONLY;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);


//    mountpoint


            tmp.Name="mountpoint";
            tmp.Description=tr("Controls the mount point used for this file system. When the mountpoint property\n"
                               "is changed for a file system, the file system and any descendents that inherit the\n"
                               "mount point are unmounted. If the new value is legacy, then they remain unmounted.\n"
                               "Otherwise, they are automatically remounted in the new location if the property was\n"
                               "previously legacy or none, or if they were mounted before the property was changed.\n"
                               "In addition, any shared file systems are unshared and shared in the new location.");
            tmp.Flags=PROP_ISPATH;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);



//    primarycache



            tmp.Name="primarycache";
            tmp.Description=tr("Controls what is cached in the primary cache (ARC). Possible values are all,\n"
                               "none, and metadata. If set to all, both user data and metadata are cached. If\n"
                               "set to none, neither user data nor metadata is cached. If set to metadata, only\n"
                               "metadata is cached.");
            tmp.Flags=PROP_ISOPTION;
            tmp.ValidOptions.clear();
            tmp.ValidOptions.append("none");
            tmp.ValidOptions.append("metadata");
            tmp.ValidOptions.append("all");
            AllProperties.append(tmp);


//    origin
            tmp.Name="origin";
            tmp.Description=tr("Read-only property for cloned file systems or volumes that identifies\n"
                               "the snapshot from which the clone was created. The origin cannot be\n"
                               "destroyed (even with the -r or -f option) as long as a clone exists.\n"
                               "Non-cloned file systems have an origin of none.");
            tmp.Flags=PROP_READONLY;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);



//    quota

            tmp.Name="quota";
            tmp.Description=tr("Limits the amount of disk space a dataset and its descendents can consume.\n"
                               "This property enforces a hard limit on the amount of disk space used,\n"
                               "including all space consumed by descendents, such as file systems and snapshots.\n"
                               "Setting a quota on a descendent of a dataset that already has a quota does not\n"
                               "override the ancestor's quota, but rather imposes an additional limit.");
            tmp.Flags=PROP_ISSTRING;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);

//    readonly
            tmp.Name="readonly";
            tmp.Description=tr("Controls whether a dataset can be modified. When set to on,\n"
                               "no modifications can be made.");
            tmp.Flags=PROP_ISOPTION;
            tmp.ValidOptions.clear();
            tmp.ValidOptions.append("on");
            tmp.ValidOptions.append("off");
            AllProperties.append(tmp);

//    recordsize
            tmp.Name="recordsize";
            tmp.Description=tr("Specifies a suggested block size for files in the file system. This\n"
                               "property is designed solely for use with database workloads that\n"
                               "access files in fixed-size records.  ZFS automatically tunes block\n"
                               "sizes according to internal algorithms optimized for typical access\n"
                               "patterns.\n"
                               "Use of this property for general purpose file systems is strongly\n"
                               "discouraged, and may adversely affect performance.\n"
                               "The size specified must be a power of two greater than or equal to\n"
                               "512 and less than or equal to 128 Kbytes.");
            tmp.Flags=PROP_ISNUMBER;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);





//    referenced
            tmp.Name="referenced";
            tmp.Description=tr("Read-only property that identifies the amount of data accessible by a\n"
                               "dataset, which might or might not be shared with other datasets in the\n"
                               "pool.");
            tmp.Flags=PROP_READONLY;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);


//    refquota
            tmp.Name="refquota";
            tmp.Description=tr("Sets the amount of disk space that a dataset can consume. This property\n"
                               "enforces a hard limit on the amount of space used. This hard limit does\n"
                               "not include disk space used by descendents, such as snapshots and clones.");
            tmp.Flags=PROP_ISNUMBER;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);

//    refreservation

            tmp.Name="refreservation";
            tmp.Description=tr("Sets the minimum amount of disk space that is guaranteed to a dataset, not\n"
                               "including descendents, such as snapshots and clones. When the amount of disk\n"
                               "space used is below this value, the dataset is treated as if it were taking up\n"
                               "the amount of space specified by refreservation. The refreservation reservation\n"
                               "is accounted for in the parent dataset's disk space used, and counts against\n"
                               "the parent dataset's quotas and reservations.\n"
                               "If refreservation is set, a snapshot is only allowed if enough free pool space\n"
                               "is available outside of this reservation to accommodate the current number of\n"
                               "referenced bytes in the dataset.");
            tmp.Flags=PROP_ISNUMBER;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);


//    reservation
            tmp.Name="reservation";
            tmp.Description=tr("Sets the minimum amount of disk space guaranteed to a dataset and its descendents.\n"
                               "When the amount of disk space used is below this value, the dataset is treated as if\n"
                               "it were using the amount of space specified by its reservation. Reservations are\n"
                               "accounted for in the parent dataset's disk space used, and count against the parent\n"
                               "dataset's quotas and reservations.");
            tmp.Flags=PROP_ISNUMBER;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);



//    secondarycache
            tmp.Name="secondarycache";
            tmp.Description=tr("Controls what is cached in the secondary cache (L2ARC). Possible values are all, none,\n"
                               "and metadata. If set to all, both user data and metadata are cached. If set to none,\n"
                               "neither user data nor metadata is cached. If set to metadata, only metadata is cached.");
            tmp.Flags=PROP_ISOPTION;
            tmp.ValidOptions.clear();
            tmp.ValidOptions.append("none");
            tmp.ValidOptions.append("metadata");
            tmp.ValidOptions.append("all");
            AllProperties.append(tmp);

//    setuid
            tmp.Name="setuid";
            tmp.Description=tr("Controls whether the setuid bit is honored in a file system.");
            tmp.Flags=PROP_ISOPTION;
            tmp.ValidOptions.clear();
            tmp.ValidOptions.append("on");
            tmp.ValidOptions.append("off");
            AllProperties.append(tmp);


//    sharenfs
            tmp.Name="sharenfs";
            tmp.Description=tr("Controls whether a file system is available over NFS and what\n"
                               "options are used. If set to on, the zfs share command is invoked\n"
                               "with no options. Otherwise, the zfs share command is invoked with\n"
                               "options equivalent to the contents of this property.");
            tmp.Flags=PROP_ISSTRING;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);

//    sharesmb
            tmp.Name="sharesmb";
            tmp.Description=tr("The sharesmb property has currently no effect on FreeBSD.");
            tmp.Flags=PROP_ISSTRING;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);



//    snapdir
            tmp.Name="snapdir";
            tmp.Description=tr("Controls whether the .zfs directory is hidden or visible in the root of the file system.");
            tmp.Flags=PROP_ISOPTION;
            tmp.ValidOptions.clear();
            tmp.ValidOptions.append("hidden");
            tmp.ValidOptions.append("visible");
            AllProperties.append(tmp);

//    type
    tmp.Name="type";
    tmp.Description=tr("Read-only property that identifies the dataset type as filesystem (file system or clone), volume, or snapshot.");
    tmp.Flags=PROP_READONLY;
    tmp.ValidOptions.clear();
    AllProperties.append(tmp);

//    used
            tmp.Name="used";
            tmp.Description=tr("Read-only property that identifies the amount of disk space consumed by a dataset and all its descendents.");
            tmp.Flags=PROP_READONLY;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);


//    usedbychildren
            tmp.Name="usedbychildren";
            tmp.Description=tr("Read-only property that identifies the amount of disk space that is used by\n"
                               "children of this dataset, which would be freed if all the dataset's children\n"
                               "were destroyed.");
            tmp.Flags=PROP_READONLY;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);

//    usedbydataset
            tmp.Name="usedbydataset";
            tmp.Description=tr("Read-only property that identifies the amount of disk space that is used by\n"
                               "a dataset itself, which would be freed if the dataset was destroyed, after\n"
                               "first destroying any snapshots and removing any refreservation reservations.");
            tmp.Flags=PROP_READONLY;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);

//    usedbyrefreservation
            tmp.Name="usedbyrefreservation";
            tmp.Description=tr("Read-only property that identifies the amount of disk space that is used by a\n"
                               "refreservation set on a dataset, which would be freed if the refreservation was\n"
                               "removed.");
            tmp.Flags=PROP_READONLY;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);
//    usedbysnapshots
            tmp.Name="usedbysnapshots";
            tmp.Description=tr("Read-only property that identifies the amount of disk space that is consumed\n"
                               "by snapshots of a dataset. In particular, it is the amount of disk space that\n"
                               "would be freed if all of this dataset's snapshots were destroyed. Note that\n"
                               "this value is not simply the sum of the snapshots' used properties, because\n"
                               "space can be shared by multiple snapshots.");
            tmp.Flags=PROP_READONLY;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);
//    version
            tmp.Name="version";
            tmp.Description=tr("Identifies the on-disk version of a file system, which is independent of the\n"
                               "pool version. This property can only be set to a later version that is available\n"
                               "from the supported software release.");
            tmp.Flags=PROP_READONLY;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);

//    volsize
            tmp.Name="volsize";
            tmp.Description=tr("For volumes, specifies the logical size of the volume.");
            tmp.Flags=PROP_ISNUMBER;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);

//    volblocksize
            tmp.Name="volblocksize";
            tmp.Description=tr("For volumes, specifies the block size of the volume.");
            tmp.Flags=PROP_READONLY;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);

//    jailed
            tmp.Name="jailed";
            tmp.Description=tr("Indicates whether a dataset has been added to a jail.");
            tmp.Flags=PROP_ISOPTION;
            tmp.ValidOptions.clear();
            tmp.ValidOptions.append("on");
            tmp.ValidOptions.append("off");
            AllProperties.append(tmp);

//    mlslabel
            tmp.Name="mlslabel";
            tmp.Description=tr("This property is not supported on FreeBSD.");
            tmp.Flags=PROP_READONLY;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);
//    nbmand
            tmp.Name="nbmand";
            tmp.Description=tr("This property is not supported on FreeBSD.");
            tmp.Flags=PROP_READONLY;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);
//    vscan
            tmp.Name="vscan";
            tmp.Description=tr("This property is not supported on FreeBSD.");
            tmp.Flags=PROP_READONLY;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);

//    xattr
            tmp.Name="xattr";
            tmp.Description=tr("This property is not supported on FreeBSD.");
            tmp.Flags=PROP_READONLY;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);
//    clones
            tmp.Name="clones";
            tmp.Description=tr("This property is not supported on FreeBSD.");
            tmp.Flags=PROP_READONLY;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);

//    defer_destroy
            tmp.Name="defer_destroy";
            tmp.Description=tr("The file system is marked to be destroyed after unmount.");
            tmp.Flags=PROP_READONLY;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);


//    refcompressratio
            tmp.Name="refcompressratio";
            tmp.Description=tr("The compression ratio achieved for the referenced space of this\n"
                               "dataset, expressed as a multiplier.");
            tmp.Flags=PROP_READONLY;
            tmp.ValidOptions.clear();
            AllProperties.append(tmp);

//    written
             tmp.Name="written";
             tmp.Description=tr("The amount of referenced space written to this dataset since the\n"
                                "previous snapshot.");
             tmp.Flags=PROP_READONLY;
             tmp.ValidOptions.clear();
             AllProperties.append(tmp);
//    userrefs
             tmp.Name="userrefs";
             tmp.Description=tr("This property is set to the number of user holds on this snapshot.\n");
             tmp.Flags=PROP_READONLY;
             tmp.ValidOptions.clear();
             AllProperties.append(tmp);

//    logbias
             tmp.Name="logbias";
             tmp.Description=tr("Provide a hint to ZFS about handling of synchronous requests in this\n"
                                "dataset.  If logbias is set to latency (the default), ZFS will use\n"
                                "pool log devices (if configured) to handle the requests at low\n"
                                "latency. If logbias is set to throughput, ZFS will not use configured\n"
                                "pool log devices.  ZFS will instead optimize synchronous operations\n"
                                "for global pool throughput and efficient use of resources.");
             tmp.Flags=PROP_ISOPTION;
             tmp.ValidOptions.clear();
             tmp.ValidOptions.append("latency");
             tmp.ValidOptions.append("throughput");
             AllProperties.append(tmp);
//    sync
             tmp.Name="sync";
             tmp.Description=tr("Controls the behavior of synchronous requests (e.g.  fsync(2),\n"
                                "O_DSYNC). This property accepts the following values:\n"
                                    "standard  This is the POSIX specified behavior of ensuring all\n"
                                    "          synchronous requests are written to stable storage and\n"
                                    "          all devices are flushed to ensure data is not cached by\n"
                                    "          device controllers (this is the default).\n"
                                    "always    All file system transactions are written and flushed\n"
                                    "          before their system calls return. This has a large\n"
                                    "          performance penalty.\n"
                                    "disabled  Disables synchronous requests. File system transactions\n"
                                    "          are only committed to stable storage periodically. This\n"
                                    "          option will give the highest performance.  However, it\n"
                                    "          is very dangerous as ZFS would be ignoring the synchro-\n"
                                    "          nous transaction demands of applications such as data-\n"
                                    "          bases or NFS.  Administrators should only use this\n"
                                    "          option when the risks are understood.");
             tmp.Flags=PROP_ISOPTION;
             tmp.ValidOptions.clear();
             tmp.ValidOptions.append("standard");
             tmp.ValidOptions.append("always");
             tmp.ValidOptions.append("disabled");
             AllProperties.append(tmp);
//    casesensitivity
             tmp.Name="casesensitivity";
             tmp.Description=tr("This property is not supported on FreeBSD.");
             tmp.Flags=PROP_READONLY;
             tmp.ValidOptions.clear();
             AllProperties.append(tmp);
//    normalization
             tmp.Name="normalization";
             tmp.Description=tr("This property cannot be changed after the system is created.");
             tmp.Flags=PROP_READONLY;
             tmp.ValidOptions.clear();
             AllProperties.append(tmp);
//    utf8only
             tmp.Name="utf8only";
             tmp.Description=tr("This property cannot be changed after the system is created.");
             tmp.Flags=PROP_READONLY;
             tmp.ValidOptions.clear();
             AllProperties.append(tmp);

}

QString DialogFSProp::getFSProperty(QString Property)
{
    zprop_t tmp;

    foreach(tmp,fsprops->Properties) {
        if(tmp.Name==Property) {
            if(tmp.Value=="-") return "";
            else return tmp.Value;
        }
    }

    return "";

}

QString DialogFSProp::getFSPropertySource(QString Property)
{
    zprop_t tmp;

    foreach(tmp,fsprops->Properties) {
        if(tmp.Name==Property) {
            return tmp.From;
        }
    }

    return "";

}


bool DialogFSProp::FSPropertyExists(QString Property)
{

    QList<zprop_t>::Iterator it=fsprops->Properties.begin();

    while(it!=fsprops->Properties.end())
    {
        if((*it).Name==Property) return true;
        ++it;
    }
    return false;

}




zproperty *DialogFSProp::getFSBaseProperty(QString Property)
{

    QList<zproperty>::Iterator it=AllProperties.begin();

    while(it!=AllProperties.end())
    {
        if((*it).Name==Property) return &(*it);
        ++it;
    }
    return NULL;

}


void DialogFSProp::setDataset(zfs_t *props)
{
    fsprops=props;
    refreshList();
}


void DialogFSProp::refreshList()
{

    // FIRST MERGE THE PROPERTIES LIST

    // ANY NATIVE PROPERTIES NOT IN fs.Properties ARE BECAUSE SOME PROPERTIES ARE SNAPSHOT ONLY, ETC AND NEED TO BE REMOVED FROM THE LIST
    int idx;

    for(idx=0;idx<AllProperties.size();++idx)
    {
        if(!FSPropertyExists(AllProperties[idx].Name) && !(AllProperties.at(idx).Flags&PROP_CREATE)) { AllProperties.removeAt(idx); --idx; }
    }

    // ANY USER PROPERTIES IN fs.Properties NOT IN AllProperties NEED TO BE ADDED


    for(idx=0;idx<fsprops->Properties.size();++idx)
    {
        if(getFSBaseProperty(fsprops->Properties.at(idx).Name)==NULL) {
            zproperty tmp;
            tmp.Name=fsprops->Properties[idx].Name;
            tmp.Flags=PROP_ISSTRING | (fsprops->Properties[idx].Source*PROP_SRCDEFAULT);
            tmp.ValidOptions.clear();
            if(tmp.Name.contains(":")) {
                tmp.Flags|=PROP_ISUSER;
                tmp.Description=tr("User Property");
            }
            else tmp.Description=tr("Unknown property or information not available.\nRead ZFS manual for more information.");
            AllProperties.append(tmp);
        }
    }

    zproperty pr;

    // PREVENT SIGNALS FROM BEING SENT
    ui->propList->blockSignals(true);


    foreach(pr,AllProperties)
    {
        QTreeWidgetItem *item=new QTreeWidgetItem(ui->propList);
        if(!item) return;
        item->setText(0,pr.Name);
        item->setToolTip(0,pr.Description);
        item->setData(0,Qt::UserRole,QVariant(pr.Name));    // SET DATA TO THE PROPERTY NAME

        if(pr.Flags&PROP_ISOPTION) {
            QComboBox *opt=new QComboBox();
            if(!opt) return;    // EXIT IF OUT OF MEMORY
            opt->setEditable(false);
            opt->addItems(pr.ValidOptions);
            opt->setItemData(0,QVariant(pr.Name));

            opt->setCurrentIndex(pr.ValidOptions.indexOf(QRegExp(getFSProperty(pr.Name))));
            ui->propList->setItemWidget(item,1,opt);
            connect(opt,SIGNAL(currentIndexChanged(int)),SLOT(on_propList_comboChanged(int)));
        }
        else{
        item->setText(1,getFSProperty(pr.Name));
        }
        if(pr.Flags&PROP_READONLY) {
            item->setIcon(0,QIcon(":/icons/user-busy-small-32.png"));
        }
        else {
            if(pr.Flags&PROP_ISUSER)
                item->setIcon(0,QIcon(":/icons/list-add-user-32.png"));
            else item->setIcon(0,QIcon(":/icons/user-online-small-32.png"));
            item->setFlags(item->flags()|Qt::ItemIsEditable);
        }

        item->setText(2,getFSPropertySource(pr.Name));



    }


    // RE-ENABLE SIGNALS
    ui->propList->blockSignals(false);


}

void DialogFSProp::on_propList_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

    if(column==0)
    {
        QString prop=item->data(0,Qt::UserRole).toString();
        if(prop.isEmpty()) ui->propList->editItem(item,0);
        else {
        if(item->text(0).contains(":"))  ui->propList->editItem(item,0);
            else ui->propList->editItem(item,1);
        }

    }
    else ui->propList->editItem(item,1);
}


QStringList DialogFSProp::getAllChangedProps()
{
    QStringList result;
    zproperty pr;

    foreach(pr,AllProperties)
    {
        if( !(pr.Flags&PROP_READONLY) && (pr.Flags&(PROP_CHANGED|PROP_CREATE|PROP_INHERIT)))
        {
            result.append(pr.Name);
        }
    }
    return result;
}

QList<int> DialogFSProp::getAllChangedFlags()
{
    QList<int> result;
    zproperty pr;

    foreach(pr,AllProperties)
    {
        if( !(pr.Flags&PROP_READONLY) && (pr.Flags&(PROP_CHANGED|PROP_CREATE|PROP_INHERIT)))
        {
            result.append(pr.Flags);
        }
    }
    return result;
}




QStringList DialogFSProp::getAllChangedValues()
{
    QStringList result;
    zproperty pr;

    foreach(pr,AllProperties)
    {
        if( !(pr.Flags&PROP_READONLY) && (pr.Flags&(PROP_CHANGED|PROP_CREATE|PROP_INHERIT)))
        {
            QTreeWidgetItemIterator it(ui->propList);
            while(*it) {
                if((*it)->text(0)==pr.Name) {
                    if(pr.Flags&PROP_ISOPTION) {
                        // READ IT FROM THE COMBO WIDGET
                        QComboBox *combo=(QComboBox *)ui->propList->itemWidget((*it),1);
                        result.append(combo->currentText());
                    }
                    else {
                        // READ VALUE DIRECTLY
                        result.append((*it)->text(1));
                    }
                }
                ++it;
            }
        }
    }
    return result;
}

void DialogFSProp::itemRevertChanges(QTreeWidgetItem *item,zproperty *prop)
{

    ui->propList->blockSignals(true);

    prop->Flags&=~(PROP_CHANGED|PROP_INHERIT);

    if(prop->Flags&PROP_ISOPTION) {
        // CHANGE TO THE PROPER OPTION IN THE COMBOBOX
        QComboBox *opt=(QComboBox *)ui->propList->itemWidget(item,1);
        opt->blockSignals(true);
        opt->clear();
        opt->addItems(prop->ValidOptions);
        opt->setItemData(0,QVariant(prop->Name));
        opt->setCurrentIndex(prop->ValidOptions.indexOf(QRegExp(getFSProperty(prop->Name))));
        opt->blockSignals(false);
    }
    else {
        item->setText(1,getFSProperty(prop->Name));
        item->setFlags(item->flags()|Qt::ItemIsEditable);
    }

    item->setText(2,getFSPropertySource(prop->Name));

    item->setBackgroundColor(0,Qt::white);
    item->setBackgroundColor(1,Qt::white);
    item->setBackgroundColor(2,Qt::white);

    ui->propList->blockSignals(false);

}

void DialogFSProp::itemInherit(QTreeWidgetItem *item,zproperty *prop)
{

    ui->propList->blockSignals(true);

    prop->Flags|=PROP_CHANGED|PROP_INHERIT;

    if(prop->Flags&PROP_ISOPTION) {
        // CHANGE TO THE PROPER OPTION IN THE COMBOBOX
        QComboBox *opt=(QComboBox *)ui->propList->itemWidget(item,1);
        //opt->setCurrentIndex(prop->ValidOptions.indexOf(QRegExp(getFSProperty(prop->Name))));
        opt->clear();
    }
    else item->setText(1,"");

    item->setFlags(item->flags()&~Qt::ItemIsEditable);

    ui->propList->blockSignals(false);

    on_propList_itemChanged(item,1);

}



void DialogFSProp::on_propList_itemChanged(QTreeWidgetItem *item, int column)
{

    QString prop=item->data(0,Qt::UserRole).toString();

    QList<zproperty>::iterator pr=AllProperties.begin();

    while(pr!=AllProperties.end())
    {
        if((*pr).Name==prop) {

            if(column==0) {
             // THE USER PROPERTY WAS RENAMED
                if(!((*pr).Flags&PROP_CREATE)) {
                    // PROPERTY WAS EXISTING, CANNOT BE RENAMED
                    item->setText(0,(*pr).Name);
                    return;
                }
                else {
                    // IT'S OK TO RENAME PROPERTIES THAT ARE ABOUT TO BE CREATED
                    if(item->text(0).contains(":")) {

                        // CHECK IF THE NEW NAME IS REPEATED
                        if(getFSBaseProperty(item->text(0))!=NULL) {
                            // A PROPERTY WITH THAT NAME ALREADY EXISTS
                            ui->propList->blockSignals(true);
                            item->setBackgroundColor(0,QColor(255,200,200));
                            item->setBackgroundColor(1,QColor(255,200,200));
                            item->setBackgroundColor(2,QColor(255,200,200));
                            item->setText(2,tr("[Invalid]"));
                            item->setData(0,Qt::UserRole,QVariant());
                            ui->propList->blockSignals(false);
                            QTimer::singleShot(20,this,SLOT(scrollToCurrentItemAndEdit()));
                            return;
                        }
                        (*pr).Name=item->text(0);

                    }



                    else {
                        // INVALID NAME, MARK IN RED AND RETURN
                        ui->propList->blockSignals(true);
                        item->setBackgroundColor(0,QColor(255,200,200));
                        item->setBackgroundColor(1,QColor(255,200,200));
                        item->setBackgroundColor(2,QColor(255,200,200));
                        item->setText(2,tr("[Invalid]"));
                        item->setData(0,Qt::UserRole,QVariant());
                        ui->propList->blockSignals(false);
                        QTimer::singleShot(20,this,SLOT(scrollToCurrentItemAndEdit()));

                        return;
                    }


                }
            }

            (*pr).Flags|=PROP_CHANGED;

            ui->propList->blockSignals(true);
            item->setBackgroundColor(0,QColor(255,240,200));
            item->setBackgroundColor(1,QColor(255,240,200));
            item->setBackgroundColor(2,QColor(255,240,200));
            if((*pr).Flags&PROP_INHERIT)  item->setText(2,tr("[Inherit]"));
            else if((*pr).Flags&PROP_CREATE) item->setText(2,tr("[Added]"));
                    else item->setText(2,tr("[Changed]"));
            ui->propList->blockSignals(false);

            QTimer::singleShot(20,this,SLOT(scrollToCurrentItem()));


            return;
        }
    ++pr;
    }

    // NO PROPERTY WAS FOUND! THIS CAN ONLY HAPPEN IMMEDIATELY AFTER A NEW PROPERTY IS CREATED

    if(column==0) {
        if(!item->text(0).contains(":"))  {
            // INVALID NAME, MARK IN RED AND RETURN
            ui->propList->blockSignals(true);
            item->setBackgroundColor(0,QColor(255,200,200));
            item->setBackgroundColor(1,QColor(255,200,200));
            item->setBackgroundColor(2,QColor(255,200,200));
            item->setText(2,tr("[Invalid]"));
            ui->propList->blockSignals(false);

            QTimer::singleShot(20,this,SLOT(scrollToCurrentItemAndEdit()));
            return;
        }

        // CHECK IF THE NEW NAME IS REPEATED
        if(getFSBaseProperty(item->text(0))!=NULL) {
            // A PROPERTY WITH THAT NAME ALREADY EXISTS
            ui->propList->blockSignals(true);
            item->setBackgroundColor(0,QColor(255,200,200));
            item->setBackgroundColor(1,QColor(255,200,200));
            item->setBackgroundColor(2,QColor(255,200,200));
            item->setText(2,tr("[Invalid]"));
            item->setData(0,Qt::UserRole,QVariant());
            ui->propList->blockSignals(false);
            QTimer::singleShot(20,this,SLOT(scrollToCurrentItemAndEdit()));
            return;
        }


        zproperty tmp;

        tmp.Name=item->text(0);
        tmp.Flags=PROP_ISSTRING|PROP_ISUSER|PROP_CREATE|PROP_CHANGED|PROP_SRCLOCAL;
        tmp.ValidOptions.clear();
        tmp.Description=tr("User property");

        AllProperties.append(tmp);

        ui->propList->blockSignals(true);
        item->setBackgroundColor(0,QColor(255,240,200));
        item->setBackgroundColor(1,QColor(255,240,200));
        item->setBackgroundColor(2,QColor(255,240,200));
        if((*pr).Flags&PROP_INHERIT)  item->setText(2,tr("[Inherit]"));
        else if((*pr).Flags&PROP_CREATE) item->setText(2,tr("[Added]"));
                else item->setText(2,tr("[Changed]"));

        item->setData(0,Qt::UserRole,QVariant(tmp.Name));

        ui->propList->blockSignals(false);

        QTimer::singleShot(20,this,SLOT(scrollToCurrentItem()));

    }

}


void DialogFSProp::on_propList_comboChanged(int index)
{
    Q_UNUSED(index);

    QComboBox *caller=(QComboBox *)sender();
    QString prop=caller->itemData(0).toString();

    QTreeWidgetItemIterator it(ui->propList);
    while((*it)) {
        if((*it)->text(0)==prop) { on_propList_itemChanged((*it),1); return; }
        ++it;
    }

    return;
}

void DialogFSProp::on_propList_customContextMenuRequested(const QPoint &pos)
{
    QMenu m(tr("Device Menu"),this);

    QTreeWidgetItem *item=ui->propList->itemAt(pos);

    zproperty *prop = getFSBaseProperty(item->text(0));

    m.addAction(tr("Add new user property"))->setData(QVariant(1));
    if(prop && ((prop->Flags&(PROP_ISUSER|PROP_CREATE))==(PROP_ISUSER|PROP_CREATE))) m.addAction(tr("Delete user property"))->setData(QVariant(2));
    if(prop && !(prop->Flags&PROP_READONLY) && !(prop->Flags&(PROP_SRCINHERIT|PROP_INHERIT))) {
        m.addAction(tr("Inherit from parent"))->setData(QVariant(3));
    }
    if(prop && (prop->Flags&(PROP_CHANGED))) {
        m.addAction(tr("Revert changes"))->setData(QVariant(4));
    }



    QAction *result=m.exec(ui->propList->viewport()->mapToGlobal(pos));

    if(result!=NULL) {
        int selected=result->data().toInt();
        switch(selected)
        {
        case 1:
            // ADD NEW PROPERTY
            itemNewProperty();
            on_propList_currentItemChanged(ui->propList->currentItem(),NULL);
            break;
        case 2:
            // REMOVE USER PROPERTY
            itemRemoveProperty();
            on_propList_currentItemChanged(ui->propList->currentItem(),NULL);

            break;
        case 3:
            // INHERIT FROM PARENT
            itemInherit(item,prop);
            on_propList_currentItemChanged(ui->propList->currentItem(),NULL);
            break;
        case 4:
            // REVERT LOCAL CHANGES
            itemRevertChanges(item,prop);
            on_propList_currentItemChanged(ui->propList->currentItem(),NULL);
            break;

        }
    }



}


void DialogFSProp::on_propList_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);

    if(current) {
        zproperty *prop=getFSBaseProperty(current->text(0));

        if(prop) {
            if((prop->Flags&(PROP_ISUSER|PROP_CREATE))==(PROP_ISUSER|PROP_CREATE)) ui->buttonRemove->setEnabled(true);
            else ui->buttonRemove->setEnabled(false);
            if(!(prop->Flags&(PROP_INHERIT|PROP_SRCINHERIT))&& !(prop->Flags&PROP_READONLY)) ui->buttonInherit->setEnabled(true);
            else ui->buttonInherit->setEnabled(false);
            if(prop->Flags&PROP_CHANGED) ui->buttonRevert->setEnabled(true);
            else ui->buttonRevert->setEnabled(false);
        }
    }
    else {
        ui->buttonRemove->setEnabled(false);
        ui->buttonInherit->setEnabled(false);
        ui->buttonRevert->setEnabled(false);
    }
}


void DialogFSProp::itemNewProperty()
{
    ui->propList->blockSignals(true);
    QTreeWidgetItem *item=new QTreeWidgetItem(ui->propList);
    item->setIcon(0,QIcon(":/icons/list-add-user-32.png"));
    item->setText(2,tr("[Added]"));
    item->setFlags(item->flags()|Qt::ItemIsEditable);
    ui->propList->setCurrentItem(item,0);

    ui->propList->blockSignals(false);

    ui->propList->editItem(item,0);


}

void DialogFSProp::scrollToCurrentItem()
{
    ui->propList->scrollToItem(ui->propList->currentItem(),QAbstractItemView::EnsureVisible);
}

void DialogFSProp::scrollToCurrentItemAndEdit()
{
    ui->propList->scrollToItem(ui->propList->currentItem(),QAbstractItemView::EnsureVisible);
    ui->propList->editItem(ui->propList->currentItem(),0);
}

void DialogFSProp::on_buttonAdd_clicked()
{
    itemNewProperty();
}


void DialogFSProp::itemRemoveProperty()
{

    if(!ui->propList->currentItem()) return;
    QString propname=ui->propList->currentItem()->data(0,Qt::UserRole).toString();

    delete ui->propList->currentItem();

    zproperty *prop=getFSBaseProperty(propname);

    int i;

    for(i=0;i<AllProperties.count();++i)
    {
        if(&(AllProperties[i])==prop) {
            AllProperties.removeAt(i);
            return;
        }
    }

}

void DialogFSProp::on_buttonRemove_clicked()
{
    itemRemoveProperty();

    on_propList_currentItemChanged(ui->propList->currentItem(),NULL);
}



void DialogFSProp::on_buttonRevert_clicked()
{
    if(!ui->propList->currentItem()) return;
    QString propname=ui->propList->currentItem()->data(0,Qt::UserRole).toString();

    zproperty *prop=getFSBaseProperty(propname);

    itemRevertChanges(ui->propList->currentItem(),prop);
    on_propList_currentItemChanged(ui->propList->currentItem(),NULL);

}

void DialogFSProp::on_buttonInherit_clicked()
{
    if(!ui->propList->currentItem()) return;
    QString propname=ui->propList->currentItem()->data(0,Qt::UserRole).toString();

    zproperty *prop=getFSBaseProperty(propname);

    itemInherit(ui->propList->currentItem(),prop);
    on_propList_currentItemChanged(ui->propList->currentItem(),NULL);

}


void DialogFSProp::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->buttonRole(button)==QDialogButtonBox::ApplyRole)
        this->accept();
}


bool        DialogFSProp::applyRecursively()
{
    return ui->recursiveCheck->isChecked();
}
