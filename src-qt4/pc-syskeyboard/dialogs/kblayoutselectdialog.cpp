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
void KbLayoutSelectDialog::disableLayouts(QStringList lv)
{
    disabledLayouts = lv;
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
        bool disable = disabledLayouts.contains(layouts[i]);

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

///////////////////////////////////////////////////////////////////////////////
void KbLayoutSelectDialog::on_searchLE_textChanged(const QString &arg1)
{
    if (!arg1.length())
        return;

    QString txt = ui->searchLE->text();

    QList<QTreeWidgetItem*> lst= ui->layoutsTW->findItems(arg1,Qt::MatchStartsWith,1);
    if (!lst.size())
       lst= ui->layoutsTW->findItems(arg1,Qt::MatchStartsWith,2);
    if (lst.size())
    {
        QTreeWidgetItem* item = lst[0];
        ui->layoutsTW->setCurrentItem(item);
    }
}
