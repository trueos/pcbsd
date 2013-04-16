/***************************************************************************
 *   Copyright (C) 2006 - 2011 PC-BSD Software  			   *
 *   kris@pcbsd.org 							   *
 *                                                                         *
 *   Permission is hereby granted, free of charge, to any person obtaining *
 *   a copy of this software and associated documentation files (the       *
 *   "Software"), to deal in the Software without restriction, including   *
 *   without limitation the rights to use, copy, modify, merge, publish,   *
 *   distribute, sublicense, and/or sell copies of the Software, and to    *
 *   permit persons to whom the Software is furnished to do so, subject to *
 *   the following conditions:                                             *
 *                                                                         *
 *   The above copyright notice and this permission notice shall be        *
 *   included in all copies or substantial portions of the Software.       *
 &nimize*                                                                         *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/
/* QT4 */
#include <QPixmap>
#include <QTextStream>
#include <QSettings>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QTimer>
#include <QMenu>

#include <qmenu.h>

/* Local includes */
#include "ui_lifePreserverMain.h"
#include "lifePreserverMain.h"
#include "ui_lifePreserverWizard.h"
#include "lifePreserverWizard.h"
#include "ui_lifePreserverSettings.h"
#include "lifePreserverSettings.h"
#include "ui_lifePreserverListBackups.h"
#include "lifePreserverListBackups.h"
#include "preserver.h"
#include "externals.h"

void lifePreserver::ProgramInit()
{ 
   qDebug("Application starting...");
   setWindowIcon(QIcon(QString::fromUtf8(":/images/images/lifepreserver.png")));

   QSettings settings("PCBSD", "LifePreserver");
   runAtStart = settings.value("autostart", true).toBool();

   aniStep=0;
   curActive=false;
   failedBackups=false;
   loadPreservers();

   createActions();
   createTrayIcon();

   treePreservers->setContextMenuPolicy( Qt::CustomContextMenu);
   connect( treePreservers, SIGNAL(customContextMenuRequested (const QPoint & ) ), this, SLOT(slotContextMenuPreserver() ) );

   connect(pushPreserverStart, SIGNAL( clicked() ), this, SLOT(slotPushPreserverStartClicked() ) );
   treePreservers->resizeColumnToContents(0);

   // Connect our menu actions
   connect(action_Quit, SIGNAL( triggered(bool) ), this, SLOT(slotPushQuitClicked() ) );
   connect(action_New_Preserver, SIGNAL( triggered(bool) ), this, SLOT(slotPushNewPreserverClicked() ) );
   connect(action_Edit, SIGNAL( triggered(bool) ), this, SLOT(slotPushEditPreserverClicked() ) );
   connect(action_Remove, SIGNAL( triggered(bool) ), this, SLOT(slotPushRemovePreserverClicked() ) );
   connect(actionRestore, SIGNAL( triggered(bool) ), this, SLOT(slotPushRestorePreserverClicked() ) );

   // Start monitoring for changes
   QTimer *traytimer = new QTimer(this);
   connect(traytimer, SIGNAL(timeout()), this, SLOT(slotScanPreserversForChanges()));
   traytimer->start(10000);

   // See if we need to start a backup now
   QString cronscript = LIFEPRESERVERPATH + "/scripts/check-backups.sh &";
   system(cronscript.toLatin1());

}

void lifePreserver::slotTrayAnimation()
{
   QString setIcon = ":/images/images/tray-icon-idle.png";
   QString tmp;

   if ( failedBackups )
     setIcon = ":/images/images/tray-icon-failed.png";

   if ( curActive )
     setIcon = ":/images/images/tray-icon-active7.png";

   if ( lastIcon == setIcon )
      return;

   trayIcon->setIcon(QIcon(setIcon));
   setWindowIcon(QIcon(setIcon));
   lastIcon = setIcon;

}

// Slot which brings up the restore widgets for a preserver
void lifePreserver::slotPushRestorePreserverClicked()
{
  if (treePreservers->currentColumn() == -1)
  {
    return;
  }

  QString host = treePreservers->currentItem()->text(0);

  lifeListBackups *lplb = new lifeListBackups();
  lplb->widgetInit(host);
  lplb->show();
}

// Slot which brings up a context menu for the preserver
void lifePreserver::slotContextMenuPreserver()
{
  if (treePreservers->currentColumn() == -1)
  {
     return;
  }

  QMenu *popup = new QMenu();
  popup->addAction( tr("Edit"), this, SLOT(slotPushEditPreserverClicked()));
  popup->addAction( tr("Restore From"), this, SLOT(slotPushRestorePreserverClicked()));
  popup->addSeparator();
  popup->addAction( tr("Remove"), this, SLOT(slotPushRemovePreserverClicked()));
  popup->exec( QCursor::pos() );

}

// Slot which monitors for changes in our preservers
void lifePreserver::slotScanPreserversForChanges()
{
  bool anyActive = false;
  QString tooltip, status;
  failedBackups = false;
  tooltip = tr("Life Preserver Backups") + "<hr>";

  QTreeWidgetItemIterator it(treePreservers);
  while (*it) {
    QString host = (*it)->text(0);
    PRESERVER myPreserver(host);
    int pid = myPreserver.getPID();
    if ( pid == -1 ) {
      pushPreserverStart->setText(tr("Start"));
      (*it)->setText(3, tr("Not running"));
    } else {
      pushPreserverStart->setText(tr("Stop"));
      (*it)->setText(3, tr("Running..."));
       anyActive = true;
    }
    status = getPreserverLastBackup(host);
    tooltip+= status + "<br>";

    if ( status.indexOf("Failed:") != -1 ) {
      failedBackups=true;
      (*it)->setIcon(1, QIcon(":/images/images/backup-failed.png"));
    } else {
      (*it)->setIcon(1, QIcon(":/images/images/backup-ok.png"));
    }
    
    (*it)->setText(1, status);
    (*it)->setText(2, getPreserverFrequency(host));
  
    ++it;
  }

  // Set the active flag
  if ( anyActive ) 
    curActive = true;
  else
    curActive = false;


  if ( curActive )
    tooltip+= tr("Backup in progress...");

  trayIcon->setToolTip(tooltip);

  slotTrayAnimation();
}

// Slot which checks which preserver is currently highlighted
void lifePreserver::slotCurrentTreePreserverChanged(QTreeWidgetItem *curItem)
{
   QString host = curItem->text(0);
   pushPreserverStart->setEnabled(true);
   action_Edit->setEnabled(true);
   action_Remove->setEnabled(true);

   PRESERVER myPreserver(host);
   int pid = myPreserver.getPID();
   if ( pid == -1 )
     pushPreserverStart->setText(tr("Start"));
   else
     pushPreserverStart->setText(tr("Stop"));
  
}

// Slot to start a preserver process
void lifePreserver::slotPushPreserverStartClicked()
{
   if (treePreservers->currentColumn() == -1)
   {
      return;
   }

   QString host = treePreservers->currentItem()->text(0);
   PRESERVER myPreserver(host);
   int pid = myPreserver.getPID();
   if ( pid == -1 )
     myPreserver.startBackup();
   else
     myPreserver.stopBackup();
}

// Slot to remove a preserver host
void lifePreserver::slotPushRemovePreserverClicked()
{
   if (treePreservers->currentColumn() == -1)
   {
      return;
   }

   QString removeHost = treePreservers->currentItem()->text(0);
   PRESERVER myPreserver(removeHost);
   myPreserver.remove();

   // Reload our list
   loadPreservers();

   // Make sure the right widget is shown if we remove the last preserver
   if ( havePreservers ) {
	showNormal();
	w->hide();
   } else {
	hide();
	w->showNormal();
   }
}

// Slot to edit a preserver host
void lifePreserver::slotPushEditPreserverClicked()
{
   if (treePreservers->currentColumn() == -1)
   {
      return;
   }

   lps = new pSettings();
   lps->widgetInit(treePreservers->currentItem()->text(0));
   lps->setWindowModality( Qt::ApplicationModal );
   lps->show();
}

// Slot to start the creation of a new life preserver
void lifePreserver::slotPushNewPreserverClicked()
{
   lpw = new lifePreserverWizard();
   lpw->widgetInit();
   lpw->setWindowModality( Qt::WindowModal );
   connect(lpw, SIGNAL( added() ), this, SLOT(loadPreservers() ) );
   lpw->show();

}

// Slot to exit the application
void lifePreserver::slotPushQuitClicked()
{
   qApp->quit();
}

void lifePreserver::loadPreserverFromConf(QString connectHost)
{
  QString host, port;
  PRESERVER myPreserver(connectHost);
  host = myPreserver.getHost();
  port.setNum(myPreserver.getPort());

  QStringList itemTexts;
  itemTexts << host << getPreserverLastBackup(host) << getPreserverFrequency(host)  << getPreserverStatus(host);
  QTreeWidgetItem *newitem = new QTreeWidgetItem(treePreservers, itemTexts);
  treePreservers->setCurrentItem(newitem);
  slotCurrentTreePreserverChanged(newitem);

}

void lifePreserver::loadPreservers()
{
   havePreservers = false;

   // Make sure we disconnect our notifier before clearing the list
   disconnect(treePreservers, SIGNAL( currentItemChanged ( QTreeWidgetItem *, QTreeWidgetItem *) ), 0, 0 );
   treePreservers->clear();

   pushPreserverStart->setEnabled(false);
   action_Edit->setEnabled(false);
   action_Remove->setEnabled(false);

   QDir confdir;
   confdir.setPath(LIFEPRESERVERSDIR);
   // Make sure the conf path exists
   if ( ! confdir.exists() )
   {
      confdir.mkpath(confdir.path());
   }

   // Now read in all the directories and look for preservers
   confdir.setFilter(QDir::AllDirs | QDir::NoSymLinks | QDir::NoDotAndDotDot );

   QStringList listPreservers = confdir.entryList();
   for (int i = 0; i < listPreservers.size(); ++i) {
       QString preserverDir = listPreservers.at(i); 
       QFile confFile(confdir.path() + "/" + preserverDir + "/settings.conf");
       if ( confFile.exists() )
       {
         // Load this preserver from its conf
	 havePreservers=true;
         loadPreserverFromConf(preserverDir);
       }
   }


   // Connect our slot to determine when the selected item has changed
   connect(treePreservers, SIGNAL( currentItemChanged ( QTreeWidgetItem *, QTreeWidgetItem *) ), this, SLOT(slotCurrentTreePreserverChanged(QTreeWidgetItem *) ) );

}

QString lifePreserver::getPreserverLastBackup(QString host)
{
   PRESERVER myPreserver(host);
   return myPreserver.getLastBackup();
}

QString lifePreserver::getPreserverFrequency(QString host)
{
   PRESERVER myPreserver(host);
   return myPreserver.getFrequency();
}

QString lifePreserver::getPreserverStatus(QString host)
{
   QString result;
   PRESERVER myPreserver(host);
   int pid = myPreserver.getPID();
   if ( pid == -1 )
   {
     result="Not running";
   } else {
     result="Running...";
   }
   return result;
}

void lifePreserver::createActions()
{
   minimizeAction = new QAction(tr("Mi&nimize"), this);
   connect(minimizeAction, SIGNAL(triggered()), this, SLOT(slotContextMenuHide()));

   restoreAction = new QAction(tr("&Restore"), this);
   connect(restoreAction, SIGNAL(triggered()), this, SLOT(slotContextMenuShow()));

   quitAction = new QAction(tr("&Quit"), this);
   connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

   autoStartAction = new QAction(tr("&Start at login"), this);
   autoStartAction->setCheckable(true);
   autoStartAction->setChecked(runAtStart);
   connect(autoStartAction, SIGNAL(triggered()), this, SLOT(slotToggleRunAtStart()));

}

void lifePreserver::createTrayIcon()
{
   trayIconMenu = new QMenu(this);
   trayIconMenu->addAction(minimizeAction);
   trayIconMenu->addAction(restoreAction);
   trayIconMenu->addSeparator();
   trayIconMenu->addAction(autoStartAction);
   trayIconMenu->addSeparator();
   trayIconMenu->addAction(quitAction);

   trayIcon = new QSystemTrayIcon(this);
   trayIcon->setContextMenu(trayIconMenu);
   trayIcon->setIcon(QIcon(":/images/images/tray-icon-idle.png"));
   trayIcon->show();

   connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(slotTrayActivated(QSystemTrayIcon::ActivationReason)));
}

void lifePreserver::slotContextMenuShow()
{
        if ( havePreservers ) {
	    showNormal();
	    w->hide();
        } else {
	    hide();
	    w->showNormal();
        }
}

void lifePreserver::slotContextMenuHide()
{
    w->hide();
    hide();
}

void lifePreserver::slotTrayActivated(QSystemTrayIcon::ActivationReason reason){
   if(reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::DoubleClick)
   {
        // We have preservers setup
        if ( havePreservers ) {
	  if ( isVisible() ) {
	    hide();
	    w->hide();
	  }else {
	    showNormal();
	    w->hide();
          }
        } else {
          // No preservers setup
	  if ( w->isVisible() ) {
	    hide();
	    w->hide();
	  }else {
	    hide();
	    w->showNormal();
          }
        } // End of have Preservers
   } // End of triggered
}

void lifePreserver::closeEvent(QCloseEvent *event)
{
   if(isVisible() )
     hide();
   event->ignore();
}

void lifePreserver::slotSingleInstance()
{
   slotTrayActivated(QSystemTrayIcon::Trigger);
}


void lifePreserver::slotToggleRunAtStart()
{
   QSettings settings("PCBSD", "LifePreserver");
   bool autoRun = settings.value("autostart", true).toBool();
   if ( autoRun )
      runAtStart = false;
   else
      runAtStart = true;

   settings.setValue("autostart", runAtStart);

   // Update check state
   createActions();
}
