#include <QRegExp>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QApplication>
#include <QDebug>

#include <QObject>
#include <QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>

//#include "pcbsd-utils.h"
#include "pcbsd-ui.h"
#include "dialogInfoBox.h"
#include "dialogMetaProgress.h"

#include "../../config.h"

////////////////////////////////////////////////////////////////////////////
// Code for the meta-pkg widget
///////////////////////////////////////////////////////////////////////////
void metaWidget::init(QString chroot)
{
  stopped = false;
  qDebug() << "Starting metaWidget...";
  treeMetaPkgs = new QTreeWidget();
  if ( chroot.isEmpty() ) {
    treeMetaPkgs->setHeaderLabel("System Meta Pkgs");
  } else {
    treeMetaPkgs->setHeaderLabel(QString("Meta Pkgs for") + QString(" ") + chroot);
    chrootArg1="--chroot";
    chrootArg2=chroot;
  }
  
  QVBoxLayout *verticalLayout = new QVBoxLayout(this);
  verticalLayout->addWidget(treeMetaPkgs);
  pushApplyMeta = new QPushButton(tr("Apply"));
  verticalLayout->addWidget(pushApplyMeta);
  QHBoxLayout *hLayout = new QHBoxLayout();
  QSpacerItem *dSpace = new QSpacerItem(10,30, QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
  hLayout->insertSpacerItem(0, dSpace);
  hLayout->addWidget(pushApplyMeta);
  hLayout->insertSpacerItem(-1, dSpace);
  verticalLayout->insertLayout(-1, hLayout);
  qDebug() << "Done metaWidget...";
  PkgSet = "pcbsd";

  Arch = getLineFromCommandOutput("uname -m");

  populateMetaPkgs();

  // Connect our slots
  treeMetaPkgs->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(treeMetaPkgs, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(slotMetaRightClick()) );
  connect(pushApplyMeta, SIGNAL( clicked() ), this, SLOT( slotApplyMetaChanges() ) );


}


// Display found meta-pkg data
void metaWidget::populateMetaPkgs()
{
  pushApplyMeta->setEnabled(false);
  treeMetaPkgs->clear();
  tmpMetaPkgList.clear();
  new QTreeWidgetItem(treeMetaPkgs, QStringList() << tr("Loading... Please wait...") );

  if ( ! metaPkgList.isEmpty() )
  	disconnect(treeMetaPkgs, SIGNAL(itemChanged(QTreeWidgetItem *, int)), 0, 0);
  metaPkgList.clear();

  // Start the process to get meta-pkg info
  getMetaProc = new QProcess();
  qDebug() << "Searching for meta-pkgs...";
  connect( getMetaProc, SIGNAL(readyReadStandardOutput()), this, SLOT(slotGetPackageDataOutput()) );
  connect( getMetaProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotFinishLoadingMetaPkgs()) );
  getMetaProc->setProcessChannelMode(QProcess::MergedChannels);
  getMetaProc->start(QString("pc-metapkgmanager"), QStringList() << chrootArg1 << chrootArg2 << "list");

}

// Display found meta-pkg data
void metaWidget::slotFinishLoadingMetaPkgs()
{

  // Populate the metaPkgList
  parseTmpMetaList();

  treeMetaPkgs->clear();

  addTreeItems(QString()); 

  connect(treeMetaPkgs, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(slotDeskPkgsChanged(QTreeWidgetItem *, int)));
}

void metaWidget::addTreeItems(QString parent)
{
  for (int z=0; z < metaPkgList.count(); ++z) {
    if ( metaPkgList.at(z).at(3) != parent )
      continue;

    // Hide the "base-system" package, since we are installing it anyway
    if (metaPkgList.at(z).at(0) == "base-system" )
      return;

    QTreeWidgetItem *deskItem = new QTreeWidgetItem(QStringList() << metaPkgList.at(z).at(0) );
    deskItem->setIcon(0, QIcon(metaPkgList.at(z).at(2)));
    deskItem->setToolTip(0, metaPkgList.at(z).at(1));
    deskItem->setCheckState(0, Qt::Unchecked);

    if ( metaPkgList.at(z).at(5) == "YES" )
      deskItem->setCheckState(0, Qt::Checked);

    if ( parent.isEmpty() ) {
      treeMetaPkgs->addTopLevelItem(deskItem);
    } else {
      // Locate the parent to attach to
      QTreeWidgetItemIterator it(treeMetaPkgs);
      while (*it) {
        if ((*it)->text(0) == parent ) {
	  (*it)->addChild(deskItem);
          if ( metaPkgList.at(z).at(5) == "YES" && (*it)->checkState(0) == Qt::Unchecked)
	    (*it)->setCheckState(0, Qt::PartiallyChecked);
          if ( metaPkgList.at(z).at(5) == "NO" && (*it)->checkState(0) == Qt::Checked)
	    (*it)->setCheckState(0, Qt::PartiallyChecked);
	  break;
	}
        it++;
      }
    }

    // Now look for any possible children
    addTreeItems(metaPkgList.at(z).at(0));    
  }
}

// Check if a meta-pkg is installed
bool metaWidget::isMetaPkgInstalled(QString mPkg)
{
  QString tmp;
  QProcess pcmp;
  pcmp.start(QString("pc-metapkgmanager"), QStringList() << chrootArg1 << chrootArg2 << "status" << mPkg);
  while ( pcmp.state() != QProcess::NotRunning ) {
     pcmp.waitForFinished(50);
     QCoreApplication::processEvents();
  }

  while (pcmp.canReadLine()) {
     tmp = pcmp.readLine().simplified();
     if ( tmp.indexOf("is installed") != -1 )
     return true;
  }

  return false;
}

// Function which checks for our GUI package schema data
void metaWidget::slotGetPackageDataOutput()
{
  while (getMetaProc->canReadLine())
	tmpMetaPkgList << getMetaProc->readLine().simplified();
}

// Parse the pc-metapkg saved output
void metaWidget::parseTmpMetaList()
{
  QString tmp, mName, mDesc, mIcon, mParent, mDesktop, mInstalled, mPkgFileList;
  QStringList package;

  for ( int i = 0 ; i < tmpMetaPkgList.size(); i++ )
  {
	QApplication::processEvents();

        // If we've requested to stop this widget, we can safely quit here
        if ( stopped )
	  return;

        tmp = tmpMetaPkgList.at(i);

	if ( tmp.indexOf("Meta Package: ") == 0) {
		mName = tmp.replace("Meta Package: ", "");
		continue;
	}
	if ( tmp.indexOf("Description: ") == 0) {
		mDesc = tmp.replace("Description: ", "");
		continue;
	}
	if ( tmp.indexOf("Icon: ") == 0) {
		mIcon = tmp.replace("Icon: ", "");
		mPkgFileList = mIcon;
		mPkgFileList.replace("pkg-icon.png", "pkg-list");
		continue;
	}
	if ( tmp.indexOf("Parent: ") == 0) {
		mParent = tmp.replace("Parent: ", "");
		continue;
	}
	if ( tmp.indexOf("Desktop: ") == 0) {
		mDesktop = tmp.replace("Desktop: ", "");
		continue;
	}

	// This is an empty category
	if ( tmp.indexOf("Category Entry") == 0) {
		// Now add this category to the string list
		package.clear();
		qDebug() << "Found Package" << mName << mDesc << mIcon << mParent << mDesktop;
		mInstalled = "CATEGORY";
		package << mName << mDesc << mIcon << mParent << mDesktop << mInstalled;
		metaPkgList.append(package);
		mName=""; mDesc=""; mIcon=""; mParent=""; mDesktop=""; mInstalled=""; mPkgFileList="";
	}

	// We have a Meta-Pkg
	if ( tmp.indexOf("Required Packages:") == 0) {
		// Now add this meta-pkg to the string list
		package.clear();
		qDebug() << "Found Package" << mName << mDesc << mIcon << mParent << mDesktop << mPkgFileList;

		if ( isMetaPkgInstalled(mName) )
			mInstalled = "YES";
		else
			mInstalled = "NO";

		package << mName << mDesc << mIcon << mParent << mDesktop << mInstalled << mPkgFileList;
		metaPkgList.append(package);
		mName=""; mDesc=""; mIcon=""; mParent=""; mDesktop=""; mInstalled=""; mPkgFileList="";
	}

    }

}

void metaWidget::saveMetaPkgs()
{
	if ( ! haveMetaPkgChanges() )
		return;

	addPkgs = getAddPkgs();
	delPkgs = getDelPkgs();	

	startMetaChanges();

}

void metaWidget::slotMetaDelCanceled()
{
  canceled=true;
  qDebug() << "Stopping pc-metapkgmanager";
  delMetaProc->terminate();
  delMetaProc->waitForFinished(500);
  while ( delMetaProc->state() != QProcess::NotRunning ) {
     QCoreApplication::processEvents();
     delMetaProc->waitForFinished(100);
     delMetaProc->kill();
  }
  qDebug() << "Done stopping pc-metapkgmanager";
}

void metaWidget::startMetaChanges()
{

        canceled=false;
        qDebug() << "Starting meta deletion";
	// We've gotten this far, now time to do the actual meta pkg changes
	if ( ! delPkgs.isEmpty() ) {

		metaProgressDel = new metaProgressDialog();
                metaProgressDel->programInit(tr("Performing system-package changes."));
		// Connect canceled slot
		connect( (QObject*)metaProgressDel, SIGNAL(canceled()), (QObject*)this, SLOT(slotMetaDelCanceled()) );
		metaProgressDel->show();

		delMetaProc = new QProcess();
		delMetaProc->setProcessChannelMode(QProcess::MergedChannels);
		connect( delMetaProc, SIGNAL(readyReadStandardOutput()), this, SLOT(slotMetaDelRead()) );
		connect( delMetaProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotMetaDelDone()) );
     		delMetaProc->start("pc-metapkgmanager", QStringList() << chrootArg1 << chrootArg2 << "del" << delPkgs);
	} else
		slotMetaDelDone();


}

void metaWidget::slotMetaDelRead()
{
	bool ok;

	while ( delMetaProc->canReadLine() ) {
		QString line = delMetaProc->readLine();
		if ( line.indexOf("Pending Meta-Package changes: ") != -1 ) {
			line = line.replace("Pending Meta-Package changes: ", "");
			line.toInt(&ok);
			if ( ok ) {
				metaProgressDel->setTotalRange(0, line.toInt(&ok) + 1);
				metaProgressDel->setTotalVal(0);
			}
			continue;
		}

		if ( line.indexOf("Removing Meta-Package: ") != -1 ) { 
			metaProgressDel->setTotalVal(metaProgressDel->getTotalVal() + 1);
			metaProgressDel->setTotalDesc(line);
			continue;
		}

		if ( line.indexOf("Pending package changes: ") != -1 ) {
			line = line.replace("Pending package changes: ", "");
			line.toInt(&ok);
			if ( ok ) {
				metaProgressDel->setSubRange(0, line.toInt(&ok) + 1);
				metaProgressDel->setSubVal(0);
			}
			continue;
			
		} else {
			metaProgressDel->setSubVal(metaProgressDel->getSubVal() + 1);
			metaProgressDel->setSubDesc(line);
		}
	}
}

void metaWidget::slotMetaAddCanceled()
{
  canceled=true;
  qDebug() << "Stopping pc-metapkgmanager";
  addMetaProc->terminate();
  addMetaProc->waitForFinished(500);
  while ( addMetaProc->state() != QProcess::NotRunning ) {
     QCoreApplication::processEvents();
     addMetaProc->waitForFinished(100);
     addMetaProc->kill();
  }
  qDebug() << "Done stopping pc-metapkgmanager";
}

void metaWidget::slotMetaDelDone()
{
	if ( ! delPkgs.isEmpty() ) {
		metaProgressDel->hide();
		delete metaProgressDel;
	}

        // If the user canceled the process, we can stop here
        if ( canceled )
	      return;

	// We've gotten this far, now time to do the actual meta pkg changes
	if ( ! addPkgs.isEmpty() ) {
	
		metaProgressAdd = new metaProgressDialog();
                metaProgressAdd->programInit(tr("Performing system-package changes."));
		// Connect canceled slot
		connect( (QObject*)metaProgressAdd, SIGNAL(canceled()), (QObject*)this, SLOT(slotMetaAddCanceled()) );
		metaProgressAdd->show();

		addMetaProc = new QProcess();
		addMetaProc->setProcessChannelMode(QProcess::MergedChannels);
		// Set the process environment flag for parsing download status
  		QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  		env.insert("GUI_FETCH_PARSING", "YES");
  		addMetaProc->setProcessEnvironment(env);

		connect( addMetaProc, SIGNAL(readyReadStandardOutput()), this, SLOT(slotMetaAddRead()) );
		connect( addMetaProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotMetaAddDone()) );
     		addMetaProc->start("pc-metapkgmanager", QStringList() << chrootArg1 << chrootArg2 <<"add" << addPkgs);
		qDebug() << "pc-metapkgmanager" << chrootArg1 << chrootArg2 << "add" << addPkgs;
	} else {
			populateMetaPkgs();
	}
}

void metaWidget::slotMetaAddRead()
{
	bool ok, ok2;

	// This functionality needs to be re-written to not loop, uses to much CPU
	while ( addMetaProc->canReadLine() ) {
		QString line = addMetaProc->readLine();

		// Read the number of meta-pkgs we will be adding
		if ( line.indexOf("Pending Meta-Package changes: ") != -1 ) {
			line = line.replace("Pending Meta-Package changes: ", "");
			line.toInt(&ok);
			if ( ok ) {
				metaProgressAdd->setTotalRange(0, line.toInt(&ok) + 1);
				metaProgressAdd->setTotalVal(0);
			}
			continue;
		}

		// Check if on a new meta-pkg now
		if ( line.indexOf("Installing Meta-Package: ") != -1 ) { 
			metaProgressAdd->setTotalVal(metaProgressAdd->getTotalVal() + 1);
			metaProgressAdd->setTotalDesc(line);
			continue;
		}

		if ( line.indexOf("Pending package changes: ") != -1 ) {
			line = line.replace("Pending package changes: ", "");
			line.toInt(&ok);
			if ( ok ) {
				metaProgressAdd->setSubRange(0, (line.toInt(&ok) * 2) + 1 );
				metaProgressAdd->setSubVal(0);
			}
			continue;	
		}

		if ( line.indexOf("Downloading package: ") != -1 ) {
			line = line.replace("Downloading package: ", "");
			metaProgressAdd->setDLVisible(true);
			metaProgressAdd->setDLRange(0, 10);
			metaProgressAdd->setDLVal(0);
			metaProgressAdd->setDLDesc("");

			// Increment the sub-bar
			metaProgressAdd->setSubVal(metaProgressAdd->getSubVal() + 1);
			metaProgressAdd->setSubDesc(tr("Downloading: %1").arg(line) );
			continue;	
		}

 	    	if ( line.indexOf("SIZE: ") == 0 ) {
       			line.replace("SIZE: ", "");
        		line.replace("DOWNLOADED: ", "");
        		line.replace("SPEED: ", "");
        		line.section(" ", 0, 0).toInt(&ok);
        		line.section(" ", 1, 1).toInt(&ok2);
   
			qDebug() << line;
        		if ( ok && ok2 ) {
			   QString unit;
            		   int tot = line.section(" ", 0, 0).toInt(&ok);
            		   int cur = line.section(" ", 1, 1).toInt(&ok2);
            		   QString percent = QString::number( (float)(cur * 100)/tot );
            		   QString speed = line.section(" ", 2, 3);

  			   // Get the MB downloaded / total
			   if ( tot > 2048 ) {
			      unit="MB";
			      tot = tot / 1024;
            		      cur = cur / 1024;
			   } else {
			      unit="KB";
                           }

            		   QString ProgressString=QString("%1" + unit + " of %2" + unit + " at %3").arg(cur).arg(tot).arg(speed);
			   metaProgressAdd->setDLDesc(ProgressString);
			   metaProgressAdd->setDLRange(0, tot);
			   metaProgressAdd->setDLVal(cur);
     			   QCoreApplication::processEvents();
			}
			continue;
		}

		if ( line.indexOf("Installing package: ") != -1 ) {
			metaProgressAdd->setDLVisible(false);
			metaProgressAdd->setSubVal(metaProgressAdd->getSubVal() + 1);
			metaProgressAdd->setSubDesc(line);
		}
	}

}

void metaWidget::slotMetaAddDone()
{
	//if ( ! addPkgs.isEmpty() )
	metaProgressAdd->hide();
	delete metaProgressAdd;

 
	populateMetaPkgs();

        // If the user canceled the process, we can stop here
        if ( canceled )
	      return;

	if ( addMetaProc->exitCode() != 0 || addMetaProc->exitStatus() != QProcess::NormalExit ) {
        	QMessageBox::critical(this, tr("System Packages"),
        	tr("The meta-pkg manager returned an error. For more details please look at the log file:") + " /tmp/.pc-metapkgmanager.log",
        	QMessageBox::Ok,
        	QMessageBox::Ok);
	} else {
        	QMessageBox::information(this, tr("System Packages"),
        	tr("System packages updated successfully."),
        	QMessageBox::Ok,
        	QMessageBox::Ok);
        }
}

bool metaWidget::haveAMetaDesktop()
{
	// If running in a chroot we can skip this check
	if ( ! chrootArg1.isEmpty() )
	  return true;
        
        QTreeWidgetItemIterator it(treeMetaPkgs);
        while (*it) {
         if ( ((*it)->checkState(0) == Qt::Checked) || ((*it)->checkState(0) == Qt::PartiallyChecked) )
	   for (int z=0; z < metaPkgList.count(); ++z)
	     if ( (*it)->text(0) == metaPkgList.at(z).at(0) && metaPkgList.at(z).at(4) == "YES" )
		return true;
         ++it;
        }

        QMessageBox::warning(this, tr("No Desktop"),
          tr("No desktops have been selected! Please choose at least one desktop before saving."),
          QMessageBox::Ok,
          QMessageBox::Ok);

	return false;
}

bool metaWidget::haveMetaPkgChanges()
{
        QTreeWidgetItemIterator it(treeMetaPkgs);
        while (*it) {
	  for (int z=0; z < metaPkgList.count(); ++z)
	    // See if any packages status have changed
	    if ( ( (*it)->text(0) == metaPkgList.at(z).at(0) && metaPkgList.at(z).at(5) == "YES" && (*it)->checkState(0) == Qt::Unchecked ) \
	      || ( (*it)->text(0) == metaPkgList.at(z).at(0) && metaPkgList.at(z).at(5) == "YES" && (*it)->checkState(0) == Qt::PartiallyChecked ) \
	      || ( (*it)->text(0) == metaPkgList.at(z).at(0) && metaPkgList.at(z).at(5) == "NO" && (*it)->checkState(0) == Qt::Checked ) )
		return true;
         ++it;
        }

	return false;
}

QString metaWidget::getAddPkgs()
{
	QString tmp;
        QTreeWidgetItemIterator it(treeMetaPkgs);
        while (*it) {
	  for (int z=0; z < metaPkgList.count(); ++z)
	    // See if any packages status have changed
	    if ( ( (*it)->text(0) == metaPkgList.at(z).at(0) && metaPkgList.at(z).at(5) == "NO" && (*it)->checkState(0) == Qt::Checked ) || \
	         ( (*it)->text(0) == metaPkgList.at(z).at(0) && metaPkgList.at(z).at(5) == "NO" && (*it)->checkState(0) == Qt::PartiallyChecked ) ) {
		if ( tmp.isEmpty() ) {
			tmp = (*it)->text(0);
		} else {
			tmp = tmp + "," + (*it)->text(0);
		}
	    }
         ++it;
        }

	return tmp;
}

QString metaWidget::getDelPkgs()
{
	QString tmp;
        QTreeWidgetItemIterator it(treeMetaPkgs);
        while (*it) {
	  for (int z=0; z < metaPkgList.count(); ++z)
	    // See if any packages status have changed
	    if ( (*it)->text(0) == metaPkgList.at(z).at(0) && metaPkgList.at(z).at(5) == "YES" && (*it)->checkState(0) == Qt::Unchecked ) {
		if ( tmp.isEmpty() ) {
			tmp = (*it)->text(0);
		} else {
			tmp = tmp + "," + (*it)->text(0);
		}
            }
         ++it;
        }

	return tmp;
}


// Time to save meta-pkgs
void metaWidget::slotApplyMetaChanges() {
    saveMetaPkgs();
}



// The User changed the tree widget checked / unchecked stuff sanity check
void metaWidget::slotDeskPkgsChanged(QTreeWidgetItem *aItem, int __unused)
{
	if (!aItem)
  	  return;

        disconnect(treeMetaPkgs, SIGNAL(itemChanged(QTreeWidgetItem *, int)), 0, 0);

	if (aItem->childCount() == 0) {
		if (aItem->checkState(0) == Qt::Checked && aItem->parent() ){
			if ( allChildrenPkgsChecked(aItem->parent()->text(0))){
				aItem->parent()->setCheckState(0, Qt::Checked);	
			}else{
				aItem->parent()->setCheckState(0, Qt::PartiallyChecked);	
			}
		}
		if (aItem->checkState(0) == Qt::Unchecked && aItem->parent() ){
			if ( ! allChildrenPkgsUnchecked(aItem->parent()->text(0)))
				aItem->parent()->setCheckState(0, Qt::PartiallyChecked);	
		}

	} else {
		if (aItem->checkState(0) == Qt::Checked )
			checkAllChildrenPkgs(aItem->text(0));
		else
			uncheckAllChildrenPkgs(aItem->text(0));
	}
	

    connect(treeMetaPkgs, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(slotDeskPkgsChanged(QTreeWidgetItem *, int)));

	if ( haveMetaPkgChanges() )
		pushApplyMeta->setEnabled(true);
	else
		pushApplyMeta->setEnabled(false);
}

// Check the "parent" app to see if all its children are checked or not
bool metaWidget::allChildrenPkgsChecked(QString parent)
{
        QTreeWidgetItemIterator it(treeMetaPkgs);
        while (*it) {
         if ((*it)->text(0) == parent ) {
	   if ( (*it)->childCount() <= 0)
	     return true;

	   for ( int i = 0; i < (*it)->childCount() ; ++i) {
	     if ( ! allChildrenPkgsChecked((*it)->child(i)->text(0)))
	       return false;

             if ((*it)->child(i)->checkState(0) != Qt::Checked ) 
	       return false;
	   }
         }
         ++it;
        }
	return true;
}

// Check the "parent" app to see if all its children are unchecked or not
bool metaWidget::allChildrenPkgsUnchecked(QString parent)
{
        QTreeWidgetItemIterator it(treeMetaPkgs);
        while (*it) {
         if ((*it)->text(0) == parent ) {
	   if ( (*it)->childCount() <= 0)
	     return true;

	   for ( int i = 0; i < (*it)->childCount() ; ++i) {
	     if ( ! allChildrenPkgsUnchecked((*it)->child(i)->text(0)))
	       return false;

             if ((*it)->child(i)->checkState(0) != Qt::Unchecked ) 
	       return false;
	   }
         }
         ++it;
        }
	return true;
}

// Check all children of parent
void metaWidget::checkAllChildrenPkgs(QString parent)
{
        QTreeWidgetItemIterator it(treeMetaPkgs);
        while (*it) {
         if (! (*it)->parent()) {
           ++it;
	   continue;
	 } 

         // Lets walk the tree see what pops up
	 bool pFound=false;
         QTreeWidgetItem *itP = (*it)->parent();
	 do {
	   pFound=false;
	   if (itP->text(0) == parent) {
	     (*it)->setCheckState(0, Qt::Checked);
	     break;
	   }
	   if ( itP->parent() ) {
	     itP = itP->parent();
             pFound=true;
           }
         } while (pFound);

         ++it;
       }
}

// UnCheck all children of parent
void metaWidget::uncheckAllChildrenPkgs(QString parent)
{
        QTreeWidgetItemIterator it(treeMetaPkgs);
        while (*it) {
         if (! (*it)->parent()) {
           ++it;
	   continue;
	 } 

         // Lets walk the tree see what pops up
	 bool pFound=false;
         QTreeWidgetItem *itP = (*it)->parent();
	 do {
	   pFound=false;
	   if (itP->text(0) == parent) {
	     (*it)->setCheckState(0, Qt::Unchecked);
	     break;
	   }
	   if ( itP->parent() ) {
	     itP = itP->parent();
             pFound=true;
           }
         } while (pFound);

         ++it;
       }
}

void metaWidget::slotMetaRightClick()
{
        QTreeWidgetItemIterator it(treeMetaPkgs);
        while (*it) {
          for (int z=0; z < metaPkgList.count(); ++z) {
            if ( (*it)->isSelected() && (*it)->text(0) == metaPkgList.at(z).at(0) ) {
	      if (metaPkgList.at(z).at(5) == "CATEGORY")
	        return;
	      popup = new QMenu;
	      popup->setTitle((*it)->text(0));
	      popup->addAction(tr("View Packages"), this, SLOT(slotMetaViewPkgs()));
	      popup->exec( QCursor::pos() );
	    }
	  }
         ++it;
        }
}

void metaWidget::slotMetaViewPkgs()
{
	QStringList packageList;
        QTreeWidgetItemIterator it(treeMetaPkgs);
        while (*it) {
          for (int z=0; z < metaPkgList.count(); ++z) {
            if ( (*it)->isSelected() && (*it)->text(0) == metaPkgList.at(z).at(0) ) {
 
		QFile pList(metaPkgList.at(z).at(6));
		if ( ! pList.exists() )
		  return;
		
 	        if ( ! pList.open(QIODevice::ReadOnly | QIODevice::Text))
                  return;

                while ( !pList.atEnd() )
     		  packageList << pList.readLine().simplified();

		pList.close();
	        packageList.sort();
			
		dIB = new dialogInfo();
    		dIB->programInit(tr("Package Listing for:") + " " + (*it)->text(0));
		dIB->setInfoText(packageList.join("\n"));
                dIB->show();
	    }
	  }
         ++it;
        }
}

QString metaWidget::getLineFromCommandOutput( QString cmd )
{
        FILE *file = popen(cmd.toLatin1(),"r");
 
        char buffer[100];
 
        QString line = "";
        char firstChar;

        if ((firstChar = fgetc(file)) != -1){
                line += firstChar;
                line += fgets(buffer,100,file);
        }
        pclose(file);
        return line.simplified();
}


void metaWidget::stop()
{
   if ( getMetaProc->state() != QProcess::NotRunning ) {
      getMetaProc->kill();
      getMetaProc->waitForFinished(2000);
   }
   stopped=true;
   setHidden(true);
}

void metaWidget::setPackageSet(QString set)
{
   PkgSet=set;
}
