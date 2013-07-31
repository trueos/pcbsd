#ifndef _PC_BOOTCFG_MAINUI_H
#define _PC_BOOTCFG_MAINUI_H

#include <QMainWindow>
#include <QStringList>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QProcess>
#include <QDebug>

namespace Ui {
    class mainUI;
}

class mainUI : public QMainWindow{
	Q_OBJECT
	
public:
	explicit mainUI(QWidget *parent = 0);
	~mainUI();
	
public slots:
	void slotSingleInstance();

private:
	Ui::mainUI *ui;
	QProcess *proc;
	QString file_GRUBdefaults, file_GRUBentries;
	QString dir_GRUBthemes, dir_GRUBfonts;

	//Random usage functions
	int getSelectedBE(); //get the currently selected Boot Environment index
	bool checkName(QString); //make sure the selected name is valid
	void runLongCMD(QString); 

	//Boot Environment Management Functions
	void beadmActivate(QString name);
	void beadmCreate(QString name);
	void beadmCopy(QString name, QString newname);
	void beadmRemove(QString name);
	void beadmRename(QString name, QString newname);
	void beadmMount(QString name, QString mntpt);
	void beadmUnmount(QString name);
	QStringList beadmList();

	//GRUB Configuration Management Functions
	bool loadGRUBdefaults(QString&, QString&, int&, bool&);
	bool saveGRUBdefaults(QString, QString, int, bool);
	bool loadGRUBcustomentries(QStringList&);
	bool saveGRUBcustomentries(QStringList);

private slots:
	//UI update functions
	void updateBEList();
	void updateGRUBdefaults();
	void updateGRUBentries();
	//UI Buttons - Boot Environments
	void on_tool_BEadd_clicked();
	void on_tool_BEactivate_clicked();
	void on_tool_BEcp_clicked();
	void on_tool_BEmv_clicked();
	void on_tool_BErem_clicked();
	//UI Buttons - GRUB Config
	void on_tool_GRUBsavedefaults_clicked();
	void on_tool_GRUBresetdefaults_clicked();
	void on_tool_GRUBthemefile_clicked();
	void on_tool_GRUBfontfile_clicked();
	void on_tool_GRUBsaveentries_clicked();
	void on_tool_GRUBresetentries_clicked();
	//UI Buttons - other
	void on_actionClose_triggered();
	void GRUBchangedefaults();
	void GRUBchangeentries();
	
};
#endif
