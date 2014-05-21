
#include <QAction>
#include <QListWidgetItem>

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
    bool     misNotCheckInstalled;
}SDEEntry;

SDEEntry DEEntries[]=\
{{"System only", ":/images/system_only.png", NULL, QStringList()<<"---pcbsd---", true},
 {"All", ":/images/all_desktops.png", NULL, QStringList(), true},
 {"KDE", ":/images/kde.png", NULL, QStringList()<<"KDE", false},
 {"Gnome",":/images/gnome.png", NULL, QStringList()<<"Gnome", false},
 {"Cinnamon",":/images/cinnamon.png", NULL, QStringList()<<"Cinnamon", false},
 {"MATE",":/images/mate.png", NULL, QStringList()<<"Mate", false},
 {"XFCE",":/images/xfce.png", NULL, QStringList()<<"XFCE", false},
 {"LXDE",":/images/lxde.png", NULL, QStringList()<<"lxde", false},
 {"Lumina",":/images/lumina.png", NULL, QStringList()<<"Lumina", false},
};
const int DEntriesSize = sizeof(DEEntries)/sizeof(SDEEntry);

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

    for (int i=0; i<DEntriesSize; i++)
    {
        if (DEEntries[i].misNotCheckInstalled)
        {
            // This is special menu entry. We dont need to check installed desktops
            QAction* action = new QAction(QIcon(DEEntries[i].mIconPath), DEEntries[i].mDisplayName, this);
            DEEntries[i].mAction = action;
            continue;
        }
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
                    }
                    QAction* action = new QAction(QIcon(DEEntries[i].mIconPath), DisplayName, this);
                    DEEntries[i].mAction = action;
                }
            }//for all installed DEs
        }//for all DEs inside UI desktop entry
    }//for all desktop entries
    ui->DEChooserButton->setMenu(DEChoiseMenu);

    //----Add actions to menu ad connect signals
    for (int i=0; i<DEntriesSize; i++)
    {
        if (!DEEntries[i].mAction)
            continue;

        DEChoiseMenu->addAction(DEEntries[i].mAction);

        connect(DEEntries[i].mAction, SIGNAL(triggered(bool)), this, SLOT(slotDEChooserActionTriggered(bool)));
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
    //Fill widget
    QAutoExpandList* widget= itemsGroup->mListWidget;
    if (!widget)
        return;
    widget->clear();
    QVector<CControlPanelItem> items = itemsGroup->mItemGroup->items(mEnabledDEs, ui->filterEdit->text());
    for (int i=0; i<items.size(); i++)
    {
        QListWidgetItem* lw_item = new QListWidgetItem( items[i].displayIcon(), items[i].displayName(), widget);
        qDebug()<<items[i].displayComment();
        if ((widget->viewMode() == QListWidget::ListMode) && (items[i].displayComment().size()))
        {
            QString comment = items[i].displayComment();
            if (comment.toLower().trimmed() != items[i].displayName().toLower().trimmed())
            {
                lw_item->setText(QString("<b>") + items[i].displayName() + " (" + comment + ")");
            }
        }
        widget->addItem(lw_item);
    }
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
void MainWindow::slotDEChooserActionTriggered(bool checked)
{
    Q_UNUSED(checked);
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
        fillGroupWidget(&mItemGropus[i]);
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
