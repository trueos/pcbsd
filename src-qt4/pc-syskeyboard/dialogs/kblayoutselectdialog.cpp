#include "kblayoutselectdialog.h"
#include "ui_kblayoutselectdialog.h"

#include <QIcon>

using namespace pcbsd::keyboard;

///////////////////////////////////////////////////////////////////////////////
KbLayoutSelectDialog::KbLayoutSelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KbLayoutSelectDialog)
{
    ui->setupUi(this);    
    fillList();
}

///////////////////////////////////////////////////////////////////////////////
KbLayoutSelectDialog::~KbLayoutSelectDialog()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////
void KbLayoutSelectDialog::disableItemsFromSettings(KeyboardSettings ks)
{
    settingsToDisable= ks;
    ui->layoutsTW->clear();
    fillList();
}

///////////////////////////////////////////////////////////////////////////////
void KbLayoutSelectDialog::setCurrent(QString layout_id)
{
    QList<QTreeWidgetItem*> items = ui->layoutsTW->findItems(layout_id,Qt::MatchExactly, 1);
    if (items.size() && items[0])
    {
        items[0]->setFlags(items[0]->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->layoutsTW->setCurrentItem(items[0]);
    }
}

///////////////////////////////////////////////////////////////////////////////
SLayout KbLayoutSelectDialog::selectedLayout()
{
    SLayout out;
    QTreeWidgetItem* item = ui->layoutsTW->currentItem();
    if (item)
    {
        out.layout_id = item->text(1);
        if (ui->variantsDB->currentIndex()>1)
        {
            QStringList variant_ids = possibleVariants(out.layout_id);
            out.variant_id = variant_ids[ui->variantsDB->currentIndex()-1];
        }
    }

    return out;
}

///////////////////////////////////////////////////////////////////////////////
void KbLayoutSelectDialog::fillList()
{
    QStringList layouts = possibleLayouts();

    for (int i=0 ; i<layouts.size(); i++)
    {
        bool disable = settingsToDisable.hasLayout(layouts[i]);

        QTreeWidgetItem* item = new QTreeWidgetItem;
        item->setText(1, layouts[i]);
        item->setText(2, layoutDescription(layouts[i]));
        QIcon flag(QString(":/images/flags/") + layouts[i] + QString(".png"));
        item->setIcon(0, flag);
        if (disable)
            item->setFlags(item->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEnabled));
        ui->layoutsTW->addTopLevelItem(item);
    }
}

///////////////////////////////////////////////////////////////////////////////
void KbLayoutSelectDialog::on_layoutsTW_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous)

    QString layout = current->text(1);    
    KeyboardSettings cs = currentSettings();
    QStringList variant_ids = possibleVariants(layout);

    //now fill variants

    ui->variantsDB->clear();
    ui->variantsDB->addItem(tr("Typical"));

    bool isCurr = cs.hasLayout(layout);

    for (int i=0; i<variant_ids.size(); i++)
    {
        QString descr = variantDescription(layout, variant_ids[i]);
        if (!descr.length())
            descr = variant_ids[i];
        ui->variantsDB->addItem(descr);
        if (isCurr && (cs.layout(layout).variant_id == variant_ids[i]))
            ui->variantsDB->setCurrentIndex(i+1);
    }

    bool isEnable = current->flags() & Qt::ItemIsEnabled;
    ui->buttonBox->setEnabled(isEnable);
}
