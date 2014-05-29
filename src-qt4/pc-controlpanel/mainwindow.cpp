
#include <QAction>
#include <QListWidgetItem>
#include <QToolTip>
#include <QDir>

#include <unistd.h>

#include "pcbsd-utils.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "backend/cp-itemgroup.h"
#include "controls/qautoexpandlist.h"

#define ITEMS_PREFIX "/usr/local/share/pcbsd/pc-controlpanel/items/"

CItemGroup softwareItems (ITEMS_PREFIX"software", "Software and updates");
CItemGroup systemItems (ITEMS_PREFIX"system", "System management");
CItemGroup hardwareItems (ITEMS_PREFIX"hardware", "Hardware");
CItemGroup networkingItems (ITEMS_PREFIX"network", "Hardware");
CItemGroup deItems (ITEMS_PREFIX"de", "Desktop environment");
CItemGroup toolsItems (ITEMS_PREFIX"tools", "Tools");

//! Structure that holds info about desktop environment related UI resources
typedef struct _SDEEntry
{
    QString mDisplayName;
    QString mIconPath;
    QAction* mAction;
    QStringList mDENames;
    pcbsd::DesktopEnvironmentInfo mDEInfo;
}SDEEntry;

SDEEntry DEEntries[]=\
{{"KDE", ":/images/kde.png", NULL, QStringList()<<"KDE", pcbsd::DesktopEnvironmentInfo()},
 {"Gnome",":/images/gnome.png", NULL, QStringList()<<"Gnome", pcbsd::DesktopEnvironmentInfo()},
 {"Cinnamon",":/images/cinnamon.png", NULL, QStringList()<<"Cinnamon", pcbsd::DesktopEnvironmentInfo()},
 {"MATE",":/images/mate.png", NULL, QStringList()<<"Mate", pcbsd::DesktopEnvironmentInfo()},
 {"XFCE",":/images/xfce.png", NULL, QStringList()<<"XFCE", pcbsd::DesktopEnvironmentInfo()},
 {"LXDE",":/images/lxde.png", NULL, QStringList()<<"lxde", pcbsd::DesktopEnvironmentInfo()},
 {"Lumina",":/images/lumina.png", NULL, QStringList()<<"Lumina", pcbsd::DesktopEnvironmentInfo()},
};
const int DEntriesSize = sizeof(DEEntries)/sizeof(SDEEntry);

static const QString UNSUPPORTED_DE_ICON = ":/images/unsupported_de.png";

Q_DECLARE_METATYPE(CControlPanelItem*);

#define CONFIG_FILE (QDir::homePath()+QString("/.config/PC-BSD/ControlPanel.conf"))
//const char* const CONFIG_FILE = "~/ControlPanel.conf";
const char* const CONFIG_GENERAL_GROUP = "General";
const char* const CONFIG_HIDE_DE_ITEMS = "HideDeItems";
const char* const CONFIG_VIEW_TYPE = "ViewType";
const char* const CONFIG_VIEW_TYPE_GRID = "Grid";
const char* const CONFIG_VIEW_TYPE_LIST = "List";
const char* const CONFIG_ICON_SIZE = "IconSize";
const char* const CONFIG_ICON_SIZE_NORMAL = "Normal";
const char* const CONFIG_ICON_SIZE_LARGE = "Large";
const char* const CONFIG_FIXED_LAYOUT = "FixedLayout";


///////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mItemGropus[0]= SUIItemsGroup(&softwareItems, ui->softwareLW, ui->softwareGroupName);
    mItemGropus[1]= SUIItemsGroup(&systemItems, ui->systemLW, ui->systemGroupName);
    mItemGropus[2]= SUIItemsGroup(&hardwareItems, ui->hardwareLW, ui->hardwareGroupName);
    mItemGropus[3]= SUIItemsGroup(&networkingItems, ui->networkingLW, ui->networkingGroupName);
    mItemGropus[4]= SUIItemsGroup(&deItems, ui->deLW, ui->deGroupName);
    mItemGropus[5]= SUIItemsGroup(&toolsItems, ui->toolsLW, ui->toolsGroupName);

    loadSettings();

    setupDEChooser();
    setupGroups();

    QMenu* viewPopup= new QMenu(ui->viewModeButton);
    viewPopup->addAction(ui->actionNormal_icons);
    viewPopup->addAction(ui->actionLarge_icons);
    viewPopup->addSeparator();
    viewPopup->addAction(ui->actionGrid_view);
    viewPopup->addAction(ui->actionList_view);
    viewPopup->addSeparator();
    viewPopup->addAction(ui->actionFixed_item_width);

    connect(ui->actionNormal_icons, SIGNAL(triggered()), this, SLOT(slotIconSizeActionTriggered()));
    connect(ui->actionLarge_icons, SIGNAL(triggered()), this, SLOT(slotIconSizeActionTriggered()));
    connect(ui->actionGrid_view,  SIGNAL(triggered()), this, SLOT(slotViewModeActionTriggered()));
    connect(ui->actionList_view,  SIGNAL(triggered()), this, SLOT(slotViewModeActionTriggered()));

    ui->viewModeButton->setMenu(viewPopup);

    QPalette tP;
    tP.setColor(QPalette::Inactive, QPalette::ToolTipBase, QColor("white"));
    tP.setColor(QPalette::Inactive, QPalette::ToolTipText, QColor("black"));
    QToolTip::setPalette(tP);

    mLastFilterLength= 0;
}

///////////////////////////////////////////////////////////////////////////////
MainWindow::~MainWindow()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::setupDEChooser()
{
    // Create actions
    QVector<pcbsd::DesktopEnvironmentInfo> installedDEs = pcbsd::Utils::installedDesktops();

    QMenu* DEChoiseMenu = new QMenu("", this);
    bool isCurrentFound= false;

    DEChoiseMenu->addAction(ui->actionSystem_only);
    DEChoiseMenu->addAction(ui->actionAll_desktops);

    for (int i=0; i<DEntriesSize; i++)
    {
        for (int j=0; j<DEEntries[i].mDENames.size(); j++)
        {
            for (int k=0; k<installedDEs.size(); k++)
            {
                if (DEEntries[i].mDENames[j].toLower().trimmed() == installedDEs[k].Name.toLower().trimmed())
                {
                    //----if DE is isntalled
                    QString DisplayName= DEEntries[i].mDisplayName;
                    //If current DE
                    if (installedDEs[k].isActive)
                    {
                        DisplayName+=QString(" ")+tr("(Current)");                        
                        mEnabledDEs<<installedDEs[k].Name;
                        ui->DEChooserButton->setIcon(QIcon(DEEntries[i].mIconPath));
                        ui->DELaunchConfigApp->setIcon(QIcon(DEEntries[i].mIconPath));
                        if (installedDEs[k].ConfigurationApplication.length())
                        {
                            ui->DELaunchConfigApp->setVisible(true);
                        }//if have config app
                        else
                        {
                            ui->DELaunchConfigApp->setVisible(false);
                        }//if not have config app
                        isCurrentFound= true;
                    }//if active DE

                    QAction* action = new QAction(QIcon(DEEntries[i].mIconPath), DisplayName, this);
                    DEEntries[i].mAction = action;
                    DEEntries[i].mDEInfo = installedDEs[k];
                }
            }//for all installed DEs
        }//for all DEs inside UI desktop entry
    }//for all desktop entries    
    ui->DEChooserButton->setMenu(DEChoiseMenu);

    if ((!isCurrentFound) && (!misSettingsSystemOnly))
    {
        ui->DEChooserButton->setIcon(QIcon(UNSUPPORTED_DE_ICON));
        ui->DELaunchConfigApp->setVisible(false);
    }

    if (misSettingsSystemOnly)
    {
        mEnabledDEs.clear();
        ui->DEChooserButton->setIcon(ui->actionSystem_only->icon());
        ui->DELaunchConfigApp->setVisible(false);
    }

    //----Add actions to menu ad connect signals
    for (int i=0; i<DEntriesSize; i++)
    {
        if (!DEEntries[i].mAction)
            continue;

        DEChoiseMenu->addAction(DEEntries[i].mAction);

        connect(DEEntries[i].mAction, SIGNAL(triggered()), this, SLOT(slotDEChooserActionTriggered()));
    }//for all DE entries
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::setupGroups()
{    
    bool isAbleToSU = checkUserGroup("wheel") | checkUserGroup("opearator");

    for(int i=0; i<6; i++)
    {
        QAutoExpandList* widget = mItemGropus[i].mListWidget;
        if (!widget)
            continue;        
        widget->setWordWrap(true);
        widget->setFrameStyle(QFrame::NoFrame);
        widget->setSortingEnabled(true);
        widget->setVisible(false);

        connect(widget, SIGNAL(itemActivated ( QListWidgetItem*)), this, SLOT(slotItemActivated(QListWidgetItem*)));

        mItemGropus[i].mGroupNameWidget->setVisible(false);

    }

    //Connect signals and read all items
    for (int i=0; i<6; i++)
    {
        if (!mItemGropus[i].mItemGroup)
            continue;

        connect(mItemGropus[i].mItemGroup, SIGNAL(itemsReady()), this, SLOT(slotItemsReady()), Qt::QueuedConnection);
        connect(mItemGropus[i].mGroupNameWidget, SIGNAL(stateChanged(int)), this, SLOT(slotGropTextStateChanged(int)));

        mItemGropus[i].mItemGroup->setSkipRootRequiredItems(!isAbleToSU);

        mItemGropus[i].mItemGroup->readAssync();
    }
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::loadSettings()
{
    QSettings reader(CONFIG_FILE, QSettings::IniFormat);
    qDebug()<<CONFIG_FILE;
    QString str;
    if (reader.status() != QSettings::NoError)
    {
        return;
    }
    reader.setIniCodec("UTF-8");

    reader.beginGroup(CONFIG_GENERAL_GROUP);

    misSettingsSystemOnly = reader.value(CONFIG_HIDE_DE_ITEMS, QVariant(false)).toBool();
    str = reader.value(CONFIG_ICON_SIZE, QVariant(CONFIG_ICON_SIZE_NORMAL)).toString();
    setBigIcons(str == CONFIG_ICON_SIZE_LARGE);
    str = reader.value(CONFIG_VIEW_TYPE, QVariant(CONFIG_VIEW_TYPE_GRID)).toString();
    setListMode(str == CONFIG_VIEW_TYPE_LIST);

}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::saveSettings()
{
    QSettings writer(CONFIG_FILE, QSettings::IniFormat);
    QString str;
    if (writer.status() != QSettings::NoError)
    {
        return;
    }

    writer.beginGroup(CONFIG_GENERAL_GROUP);

    writer.setValue(CONFIG_HIDE_DE_ITEMS, QVariant(mEnabledDEs.size() == 0));

    str= (ui->actionLarge_icons->isChecked())?CONFIG_ICON_SIZE_LARGE:CONFIG_ICON_SIZE_NORMAL;
    writer.setValue(CONFIG_ICON_SIZE, QVariant(str));

    str= (ui->actionList_view->isChecked())?CONFIG_VIEW_TYPE_LIST:CONFIG_VIEW_TYPE_GRID;
    writer.setValue(CONFIG_VIEW_TYPE, QVariant(str));

    writer.endGroup();
}

///////////////////////////////////////////////////////////////////////////////
bool MainWindow::checkUserGroup(QString groupName)
{
       QString loginName = getlogin();
       QStringList gNames;
       if ( loginName == "root" )
         return true;

       QString tmp;
       QFile iFile("/etc/group");
       if ( ! iFile.open(QIODevice::ReadOnly | QIODevice::Text))
         return true; //or FALSE?

       while ( !iFile.atEnd() ) {
         tmp = iFile.readLine().simplified();
         if ( tmp.indexOf(groupName) == 0 ) {
        gNames = tmp.section(":", 3, 3).split(",");
        break;
         }
       }
       iFile.close();

       for ( int i = 0; i < gNames.size(); ++i )
          if ( gNames.at(i).indexOf(loginName) == 0 )
                return true;

       return false;
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::fillGroupWidget(SUIItemsGroup *itemsGroup)
{
    QApplication::processEvents();
    itemsGroup->mListWidget->setEnabled(false);

    itemsGroup->mItems = itemsGroup->mItemGroup->items(mEnabledDEs, ui->filterEdit->text());

    itemsGroup->mListWidget->setEnabled(true);

    repaintGroupWidget(itemsGroup);
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::repaintGroupWidget(MainWindow::SUIItemsGroup *itemsGroup)
{
    QAutoExpandList* widget= itemsGroup->mListWidget;
    if (!widget)
        return;
    widget->clear();

    if (!itemsGroup->mItems.size())
    {
        itemsGroup->mListWidget->setVisible(false);
        itemsGroup->mGroupNameWidget->setVisible(false);
        return;
    }

    itemsGroup->mListWidget->setVisible(itemsGroup->mGroupNameWidget->isChecked());
    itemsGroup->mGroupNameWidget->setVisible(true);

    int disabled_count = 0;

    for (int i=0; i<itemsGroup->mItems.size(); i++)
    {
        QListWidgetItem* lw_item = new QListWidgetItem( itemsGroup->mItems[i].displayIcon(),
                                                        itemsGroup->mItems[i].displayName(),
                                                        widget);

        QString comment = itemsGroup->mItems[i].displayComment();
        QString item_text = itemsGroup->mItems[i].displayName();

        //If we in list mode - add comment to item text
        if ((widget->viewMode() == QListWidget::ListMode) && (itemsGroup->mItems[i].displayComment().size()))
        {
            if (comment.toLower().trimmed() != itemsGroup->mItems[i].displayName().toLower().trimmed())
            {
                item_text+=QString(" - ") + comment;
            }
        }
        else
        {
            //If we in Grid mode add comment as tooltip
            lw_item->setToolTip(comment);
        }

        //Check if multiple DEs selected and add DE name to the itetim text
        if ((mEnabledDEs.size()>1) && (itemsGroup->mItems[i].showIn().size()))
        {
            QString de_text;
            de_text= itemsGroup->mItems[i].showIn()[0];

            for (int k=1; k<itemsGroup->mItems[i].showIn().size(); k++)
            {
                de_text+=QString(",") + itemsGroup->mItems[i].showIn()[k];
            }
            item_text=QString("(")+de_text+") "+item_text;
        }

        lw_item->setText(item_text);
        bool is_enabled = itemsGroup->mItems[i].matchWithFilter(ui->filterEdit->text());

        lw_item->setFlags((is_enabled)?lw_item->flags() | Qt::ItemIsEnabled
                                     :lw_item->flags() & (~Qt::ItemIsEnabled));
        if (!is_enabled)
            disabled_count++;

        QVariant v;
        v.setValue(&itemsGroup->mItems[i]);
        lw_item->setData(Qt::UserRole, v);

        widget->addItem(lw_item);
    }

    if (ui->filterEdit->text().length())
    {
        if (disabled_count>=itemsGroup->mItems.size())
        {
            itemsGroup->mGroupNameWidget->setChecked(false);
        }
        else
        {
            itemsGroup->mGroupNameWidget->setChecked(true);
        }
    }

    QApplication::processEvents();
    widget->fitSize();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::setBigIcons(bool isBig)
{
    QSize icon_size;
    if (!isBig)
    {
        ui->actionNormal_icons->setChecked(true);
        ui->actionLarge_icons->setChecked(false);
        icon_size = QSize(32,32);
    }
    else
    {
        ui->actionNormal_icons->setChecked(false);
        ui->actionLarge_icons->setChecked(true);
        icon_size = QSize(64,64);
    }

    for (int i=0; i<6; i++)
    {
        mItemGropus[i].mListWidget->setIconSize(icon_size);
    }

    on_actionFixed_item_width_triggered();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::setListMode(bool isListMode)
{
    QListView::ViewMode view_mode;
    if (!isListMode)
    {
        ui->actionGrid_view->setChecked(true);
        ui->actionList_view->setChecked(false);
        view_mode= QListView::IconMode;
    }
    else
    {
        ui->actionGrid_view->setChecked(false);
        ui->actionList_view->setChecked(true);
        view_mode= QListView::ListMode;
    }
    for (int i=0; i<6; i++)
    {
        mItemGropus[i].mListWidget->setViewMode(view_mode);
        repaintGroupWidget(&mItemGropus[i]);
    }

    on_actionFixed_item_width_triggered();

}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::setFixedItemsLayout(bool isFixedLayout)
{
    int w = 0;
    int h = 0;

    for (int i=0; i<6; i++)
    {
        if (w < mItemGropus[i].mListWidget->sizeHintForColumn(mItemGropus[i].mListWidget->modelColumn()))
            w=  mItemGropus[i].mListWidget->sizeHintForColumn(mItemGropus[i].mListWidget->modelColumn());
        if (h < mItemGropus[i].mListWidget->sizeHintForRow(0))
            h = mItemGropus[i].mListWidget->sizeHintForRow(0);
    }

    for(int i=0; i<6; i++)
    {
        QSize grid_size;
        if (isFixedLayout && (!ui->actionList_view->isChecked()))
        {
           grid_size = QSize(w, h);

            qDebug()<<grid_size;
        }
        mItemGropus[i].mListWidget->setGridSize(grid_size);
    }
    ui->actionFixed_item_width->setChecked(isFixedLayout);
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::slotItemsReady()
{
    //Find corresponding ItemGroup
    SUIItemsGroup* items_group= NULL;
    CItemGroup *sender = (CItemGroup *)QObject::sender();
    for (int i=0; i<6; i++)
    {
        if (mItemGropus[i].mItemGroup == sender)
        {
            items_group = &mItemGropus[i];
            break;
        }
    }
    if (!items_group)
        return;

    fillGroupWidget(items_group);
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::slotDEChooserActionTriggered()
{
    //Find corresponding ItemGroup
    QAction* source= (QAction*)QObject::sender();
    for (int i=0; i<DEntriesSize; i++)
    {
        if (DEEntries[i].mAction == source)
        {
            mEnabledDEs= DEEntries[i].mDENames;

            ui->DEChooserButton->setIcon(source->icon());
            qDebug()<<DEEntries[i].mDEInfo.ConfigurationApplication;
            if (DEEntries[i].mDEInfo.ConfigurationApplication.length())
            {
                ui->DELaunchConfigApp->setIcon(source->icon());
                ui->DELaunchConfigApp->setVisible(true);
            }
            else
            {
                ui->DELaunchConfigApp->setVisible(false);
            }

            for (int j=0; j<6; j++)
            {
                fillGroupWidget(&mItemGropus[j]);
            }

        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::slotGropTextStateChanged(int state)
{
    //Find corresponding ItemGroup
    QCheckBox *sender = (QCheckBox *)QObject::sender();
    for (int i=0; i<6; i++)
    {
        if ((mItemGropus[i].mGroupNameWidget == sender)&&(mItemGropus[i].mListWidget))
        {
           mItemGropus[i].mListWidget->setVisible(state==Qt::Checked);           
           break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::slotIconSizeActionTriggered()
{    
    QAction* source= (QAction*)QObject::sender();
    if (!source)
        return;
    if (source == ui->actionNormal_icons)
    {
        setBigIcons(false);
    }
    else
    {
        setBigIcons(true);
    }    
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::slotViewModeActionTriggered()
{

    QAction* source= (QAction*)QObject::sender();
    if (!source)
        return;
    if (source == ui->actionGrid_view)
    {
        setListMode(false);
    }
    else
    {
        setListMode(true);
    }    
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_refreshButton_clicked()
{
    for (int i=0; i<6; i++)
    {
        if (mItemGropus[i].mItemGroup)
            mItemGropus[i].mItemGroup->readAssync();
    }
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_filterEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);

    for (int i=0; i<6; i++)
    {
        if (!mLastFilterLength)
        {
            mItemGropus[i].mStoredNameState = mItemGropus[i].mGroupNameWidget->isChecked();
        }

        if (!ui->filterEdit->text().length())
        {
            mItemGropus[i].mGroupNameWidget->setChecked(mItemGropus[i].mStoredNameState);
        }
        repaintGroupWidget(&mItemGropus[i]);
    }

    mLastFilterLength = ui->filterEdit->text().length();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::slotItemActivated(QListWidgetItem *item)
{
    CControlPanelItem* backend_item = (CControlPanelItem*)(item->data(Qt::UserRole).value<CControlPanelItem*>());
    if (backend_item)
        backend_item->launch();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionSystem_only_triggered()
{
    QAction* source= (QAction*)QObject::sender();
    if (!source)
        return;
    ui->DEChooserButton->setIcon(source->icon());
    ui->DELaunchConfigApp->setVisible(false);
    mEnabledDEs.clear();
    for (int i=0; i<6; i++)
        fillGroupWidget(&mItemGropus[i]);
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionAll_desktops_triggered()
{
    QAction* source= (QAction*)QObject::sender();
    if (!source)
        return;
    ui->DEChooserButton->setIcon(source->icon());
    ui->DELaunchConfigApp->setVisible(false);
    mEnabledDEs.clear();

    for (int i=0; i<DEntriesSize; i++)
    {
        mEnabledDEs+=DEEntries[i].mDENames;
    }

    for (int i=0; i<6; i++)
        fillGroupWidget(&mItemGropus[i]);
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::slotSingleInstance()
{
    this->hide();
    this->showNormal();
    this->activateWindow();
    this->raise();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionFixed_item_width_triggered()
{
    setFixedItemsLayout(ui->actionFixed_item_width->isChecked());
}