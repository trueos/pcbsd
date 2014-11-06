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
    isDisableCurrent = false;
    fillList();
}

///////////////////////////////////////////////////////////////////////////////
KbLayoutSelectDialog::~KbLayoutSelectDialog()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////
void KbLayoutSelectDialog::setDisableCurrent(bool isDisbale)
{
    isDisableCurrent = isDisbale;
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
        QStringList variant_ids = possibleVariants(out.layout_id);
        out.variant_id = variant_ids[ui->variantsDB->currentIndex()];
    }

    return out;
}

///////////////////////////////////////////////////////////////////////////////
void KbLayoutSelectDialog::fillList()
{
    QStringList layouts = possibleLayouts();
    LayoutsVector currLayouts = currentLeayouts();
    for (int i=0 ; i<layouts.size(); i++)
    {
        bool disable = false;
        if (isDisableCurrent)
        {
            for(int j=0; j<currLayouts.size(); j++)
            {
                if (layouts[i] == currLayouts[j].layout_id)
                    disable = true;
            }
        }
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
    //now fill variants
    LayoutsVector currLayouts = currentLeayouts();
    QStringList variant_ids = possibleVariants(layout);

    int currVariantIndex = 0;
    QString currVariantId;

    //Now check current settings and set proper variant by default
    for(int i=0; i<currLayouts.size(); i++)
    {
        if (currLayouts[i].layout_id == layout)
            currVariantId = currLayouts[i].variant_id;
    }

    QStringList variants;
    variants<<tr("Typical");
    for (int i=0; i<variant_ids.size(); i++)
    {
        QString descr = variantDescription(layout, variant_ids[i]);
        if (!descr.length())
            descr = variant_ids[i];
        variants<<descr;

        if (currVariantId.length() && (currVariantId == variant_ids[i]))
            currVariantIndex = i+1;
    }

    ui->variantsDB->clear();
    ui->variantsDB->addItems(variants);
    ui->variantsDB->setCurrentIndex(currVariantIndex);
}
