#include "toolbox.h"
#include "ui_toolbox.h"
#include "showinfodialog.h"

#include <QFile>
#include <QDir>
#include <QProcess>
#include <QDate>
#include <QDebug>
#include <QMessageBox>

#define PREFIX "/usr/local/"
static const QString KSANPSHOT_FILE(PREFIX"bin/ksnapshot");
static const QString GNOME_PANEL_SCREENSHOT_FILE(PREFIX"bin/gnome-panel-screenshot");

Toolbox::Toolbox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Toolbox)
{
    ui->setupUi(this);

	ui->LogsButton->addAction(ui->actionDiagnostic_report);
	ui->LogsButton->addAction(ui->actionUname);
	ui->LogsButton->addAction(ui->actionDmesg);
	ui->LogsButton->addAction(ui->actionXorg_version);
	ui->LogsButton->addAction(ui->actionXorg_log_file);

	ui->HardwareButton->addAction(ui->actionPCIConf);

	// Setup screenshot button. Check for present screenshot tools
	ui->ScreenshotButton->setVisible(false);
	if (QFile::exists(KSANPSHOT_FILE))
	{
		mScreenshotCommand= KSANPSHOT_FILE;
		ui->ScreenshotButton->setVisible(true);
	}
	else if (QFile::exists(GNOME_PANEL_SCREENSHOT_FILE))
	{
		mScreenshotCommand= GNOME_PANEL_SCREENSHOT_FILE;
		ui->ScreenshotButton->setVisible(true);
	}
}

Toolbox::~Toolbox()
{
    delete ui;
}

void Toolbox::changeEvent(QEvent *e)
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

void Toolbox::on_actionXorg_log_file_triggered()
{
	ShowInfoDialog* dlg = new ShowInfoDialog(this);
	dlg->show(QString("/var/log/Xorg.0.log"));
}

void Toolbox::on_actionDmesg_triggered()
{
	ShowInfoDialog* dlg = new ShowInfoDialog(this);
	dlg->show("dmesg", QStringList());
}

void Toolbox::on_actionDiagnostic_report_triggered()
{
	qDebug() << "Generate Diagnostic Sheet";
	//Generate the diagnostic file on the user desktop
	QString filename = QDir::homePath()+"/Desktop/diagnostic-"+QDate::currentDate().toString("yyyyMMdd")+".txt";
	QString username = getenv("LOGNAME");
	QString cmd = "/usr/local/share/pcbsd/scripts/GenDiagSheet.sh "+filename+" "+username;
	QProcess p;
	p.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
	//qDebug() << " - cmd:" << cmd;
	if(QFile::exists(filename)){ QFile::remove(filename); } //remove any old file first
	p.start(cmd);
	while(p.waitForFinished(200)){
	  QApplication::processEvents();
	}
	if(QFile::exists(filename)){
	  qDebug() << " - success";
	  //Now show the info dialog for the newly generated diagnostic file
	  ShowInfoDialog *dlg = new ShowInfoDialog(this);
	  dlg->show(filename);
	}else{
	  qDebug() << " - failure";
	  QMessageBox::warning(this, tr("Failure"), tr("Failed to create diagnostic sheet"));
	}
}

void Toolbox::on_actionPCIConf_triggered()
{
	ShowInfoDialog* dlg = new ShowInfoDialog(this);
	dlg->show("pciconf", QStringList()<<"-l"<<"-v");
}

void Toolbox::on_ScreenshotButton_clicked()
{
	if (mScreenshotCommand.length())
	{
		QProcess proc;
		proc.startDetached(mScreenshotCommand);
	}
}

void Toolbox::on_actionXorg_version_triggered()
{
	ShowInfoDialog* dlg = new ShowInfoDialog(this);
	dlg->show("Xorg", QStringList()<<"-version");
}

void Toolbox::on_actionUname_triggered()
{
	ShowInfoDialog* dlg = new ShowInfoDialog(this);
	dlg->show("uname", QStringList()<<"-a");
}
