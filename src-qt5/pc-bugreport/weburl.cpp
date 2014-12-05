#include "weburl.h"
#include "ui_weburl.h"

#include <QDesktopServices>
#include <QUrl>

WebURL::WebURL(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WebURL)
{
    ui->setupUi(this);
}

WebURL::~WebURL()
{
    delete ui;
}

void WebURL::changeEvent(QEvent *e)
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

void WebURL::setup(const QString& URL, const char* PictureFile)
{
	if (PictureFile)
	{
		ui->Img->setPixmap(QPixmap(QString(PictureFile)));
	}
	ui->URLLabel->setText(URL);
}

void WebURL::setup(const QString& URL, const QIcon* ico)
{
    ui->Img->setPixmap(ico->pixmap(64, 64));
    ui->URLLabel->setText(URL);
}

void WebURL::on_LaunchBrowserButton_clicked()
{
	QDesktopServices::openUrl(QUrl(ui->URLLabel->text()));
}
