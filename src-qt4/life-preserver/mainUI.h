#ifndef _LP_MAINUI_H
#define _LP_MAINUI_H

#include <QMainWindow>
#include <QHash>
#include <QStringList>
#include <QString>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFileDialog>

#include "LPBackend.h"
#include "LPWizard.h"
#include "LPContainers.h"
#include "LPConfig.h"

//TERMINOLOGY NOTE: DS=DataSet, SNAP=Snapshot

namespace Ui {
    class mainUI;
}

class mainUI : public QMainWindow{
	Q_OBJECT
	
public:
	explicit mainUI(QWidget* parent = 0);
	~mainUI();
	void setupUI();

	void updateDisplay(); //for the tray to call it as necessary

private:
	Ui::mainUI *ui;
	QHash<QString,LPDataset> HLIST;
	QStringList RLIST; //datasets that have replication enabled
	QStringList SLIST; //available datasets on the system
	QStringList CLIST; //current status for all datasets
	QMenu *revMenu, *brMenu, *addMenu; //revert/browse menu's

	void updateHash(QString ds="");
	void updateUI();
	void updateMenus();
	QString getSelectedDS();
	LPDataset newDataset(QString);

private slots:
	void on_treeWidget_itemSelectionChanged();
	void on_tool_config_clicked();
	void on_tool_remove_clicked();

	void slotRevertToSnapshot(QAction*);
	void slotBrowseSnapshot(QAction*);
	void slotAddDataset(QAction*);

	void on_actionClose_triggered();

protected:
	void closeEvent(QCloseEvent*);

};
#endif
