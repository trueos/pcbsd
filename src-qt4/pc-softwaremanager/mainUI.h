#ifndef _APPCAFE_MAINUI_H
#define _APPCAFE_MAINUI_H


#ifndef PREFIX
#define PREFIX QString("/usr/local")
#endif

/* QT4 */
#include <QDebug>
#include <QDialog>
#include <QMenu>
#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTreeWidgetItem>
#include <QScrollArea>
#include <QScrollBar>
#include <QFileDialog>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QWidgetAction>

/* LibPCBSD Includes */
#include <pcbsd-sysFlags.h>

/* Local Includes */
#include "pbiNgBackend.h"
#include "extras.h"
#include "largeItemWidget.h"
#include "smallItemWidget.h"

namespace Ui {
    class MainUI;
}

class MainUI : public QMainWindow
{
        Q_OBJECT

public:
   explicit MainUI(QWidget* parent = 0);
   void ProgramInit();
   //void setWardenMode(QString dir,QString ip); //call before ProgramInit

public slots:
    void slotSingleInstance();
    
protected:
    void closeEvent(QCloseEvent *event);

private slots:
  //MENU OPTIONS
  void on_actionImport_PBI_List_triggered();
  void on_actionExport_PBI_List_triggered();
  void on_actionQuit_triggered();
  void on_actionAppCafe_Settings_triggered();
  void on_actionDeveloper_Mode_triggered();

  //INSTALLED TAB
  void slotRefreshInstallTab();
  void slotCheckSelectedItems();
  void slotPBIStatusUpdate(QString);
  void on_tool_install_details_clicked();
  void on_tool_install_toggleall_clicked();
  void on_tree_install_apps_itemSelectionChanged();
  void on_tree_install_apps_itemDoubleClicked(QTreeWidgetItem *item);

  void on_tool_install_maintainer_clicked();
  void slotInstalledAppRightClicked(const QPoint &);
  void contextMenuFinished();
  void slotActionAddDesktop();
  void slotActionRemoveDesktop();
  void slotActionRemove();
  void slotActionCancel();
  void slotStartApp(QAction*);

  //BROWSER TAB
  void slotDisableBrowser(bool shownotification = TRUE);
  void slotEnableBrowser();
  void slotUpdateBrowserHome();
  void slotGoToHome();
  void slotGoToCatBrowser();
  void slotGoToCategory(QString);
  void slotGoToApp(QString, bool goback = false);
  void slotBackToApp(QAction*);
  void slotUpdateAppDownloadButton();
  void slotGoToSearch();
  void slotShowSimilarApps(QStringList);
  void slotShowSearchResults(QStringList, QStringList);
  void on_tabWidget_currentChanged();
  void on_tool_browse_home_clicked();
  void on_tool_browse_cat_clicked();
  void on_tool_browse_app_clicked();
  void on_line_browse_searchbar_textChanged();
  void on_tool_bapp_download_clicked();
  void on_group_br_home_newapps_toggled(bool);
  void on_tool_app_nextScreen_clicked();
  void on_tool_app_prevScreen_clicked();
  void on_tool_app_rank_clicked();
  void on_tool_app_openweb_clicked();
  void on_tool_br_back_clicked();
  void browserViewSettingsChanged();

  void installAppIntoJail(QAction *act);

  //OTHER
  void slotDisplayError(QString,QString,QStringList);
  void slotScreenshotAvailable(QNetworkReply*);
  
private:
  Ui::MainUI *ui;
  QString defaultIcon;
  PBIBackend *PBI;
  QNetworkAccessManager *netman;
  QNetworkReply *netreply;
  //INSTALLED TAB
  QMenu *actionMenu, *appBinMenu, *sDeskMenu, *contextActionMenu, *jailMenu, *backMenu;
  QString cDetails;
  void initializeInstalledTab();
  void formatInstalledItemDisplay(QTreeWidgetItem *item);
  QStringList getCheckedItems();
  //BROWSER TAB
  QTimer *searchTimer;
  QString cCat, cApp;
  QString bCat, bApp; //current cat/app for the buttons
  QStringList backApps;
  void initializeBrowserTab();
  //OTHER
  QLabel *statusLabel, *jailLabel;
  QWidgetAction *jailAction;
  void clearScrollArea(QScrollArea*);
  bool fillVerticalAppArea(QScrollArea *area, QStringList applist, bool filter = false);
  void showScreenshot(int num=0);
  QString checkIcon(QString icon, QString type);
  void slotDisplayStats();
  QStringList generateRemoveMessage(QStringList apps);
    
} ;

#endif

