#ifndef _APPCAFE_QT_CONFIG_DIALOG_H
#define _APPCAFE_QT_CONFIG_DIALOG_H

#include <QDialog>
#include <QString>
#include <QFile>
#include <QProcess>
#include <QTextStream>
#include <QSettings>
#include <QInputDialog>
#include <QDebug>
#include <QDir>

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
	bool cEnable, repoChanged, remoteChanged;
	QString cRepo; //current saved repo
	int cPort;
	QSettings *settings;

	void saveFile(QString file, QStringList contents);
	QStringList readFile(QString file);

private slots:
	void LoadConfig(); //web config
	void SaveConfig(); //web config
	void loadPbiConf(); //pkg/PBI config
	void savePbiConf(); //pkg/PBI config

	void checkOptions();

	void ApplyClicked();
	void CancelClicked();

	//Custom Repo Management
	void readSavedRepos();
	void customChanged();
	void addCustom();
	void removeCustom();
};

#endif