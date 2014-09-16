#ifndef _APPCAFE_QT_CONFIG_DIALOG_H
#define _APPCAFE_QT_CONFIG_DIALOG_H

#include <QDialog>
#include <QString>
#include <QFile>
#include <QProcess>
#include <QTextStream>

#define CFGFILE QString("/usr/local/etc/appcafe.conf")

namespace Ui{
	class ConfigDlg;
};

class ConfigDlg : public QDialog{
	Q_OBJECT
public:
	ConfigDlg(QWidget *parent=0);
	~ConfigDlg();

	bool savedChanges;

private:
	Ui::ConfigDlg *ui;
	bool cEnable;
	int cPort;

	void saveFile(QString file, QStringList contents);

private slots:
	void LoadConfig();
	void SaveConfig();
	void checkOptions();

	void ApplyClicked();
	void CancelClicked();

};

#endif