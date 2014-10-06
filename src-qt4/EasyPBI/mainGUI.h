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

#include "backend.h"
#include "config.h"
#include "PBIModule.h"
#include "ModuleUtils.h"
#include "pkgSelect.h"
#include "bulkModDialog.h"

#include "SConfigDialog.h"

namespace Ui {
    class MainGUI;
}

class MainGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainGUI(QWidget *parent = 0);
    ~MainGUI();

    void loadModule(QString confFile);

private slots:
    //general purpose functions
    void slotSingleInstance();
    void refreshGUI(QString);
    //menubar functions
    void on_actionBulk_Module_Creator_triggered();
    void on_actionExit_triggered();
    void on_actionNew_Module_triggered();
    void on_actionLoad_Module_triggered();
    void on_actionLoad_Files_triggered();
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
    void on_tool_addportafter_clicked();
    void on_tool_rmportafter_clicked();
    void on_push_config_save_clicked();
    void slotOptionChanged(QString tmp="");
    void slotSetRepoType(QAction* act);
    void on_tool_addplugin_clicked();
    void on_tool_rmplugin_clicked();
    void on_tool_addscreenshot_clicked();
    void on_tool_rmscreenshot_clicked();
    void on_tool_addsimilar_clicked();
    void on_tool_rmsimilar_clicked();
    void on_tool_showicon_clicked();
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
    //Service Config Tab
    void refresh_scfg_list();
    void on_tool_scfg_add_clicked();
    void on_tool_scfg_rm_clicked();
    void on_tool_scfg_edit_clicked();
    void on_tool_scfg_up_clicked();
    void on_tool_scfg_down_clicked();
    void slotSCFGOptionChanged();
    void slotSCFGScriptEdited();
    void slotSCFGChanged();
    void on_tool_scfg_newtemplate_clicked();
    void on_push_scfg_savescript_clicked();
    void slotSCFGScriptChanged();

private:
    Ui::MainGUI *ui;
    PBIModule MODULE;
    Config *settings;
    QLineEdit *line_module;
    QMenu menu_bins, menu_validMenuCats, menu_validRepoCats, menu_validRepoTypes, menu_portopts;
    bool XDGUPDATING;
    QString lastModuleDir;
    QStringList pkgplist;

    void SetupDefaults();
    void checkMime();
    QString getPortPackage(); //single selection mode
    QStringList getPackages();  //multi-selection mode

};

#endif // MAINGUI_H
