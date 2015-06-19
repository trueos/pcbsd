#include "eolDialog.h"
#include "ui_eolDialog.h"

#include "pcbsd-utils.h"

const int WARNING_DAYS = 30;

const char* const CHECK_COMMAND = "pc-updatemanager showeol";
const char* const EOL_TAG = "has an EOL date of:";
const char* const EOL_PASSED_TAG = "has passed its EOL date of:";
const char* const EOL_IN_DAYS_TAG = "End of support (in days):";

const char* const ERROR_IMAGE = ":/images/syserror.png";
const char* const OK_IMAGE = ":/images/showeol.png";
const char* const WARNING_IMAGE = ":/images/warning.png";
const char* const PASSED_EOL_IMAGE = ":/images/application-exit.png";

EOLDialog::EOLDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EOLDialog)
{
    ui->setupUi(this);
    //Ensure the dialog is centered on the parent
    if(parent!=0){
       QPoint ctr = parent->geometry().center();
       this->move( ctr.x()-(this->width()/2), ctr.y()-(this->height()/2) );
    }
}

EOLDialog::~EOLDialog()
{
    delete ui;
}

void EOLDialog::setupDialog()
{
    this->show();
    check();
}

bool EOLDialog::check()
{
    bool success;
    QStringList out = pcbsd::Utils::runShellCommand(CHECK_COMMAND, success);
    if ((!success) || out.size() < 5)
    {
        ui->statusIcon->setPixmap(QPixmap(ERROR_IMAGE));
        return false;
    }
    for (int i=0; i<out.size()-1; i++)
    {
        if (out[i].indexOf(EOL_TAG)>=0)
        {
            ui->eolDate->setText(out[++i]);
            ui->statusIcon->setPixmap(QPixmap(ERROR_IMAGE));
            continue;
        }
        qDebug()<<out[i];
        if (out[i].indexOf(EOL_PASSED_TAG)>=0)
        {
            ui->eolDate->setText(out[++i]);
            ui->statusIcon->setPixmap(QPixmap(PASSED_EOL_IMAGE));
            ui->eolDays->setText(tr("PASSED"));
            ui->msgLabel->setText(tr("Your version is not supported now!\nPlease update your PC-BSD ASAP!"));
            break;
        }
        qDebug()<<out[i];
        if (out[i].indexOf(EOL_IN_DAYS_TAG)>=0)
        {
            QString days =out[i].replace(EOL_IN_DAYS_TAG,"").trimmed();
            qDebug()<<days;
            int days_int = days.toInt();
            ui->eolDays->setText(days);
            if (days_int>WARNING_DAYS)
            {
               ui->statusIcon->setPixmap(QPixmap(OK_IMAGE));
               ui->msgLabel->setVisible(false);
            }
            else
            {
                ui->statusIcon->setPixmap(QPixmap(WARNING_IMAGE));
                ui->msgLabel->setText(tr("Support of your PC-BSD will end soon!\nPlease update your system"));
            }
            break;
        }
    }

    return true;
}
