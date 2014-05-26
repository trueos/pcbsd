
#include <QAction>
#include <QListWidgetItem>
#include <QToolTip>

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

///////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupDEChooser();
    setupGroups();

    QMenu* viewPopup= new QMenu(ui->viewModeButton);
    viewPopup->addAction(ui->actionNormal_icons);
    viewPopup->addAction(ui->actionLarge_icons);
    viewPopup->addSeparator();
    viewPopup->addAction(ui->actionGrid_view);
    viewPopup->addAction(ui->actionList_view);

    connect(ui->actionNormal_icons, SIGNAL(triggered()), this, SLOT(slotIconSizeActionTriggered()));
    connect(ui->actionLarge_icons, SIGNAL(triggered()), this, SLOT(slotIconSizeActionTriggered()));
    connect(ui->actionGrid_view,  SIGNAL(triggered()), this, SLOT(slotViewModeActionTriggered()));
    connect(ui->actionList_view,  SIGNAL(triggered()), this, SLOT(slotViewModeActionTriggered()));

    ui->viewModeButton->setMenu(viewPopup);

    QPalette tP;
    tP.setColor(QPalette::Inactive, QPalette::ToolTipBase, QColor("white"));
    tP.setColor(QPalette::Inactive, QPalette::ToolTipText, QColor("black"));
    QToolTip::setPalette(tP);
}

///////////////////////////////////////////////////////////////////////////////
MainWindow::~MainWindow()
{
    delete ui;
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

    if (!isCurrentFound)
    {
        ui->DEChooserButton->setIcon(QIcon(UNSUPPORTED_DE_ICON));
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
    mItemGropus[0]= SUIItemsGroup(&softwareItems, ui->softwareLW, ui->softwareGroupName);
    mItemGropus[1]= SUIItemsGroup(&systemItems, ui->systemLW, ui->systemGroupName);
    mItemGropus[2]= SUIItemsGroup(&hardwareItems, ui->hardwareLW, ui->hardwareGroupName);
    mItemGropus[3]= SUIItemsGroup(&networkingItems, ui->networkingLW, ui->networkingGroupName);
    mItemGropus[4]= SUIItemsGroup(&deItems, ui->deLW, ui->deGroupName);
    mItemGropus[5]= SUIItemsGroup(&toolsItems, ui->toolsLW, ui->toolsGroupName);

    for(int i=0; i<6; i++)
    {
        QAutoExpandList* widget = mItemGropus[i].mListWidget;
        if (!widget)
            continue;
        widget->setIconSize(QSize(32, 32));
        widget->setViewMode(QListView::IconMode);
        //widget->setViewMode(QListView::ListMode);
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

        mItemGropus[i].mItemGroup->readAssync();
    }
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

    for (int i=0; i<itemsGroup->mItems.size(); i++)
    {
        QListWidgetItem* lw_item = new QListWidgetItem( itemsGroup->mItems[i].displayIcon(),
                                                        itemsGroup->mItems[i].displayName(),
                                                        widget);

        QString comment = itemsGroup->mItems[i].displayComment();

        //If we in list mode - add comment to item text
        if ((widget->viewMode() == QListWidget::ListMode) && (itemsGroup->mItems[i].displayComment().size()))
        {
            if (comment.toLower().trimmed() != itemsGroup->mItems[i].displayName().toLower().trimmed())
            {
                lw_item->setText(itemsGroup->mItems[i].displayName() + " - " + comment);
            }
        }
        else
        {
            //If we in Grid mode add comment as tooltip
            lw_item->setToolTip(comment);
        }

        lw_item->setFlags((itemsGroup->mItems[i].matchWithFilter(ui->filterEdit->text())?lw_item->flags() | Qt::ItemIsEnabled
                                                                                        :lw_item->flags() & (~Qt::ItemIsEnabled)));
        //lw_item->setData(Qt::UserRole, QVariant(QVariant::UserType, &itemsGroup->mItems[i]));

        QVariant v;
        v.setValue(&itemsGroup->mItems[i]);
        lw_item->setData(Qt::UserRole, v);

        //void* ptr = lw_item->data(Qt::UserRole).value<CControlPanelItem*>();
        widget->addItem(lw_item);
    }

    QApplication::processEvents();
    widget->fitSize();
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
    QSize icon_size;
    QAction* source= (QAction*)QObject::sender();
    if (!source)
        return;
    if (source == ui->actionNormal_icons)
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
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::slotViewModeActionTriggered()
{
    QListView::ViewMode view_mode;
    QAction* source= (QAction*)QObject::sender();
    if (!source)
        return;
    if (source == ui->actionGrid_view)
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
        repaintGroupWidget(&mItemGropus[i]);
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
