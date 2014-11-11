#include "kboptionswidget.h"
#include "ui_kboptionswidget.h"

#include <QTreeWidgetItem>

#include <QMap>

using namespace pcbsd::keyboard;

KbOptionsWidget::KbOptionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KbOptionsWidget)
{
    ui->setupUi(this);
    fillTree();
}

KbOptionsWidget::~KbOptionsWidget()
{
    delete ui;
}

void KbOptionsWidget::fillTree()
{
    KeyboardSettings ks = currentSettings();
    QMap<QString, QTreeWidgetItem*> grpRoot;

    QStringList grps = possibleOptionGroups();
    for(int i=0; i<grps.size(); i++)
    {
        //create group root
        QString grp_id = grps[i];
        QTreeWidgetItem* parrent = new QTreeWidgetItem();
        parrent->setText(0,optionGroupDescription(grp_id));
        ui->optionsTW->addTopLevelItem(parrent);

        //add options
        QStringList opts = possibleOptions(grp_id);
        for (int j=0; j<opts.size(); j++)
        {
            QString opt_id = opts[j];
            QString opt_descr = optionDescription(grp_id, opt_id);
            QTreeWidgetItem* option = new QTreeWidgetItem();
            option->setText(0, opt_descr);
            option->setFlags(option->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
            option->setCheckState(0,(ks.hasOption(Option(grp_id, opt_id)))?Qt::Checked:Qt::Unchecked);
            parrent->addChild(option);
        }
    }
}
