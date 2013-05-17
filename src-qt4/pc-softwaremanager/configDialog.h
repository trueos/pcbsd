#ifndef _APPCAFE_CONFIG_DIALOG_H
#define _APPCAFE_CONFIG_DIALOG_H

#include <QDialog>
#include <QWidget>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>

#include "pbiDBAccess.h"

namespace Ui {
    class ConfigDialog;
}

class ConfigDialog : public QDialog{
	Q_OBJECT
	
public:
	explicit ConfigDialog(QWidget* parent = 0);
	virtual ~ConfigDialog();
	
	void setupDone(); //make sure the public variables below are set before running this
	
	//Data setup/retrieval variables
	bool applyChanges;
	QStringList xdgOpts;
	bool keepDownloads;
	QString downloadDir;
	PBIDBAccess *DB;
	
private:
	Ui::ConfigDialog *ui;
	QString repoID;
	
private slots:
	//Repo Tab
	void refreshRepoTab();
	void on_combo_repo_currentIndexChanged();
	void on_tool_repo_add_clicked();
	void on_tool_repo_remove_clicked();
	void on_tool_repomirror_add_clicked();
	void on_tool_repomirror_remove_clicked();
	void on_tool_repomirror_up_clicked();
	void on_tool_repomirror_down_clicked();
	
	//Config Tab
	void on_group_download_toggled(bool);
	void on_tool_getDownloadDir_clicked();
	
	//ButtonBox
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();
	
};

#endif
