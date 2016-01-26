/**************************************************************************
*   Copyright (C) 2014 by Yuri Momotyuk                                   *
*   yurkis@pcbsd.org                                                      *
*                                                                         *
*   Permission is hereby granted, free of charge, to any person obtaining *
*   a copy of this software and associated documentation files (the       *
*   "Software"), to deal in the Software without restriction, including   *
*   without limitation the rights to use, copy, modify, merge, publish,   *
*   distribute, sublicense, and/or sell copies of the Software, and to    *
*   permit persons to whom the Software is furnished to do so, subject to *
*   the following conditions:                                             *
*                                                                         *
*   The above copyright notice and this permission notice shall be        *
*   included in all copies or substantial portions of the Software.       *
*                                                                         *
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
*   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
*   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*
*   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
*   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
*   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
*   OTHER DEALINGS IN THE SOFTWARE.                                       *
***************************************************************************/

#include <QAction>
#include <QListWidgetItem>
#include <QToolTip>
#include <QDir>
#include <QPainter>

#include <unistd.h>
#include <sys/types.h>

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

    ui->refreshButton->setVisible(false); // Refresh button not work as expected now, so disable it

    mGroupsLoaded = 0;

    //Find DE icon file name for loading screen
    QString curr_de = pcbsd::Utils::currentDesktop().Name.toLower().trimmed();
    QString de_icon_file;
    for (int i=0; i<DEntriesSize; i++)
    {
        for (int j=0; j<DEEntries[i].mDENames.size(); j++)
        {
            if (DEEntries[i].mDENames[j].toLower().trimmed() == curr_de)
            {
                de_icon_file = DEEntries[i].mIconPath;
            }
        }//for all DE names
    }// for all entries
    if (!de_icon_file.length())
        de_icon_file = UNSUPPORTED_DE_ICON;

    // Setup items group
    mItemGropus[0]= SUIItemsGroup(&softwareItems, ui->softwareLW, ui->softwareGroupName, new QIcon(":/images/loading-software.png"), ui->loadingSoftware);
    mItemGropus[1]= SUIItemsGroup(&systemItems, ui->systemLW, ui->systemGroupName, new QIcon(":/images/loading-system.png"), ui->loadingSystem);
    mItemGropus[2]= SUIItemsGroup(&hardwareItems, ui->hardwareLW, ui->hardwareGroupName, new QIcon(":/images/loading-hardware.png"), ui->loadingHardware);
    mItemGropus[3]= SUIItemsGroup(&networkingItems, ui->networkingLW, ui->networkingGroupName, new QIcon(":/images/loading-network.png"), ui->loadingNetwork);
    mItemGropus[4]= SUIItemsGroup(&deItems, ui->deLW, ui->deGroupName,  new QIcon(de_icon_file), ui->loadingDE);
    mItemGropus[5]= SUIItemsGroup(&toolsItems, ui->toolsLW, ui->toolsGroupName, new QIcon(":/images/loading-tools.png"), ui->loadingTools);

    setupLoadingScreen();

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
    //delete ui;
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    saveSettings();
    //Stop all the extra QThreads
    //qDebug() << "Stopping Threads:";
    //qDebug() << " - softwareItems";
    softwareItems.exit();
    //qDebug() << " - systemItems";
    systemItems.exit();
    //qDebug() << " - hardwareItems";
    hardwareItems.exit();
    //qDebug() << " - networkingItems";
    networkingItems.exit();
    //qDebug() << " - deItems";
    deItems.exit();
    //qDebug() << " - toolsItems";
    toolsItems.exit();
    //qDebug() << " - done";
    exit(0); //Make sure to quit now - something in the backend destructors is causing a seg fault during close
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::setupLoadingScreen()
{
    ui->mainStack->setCurrentIndex(0);
    for (int i=0; i<6; i++)
    {
        mItemGropus[i].mLoadingIcon->setPixmap(mItemGropus[i].mGroupIcon->pixmap(QSize(64,64),QIcon::Disabled));
    }
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
                        setDEConfigAllIcon(QIcon(DEEntries[i].mIconPath));
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
    misSettingsFixedLayout = reader.value(CONFIG_FIXED_LAYOUT, QVariant(true)).toBool();

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

    writer.setValue(CONFIG_FIXED_LAYOUT, QVariant(ui->actionFixed_item_width->isChecked()));

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
void MainWindow::setDEConfigAllIcon(QIcon de_icon)
{
    QIcon btn_icon = de_icon;
    if (!btn_icon.availableSizes().size())
        return;
    QSize orig_size = btn_icon.availableSizes()[0]; //It should be loaded in read() and should have one size;
    int orig_h = orig_size.height();
    int orig_w = orig_size.width();

    QPixmap orig_pixmap =btn_icon.pixmap(orig_size);
    QPainter painter(&orig_pixmap);
    QPixmap mark;
    mark.load(":/images/config.png");
    //QRect draw_rect=QRect(orig_w - orig_w/2, orig_h - orig_h/2, orig_w/2, orig_h/2);
    //QRect draw_rect=QRect(0, 0, orig_w, orig_h);
    QRect draw_rect=QRect(orig_w - orig_w*0.7, orig_h - orig_h*0.7, orig_w*0.7, orig_h*0.7);
    painter.drawPixmap(draw_rect, mark);

    ui->DELaunchConfigApp->setIcon(QIcon(orig_pixmap));
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::onStartupFinished()
{
    ui->mainStack->setCurrentIndex(1);
    ui->actionFixed_item_width->setChecked(misSettingsFixedLayout);
    on_actionFixed_item_width_triggered();
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

    if (++mGroupsLoaded<5)
    {
        items_group->mLoadingIcon->setPixmap(items_group->mGroupIcon->pixmap(QSize(64,64),QIcon::Normal));
	QApplication::processEvents();
    }

    if (mGroupsLoaded == 5 )
    {
        onStartupFinished();
    }


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
                setDEConfigAllIcon(source->icon());
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
        backend_item->launch( !(pcbsd::Utils::currentDesktop().isXDG) );
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

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_DELaunchConfigApp_clicked()
{
    if (mEnabledDEs.size()<1)
        return;
    //find corresponding DE information
    for (int i=0; i<DEntriesSize; i++)
    {
        for (int j=0; j<DEEntries[i].mDENames.size(); j++ )
        {
            if (mEnabledDEs[0].trimmed().toLower() == DEEntries[i].mDENames[j].trimmed().toLower())
            {
                QProcess* proc = new QProcess();
                proc->startDetached(DEEntries[i].mDEInfo.ConfigurationApplication);
            }//if match
        }//for all DE names
    }//for all DEEntries
}
