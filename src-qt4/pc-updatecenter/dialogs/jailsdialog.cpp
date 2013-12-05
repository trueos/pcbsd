#include "jailsdialog.h"
#include "ui_jailsdialog.h"

#include <QTreeWidgetItem>
#include "utils.h"

__string_constant RUNNING_IMG = ":/images/running.png";
__string_constant PAUSED_IMG = ":/images/paused.png";
__string_constant STOPPED_IMG = ":/images/stopped.png";

///////////////////////////////////////////////////////////////////////////////
JailsDialog::JailsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JailsDialog)
{
    ui->setupUi(this);
}

///////////////////////////////////////////////////////////////////////////////
JailsDialog::~JailsDialog()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////
int JailsDialog::execDialog(CJailsBackend *pBackend)
{
    if (!pBackend)
        return QDialog::Rejected;

    mpJail = pBackend;

    QVector<CJailsBackend::SJailDescription> jails;
    jails = CJailsBackend::jailsList();

    for (int i=0; i<jails.size(); i++)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem;
        item->setText(0, jails[i].mName);
        QString ico_path;
        if (jails[i].misStarted)
        {
            ico_path= RUNNING_IMG;
        }
        else
        if (jails[i].misEnabled)
        {
            ico_path= PAUSED_IMG;
        }
        else
        {
            ico_path= STOPPED_IMG;
        }
        item->setIcon(0, QIcon(ico_path));
        ui->jailsTW->addTopLevelItem(item);
        if (i==0)
            ui->jailsTW->setCurrentItem(item);
        if (mpJail->jailName().trimmed() == jails[i].mName.trimmed())
        {
            ui->jailsTW->setCurrentItem(item);
        }
    }
    return exec();
}

void JailsDialog::on_okBtn_clicked()
{
    if (!mpJail)
    {
        setResult(QDialog::Rejected);
        return;
    }

    QTreeWidgetItem* item = ui->jailsTW->currentItem();
    if (!item)
    {
        setResult(QDialog::Rejected);
        return;
    }

    if (ui->jailsGB->isChecked())
    {
        mpJail->setJailEnabled(true);
        mpJail->setJail(item->text(0));
    }
    else
    {
        mpJail->setJailEnabled(false);
    }

    setResult(QDialog::Accepted);
    close();
}
