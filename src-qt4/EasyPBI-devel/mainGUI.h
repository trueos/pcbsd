#ifndef MAINGUI_H
#define MAINGUI_H

#include <QMainWindow>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QString>
#include <QDir>
#include <QProcess>
#include <QTimer>
#include <QDateTime>
#include <QRadioButton>
#include <QMenu>
#include <QFileDialog>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "modBuild.h"
#include "backend.h"
#include "config.h"
#include "PBIModule.h"
#include "ModuleUtils.h"

namespace Ui {
    class MainGUI;
}

class MainGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainGUI(QWidget *parent = 0);
    ~MainGUI();


private slots:
    //general purpose functions
    void slotSingleInstance();
    void refreshGUI(QString);
    //menubar functions
    void on_actionExit_triggered();
    void on_actionNew_Module_triggered();
    void on_actionLoad_Module_triggered();
    void on_actionPreferences_triggered();
    void on_actionPackage_Module_triggered();
    void on_actionRefresh_Module_triggered();
    void on_actionFreeBSD_Ports_triggered();
    void on_actionPBI_Modules_triggered();
    void on_actionAbout_EasyPBI_triggered();
    //editor functions
    void updateConfigVisibility();
    void slotModTabChanged(int);
    //pbi.conf functions
    void on_push_change_makeport_clicked();
    //void on_push_change_progdir_clicked();
    //void slotAddMakeOption(QAction*);
    //void on_push_addportbefore_clicked();
    //void on_push_rmportbefore_clicked();
    void on_push_addportafter_clicked();
    void on_push_rmportafter_clicked();
    void on_push_config_save_clicked();
    void slotOptionChanged(QString tmp="");
    //resources functions
    void slotResourceChanged();
    void on_push_resources_add_clicked();
    void on_push_resources_remove_clicked();
    void on_push_resources_mkwrapper_clicked();
    void slotResourceScriptSaved();
    void slotResourceScriptChanged();
    //XDG functions
    void slotXdgTypeChanged();
    void slotXdgFileChanged();
    void slotAddMenuCat(QAction*);
    void slotAddBin(QAction*);
    void on_push_xdg_remove_clicked();
    void on_push_xdg_savechanges_clicked();
    void on_push_xdg_savenew_clicked();
    void on_push_xdg_clearentry_clicked();
    void slotXDGOptionChanged(QString tmp="");
    //Scripts functions
    void slotScriptChanged(int);
    void on_push_scripts_create_clicked();
    void on_push_scripts_remove_clicked();
    void on_push_scripts_save_clicked();
    void slotScriptModified();
    //External-Links functions
    void on_push_el_add_clicked();
    void on_push_el_remove_clicked();
    void slotELSetFile(QAction*);
    void slotELSetType(QAction*);
    //PBI Build functions
    void on_push_build_start_clicked();
    void on_push_build_stop_clicked();
    void on_push_build_save_clicked();
    void slotUpdatePBIBuild();
    void slotPBIbuildFinished(int,QProcess::ExitStatus);
    void slotKillPBIBuild();
    void slotBuildError(QProcess::ProcessError);


private:
    Ui::MainGUI *ui;
    //ModBuild *currentModule;
    PBIModule MODULE;
    Config *settings;
    QLineEdit *line_module;
    //QRadioButton *radio_module_port, *radio_module_local;
    QMenu menu_elOpts, menu_validMenuCats, menu_bins, menu_el_bins;
    bool XDGUPDATING;

    QString PBI_BUILDING_NOW;
    bool PBI_BUILD_TERMINATED;
    QProcess *p;

    void SetupDefaults();
    //bool isValidPort(QString, bool allowOverride = FALSE);
    void checkMime();

};

#endif // MAINGUI_H
