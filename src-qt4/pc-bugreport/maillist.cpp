#include "maillist.h"
#include "ui_maillist.h"

#include <QDesktopServices>
#include <QUrl>

maillist::maillist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::maillist)
{
    ui->setupUi(this);
}

maillist::~maillist()
{
    delete ui;
}

void maillist::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void maillist::setup(QString Name, QString URL, QString EMail, bool canLaunch)
{
	ui->ListNameLabel->setText(Name);
	ui->ListURLLabel->setText(URL);
	ui->ListEmailLabel->setText(EMail);

	ui->BrowserButton->setVisible(canLaunch);
	ui->EMailButton->setVisible(canLaunch);
}

void maillist::on_BrowserButton_clicked()
{
	QDesktopServices::openUrl(QUrl(ui->ListURLLabel->text()));
}

void maillist::on_EMailButton_clicked()
{
	QDesktopServices::openUrl(QUrl(QString("mailto:")+ui->ListEmailLabel->text()));
}
