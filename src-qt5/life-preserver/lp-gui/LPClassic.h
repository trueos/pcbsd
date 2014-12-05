#ifndef _LP_GUI_CLASSIC_BACKUP_H
#define _LP_GUI_CLASSIC_BACKUP_H

#include <QDialog>
#include <QTimer>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QProcess>
#include <QProcessEnvironment>
#include <QDateTime>
#include <QFileDialog>

namespace Ui{
	class LPClassic;
};

class LPClassic : public QDialog{
	Q_OBJECT
public:
	LPClassic(QWidget *parent = 0);
	~LPClassic();

	void setHomeDir(QString home);

	bool running;

private:
	Ui::LPClassic *ui;
	QTimer *fCheck;
	QProcess *tarProc;
	QString tarBaseDir, tarDir, tarFile;
	bool stopped;
	QFileInfo *tarInfo;

private slots:
	void updateFCheck();
	void slotTarDone();

	//Exclude list controls
	void on_tool_rmexclude_clicked();
	void on_tool_addexcludefile_clicked();
	void on_tool_addexcludedir_clicked();
	void on_list_exclude_itemSelectionChanged();

	//Main Buttons
	void on_push_cancel_clicked();
	void on_push_start_clicked();
	void on_push_stop_clicked();
	void on_push_finished_clicked();
	
};

#endif
