#include "kblayoutsetupwidget.h"
#include "ui_kblayoutsetupwidget.h"

#include "../keyboardsettings.h"

#include "../dialogs/kblayoutselectdialog.h"

#include <QTreeWidgetItem>

using namespace pcbsd::keyboard;

KbLayoutSetupWidget::KbLayoutSetupWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KbLayoutSetupWidget)
{
    ui->setupUi(this);
    cs = currentSettings();
    fillList();
}

KbLayoutSetupWidget::~KbLayoutSetupWidget()
{
    delete ui;
}

void KbLayoutSetupWidget::fillList()
{
    ui->layoutsTW->clear();

    ui->layoutsTW->header()->resizeSection(0,40);
    ui->layoutsTW->header()->resizeSection(1,40);

    for (int i=0; i<cs.layoutsCount(); i++)
    {
        Layout layout= cs.layout(i);
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->layoutsTW);

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

    }//for all current layouts

    ui->layoutsTW->setCurrentItem(ui->layoutsTW->topLevelItem(0));
}

void KbLayoutSetupWidget::on_addBtn_clicked()
{
    KbLayoutSelectDialog dlg;
    dlg.disableItemsFromSettings(cs);

    if (dlg.exec() == QDialog::Accepted)
    {
        cs.addLayout(dlg.selectedLayout());
        fillList();
    }
    ui->layoutsTW->setCurrentItem(ui->layoutsTW->topLevelItem(ui->layoutsTW->topLevelItemCount()-1));
}
