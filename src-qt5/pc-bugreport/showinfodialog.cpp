#include "showinfodialog.h"
#include "ui_showinfodialog.h"
#include <QFile>
#include <QProcess>

ShowInfoDialog::ShowInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowInfoDialog)
{
    ui->setupUi(this);
}

ShowInfoDialog::~ShowInfoDialog()
{
    delete ui;
}

void ShowInfoDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

bool ShowInfoDialog::show(const QString &program, const QStringList &arguments)
{
	QProcess* proc = new QProcess();
	proc->setProcessChannelMode(QProcess::MergedChannels);
	proc->start(program, arguments);
	proc->waitForFinished(-1);

	QString Str;

	while ( proc->canReadLine() )
	{
		Str += proc->readLine().simplified()+"\n";
	}

	ui->textBrowser->setPlainText(Str);

	QString Title= program;
	for (int i=0; i<arguments.size(); i++)
		Title+=QString(" ") + arguments[i];

	setWindowTitle(Title);

	ui->textBrowser->selectAll();

	showNormal ();

	return true;
}

bool ShowInfoDialog::show(const QString &filename)
{
	if (!QFile::exists(filename))
		return false;

	QFile file(filename);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	ui->textBrowser->setPlainText(file.readAll());

	setWindowTitle(filename);

	ui->textBrowser->selectAll();

	showNormal ();

	return true;
}

void ShowInfoDialog::on_ClipboardButton_clicked()
{
	ui->textBrowser->copy();
}

void ShowInfoDialog::on_textBrowser_copyAvailable(bool b)
{
	ui->ClipboardButton->setEnabled(b);
}
