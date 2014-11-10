#include "kblayoutsetupwidget.h"
#include "ui_kblayoutsetupwidget.h"

#include "../keyboardsettings.h"

#include "../dialogs/kblayoutselectdialog.h"

#include <QTreeWidgetItem>
#include <QDebug>

using namespace pcbsd::keyboard;

Q_DECLARE_METATYPE(Layout);

///////////////////////////////////////////////////////////////////////////////
KbLayoutSetupWidget::KbLayoutSetupWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KbLayoutSetupWidget)
{
    ui->setupUi(this);
    fillList(currentSettings());
}

///////////////////////////////////////////////////////////////////////////////
KbLayoutSetupWidget::~KbLayoutSetupWidget()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////
void KbLayoutSetupWidget::mergeSettings(KeyboardSettings &ks)
{
    ks.clearLayouts();
    for(int i=0; i<ui->layoutsTW->topLevelItemCount(); i++)
    {
        QVariant udata = ui->layoutsTW->topLevelItem(i)->data(0,Qt::UserRole);
        ks.addLayout(udata.value<Layout>());
    }
}

///////////////////////////////////////////////////////////////////////////////
void KbLayoutSetupWidget::fillList(pcbsd::keyboard::KeyboardSettings cs)
{
    ui->layoutsTW->clear();

    ui->layoutsTW->header()->resizeSection(0,40);
    ui->layoutsTW->header()->resizeSection(1,40);

    for (int i=0; i<cs.layoutsCount(); i++)
    {
        Layout layout= cs.layout(i);
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->layoutsTW);

        fillItem(item, layout);

    }//for all current layouts

    ui->layoutsTW->setCurrentItem(ui->layoutsTW->topLevelItem(0));

    ui->removeBtn->setEnabled(ui->layoutsTW->topLevelItemCount()>1);
}

///////////////////////////////////////////////////////////////////////////////
QStringList KbLayoutSetupWidget::active()
{
    QStringList active;
    for(int i=0; i<ui->layoutsTW->topLevelItemCount(); i++)
    {
        QVariant udata = ui->layoutsTW->topLevelItem(i)->data(0,Qt::UserRole);
        active<<udata.value<Layout>().layout_id;
    }
    return active;
}

///////////////////////////////////////////////////////////////////////////////
void KbLayoutSetupWidget::fillItem(QTreeWidgetItem *item, Layout layout)
{
    QIcon flag(QString(":/images/flags/") + layout.layout_id + QString(".png"));
    item->setIcon(0, flag);


    item->setText(1, layout.layout_id);

    QString descr= layoutDescription(layout.layout_id);
    if (!descr.length())
        descr = layout.layout_id;
    item->setText(2, descr);

    descr = variantDescription(layout.layout_id, layout.variant_id);
    if (!descr.length())
        descr = layout.variant_id;
    item->setText(3, descr);

    QVariant udata;
    udata.setValue(layout);
    item->setData(0,Qt::UserRole, udata);
}

///////////////////////////////////////////////////////////////////////////////
void KbLayoutSetupWidget::moveItem(int idx, int off)
{
    QTreeWidgetItem* item = ui->layoutsTW->takeTopLevelItem(idx);
    ui->layoutsTW->insertTopLevelItem(idx + off, item);
    ui->layoutsTW->setCurrentItem(item);
}

///////////////////////////////////////////////////////////////////////////////
void KbLayoutSetupWidget::on_addBtn_clicked()
{
    KbLayoutSelectDialog dlg;

    // disable active items
    dlg.disableLayouts(active());

    if (dlg.exec() == QDialog::Accepted)
    {
        Layout layout = dlg.selectedLayout();
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->layoutsTW);

        fillItem(item, layout);

        ui->layoutsTW->setCurrentItem(item);
    }    
    ui->removeBtn->setEnabled(ui->layoutsTW->topLevelItemCount()>1);
}

///////////////////////////////////////////////////////////////////////////////
void KbLayoutSetupWidget::on_editBtn_clicked()
{
    QTreeWidgetItem* item = ui->layoutsTW->currentItem();
    if (!item)
        return;
    QVariant udata = item->data(0,Qt::UserRole);
    Layout curr= udata.value<Layout>();

    KbLayoutSelectDialog dlg;
    dlg.disableLayouts(active());
    dlg.setCurrent(curr.layout_id);

    if (dlg.exec() == QDialog::Accepted)
    {
        Layout layout = dlg.selectedLayout();
        fillItem(item, layout);
    }
}

///////////////////////////////////////////////////////////////////////////////
void KbLayoutSetupWidget::on_removeBtn_clicked()
{
    QTreeWidgetItem* item = ui->layoutsTW->currentItem();
    if (!item)
        return;
    if (ui->layoutsTW->topLevelItemCount()<2)
    {
        ui->removeBtn->setEnabled(false);
        return;
    }

    int idx = ui->layoutsTW->indexOfTopLevelItem(item);
    ui->layoutsTW->takeTopLevelItem(idx);

    delete item;

     ui->removeBtn->setEnabled(ui->layoutsTW->topLevelItemCount()>1);
}

///////////////////////////////////////////////////////////////////////////////
void KbLayoutSetupWidget::on_layoutsTW_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous)

    int idx = ui->layoutsTW->indexOfTopLevelItem(current);
    int last = ui->layoutsTW->topLevelItemCount()-1;
    ui->upBtn->setEnabled(idx>0);
    ui->downBtn->setEnabled(idx<last);
}

///////////////////////////////////////////////////////////////////////////////
void KbLayoutSetupWidget::on_upBtn_clicked()
{
    moveItem(ui->layoutsTW->indexOfTopLevelItem(ui->layoutsTW->currentItem()), -1);
}

///////////////////////////////////////////////////////////////////////////////
void KbLayoutSetupWidget::on_downBtn_clicked()
{
    moveItem(ui->layoutsTW->indexOfTopLevelItem(ui->layoutsTW->currentItem()), 1);
}
