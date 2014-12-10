#include "dialogprop.h"
#include "ui_dialogprop.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTreeWidgetItemIterator>
#include <QComboBox>
#include <QColor>

DialogProp::DialogProp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogProp)
{
    ui->setupUi(this);
    ui->propList->setIconSize(QSize(32,32));
    ui->propList->header()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->propList->header()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    InitAllProperties();

}

DialogProp::~DialogProp()
{
    delete ui;
}


void DialogProp::InitAllProperties()
{
    // INITIALIZATION CANNOT BE STATIC DUE TO CALLS TO tr() FOR PROPER TRANSLATION
    // IT MUST BE DONE AT RUNTIME AND AS PART OF A CLASS BASED IN QOBJECT
    zproperty tmp;

    // ADD ONE BY ONE ALL VALID PROPERTIES. THE FOLLOWING CORRESPOND TO ZFS VERSION 28



//     size
    tmp.Name="size";
    tmp.Description=tr("Total size of the storage pool.");
    tmp.Flags=PROP_READONLY;
    tmp.ValidOptions.clear();
    AllProperties.append(tmp);
//     capacity
    tmp.Name="capacity";
    tmp.Description=tr("Percentage of pool space used.");
    tmp.Flags=PROP_READONLY;
    tmp.ValidOptions.clear();
    AllProperties.append(tmp);
//     altroot
    tmp.Name="altroot";
    tmp.Description=tr("Alternate root directory. If set, this directory is prepended to any\n"
                       "mount points within the pool. This can be used when examining an\n"
                       "unknown pool where the mount points cannot be trusted, or in an\n"
                       "alternate boot environment, where the typical paths are not valid.\n"
                       "altroot is not a persistent property. It is valid only while the\n"
                       "system is up.  Setting altroot defaults to using cachefile=none, though\n"
                       "this may be overridden using an explicit setting.");
    tmp.Flags=PROP_READONLY;    // HAS TO BE SET ONLY WHILE IMPORTING
    tmp.ValidOptions.clear();
    AllProperties.append(tmp);



//     health

    tmp.Name="health";
    tmp.Description=tr("The current health of the pool.");
    tmp.Flags=PROP_READONLY;
    tmp.ValidOptions.clear();
    AllProperties.append(tmp);

//     guid

    tmp.Name="guid";
    tmp.Description=tr("A unique identifier for the pool.");
    tmp.Flags=PROP_READONLY;
    tmp.ValidOptions.clear();
    AllProperties.append(tmp);

//     version

    tmp.Name="version";
    tmp.Description=tr("The current on-disk version of the pool. This can be increased, but\n"
                       "never decreased. The preferred method of updating pools is with the\n"
                       "\"zpool upgrade\" command, though this property can be used when a\n"
                       "specific version is needed for backwards compatibility. This property\n"
                       "can be any number between 1 and the current version reported by\n"
                       "\"zpool upgrade -v\".");
    tmp.Flags=PROP_ISNUMBER;
    tmp.ValidOptions.clear();
    AllProperties.append(tmp);

//     bootfs
    tmp.Name="bootfs";
    tmp.Description=tr("Identifies the default bootable dataset for the root pool. This\n"
                       "property is expected to be set mainly by the installation and upgrade\n"
                       "programs.");
    tmp.Flags=PROP_ISPATH;
    tmp.ValidOptions.clear();
    AllProperties.append(tmp);



//     delegation

    tmp.Name="delegation";
    tmp.Description=tr("Controls whether a non-privileged user is granted access based on the\n"
                       "dataset permissions defined on the dataset. See zfs(8) for more\n"
                       "information on ZFS delegated administration.");
    tmp.Flags=PROP_ISOPTION;
    tmp.ValidOptions.clear();
    tmp.ValidOptions.append("on");
    tmp.ValidOptions.append("off");
    AllProperties.append(tmp);

//     autoreplace
    tmp.Name="autoreplace";
    tmp.Description=tr("Controls automatic device replacement. If set to \"off\", device\n"
                       "replacement must be initiated by the administrator by using the\n"
                       "\"zpool replace\" command. If set to \"on\", any new device, found in the\n"
                       "same physical location as a device that previously belonged to the\n"
                       "pool, is automatically formatted and replaced. The default behavior\n"
                       "is \"off\".  This property can also be referred to by its shortened\n"
                       "column name, \"replace\".");
    tmp.Flags=PROP_ISOPTION;
    tmp.ValidOptions.clear();
    tmp.ValidOptions.append("on");
    tmp.ValidOptions.append("off");
    AllProperties.append(tmp);
//     cachefile
    tmp.Name="cachefile";
    tmp.Description=tr("Controls the location of where the pool configuration is cached.\n"
                       "Discovering all pools on system startup requires a cached copy of the\n"
                       "configuration data that is stored on the root file system. All pools\n"
                       "in this cache are automatically imported when the system boots. Some\n"
                       "environments, such as install and clustering, need to cache this\n"
                       "information in a different location so that pools are not automatically\n"
                       "imported. Setting this property caches the pool configuration\n"
                       "in a different location that can later be imported with \"zpool import\n"
                       "-c\".  Setting it to the special value \"none\" creates a temporary pool\n"
                       "that is never cached, and the special value '' (empty string) uses\n"
                       "the default location.");
    tmp.Flags=PROP_ISPATH;
    tmp.ValidOptions.clear();
    AllProperties.append(tmp);


//     failmode

    tmp.Name="failmode";
    tmp.Description=tr("Controls the system behavior in the event of catastrophic pool failure\n"
                       "This condition is typically a result of a loss of connectivity\n"
                       "to the underlying storage device(s) or a failure of all devices\n"
                       "within the pool. The behavior of such an event is determined as follows\n"
                       "wait\n"
                       "Blocks all I/O access until the device connectivity is recovered\n"
                       "and the errors are cleared. This is the default behavior\n"
                       "continue\n"
                       "Returns EIO to any new write I/O requests but allows reads to\n"
                       "any of the remaining healthy devices. Any write requests that\n"
                       "have yet to be committed to disk would be blocked.\n\n"

                       "panic\n" 
                       "Prints out a message to the console and generates a system\n"
                       "crash dump.\n");
    tmp.Flags=PROP_ISOPTION;
    tmp.ValidOptions.clear();
    tmp.ValidOptions.append("wait");
    tmp.ValidOptions.append("continue");
    tmp.ValidOptions.append("panic");
    AllProperties.append(tmp);

//     listsnapshots

    tmp.Name="listsnapshots";
    tmp.Description=tr("Controls whether information about snapshots associated with this\n"
                       "pool is output when \"zfs list\" is run without the -t option. The\n"
                       "default value is off.");
    tmp.Flags=PROP_ISOPTION;
    tmp.ValidOptions.clear();
    tmp.ValidOptions.append("on");
    tmp.ValidOptions.append("off");
    AllProperties.append(tmp);

//     autoexpand

    tmp.Name="autoexpand";
    tmp.Description=tr("Controls automatic pool expansion when the underlying LUN is grown.\n"
                       "If set to \"on\", the pool will be resized according to the size of the\n"
                       "expanded device. If the device is part of a mirror or raidz then all\n"
                       "devices within that mirror/raidz group must be expanded before the\n"
                       "new space is made available to the pool. The default behavior is\n"
                       "\"off\".  This property can also be referred to by its shortened column\n"
                       "name, expand.");
    tmp.Flags=PROP_ISOPTION;
    tmp.ValidOptions.clear();
    tmp.ValidOptions.append("on");
    tmp.ValidOptions.append("off");
    AllProperties.append(tmp);


//     dedupditto

    tmp.Name="dedupditto";
    tmp.Description=tr("Threshold for the number of block ditto copies. If the reference\n"
                       "count for a deduplicated block increases above this number, a new\n"
                       "ditto copy of this block is automatically stored. Default setting is\n"
                       "0.");
    tmp.Flags=PROP_ISNUMBER;
    tmp.ValidOptions.clear();
    AllProperties.append(tmp);



//     dedupratio
    tmp.Name="dedupratio";
    tmp.Description=tr("The deduplication ratio specified for a pool, expressed as a multiplier. For\n"
                       "example, a value of 1.76 indicates that 1.76 units of data were stored but\n"
                       "only 1 unit of disk space was actually consumed. See zfs(8) for a description\n"
                       "of the deduplication feature.");
    tmp.Flags=PROP_READONLY;
    tmp.ValidOptions.clear();
    AllProperties.append(tmp);

//     free

    tmp.Name="free";
    tmp.Description=tr("Number of blocks within the pool that are not allocated.");
    tmp.Flags=PROP_READONLY;
    tmp.ValidOptions.clear();
    AllProperties.append(tmp);

//     allocated
    tmp.Name="allocated";
    tmp.Description=tr("Amount of storage space within the pool that has been physically allocated.");
    tmp.Flags=PROP_READONLY;
    tmp.ValidOptions.clear();
    AllProperties.append(tmp);
//     readonly

    tmp.Name="readonly";
    tmp.Description=tr("If set to on, pool will be imported in read-only mode with the\n"
                       "following restrictions:\n"

                       "  *   Synchronous data in the intent log will not be accessible\n"

                       "  *   Properties of the pool can not be changed\n"

                       "  *   Datasets of this pool can only be mounted read-only\n"

                       "  *   To write to a read-only pool, a export and import of the pool\n"
                       "      is required.\n");
    tmp.Flags=PROP_READONLY;    // CAN ONLY BE SET WHILE IMPORTING
    tmp.ValidOptions.clear();
    tmp.ValidOptions.append("on");
    tmp.ValidOptions.append("off");
    AllProperties.append(tmp);

//     comment

    tmp.Name="comment";
    tmp.Description=tr("An arbitrary comment string set by the administrator.");
    tmp.Flags=PROP_ISSTRING;
    tmp.ValidOptions.clear();
    AllProperties.append(tmp);


//     expandsize

    tmp.Name="expandsize";
    tmp.Description=tr("This property has currently no value on FreeBSD.");
    tmp.Flags=PROP_READONLY;
    tmp.ValidOptions.clear();
    AllProperties.append(tmp);

}

QString DialogProp::getPoolProperty(zpool_t *pool,QString Property)
{
    zprop_t tmp;

    foreach(tmp,pool->Properties) {
        if(tmp.Name==Property) {
            if(tmp.Value=="-") return "";
            else return tmp.Value;
        }
    }

    return "";

}

void DialogProp::refreshList(zpool_t *pool)
{

    zproperty pr;
    int index=0;

    // PREVENT SIGNALS FROM BEING SENT
    ui->propList->blockSignals(true);


    foreach(pr,AllProperties)
    {
        QTreeWidgetItem *item=new QTreeWidgetItem(ui->propList);
        if(!item) return;
        item->setText(0,pr.Name);
        item->setToolTip(0,pr.Description);
        if(pr.Flags&PROP_ISOPTION) {
            // TODO: ADD A DROPDOWN BOX HERE
            QComboBox *opt=new QComboBox();
            if(!opt) return;    // EXIT IF OUT OF MEMORY
            opt->setEditable(false);
            opt->addItems(pr.ValidOptions);
            opt->setItemData(0,QVariant(index));

            opt->setCurrentIndex(pr.ValidOptions.indexOf(QRegExp(getPoolProperty(pool,pr.Name))));
            ui->propList->setItemWidget(item,1,opt);
            connect(opt,SIGNAL(currentIndexChanged(int)),SLOT(on_propList_comboChanged(int)));
        }
        else{
        item->setText(1,getPoolProperty(pool,pr.Name));
        }
        if(pr.Flags&PROP_READONLY) {
            item->setIcon(0,QIcon(":/icons/user-busy-small-32.png"));
        }
        else {
            item->setIcon(0,QIcon(":/icons/user-online-small-32.png"));
            item->setFlags(item->flags()|Qt::ItemIsEditable);
        }


        ++index;

    }


    // RE-ENABLE SIGNALS
    ui->propList->blockSignals(false);


}

void DialogProp::on_propList_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

        ui->propList->editItem(item,1);
}


QStringList DialogProp::getAllChangedProps()
{
    QStringList result;
    zproperty pr;

    foreach(pr,AllProperties)
    {
        if( !(pr.Flags&PROP_READONLY) && (pr.Flags&PROP_CHANGED))
        {
            result.append(pr.Name);
        }
    }
    return result;
}

QStringList DialogProp::getAllChangedValues()
{
    QStringList result;
    zproperty pr;

    foreach(pr,AllProperties)
    {
        if( !(pr.Flags&PROP_READONLY) && (pr.Flags&PROP_CHANGED))
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

void DialogProp::on_propList_itemChanged(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

    QString prop=item->text(0);

    QList<zproperty>::iterator pr=AllProperties.begin();

    while(pr!=AllProperties.end())
    {
        if((*pr).Name==prop) { (*pr).Flags|=PROP_CHANGED; return; }
    ++pr;
    }
}


void DialogProp::on_propList_comboChanged(int index)
{
    Q_UNUSED(index);

    QComboBox *caller=(QComboBox *)sender();
    int idx=caller->itemData(0).toInt(),compare;

    QTreeWidgetItemIterator it(ui->propList);
    compare=0;
    while((*it)) {
        if(compare==idx) on_propList_itemChanged((*it),1);
        ++compare;
        ++it;
    }

    return;
}
