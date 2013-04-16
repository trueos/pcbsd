#ifndef LIFEP_H
#define LIFEP_H

/* QT4 */
#include <QDialog>
#include <QFile>
#include <QProcess>
#include <QSystemTrayIcon>

/* Local Includes */
#include "ui_lifePreserverMain.h"
#include "ui_lifePreserverWizard.h"
#include "lifePreserverWizard.h"
#include "ui_lifePreserverSettings.h"
#include "lifePreserverSettings.h"
#include "ui_lifePreserverListBackups.h"
#include "lifePreserverListBackups.h"

#include <qmenu.h>

class lifePreserver : public QMainWindow, private Ui::lifePreserver
{
        Q_OBJECT

public:
        lifePreserver() : QMainWindow()
        {
          setupUi(this);
        }

   void ProgramInit();


public slots:
   void loadPreservers();
   void slotSingleInstance();

protected:
   void closeEvent(QCloseEvent *event);

private slots:
   void slotCurrentTreePreserverChanged(QTreeWidgetItem *curItem);
   void slotContextMenuPreserver();
   void slotContextMenuShow();
   void slotContextMenuHide();
   void slotPushEditPreserverClicked();
   void slotPushNewPreserverClicked();
   void slotPushPreserverStartClicked();
   void slotPushRemovePreserverClicked();
   void slotPushRestorePreserverClicked();
   void slotPushQuitClicked();
   void slotScanPreserversForChanges();
   void slotTrayActivated(QSystemTrayIcon::ActivationReason reason);
   void slotTrayAnimation();
   void slotToggleRunAtStart();

private:
   lifePreserverWizard *lpw;
   lifeListBackups *lplb;
   pSettings *lps;
   QAction *minimizeAction;
   QAction *restoreAction;
   QAction *quitAction;
   QAction *autoStartAction;
   QMenu *trayIconMenu;
   QString getPreserverLastBackup(QString host);
   QString getPreserverFrequency(QString host);
   QString getPreserverStatus(QString host);
   QSystemTrayIcon *trayIcon;
   void createActions();
   void createTrayIcon();
   void loadPreserverFromConf(QString connectHost);
   bool failedBackups;
   bool curActive;
   bool runAtStart;
   QString lastIcon;
   int aniStep;

signals:

} ;
#endif // LIFEP_H

