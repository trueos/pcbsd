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
#include <QMenu>

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
	QString file_GRUBdefaults, file_GRUBentries, file_pcbsdconf;
	QString dir_GRUBthemes, dir_GRUBfonts;
	QStringList confcontents;
	//GRUB default values
	QMenu *quickEntryM; //Quick Entry Menu
        QString G_fontFile, G_themeFile;
	int G_timer;//, G_defaultBE;
	bool G_showMenu, G_goodLoad;

	//Random usage functions
	int getSelectedBE(); //get the currently selected Boot Environment index
	bool checkName(QString); //make sure the selected name is valid
	bool validateInput(QString); //Make sure the input text is number/letter
	void runLongCMD(QString cmd , QString info = ""); 

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
	bool loadGRUBdefaults(QString&, QString&, int&, bool&);// int&);
	bool saveGRUBdefaults(QString, QString, int, bool);//, int defaultBE=0);
	bool loadGRUBcustomentries(QStringList&);
	bool saveGRUBcustomentries(QStringList);
	
	//Maximum BE
	void loadMaxBE();
	
private slots:
	//UI update functions
	void updateBEList();
	void updateGRUBdefaults(bool withrebuild = false);
	void updateGRUBentries();
	//UI Buttons - Boot Environments
	void on_tool_BEadd_clicked(QString oldname = "");
	void on_tool_BEactivate_clicked();
	void on_tool_BEcp_clicked(QString oldname = "");
	void on_tool_BEmv_clicked(QString oldname = "");
	void on_tool_BErem_clicked();
	//UI Buttons - GRUB Config
	void on_tool_GRUBsavedefaults_clicked();
	void on_tool_GRUBresetdefaults_clicked();
	void on_tool_GRUBthemefile_clicked();
	void on_tool_GRUBfontfile_clicked();
	void on_tool_GRUBsaveentries_clicked();
	void on_tool_GRUBresetentries_clicked();
	void makeQuickEntry(QAction*);
	//UI Buttons - other
	void on_actionClose_triggered();
	void on_action_rebuildGRUBmenu_triggered();
	void on_action_restoreGRUBdefaults_triggered();
	void GRUBchangedefaults();
	void GRUBchangeentries();
	void on_spin_maxbe_valueChanged(int);
	
protected:
	void closeEvent(QCloseEvent *event);
	
};
#endif
