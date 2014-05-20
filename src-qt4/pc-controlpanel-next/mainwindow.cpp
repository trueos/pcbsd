#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "backend/cp-itemgroup.h"
#include "controls/qautoexpandlist.h"

#define ITEMS_PREFIX "/usr/local/share/pcbsd/pc-controlpanel/items/"

typedef struct _SItemsGroup
{
    CItemGroup*      mItemGroup;
    QAutoExpandList* mListWidget;
    QCheckBox*       mGroupNameWidget;
    _SItemsGroup(CItemGroup* pbackend=NULL, QAutoExpandList* lw=NULL,QCheckBox* capt=NULL)
    {
        mItemGroup = pbackend; mListWidget= lw; mGroupNameWidget= capt;
    }
}SItemsGroup;

CItemGroup softwareItems (ITEMS_PREFIX"software", "Software and updates");
CItemGroup systemItems (ITEMS_PREFIX"system", "System");

SItemsGroup ItemsGroups[4];

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupDEChooser()
{

}

void MainWindow::setupGroups()
{
    ItemsGroups[0]= SItemsGroup(&softwareItems, ui->softwareLW, ui->softwareGroupName);
}
