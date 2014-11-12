#include "kboptionswidget.h"
#include "ui_kboptionswidget.h"

#include <QTreeWidgetItem>

#include <QMap>

using namespace pcbsd::keyboard;

Q_DECLARE_METATYPE(Option);

KbOptionsWidget::KbOptionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KbOptionsWidget)
{
    ui->setupUi(this);
    fillTree(currentSettings());
}

void KbOptionsWidget::setSettings(KeyboardSettings ks)
{
    ui->optionsTW->clear();
    fillTree(ks);
}

void KbOptionsWidget::mergeSettings(KeyboardSettings &ks)
{
    ks.clearOptions();

    for(int i=0; i<ui->optionsTW->topLevelItemCount(); i++)
    {
        QTreeWidgetItem* grp_item = ui->optionsTW->topLevelItem(i);
        for(int j=0;j<grp_item->childCount(); j++)
        {
           QTreeWidgetItem* item = grp_item->child(j);
           if (item->checkState(0) == Qt::Checked)
           {
               QVariant udata = item->data(0,Qt::UserRole);
               ks.addOption(udata.value<Option>());
           }//if option is checked
        }//for all options
    }//for all groups
}

KbOptionsWidget::~KbOptionsWidget()
{
    delete ui;
}

void KbOptionsWidget::fillTree(KeyboardSettings ks)
{    
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
            QVariant udata;
            udata.setValue(Option(grp_id, opt_id));
            option->setData(0, Qt::UserRole, udata);
            parrent->addChild(option);
        }
    }
}
