/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
#include "dialogDesktopSelection.h"
#include "ui_dialogDesktopSelection.h"
#include <QTreeWidget>
#include <QMessageBox>
#include <QFile>
//#include <QtGui>

void desktopSelection::programInit(QList<QStringList> pkgList, QStringList sPkgs)
{
  listDeskPkgs = pkgList;
  selectedPkgs = sPkgs;
  connect(pushClose, SIGNAL(clicked()), this, SLOT(slotClose()));
  connect(pushSave, SIGNAL(clicked()), this, SLOT(slotSave()));
  initDesktopPkgs();
}

void desktopSelection::slotClose()
{
  close();
}

void desktopSelection::slotSave()
{
  emit saved(getDeskPkgList());
  close();
}

// Connect all the disk-related slots
void desktopSelection::initDesktopPkgs()
{
  // Start by reading our list of desktop pkgs, and populate the GUI
  addTreeItems(QString());

  treeWidgetDesktopPackages->setMouseTracking(true);
  connect(treeWidgetDesktopPackages, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(slotDeskPkgsChanged(QTreeWidgetItem *, int)));
  connect(treeWidgetDesktopPackages, SIGNAL(itemEntered(QTreeWidgetItem *, int)), this, SLOT(slotShowLabelDesc(QTreeWidgetItem *, int)));

  treeWidgetDesktopPackages->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(treeWidgetDesktopPackages, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(slotMetaRightClick()) );

}

void desktopSelection::addTreeItems(QString parent)
{
  for (int z=0; z < listDeskPkgs.count(); ++z) {

    if ( listDeskPkgs.at(z).at(3) != parent )
      continue;

    // Hide the "base-system" package, since we are installing it anyway
    if (listDeskPkgs.at(z).at(0) == "base-system" )
      continue;

    QTreeWidgetItem *deskItem = new QTreeWidgetItem(QStringList() << listDeskPkgs.at(z).at(0) );
    deskItem->setIcon(0, QIcon(listDeskPkgs.at(z).at(2)));
    deskItem->setToolTip(0, listDeskPkgs.at(z).at(1));
    deskItem->setCheckState(0, Qt::Unchecked);

    // Check if this is a selected meta-pkg
    for ( int s=0; s < selectedPkgs.count(); ++s)
      if ( listDeskPkgs.at(z).at(0) == selectedPkgs.at(s) )
        deskItem->setCheckState(0, Qt::Checked);

    if ( parent.isEmpty() ) {
      treeWidgetDesktopPackages->addTopLevelItem(deskItem);
    } else {
      // Locate the parent to attach to
      QTreeWidgetItemIterator it(treeWidgetDesktopPackages);
      while (*it) {
        if ((*it)->text(0) == parent ) {
	  (*it)->addChild(deskItem);
          if ( deskItem->checkState(0) == Qt::Checked && (*it)->checkState(0) == Qt::Unchecked)
            (*it)->setCheckState(0, Qt::PartiallyChecked);
	  break;
	}
        it++;
      }
    }

    // Now look for any possible children
    addTreeItems(listDeskPkgs.at(z).at(0));
  }

  // Sort the items
  treeWidgetDesktopPackages->sortItems(0, Qt::AscendingOrder);
}

// Slot to show meta-pkg description on mouseover
void desktopSelection::slotShowLabelDesc(QTreeWidgetItem *aItem, int aCol)
{
	Q_UNUSED(aCol);
	labelPkgDesc->setText("");

	if (!aItem)
  	  return;

	for (int z=0; z < listDeskPkgs.count(); ++z) {
		if ( aItem->text(0) == listDeskPkgs.at(z).at(0) ) {
			labelPkgDesc->setText(listDeskPkgs.at(z).at(1));
			break;
		}
	}
}

// The User changed the tree widget checked / unchecked stuff sanity check
void desktopSelection::slotDeskPkgsChanged(QTreeWidgetItem *aItem, int aCol)
{
	Q_UNUSED(aCol);
	if (!aItem)
  	  return;

        disconnect(treeWidgetDesktopPackages, SIGNAL(itemChanged(QTreeWidgetItem *, int)), 0, 0);

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
		if (aItem->checkState(0) == Qt::Checked ){
			checkAllChildrenPkgs(aItem->text(0));
		}else{
			uncheckAllChildrenPkgs(aItem->text(0));
		}
	}
	

    connect(treeWidgetDesktopPackages, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(slotDeskPkgsChanged(QTreeWidgetItem *, int)));
}

// Check if we have any desktop packages selected
bool desktopSelection::haveMetaDesktopChecked()
{
        QTreeWidgetItemIterator it(treeWidgetDesktopPackages);
        while (*it) {
         if ((*it)->checkState(0) == Qt::Checked || (*it)->checkState(0) == Qt::PartiallyChecked )
	   for (int z=0; z < listDeskPkgs.count(); ++z)
	     if ( (*it)->text(0) == listDeskPkgs.at(z).at(0) && listDeskPkgs.at(z).at(4) == "YES" )
		return true;
         ++it;
        }

        QMessageBox::warning(this, tr("No Desktop"),
          tr("No desktops have been selected! Please choose at least one desktop to continue."),
          QMessageBox::Ok,
          QMessageBox::Ok);

	return false;
}

// Check the "parent" app to see if all its children are checked or not
bool desktopSelection::allChildrenPkgsChecked(QString parent)
{
        QTreeWidgetItemIterator it(treeWidgetDesktopPackages);
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
bool desktopSelection::allChildrenPkgsUnchecked(QString parent)
{
        QTreeWidgetItemIterator it(treeWidgetDesktopPackages);
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
void desktopSelection::checkAllChildrenPkgs(QString parent)
{
        QTreeWidgetItemIterator it(treeWidgetDesktopPackages);
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
void desktopSelection::uncheckAllChildrenPkgs(QString parent)
{
        QTreeWidgetItemIterator it(treeWidgetDesktopPackages);
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

// Return the configuration for desktop packages
QStringList desktopSelection::getDeskPkgList()
{
   QStringList cfgList;
   bool skipCat;

   // For neatness sake, get the parents first
   QTreeWidgetItemIterator it(treeWidgetDesktopPackages);
   while (*it) {
     if ( !(*it)->parent() && ( (*it)->checkState(0) == Qt::Checked || (*it)->checkState(0) == Qt::PartiallyChecked ) ) {

        // Make sure this isn't just a placeholder category
	skipCat = false;
	for (int z=0; z < listDeskPkgs.count(); ++z)
		if ( (*it)->text(0) == listDeskPkgs.at(z).at(0) && \
			 listDeskPkgs.at(z).at(5) == "CATEGORY" )
			skipCat=true;
	if ( ! skipCat )
		// Add it
		cfgList << (*it)->text(0);
      }
      ++it;
   }

   // Now get the children
   QTreeWidgetItemIterator itC(treeWidgetDesktopPackages);
   while (*itC) {
     if ((*itC)->parent() && ( (*itC)->checkState(0) == Qt::Checked || (*itC)->checkState(0) == Qt::PartiallyChecked ) ) {

        // Make sure this isn't just a placeholder category
	skipCat = false;
	for (int z=0; z < listDeskPkgs.count(); ++z)
		if ( (*itC)->text(0) == listDeskPkgs.at(z).at(0) && \
			 listDeskPkgs.at(z).at(5) == "CATEGORY" )
			skipCat=true;

	if ( ! skipCat )
		cfgList << (*itC)->text(0);
      }
      ++itC;
   }

   return cfgList;
}

void desktopSelection::slotMetaRightClick()
{
        QTreeWidgetItemIterator it(treeWidgetDesktopPackages);
        while (*it) {
          for (int z=0; z < listDeskPkgs.count(); ++z) {
            if ( (*it)->isSelected() && (*it)->text(0) == listDeskPkgs.at(z).at(0) ) {
              if (listDeskPkgs.at(z).at(5) == "CATEGORY")
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

void desktopSelection::slotMetaViewPkgs()
{
        QStringList packageList;
        QTreeWidgetItemIterator it(treeWidgetDesktopPackages);
        while (*it) {
          for (int z=0; z < listDeskPkgs.count(); ++z) {
            if ( (*it)->isSelected() && (*it)->text(0) == listDeskPkgs.at(z).at(0) ) {
        
                QFile pList(listDeskPkgs.at(z).at(6));
                if ( ! pList.exists() )
                  return;
                
                if ( ! pList.open(QIODevice::ReadOnly | QIODevice::Text))
                  return;
              
                while ( !pList.atEnd() )
                  packageList << pList.readLine().simplified();
           
                pList.close();
                packageList.sort();

		// Check for deps list as well
                QFile depList("/root/pkg-dep-lists/" + (*it)->text(0) + ".deps");
                if ( depList.exists() && depList.open(QIODevice::ReadOnly | QIODevice::Text))
		{
                  packageList << " ";
                  packageList << "Additional packages:";
                  packageList << " ";
                  while ( !depList.atEnd() )
                    packageList << pList.readLine().simplified();
		}
 
                dIB = new dialogInfoBox();
                dIB->programInit(tr("Package Listing for:") + " " + (*it)->text(0));
                dIB->setInfoText(packageList.join("\n"));
                dIB->show();
            }
          }
         ++it;
        }
}
