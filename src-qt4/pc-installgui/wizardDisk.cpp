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
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include "wizardDisk.h"
#include "ui_wizardDisk.h"

void wizardDisk::programInit()
{
  prevID = 0;

  populateDiskInfo();

  //connect(pushClose, SIGNAL(clicked()), this, SLOT(slotClose()));
  connect(pushSizeMount, SIGNAL(clicked()), this, SLOT(slotResizeFS()));
  connect(pushRemoveMount, SIGNAL(clicked()), this, SLOT(slotRemoveFS()));
  connect(pushAddMount, SIGNAL(clicked()), this, SLOT(slotAddFS()));
  connect(this,SIGNAL(currentIdChanged(int)),this,SLOT(slotCheckComplete()));
  connect(lineZpoolName,SIGNAL(textChanged ( const QString & )),this,SLOT(slotCheckComplete()));
  connect(groupZFSPool,SIGNAL( clicked(bool)),this,SLOT(slotCheckComplete()));
  connect(comboDisk,SIGNAL(currentIndexChanged(int)),this,SLOT(slotCheckComplete()));
  connect(comboDisk,SIGNAL(currentIndexChanged(int)),this,SLOT(slotChangedDisk()));
  connect(comboPartition,SIGNAL(currentIndexChanged(int)),this,SLOT(slotCheckComplete()));
  connect(treeMounts,SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),this,SLOT(slotTreeDiskChanged()));
  treeMounts->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(treeMounts,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(slotTreeMountsRightClick()));
  connect(pushTerminal, SIGNAL(clicked()), this, SLOT(slotTerminal()));

  // ZFS Mode Page
  connect(comboZFSMode,SIGNAL(currentIndexChanged(int)),this,SLOT(slotCheckComplete()));
  connect(groupZFSOpts,SIGNAL(clicked(bool)),this,SLOT(slotCheckComplete()));
  connect(listZFSDisks,SIGNAL(itemClicked(QListWidgetItem *)),this,SLOT(slotCheckComplete()));
  connect(listZFSDisks,SIGNAL(itemActivated(QListWidgetItem *)),this,SLOT(slotCheckComplete()));
  connect(listZFSDisks,SIGNAL(itemChanged(QListWidgetItem *)),this,SLOT(slotCheckComplete()));

}

void wizardDisk::populateDiskInfo()
{
  qDebug() << "Loading Disk Info";
  sysDisks = Scripts::Backend::hardDrives();

  // load drives
  comboDisk->clear();
  for (int i=0; i < sysDisks.count(); ++i) {
    // Make sure to only add the drives to the comboDisk
    if ( sysDisks.at(i).at(0) == "DRIVE" )
      comboDisk->addItem(sysDisks.at(i).at(1) + " - " + sysDisks.at(i).at(2) + "MB " + sysDisks.at(i).at(3));
  }

  // Reload the slice list box
  slotChangedDisk();
}

void wizardDisk::slotChangedDisk()
{
  QString ptag;

  if ( comboDisk->currentText().isEmpty())
    return;

  comboPartition->clear();
  comboPartition->addItem(tr("Use entire disk"));
       
  QString disk = comboDisk->currentText();
  disk.truncate(disk.indexOf(" -"));
  for (int i=0; i < sysDisks.count(); ++i) {
    // Make sure to only add the slices to the listDiskSlices
    if ( sysDisks.at(i).at(0) == "SLICE" && disk == sysDisks.at(i).at(1) && sysDisks.at(i).at(4) != "Unused Space") {
      ptag = sysDisks.at(i).at(4).section(",", 0, 0);
      ptag = ptag.section("/", 0, 0);
      ptag.truncate(15);
      if ( ptag.indexOf(")") == -1 )
        ptag += ")";
      comboPartition->addItem(sysDisks.at(i).at(2) + ": " +  sysDisks.at(i).at(3) + "MB " + ptag );
    }
  }

}

void wizardDisk::slotClose()
{
  close();
}

void wizardDisk::accept()
{
  bool useGPT = false;
  QString zpoolName;
  if (comboPartition->currentIndex() == 0 )
    useGPT = checkGPT->isChecked();

  // When doing advanced ZFS setups, make sure to use GPT
  if ( radioAdvanced->isChecked() && groupZFSOpts->isChecked() )
    useGPT = true;

  if ( radioAdvanced->isChecked() && groupZFSPool->isChecked() )
     zpoolName = lineZpoolName->text();

  if ( radioExpert->isChecked() )
    emit saved(sysFinalDiskLayout, false, false, zpoolName);
  else
    emit saved(sysFinalDiskLayout, true, useGPT, zpoolName);
  close();
}

int wizardDisk::nextId() const
{
  switch (currentId()) {
     case Page_Intro:
       if (radioExpert->isChecked())
         return Page_Expert;
       if (radioBasic->isChecked()) {
	 checkGPT->setVisible(false);
	 groupZFSPool->setVisible(false);
       }
       if (radioAdvanced->isChecked()) {
	 checkGPT->setVisible(true);
	 groupZFSPool->setVisible(true);
       }
	break;
     case Page_BasicDisk:
       if (radioBasic->isChecked())
         return Page_Confirmation;
       if (comboPartition->currentIndex() != 0 ) {
	 groupZFSOpts->setEnabled(false);
         return Page_Mounts;
       } else {
         groupZFSOpts->setEnabled(true);
         return Page_ZFS;
       }
       break;
     case Page_ZFS:
       return Page_Mounts;
       break;
     case Page_Mounts:
       return Page_Confirmation;
       break;
     case Page_Confirmation:
       return -1;
       break;
     default:
	return currentId() + 1;
  }
  return currentId() + 1;
}

// Logic checks to see if we are ready to move onto next page
bool wizardDisk::validatePage()
{
  // Generate suggested disk layout and show disk tree
  if ( prevID == Page_BasicDisk && currentId() == Page_Mounts) {
    generateDiskLayout();
    populateDiskTree();
  }

  // Generate suggested disk layout and show disk tree
  if ( prevID == Page_ZFS && currentId() == Page_Mounts) {
    generateDiskLayout();
    populateDiskTree();
  } 

  // Show the other disks available
  if ( prevID == Page_BasicDisk && currentId() == Page_ZFS)
     populateZFSDisks();

  // Basic mode, generate a disk layout and show summary
  if ( prevID == Page_BasicDisk && currentId() == Page_Confirmation) {
    generateDiskLayout();
    generateConfirmationText();
  }

  // Create the final disk layout from user options
  if ( prevID == Page_Mounts && currentId() == Page_Confirmation) {
    generateCustomDiskLayout();
    generateConfirmationText();
  }

  // Create the final disk layout from user options
  if ( prevID == Page_Expert && currentId() == Page_Confirmation) {
    generateConfirmationText();
  }
  
  
  // Reset the prevID
  prevID = currentId();

  switch (currentId()) {
     case Page_Intro:
         button(QWizard::NextButton)->setEnabled(true);
         return true;
     case Page_BasicDisk:
	 if ( comboPartition->currentIndex() == 0  && radioAdvanced->isChecked())
	   checkGPT->setVisible(true);
	 else
	   checkGPT->setVisible(false);

	 // Doing a Advanced install
	 if ( radioAdvanced->isChecked() && groupZFSPool->isChecked() )
	 {
	    if ( lineZpoolName->text().isEmpty() ) {
              button(QWizard::NextButton)->setEnabled(false);
              return false;
	    }
	    if ( lineZpoolName->text().contains(" ") ) {
              button(QWizard::NextButton)->setEnabled(false);
              return false;
	    }
	    QRegExp *re = new QRegExp("^[-'a-zA-Z]*$"); 
	    if (! re->exactMatch(lineZpoolName->text()) ) {
              button(QWizard::NextButton)->setEnabled(false);
              return false;
	    }

	 }

         // Comment out this disk space check
	 // We will warn the user right before install if the selected
	 // Disk / partition looks too small
	 //if ( ! checkDiskSpace() ) {
         //  button(QWizard::NextButton)->setEnabled(false);
	 //  return false;
	 //}
	
         // if we get this far, all the fields are filled in
         button(QWizard::NextButton)->setEnabled(true);
         return true;
     case Page_ZFS:
         // Check if we have valid ZFS disk options specified
	 if ( ! groupZFSOpts->isChecked() ) {
           button(QWizard::NextButton)->setEnabled(true);
	   return true;
	 }
         // Need at least one other disk for mirroring
	 if ( comboZFSMode->currentText() == "mirror" ) {
            labelZFSMsg->setText(tr("Please select at least 1 other drive for mirroring"));
	    for ( int i = 0; i < listZFSDisks->count(); ++i )
            {
		if ( listZFSDisks->item(i)->checkState() == Qt::Checked ) {
                   button(QWizard::NextButton)->setEnabled(true);
	           return true;
                }
            }
         }
	 if ( comboZFSMode->currentText() == "raidz1" ) {
            labelZFSMsg->setText(tr("Please select 2 or 4 additional drives for raidz1"));
	    int numChecked = 0;
	    for ( int i = 0; i < listZFSDisks->count(); ++i )
		if ( listZFSDisks->item(i)->checkState() == Qt::Checked )
		   numChecked++;
            if ( numChecked == 2 || numChecked == 4 ) {
              button(QWizard::NextButton)->setEnabled(true);
	      return true;
            }
         }
	 if ( comboZFSMode->currentText() == "raidz2" ) {
            labelZFSMsg->setText(tr("Please select 3, 5, or 9 additional drives for raidz2"));
	    int numChecked = 0;
	    for ( int i = 0; i < listZFSDisks->count(); ++i )
		if ( listZFSDisks->item(i)->checkState() == Qt::Checked )
		   numChecked++;
            if ( numChecked == 3 || numChecked == 5 || numChecked == 9 ) {
              button(QWizard::NextButton)->setEnabled(true);
	      return true;
            }
         }
	 if ( comboZFSMode->currentText() == "raidz3" ) {
            labelZFSMsg->setText(tr("Please select 4, 6, or 10 additional drives for raidz3"));
	    int numChecked = 0;
	    for ( int i = 0; i < listZFSDisks->count(); ++i )
		if ( listZFSDisks->item(i)->checkState() == Qt::Checked )
		   numChecked++;
            if ( numChecked == 4 || numChecked == 6 || numChecked == 10 ) {
              button(QWizard::NextButton)->setEnabled(true);
	      return true;
            }
         }

         // Disable the next button until we get a working config
         button(QWizard::NextButton)->setEnabled(false);
         return false;
     case Page_Confirmation:
         button(QWizard::FinishButton)->setEnabled(true);
         return true;
     default:
         button(QWizard::NextButton)->setEnabled(true);
         return true;
  }

  return true;
}

void wizardDisk::populateZFSDisks()
{
   qDebug() << "Adding ZFS disks...";
   listZFSDisks->clear();

   QString curDisk = comboDisk->currentText();
   curDisk.truncate(curDisk.indexOf(" -"));

   for (int z=0; z < sysDisks.count(); ++z)
     if ( sysDisks.at(z).at(0) == "DRIVE" && sysDisks.at(z).at(1) != curDisk  )
     {
        QListWidgetItem *dItem = new QListWidgetItem(sysDisks.at(z).at(1) + " - " + sysDisks.at(z).at(2) + "MB " + sysDisks.at(z).at(3));
        dItem->setCheckState(Qt::Unchecked);
        listZFSDisks->addItem(dItem);
     }
}

bool wizardDisk::checkDiskSpace()
{
  if ( getDiskSliceSize() < 10000 )
    return false;

  return true;
}

void wizardDisk::slotCheckComplete()
{
   // Validate this page
   validatePage();
}


void wizardDisk::generateDiskLayout()
{
  QString targetType, tmp;
  int targetLoc, totalSize = 0;
  QString targetDisk, targetSlice, tmpPass, fsType, target;

  // Clear out the original disk layout
  sysFinalDiskLayout.clear();
  QStringList fileSystem;
  qDebug() << "Generating disk layout";

  if ( comboPartition->currentIndex() == 0) {
    targetType = "DRIVE";
    targetSlice = "ALL";
    targetDisk = comboDisk->currentText();
    targetDisk.truncate(targetDisk.indexOf(" -"));
    target = targetDisk;
    targetLoc = 1;
  } else {
    targetType = "SLICE";
    targetDisk = comboDisk->currentText();
    targetDisk.truncate(targetDisk.indexOf(" -"));
    targetSlice = comboPartition->currentText();
    targetSlice.truncate(targetSlice.indexOf(":"));
    targetSlice = targetSlice.remove(0, targetSlice.size() -2);
    target = targetDisk + targetSlice;
    targetLoc = 2;
  }
  

  totalSize = getDiskSliceSize();
  if ( totalSize != -1 )
  {
     fsType= "ZFS";

     // This lets the user do nifty stuff like a mirror/raid post-install with a single zpool command
    fileSystem << targetDisk << targetSlice << "/,/tmp(compress=lzjb),/usr(canmount=off),/usr/home,/usr/jails,/usr/obj(compress=lzjb),/usr/pbi,/usr/ports(compress=gzip),/usr/ports/distfiles(compress=off),/usr/src(compress=gzip),/var(canmount=off),/var/audit(compress=lzjb),/var/log(compress=gzip),/var/tmp(compress=lzjb)" << fsType << tmp.setNum(totalSize) << "" << tmpPass;
     //qDebug() << "Auto-Gen FS:" <<  fileSystem;
     sysFinalDiskLayout << fileSystem;
     fileSystem.clear();
  }
  
  qDebug() << "AutoLayout:" <<  sysFinalDiskLayout;
}

void wizardDisk::populateDiskTree()
{
  QStringList tmpList, zMnts;
  QString tmp, opts;

  treeMounts->clear();
  treeMounts->setHeaderLabels(QStringList() << "ID" << tr("ZFS Mounts") << tr("ZFS Options") );
  treeMounts->setColumnCount(3);
  labelFreeSpace->setVisible(false);
  lineFreeMB->setVisible(false);
  pushSizeMount->setVisible(false);

  treeMounts->header()->setSectionHidden(0, true);
  treeMounts->header()->setDefaultSectionSize(150);


  zMnts = sysFinalDiskLayout.at(0).at(2).split(",");

  // Now loop through ZFS mounts
  for (int i=0; i < zMnts.count(); ++i) {
    tmpList.clear();
    opts = zMnts.at(i).section("(", 1, 1).section(")", 0, 0); 
    tmpList << tmp.setNum(i+1) << zMnts.at(i).split("(").at(0) << opts ;
    QTreeWidgetItem *mItem = new QTreeWidgetItem(treeMounts, tmpList);
    mItem->setToolTip(2, opts);
  }

  treeMounts->setCurrentItem(treeMounts->findItems("0", Qt::MatchFixedString).at(0));
  slotTreeDiskChanged();  

}

int wizardDisk::getDiskSliceSize()
{
  bool ok;
  QString disk = comboDisk->currentText();
  disk.truncate(disk.indexOf(" -"));

  int safeBuf = 10;

  // Check the full disk
  if ( comboPartition->currentIndex() == 0) {
    for (int i=0; i < sysDisks.count(); ++i) {
      // Make sure to only add the drives to the comboDiskList
      if ( sysDisks.at(i).at(0) == "DRIVE" && sysDisks.at(i).at(1) == disk ) {
        //qDebug() << "Selected Disk Size: " +  sysDisks.at(i).at(2);
        sysDisks.at(i).at(2).toInt(&ok);
        if( ok )
          return sysDisks.at(i).at(2).toInt(&ok) - safeBuf;
        else
  	  return -1;
      }
    }
  } else {
    // Check the selected partition
    QString slice = comboPartition->currentText();
    slice.truncate(slice.indexOf(":"));
    for (int i=0; i < sysDisks.count(); ++i) {
      // Make sure to only add the slices to the listDiskSlices
      if ( sysDisks.at(i).at(0) == "SLICE" && slice == sysDisks.at(i).at(2)) {
        //qDebug() << "Selected Slice Size: " +  sysDisks.at(i).at(3);
        sysDisks.at(i).at(3).toInt(&ok);
        if( ok )
          return sysDisks.at(i).at(3).toInt(&ok) - safeBuf;
        else
          return -1;
      }
    }
  }

  return -1;
}

void wizardDisk::slotTreeDiskChanged()
{
   if ( ! treeMounts->currentItem() )
     return;

   pushRemoveMount->setEnabled(true);
   pushAddMount->setEnabled(true);
   pushSizeMount->setEnabled(true);

   if ( treeMounts->currentItem()->text(1) == "/boot" || treeMounts->currentItem()->text(1) == "/")
     pushRemoveMount->setEnabled(false);

   if ( lineFreeMB->text() == "0" )
     pushAddMount->setEnabled(false);
}

void wizardDisk::slotResizeFS()
{
  if ( ! treeMounts->currentItem() )
  return;

  QString mnt = treeMounts->currentItem()->text(1);
  bool ok;
  int curSize, availSize, minSize; 
  curSize =  treeMounts->currentItem()->text(2).toInt(&ok);
  availSize = curSize + lineFreeMB->text().toInt(&ok);

  minSize = 100;

  // See if we need some other sanity check on sizes
  if ( mnt == "SWAP" )
    minSize = 256;
  if ( mnt == "/" )
    minSize = 2000;
  if ( mnt == "/boot" )
    minSize = 1000;

  addingMount="";
  rFS = new dialogFSSize();
  rFS->programInit(QString(tr("Resizing") + " " + mnt), curSize, availSize, minSize);
  rFS->setWindowModality(Qt::ApplicationModal);
  connect(rFS, SIGNAL(saved(int)), this, SLOT(slotSaveFSResize(int)));
  rFS->show();
  rFS->raise();
}

void wizardDisk::slotRemoveFS()
{
  if ( ! treeMounts->currentItem() )
  return;

  int ret = QMessageBox::question(this, tr("Remove mount-point"),
              tr("Are you sure you want to remove this mount point?"),
              QMessageBox::Yes | QMessageBox::No,
              QMessageBox::No);

  if ( ret != QMessageBox::Yes )
    return;
 
  QTreeWidgetItem *rmItem = treeMounts->currentItem();
  treeMounts->setCurrentItem(treeMounts->findItems("0", Qt::MatchFixedString).at(0));

  delete rmItem;
}

void wizardDisk::slotAddFS()
{
  bool ok;
  QString tmp;
  QString nMount = QInputDialog::getText(this, tr("Enter mount-point"),
                                          tr("Please enter the new mount-point:"), QLineEdit::Normal,
                                          "/", &ok);
  if (!ok || nMount.isEmpty())
    return;

  // Sanity checks
  ////////////////////////////////////////
  if ( nMount.indexOf("/") != 0 ) {
      QMessageBox::critical(this, tr("Invalid Mount"),
              tr("Mount point should start with '/'"),
              QMessageBox::Ok,
              QMessageBox::Ok);
      return;
  }
  // End sanity checks
  ////////////////////////////////////////

  // Make sure this mount doesn't already exist
  QList<QTreeWidgetItem *> mItems = treeMounts->findItems("*", Qt::MatchWildcard);
  for ( int i = 0; i < mItems.size(); ++i) {
    if ( mItems.at(i)->text(1) == nMount )
      return;
  }

  new QTreeWidgetItem(treeMounts, QStringList() << tmp.setNum(mItems.size() + 1) << nMount );
}

void wizardDisk::slotSaveFSResize(int newSize)
{

  QString tmp, tmp2;
  bool ok;
  int oSize, availSize, tSize, newAvailSize;

  // If adding a new mount point
  if ( ! addingMount.isEmpty() ) {
    QList<QTreeWidgetItem *> mItems = treeMounts->findItems("*", Qt::MatchWildcard);
    new QTreeWidgetItem(treeMounts, QStringList() << tmp.setNum(mItems.size() + 1) << addingMount << tmp2.setNum(newSize) << "UFS+SUJ" );
    addingMount="";
    availSize = lineFreeMB->text().toInt(&ok); 
    newAvailSize = availSize - newSize;
    lineFreeMB->setText(tmp.setNum(newAvailSize));
    return;
  } 

  if ( ! treeMounts->currentItem() )
    return;

  // Just resizing an existing partition
  oSize = treeMounts->currentItem()->text(2).toInt(&ok);  
  treeMounts->currentItem()->setText(2, tmp.setNum(newSize));

  // Set the available size
  if ( oSize > newSize ) {
    tSize = oSize - newSize;
    availSize = lineFreeMB->text().toInt(&ok); 
    newAvailSize = availSize + tSize;
  } else {
    tSize = newSize - oSize;
    availSize = lineFreeMB->text().toInt(&ok); 
    newAvailSize = availSize - tSize;
  }
  lineFreeMB->setText(tmp.setNum(newAvailSize));

  // Check if we need to enable the add button
  if ( newAvailSize >= 100 )
    pushAddMount->setEnabled(true);
  else
    pushAddMount->setEnabled(false);
}

void wizardDisk::slotTreeMountsRightClick()
{
  if ( ! treeMounts->currentItem() )
    return;

  popup = new QMenu();
  popup->setTitle(tr("Editing:") + " " + treeMounts->currentItem()->text(1));
  popup->addSeparator();

  // No options to change for /swap
  if ( treeMounts->currentItem()->text(1) == "/swap" ) {
    popup->addAction( "Change size", this, SLOT(slotZSwapSize()));
    popup->exec( QCursor::pos() );
    return;
  }

  // Create atime sub-menu
  popupAT = popup->addMenu("atime");
  popupAT->addAction( "on", this, SLOT(slotZATON()));
  popupAT->addAction( "off", this, SLOT(slotZATOFF()));

  // Create canmount sub-menu
  popupCM = popup->addMenu("canmount");
  popupCM->addAction( "on", this, SLOT(slotZCMON()));
  popupCM->addAction( "off", this, SLOT(slotZCMOFF()));
  popupCM->addAction( "noauto", this, SLOT(slotZCMNOAUTO()));

  // Create Checksum sub-menu
  popupCH = popup->addMenu("checksum");
  popupCH->addAction( "on", this, SLOT(slotZChkON()));
  popupCH->addAction( "off", this, SLOT(slotZChkOFF()));

  // Create compression sub-menu for non '/' devices
  if ( treeMounts->currentItem()->text(1) != "/" ) {
    popupCmp = popup->addMenu("compression");
    popupCmp->addAction( "off", this, SLOT(slotZCmpOFF()));
    popupCmp->addAction( "lzjb", this, SLOT(slotZCmpLZJB()));
    popupCmp->addAction( "gzip", this, SLOT(slotZCmpGZIP()));
    popupCmp->addAction( "zle", this, SLOT(slotZCmpZLE()));
  }

  // Create dedup sub-menu
  // dedup is disabled for now, until such time as it is safe in all cases
  //popupDD = popup->addMenu("dedup");
  //popupDD->addAction( "off", this, SLOT(slotZDDOFF()));
  //popupDD->addAction( "on", this, SLOT(slotZDDON()));
  //popupDD->addAction( "verify", this, SLOT(slotZDDVERIFY()));

  // Create exec sub-menu
  popupNE = popup->addMenu("exec");
  popupNE->addAction( "on", this, SLOT(slotZEXON()));
  popupNE->addAction( "off", this, SLOT(slotZEXOFF()));

  popup->exec( QCursor::pos() );

}

void wizardDisk::slotZCMNOAUTO()
{
  toggleZFSOpt(QString("canmount=noauto"));
}

void wizardDisk::slotZCMOFF()
{
  toggleZFSOpt(QString("canmount=off"));
}

void wizardDisk::slotZCMON()
{
  toggleZFSOpt(QString("canmount=on"));
}

void wizardDisk::slotZDDVERIFY()
{
  toggleZFSOpt(QString("dedup=verify"));
}

void wizardDisk::slotZDDON()
{
  toggleZFSOpt(QString("dedup=on"));
}

void wizardDisk::slotZDDOFF()
{
  toggleZFSOpt(QString("dedup=off"));
}

void wizardDisk::slotZCmpZLE()
{
  toggleZFSOpt(QString("compress=zle"));
}

void wizardDisk::slotZCmpGZIP()
{
  toggleZFSOpt(QString("compress=gzip"));
}

void wizardDisk::slotZCmpLZJB()
{
  toggleZFSOpt(QString("compress=lzjb"));
}

void wizardDisk::slotZCmpOFF()
{
  toggleZFSOpt(QString("compress=off"));
}

void wizardDisk::slotZChkOFF()
{
  toggleZFSOpt(QString("checksum=off"));
}

void wizardDisk::slotZChkON()
{
  toggleZFSOpt(QString("checksum=on"));
}

void wizardDisk::slotZATON()
{
  toggleZFSOpt(QString("atime=on"));
}

void wizardDisk::slotZSwapSize()
{
  bool ok;
  QString tmp;
  int size = QInputDialog::getInt(this, tr("Enter SWAP size"),
                                  tr("Size (MB)"), 2048, 0, 1000000, 1, &ok);
  if ( ok )
  {
    tmp.setNum(size);
    QString optString = "volsize=" + tmp + "M|org.freebsd:swap=on|checksum=off";
    treeMounts->currentItem()->setText(2, optString);
    treeMounts->currentItem()->setToolTip(2, optString);
  }
}

void wizardDisk::slotZATOFF()
{
  toggleZFSOpt(QString("atime=off"));
}

void wizardDisk::slotZEXOFF()
{
  toggleZFSOpt(QString("exec=off"));
}

void wizardDisk::slotZEXON()
{
  toggleZFSOpt(QString("exec=on"));
}

// Toggle an option being on / off for ZFS
void wizardDisk::toggleZFSOpt(QString option)
{
  if ( ! treeMounts->currentItem() )
    return;

  bool found = false;
  QString optTag = option.section("=", 0, 0);
  QString tmpTag;
  QStringList newOpts;
  QStringList curOpts = treeMounts->currentItem()->text(2).split("|");
  for (int i=0; i < curOpts.count(); ++i) {
    tmpTag=curOpts.at(i).section("=", 0, 0);
    if ( optTag == tmpTag ) {
      found = true;
      // Only save option if setting to new value
      if ( option != curOpts.at(i) )
        newOpts << option;
    } else {
      if ( !curOpts.at(i).isEmpty() )
        newOpts << curOpts.at(i);
    }
  }

  if ( ! found )
      newOpts << option;

  QString optString;
  if ( newOpts.count() <= 1)
    optString = newOpts.join("");
  else
    optString = newOpts.join("|");

  treeMounts->currentItem()->setText(2, optString);
  treeMounts->currentItem()->setToolTip(2, optString);
}

void wizardDisk::generateCustomDiskLayout()
{
  QString targetType, tmp;
  int targetLoc;
  QString targetDisk, targetSlice, tmpPass, fsType, target;

  // Clear out the original disk layout
  sysFinalDiskLayout.clear();
  QStringList fileSystem;
  qDebug() << "Generating custom disk layout";
       
  if ( comboPartition->currentIndex() == 0) {
    targetType = "DRIVE";
    targetSlice = "ALL";
    targetDisk = comboDisk->currentText();
    targetDisk.truncate(targetDisk.indexOf(" -"));
    target = targetDisk;
    targetLoc = 1;
  } else {
    targetType = "SLICE";
    targetDisk = comboDisk->currentText();
    targetDisk.truncate(targetDisk.indexOf(" -"));
    targetSlice = comboPartition->currentText();
    targetSlice.truncate(targetSlice.indexOf(":"));
    targetSlice = targetSlice.remove(0, targetSlice.size() -2);
    target = targetDisk + targetSlice;
    targetLoc = 2;
  }

  // Start building the ZFS file-systems
  QStringList zMnts;
  fsType = "ZFS";
  int zpoolSize = getDiskSliceSize();

  // Get the zfs mounts
  QList<QTreeWidgetItem *> mItems = treeMounts->findItems("*", Qt::MatchWildcard);
  for ( int i = 0; i < mItems.size(); ++i) {
    if ( mItems.at(i)->text(2).isEmpty() )
      zMnts << mItems.at(i)->text(1);
    else
      zMnts << mItems.at(i)->text(1) + "(" + mItems.at(i)->text(2) + ")";
  }

  // If we have any additional ZFS mirror / raidz devices set it up now
  QString zOpts, zDisk;
  if ( groupZFSOpts->isChecked() && comboPartition->currentIndex() == 0 ) {
     zOpts = comboZFSMode->currentText() + ":";
     for ( int i = 0; i < listZFSDisks->count(); ++i )
        if ( listZFSDisks->item(i)->checkState() == Qt::Checked ) {
           zDisk = listZFSDisks->item(i)->text();
           zDisk.truncate(zDisk.indexOf(" -"));
           zOpts = zOpts + " " + zDisk;
        }
  }

  // Save the final disk layout
  fileSystem.clear();
  fileSystem << targetDisk << targetSlice << zMnts.join(",") << fsType << tmp.setNum(zpoolSize) << zOpts << tmpPass;
  sysFinalDiskLayout << fileSystem;

  qDebug() <<"AutoLayout:" << sysFinalDiskLayout;
}

void wizardDisk::generateConfirmationText()
{
  // If running in expert mode, we just create a simple config / confirmation
  if ( radioExpert->isChecked() ) {
    QStringList filesystem;
    filesystem << "MANUAL" << "/mnt" ;
    sysFinalDiskLayout << filesystem;
    textConfirmation->setText(tr("Installing to file-system mounted at /mnt"));
    return;
  }

  QList<QStringList> copyList;
  QStringList summaryList;
  QString tmp, workingDisk, workingSlice, tmpSlice, XtraTmp, startPart, sliceSize;
  int disk = 0;

  // Copy over the list to a new variable we can mangle without modifying the original
  copyList = sysFinalDiskLayout;

  // Start our summary
  summaryList << "";
  summaryList << "<b>" + tr("The disk will be setup with the following configuration:") + "</b>";

  while ( ! copyList.empty() )
  {
    workingDisk = copyList.at(0).at(0);
    workingSlice = copyList.at(0).at(1);
    tmpSlice = workingSlice;

    // Check if this is an install to "Unused Space"
    for (int z=0; z < sysDisks.count(); ++z)
      if ( sysDisks.at(z).at(0) == "SLICE" \
        && sysDisks.at(z).at(2) == workingDisk + workingSlice \
        && sysDisks.at(z).at(4) == "Unused Space" )
          tmpSlice = "free";

    // Check for any mirror for this device
    for (int i=0; i < copyList.count(); ++i) {
       if ( copyList.at(i).at(2).indexOf("MIRROR(" + workingDisk + ")") != -1 )
       {
         summaryList << tr("Disk:") + copyList.at(i).at(0) + " " + tr("Mirroring:") + workingDisk;
         copyList.removeAt(i);
         break;
       }
    }

    // If after doing the mirror, our list is empty, break out
    if ( copyList.empty() )
      break;
    
    // If there is a dedicated /boot partition, need to list that first, see what is found
    for (int i=0; i < copyList.count(); ++i) {
      QStringList mounts = copyList.at(i).at(2).split(",");
      for (int z = 0; z < mounts.size(); ++z) {
        if ( copyList.at(i).at(0) == workingDisk \
          && copyList.at(i).at(1) == workingSlice \
          && mounts.at(z) == "/boot" )
		startPart="/boot";
      }
    }

    // If no dedicated /boot partition, then lets list "/" first
    if(startPart.isEmpty())
	startPart="/";

    // Start by looking for the root partition
    for (int i=0; i < copyList.count(); ++i) {
      QStringList mounts = copyList.at(i).at(2).split(",");
      for (int z = 0; z < mounts.size(); ++z) {
        if ( copyList.at(i).at(0) == workingDisk \
          && copyList.at(i).at(1) == workingSlice \
          && mounts.at(z) == startPart ) {

          // Check if we have any extra arguments to throw on the end
          XtraTmp="";
          if ( ! copyList.at(i).at(5).isEmpty() )
            XtraTmp=" (" + copyList.at(i).at(5) + ")" ;

          // Write the user summary
          summaryList << "";
          summaryList << tr("Partition:") + " " + workingDisk + "(" + workingSlice + "):";
          summaryList << tr("FileSystem:") + " " + copyList.at(i).at(3);
          summaryList << tr("Size:") + " " + copyList.at(i).at(4) + "MB ";

	  if ( copyList.at(i).at(3) == "ZFS" ) {
	    QStringList zDS = copyList.at(i).at(2).split(",/");
	    QString zTMP;
	    for (int ds = 0; ds < zDS.size(); ++ds) {
	      if ( zDS.at(ds) != "/" )
                zDS.replace(ds, "/" + zDS.at(ds));
	      if ( zDS.at(ds).indexOf("(") != -1 ) {
		zTMP = zDS.at(ds);
		zTMP.replace("(", " (");
                zDS.replace(ds, zTMP );
	      }
	    }
            summaryList << tr("ZFS Datasets:<br>") + " " + zDS.join("<br>");
	  } else {
            summaryList << tr("Mount:") + " " + copyList.at(i).at(2);
	  }
	  if ( ! XtraTmp.isEmpty() ) {
            summaryList << tr("Options:") + " " + copyList.at(i).at(5);
          }

          // Done with this item, remove it now
          copyList.removeAt(i);
          break;
        }
      }
    }


    // Now look for SWAP
    for (int i=0; i < copyList.count(); ++i) {
      if ( copyList.at(i).at(0) == workingDisk \
        && copyList.at(i).at(1) == workingSlice \
        && copyList.at(i).at(2) == "SWAP" ) {

        // Write the user summary
        summaryList << "";
        summaryList << tr("Partition:") + " " + workingDisk + "(" + workingSlice + "):";
        summaryList << tr("FileSystem:") + " " + copyList.at(i).at(3);
        summaryList << tr("Size:") + " " + copyList.at(i).at(4) + "MB ";

        // Done with this item, remove it now
        copyList.removeAt(i);
        break;
      }
    }

 
    // Now look for any other partitions
    int count = copyList.count();
    for (int i=0; i < count; ++i) {
      if ( copyList.at(i).at(0) == workingDisk \
        && copyList.at(i).at(1) == workingSlice ) {

        // Check if we have any extra arguments to throw on the end
        XtraTmp="";
        if ( ! copyList.at(i).at(5).isEmpty() )
          XtraTmp=" (" + copyList.at(i).at(5) + ")" ;

	// If we are working on the last partition, set the size to 0 to use remaining disk
	if ( i == (count - 1) ) 
		sliceSize = "0";
	else
		sliceSize=copyList.at(i).at(4);

        // Write the user summary
        summaryList << "";
        summaryList << tr("Partition:") + " " + workingDisk + "(" + workingSlice + "):";
        summaryList << tr("FileSystem:") + " " + copyList.at(i).at(3);
        summaryList << tr("Size:") + " " + copyList.at(i).at(4) + "MB ";
        summaryList << tr("Mount:") + " " + copyList.at(i).at(2);
	if ( ! XtraTmp.isEmpty() ) {
          summaryList << tr("Options:") + " " + copyList.at(i).at(5);
        }

        // Done with this item, remove it now
        copyList.removeAt(i);
        i--;
        count--;
      }
    }

    // Increment our disk counter
    disk++;
  }

  textConfirmation->setText(summaryList.join("<br>"));
}

void wizardDisk::slotTerminal()
{
  system("xterm &");
}

