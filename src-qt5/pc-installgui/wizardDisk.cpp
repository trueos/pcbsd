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
  restoreMode=false;

  populateDiskInfo();

  //connect(pushClose, SIGNAL(clicked()), this, SLOT(slotClose()));
  connect(radioUEFI, SIGNAL(clicked()), this, SLOT(slotUEFIClicked()));
  connect(radioBIOS, SIGNAL(clicked()), this, SLOT(slotUEFIClicked()));
  connect(pushSwapSize, SIGNAL(clicked()), this, SLOT(slotSwapSize()));
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
  connect(groupZFSCache,SIGNAL(clicked(bool)),this,SLOT(slotCheckComplete()));
  connect(listZFSCache,SIGNAL(itemClicked(QListWidgetItem *)),this,SLOT(slotCheckComplete()));
  connect(listZFSCache,SIGNAL(itemActivated(QListWidgetItem *)),this,SLOT(slotCheckComplete()));
  connect(listZFSCache,SIGNAL(itemChanged(QListWidgetItem *)),this,SLOT(slotCheckComplete()));
  connect(groupZFSLog,SIGNAL(clicked(bool)),this,SLOT(slotCheckComplete()));
  connect(listZFSLog,SIGNAL(itemClicked(QListWidgetItem *)),this,SLOT(slotCheckComplete()));
  connect(listZFSLog,SIGNAL(itemActivated(QListWidgetItem *)),this,SLOT(slotCheckComplete()));
  connect(listZFSLog,SIGNAL(itemChanged(QListWidgetItem *)),this,SLOT(slotCheckComplete()));
  connect(groupEncrypt,SIGNAL(clicked(bool)),this,SLOT(slotCheckComplete()));
  connect(lineEncPass,SIGNAL(textChanged(const QString &)),this,SLOT(slotCheckComplete()));
  connect(lineEncPass2,SIGNAL(textChanged(const QString &)),this,SLOT(slotCheckComplete()));

  // Check if we are running in EFI mode
  if ( system("kenv grub.platform | grep -q 'efi'") == 0 ) {
     radioUEFI->setChecked(true);
     efiMode=true;
  } else {
     radioBIOS->setChecked(true);
     efiMode=false;
  }
  slotUEFIClicked();
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
  bool ok;

  if ( comboDisk->currentText().isEmpty())
    return;

  comboPartition->clear();
  comboPartition->addItem(tr("Use entire disk"));
       
  QString disk = comboDisk->currentText();
  disk.truncate(disk.indexOf(" -"));
  for (int i=0; i < sysDisks.count(); ++i) {
    // Make sure to only add the slices to the listDiskSlices
    if ( sysDisks.at(i).at(0) != "SLICE" )
      continue;
    // Only add the slices for the target disk
    if ( disk != sysDisks.at(i).at(1) )
      continue;
    // If we have freespace, only list if it is -gt 10GB
    if ( sysDisks.at(i).at(4) == "Unused Space" ) {
      sysDisks.at(i).at(3).toInt(&ok);
      if ( !ok )
        continue;
      if ( sysDisks.at(i).at(3).toInt(&ok) < 6000 )
        continue;
    }

    // Add the slice / partition
    ptag = sysDisks.at(i).at(4).section(",", 0, 0);
    ptag = ptag.section("/", 0, 0);
    ptag.truncate(15);
    ptag = "(" + ptag;
    if ( ptag.indexOf(")") == -1 )
      ptag += ")";
    comboPartition->addItem(sysDisks.at(i).at(2) + ": " +  sysDisks.at(i).at(3) + "MB " + ptag );
  }

}

void wizardDisk::slotClose()
{
  close();
}

void wizardDisk::accept()
{
  QString bootLoader;
  QString partType="none";
  bool force4K = false;
  QString zpoolName;
  QString biosMode;

  if ( radioUEFI->isChecked() )
    biosMode="efi";
  else
    biosMode="pc";

  if (comboPartition->currentIndex() == 0 ) {
    if ( radioGPT->isChecked() ) {
      partType="GPT";
    } else {
      partType="MBR";
    }
  }

  // Get the boot-loader
  if ( checkGRUB->isChecked() )
    bootLoader="GRUB";
  else
    bootLoader="BSD";

  // When doing advanced ZFS setups, make sure to use GPT
  if ( radioAdvanced->isChecked() && groupZFSOpts->isChecked() )
    partType="GPT";

  // When doing advanced ZFS setups, check if 4K is enabled
  if ( radioAdvanced->isChecked() && checkForce4K->isChecked() )
    force4K = true;

  if ( radioAdvanced->isChecked() && groupZFSPool->isChecked() )
     zpoolName = lineZpoolName->text();

  if ( radioExpert->isChecked() )
    emit saved(sysFinalDiskLayout, QString("NONE"), partType, zpoolName, force4K, QString(""));
  else
    emit saved(sysFinalDiskLayout, bootLoader, partType, zpoolName, force4K, biosMode);
  close();
}

int wizardDisk::nextId() const
{
  switch (currentId()) {
     case Page_Intro:
       if (radioExpert->isChecked())
         return Page_Expert;
       if (radioBasic->isChecked()) {
         radioGPT->setChecked(true);
	 groupScheme->setVisible(false);
	 groupBIOS->setVisible(false);
	 checkForce4K->setVisible(false);
	 groupZFSPool->setVisible(false);
         // Check if we are running in EFI mode
         if ( system("sysctl -n machdep.bootmethod | grep -q 'UEFI'") == 0 ) {
            radioUEFI->setChecked(true);
	    radioMBR->setEnabled(false);
         } else {
            radioBIOS->setChecked(true);
	    radioMBR->setEnabled(true);
	 }
       }
       if (radioAdvanced->isChecked()) {
	 groupScheme->setVisible(true);
	 groupBIOS->setVisible(true);
	 checkForce4K->setVisible(true);
	 groupZFSPool->setVisible(true);
       }
	break;
     case Page_BasicDisk:
       pushSwapSize->setVisible(true);

       if (radioBasic->isChecked())
         return Page_Confirmation;
       if (comboPartition->currentIndex() != 0 ) {
	 groupZFSOpts->setEnabled(false);
         pushSwapSize->setVisible(true);
         return Page_Mounts;
       } else {
	 if ( radioGPT->isChecked() )
	   groupEncrypt->setEnabled(true);
         else
	   groupEncrypt->setEnabled(false);
         groupZFSOpts->setEnabled(true);
         return Page_ZFS;
       }
       break;
     case Page_ZFS:
       return Page_ZFS2;
       break;
     case Page_ZFS2:
       return Page_Enc;
       break;
     case Page_Enc:
       pushSwapSize->setVisible(true);
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

  bool found;

  // Generate suggested disk layout and show disk tree
  if ( prevID == Page_BasicDisk && currentId() == Page_Mounts) {
    generateDiskLayout();
    populateDiskTree();
  }

  // Generate suggested disk layout and show disk tree
  if ( prevID == Page_Enc && currentId() == Page_Mounts) {
    generateDiskLayout();
    populateDiskTree();
  } 

  // Show the other disks available
  if ( prevID == Page_BasicDisk && currentId() == Page_ZFS)
     populateZFSDisks();

  // Show the other disks available
  if ( prevID == Page_ZFS && currentId() == Page_ZFS2)
     populateZFSDisks2();

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
	
	 if ( ! radioAdvanced->isChecked() ) {
	   radioGPT->setChecked(true);
	   groupScheme->setVisible(false);
	   groupBIOS->setVisible(false);
	   checkForce4K->setVisible(false);
	   checkForce4K->setChecked(false);
	 } else {
           if ( comboPartition->currentIndex() == 0) {
	     groupScheme->setVisible(true);
	     groupBIOS->setVisible(true);
	   } else {
	     groupScheme->setVisible(false);
	     groupBIOS->setVisible(false);
           }
	   checkForce4K->setVisible(true);
	 } 

	 // Doing a Advanced install
	 if ( radioAdvanced->isChecked() && groupZFSPool->isChecked() )
	 {
	    if ( lineZpoolName->text().isEmpty() ) {
              button(QWizard::NextButton)->setEnabled(false);
              return false;
	    }
	    if ( lineZpoolName->text() == "root" ) {
              button(QWizard::NextButton)->setEnabled(false);
              return false;
	    }
	    if ( lineZpoolName->text().contains(" ") ) {
              button(QWizard::NextButton)->setEnabled(false);
              return false;
	    }
	    QRegExp *re = new QRegExp("^[-'a-zA-Z][a-zA-Z0-9]*$"); 
	    if (! re->exactMatch(lineZpoolName->text()) ) {
              button(QWizard::NextButton)->setEnabled(false);
              return false;
	    }

	 }

	 // If the select partition is just too small, we can stop here
	 if ( ! checkDiskSpace() ) {
           button(QWizard::NextButton)->setEnabled(false);
	   return false;
	 }
	
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
	 if ( comboZFSMode->currentText() == "stripe" ) {
            labelZFSMsg->setText(tr("Please select the additional disks to stripe"));
	    int numChecked = 0;
	    for ( int i = 0; i < listZFSDisks->count(); ++i )
		if ( listZFSDisks->item(i)->checkState() == Qt::Checked )
		   numChecked++;
            if ( numChecked > 1 ) {
              button(QWizard::NextButton)->setEnabled(true);
	      return true;
            }
         }


         // Disable the next button until we get a working config
         button(QWizard::NextButton)->setEnabled(false);
         return false;
     case Page_ZFS2:
	 // Not using log / cache?
	 if ( !groupZFSCache->isChecked() && ! groupZFSLog->isChecked() )
	 {
           button(QWizard::NextButton)->setEnabled(true);
	   return true;
	 }
	 // Using both? See if we have any duplicates
	 if ( groupZFSCache->isChecked() && groupZFSLog->isChecked() )
	 {
	    // Check for any matches
	    for ( int i = 0; i < listZFSCache->count(); ++i ) {
		if ( listZFSCache->item(i)->checkState() == Qt::Checked )
	          for ( int z = 0; z < listZFSLog->count(); ++z ) {
		    if ( listZFSLog->item(z)->checkState() == Qt::Checked )
		       if ( listZFSLog->item(z)->text() == listZFSCache->item(i)->text() ) {
                          button(QWizard::NextButton)->setEnabled(false);
	                  return false;
		       }
	          }
	    }
	 }

	 // Check that we have at least one item checked
	 found = false;
	 if ( groupZFSCache->isChecked() )
	 {
	    for ( int i = 0; i < listZFSCache->count(); ++i )
		if ( listZFSCache->item(i)->checkState() == Qt::Checked )
		   found = true;
            if ( found == false) {
              button(QWizard::NextButton)->setEnabled(false);
	      return false;
	    }
	 }
	 if ( groupZFSLog->isChecked() )
	 {
	    for ( int i = 0; i < listZFSLog->count(); ++i )
		if ( listZFSLog->item(i)->checkState() == Qt::Checked )
		   found = true;
            if ( found == false) {
              button(QWizard::NextButton)->setEnabled(false);
	      return false;
	    }
	 }

         button(QWizard::NextButton)->setEnabled(true);
	 return true;
     case Page_Enc:
	 if ( ! groupEncrypt->isChecked() ) {
           button(QWizard::NextButton)->setEnabled(true);
	   return true;
	 }
	 if ( lineEncPass->text().isEmpty() || lineEncPass->text() != lineEncPass2->text() ) {
           button(QWizard::NextButton)->setEnabled(false);
	   return false;
         }

         button(QWizard::NextButton)->setEnabled(true);
	 return true;
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

void wizardDisk::populateZFSDisks2()
{
   qDebug() << "Adding ZFS2 disks...";
   listZFSCache->clear();
   listZFSLog->clear();

   // What is the primary target disk
   QString curDisk = comboDisk->currentText();
   curDisk.truncate(curDisk.indexOf(" -"));

   // Get a list of other disks being used
   QStringList usedDisks;
   if ( groupZFSOpts->isChecked() ) {
     for ( int i = 0; i < listZFSDisks->count(); ++i )
        if ( listZFSDisks->item(i)->checkState() == Qt::Checked ) {
           QString zDisk = listZFSDisks->item(i)->text();
           zDisk.truncate(zDisk.indexOf(" -"));
           usedDisks << zDisk;
        }
   }

   // Now add any available disks to the combo boxes
   for (int z=0; z < sysDisks.count(); ++z)
     if ( sysDisks.at(z).at(0) == "DRIVE" && sysDisks.at(z).at(1) != curDisk && ! usedDisks.contains(sysDisks.at(z).at(1))  )
     {
        QListWidgetItem *dItem = new QListWidgetItem(sysDisks.at(z).at(1) + " - " + sysDisks.at(z).at(2) + "MB " + sysDisks.at(z).at(3));
        dItem->setCheckState(Qt::Unchecked);
        listZFSCache->addItem(dItem);
        QListWidgetItem *dItem2 = new QListWidgetItem(sysDisks.at(z).at(1) + " - " + sysDisks.at(z).at(2) + "MB " + sysDisks.at(z).at(3));
        dItem2->setCheckState(Qt::Unchecked);
        listZFSLog->addItem(dItem2);
     }
}

bool wizardDisk::checkDiskSpace()
{
  if ( getDiskSliceSize() < 4000 )
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

  // Get the size of the slice we are working on
  totalSize = getDiskSliceSize();

  // Setup some swap space
  if ( totalSize > 30000 ) {
    // 2GB if over 30GB of disk space, 512MB otherwise
    swapsize = 2000;
  } else {
    swapsize = 512;
  }
  totalSize = totalSize - swapsize;


  if ( totalSize != -1 )
  {
     fsType= "ZFS";
     if ( groupEncrypt->isChecked() ) {
        fsType="ZFS.eli";
        tmpPass=lineEncPass->text();
     }

    QString rootOpts="";
    rootOpts="(compress=lz4|atime=off)";

     // This lets the user do nifty stuff like a mirror/raid post-install with a single zpool command
    fileSystem << targetDisk << targetSlice << "/" + rootOpts + ",/tmp(compress=lz4|setuid=off),/usr(canmount=off|mountpoint=none),/usr/home(compress=lz4),/usr/jails(compress=lz4),/usr/obj(compress=lz4),/usr/ports(compress=lz4),/usr/src(compress=lz4),/var(canmount=off|atime=on|mountpoint=none),/var/audit(compress=lz4),/var/log(compress=lz4|exec=off|setuid=off),/var/mail(compress=lz4),/var/tmp(compress=lz4|exec=off|setuid=off)" << fsType << tmp.setNum(totalSize) << "" << tmpPass;
    sysFinalDiskLayout << fileSystem;
    fileSystem.clear();

  // If installing to a specific GPT slice, we can't create a 2nd swap partition
  if ( swapsize > 0 ) {
    // Now add swap space
    fileSystem << targetDisk << targetSlice << "SWAP.eli" << "SWAP.eli" << tmp.setNum(swapsize) << "" << "";
    sysFinalDiskLayout << fileSystem;
    fileSystem.clear();
  }

    //qDebug() << "Auto-Gen FS:" <<  fileSystem;
  }
  
  qDebug() << "AutoLayout:" <<  sysFinalDiskLayout;
}

void wizardDisk::populateDiskTree()
{
  QStringList tmpList, zMnts;
  QString tmp, opts;

  // If installing to Free Space, show the option to change partition size
  labelNewPartition->setVisible(false);
  spinNewPartition->setVisible(false);
  spinNewPartition->setRange(0, 0);
  spinNewPartition->setValue(0);
  if ( comboPartition->currentIndex() != 0 && comboPartition->currentText().contains("Unused") ) {
    spinNewPartition->setRange(5000, this->getDiskSliceSize());
    spinNewPartition->setValue(getDiskSliceSize());
    labelNewPartition->setVisible(true);
    spinNewPartition->setVisible(true);
  }

  treeMounts->clear();
  treeMounts->setHeaderLabels(QStringList() << "ID" << tr("ZFS Mounts") << tr("ZFS Options") );
  treeMounts->setColumnCount(3);
  labelFreeSpace->setVisible(false);
  lineFreeMB->setVisible(false);

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

  if ( restoreMode ) {
     treeMounts->setEnabled(false);
     pushRemoveMount->setEnabled(false);
     pushAddMount->setEnabled(false);
     labelMounts->setText(tr("The original system datasets / mount-points will be automatically restored."));
  }

}

int wizardDisk::getDiskSliceSize()
{
  bool ok;
  QString disk = comboDisk->currentText();
  disk.truncate(disk.indexOf(" -"));

  int safeBuf = 15;

  // If on EFI we subtract 100MiB to save for a FAT16/EFI partition
  if ( radioUEFI->isChecked() )
    safeBuf = 115;

  // Check the full disk
  if ( comboPartition->currentIndex() == 0) {
    for (int i=0; i < sysDisks.count(); ++i) {
      // Make sure to only add the drives to the comboDiskList
      if ( sysDisks.at(i).at(0) == "DRIVE" && sysDisks.at(i).at(1) == disk ) {
        //qDebug() << "Selected Disk Size: " +  sysDisks.at(i).at(2);
        sysDisks.at(i).at(2).toInt(&ok);
        if( ok )
          return (sysDisks.at(i).at(2).toInt(&ok) - safeBuf);
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
          return (sysDisks.at(i).at(3).toInt(&ok) - safeBuf);
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
  if ( mnt == "SWAP.eli" )
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

  // Create atime sub-menu
  popupAT = popup->addMenu("atime");
  popupAT->addAction( "on", this, SLOT(slotZATON()));
  popupAT->addAction( "off", this, SLOT(slotZATOFF()));

  // Create canmount sub-menu
  popupCM = popup->addMenu("canmount");
  popupCM->addAction( "on", this, SLOT(slotZCMON()));
  popupCM->addAction( "off", this, SLOT(slotZCMOFF()));
  popupCM->addAction( "noauto", this, SLOT(slotZCMNOAUTO()));

  // Case sensitivity
  popupCI = popup->addMenu("casesensitivity");
  popupCI->addAction( "sensitive", this, SLOT(slotZCION()));
  popupCI->addAction( "insensitive", this, SLOT(slotZCIOFF()));

  // Create Checksum sub-menu
  popupCH = popup->addMenu("checksum");
  popupCH->addAction( "on", this, SLOT(slotZChkON()));
  popupCH->addAction( "off", this, SLOT(slotZChkOFF()));

  // Create compression sub-menu
  popupCmp = popup->addMenu("compression");
  popupCmp->addAction( "off", this, SLOT(slotZCmpOFF()));
  popupCmp->addAction( "on", this, SLOT(slotZCmpON()));
  popupCmp->addAction( "lz4", this, SLOT(slotZCmpLZ4()));
  popupCmp->addAction( "lzjb", this, SLOT(slotZCmpLZJB()));
  popupCmp->addAction( "gzip", this, SLOT(slotZCmpGZIP()));
  popupCmp->addAction( "zle", this, SLOT(slotZCmpZLE()));

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

  // Add setuid sub-menu
  popupNE = popup->addMenu("setuid");
  popupNE->addAction( "on", this, SLOT(slotZSUIDON()));
  popupNE->addAction( "off", this, SLOT(slotZSUIDOFF()));

  popup->exec( QCursor::pos() );

}

void wizardDisk::slotZCION()
{
  toggleZFSOpt(QString("casesensitivity=sensitive"));
}

void wizardDisk::slotZCIOFF()
{
  toggleZFSOpt(QString("casesensitivity=insensitive"));
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

void wizardDisk::slotZCmpLZ4()
{
  toggleZFSOpt(QString("compress=lz4"));
}

void wizardDisk::slotZCmpLZJB()
{
  toggleZFSOpt(QString("compress=lzjb"));
}

void wizardDisk::slotZCmpOFF()
{
  toggleZFSOpt(QString("compress=off"));
}

void wizardDisk::slotZCmpON()
{
  toggleZFSOpt(QString("compress=on"));
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

void wizardDisk::slotSwapSize()
{
  bool ok;
  QString tmp;
  int size = QInputDialog::getInt(this, tr("Enter SWAP size"),
                                  tr("Size (MB)"), swapsize, 0, 1000000, 1, &ok);
  if ( ok )
    swapsize = size;
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

void wizardDisk::slotZSUIDOFF()
{
  toggleZFSOpt(QString("setuid=off"));
}

void wizardDisk::slotZSUIDON()
{
  toggleZFSOpt(QString("setuid=on"));
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

  // Check if this is an install to "Unused Space"
  for (int z=0; z < sysDisks.count(); ++z)
     if ( sysDisks.at(z).at(0) == "SLICE" \
       && sysDisks.at(z).at(2) == targetDisk + targetSlice \
       && sysDisks.at(z).at(4) == "Unused Space" ) {
       targetSlice = "free";
     }

  // Start building the ZFS file-systems
  QStringList zMnts;
  fsType = "ZFS";
  if ( groupEncrypt->isChecked() ) {
    fsType="ZFS.eli";
    tmpPass=lineEncPass->text();
  }
  int zpoolSize = getDiskSliceSize();

  // If we are using free space, get the user set size
  if ( targetSlice == "free" && spinNewPartition->value() != 0 )
    zpoolSize = spinNewPartition->value();

  // Deduct any swap space
  zpoolSize = zpoolSize - swapsize;

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
  // Any additional ZFS cache devices?
  if ( groupZFSCache->isChecked() ) {
     zOpts+=" cache";
     for ( int i = 0; i < listZFSCache->count(); ++i )
        if ( listZFSCache->item(i)->checkState() == Qt::Checked ) {
           zDisk = listZFSCache->item(i)->text();
           zDisk.truncate(zDisk.indexOf(" -"));
           zOpts = zOpts + " " + zDisk;
        }
  }
  // Any additional ZFS log devices?
  if ( groupZFSLog->isChecked() ) {
     zOpts+=" log";
     for ( int i = 0; i < listZFSLog->count(); ++i )
        if ( listZFSLog->item(i)->checkState() == Qt::Checked ) {
           zDisk = listZFSLog->item(i)->text();
           zDisk.truncate(zDisk.indexOf(" -"));
           zOpts = zOpts + " " + zDisk;
        }
  }

  // Save the final disk layout
  fileSystem.clear();
  fileSystem << targetDisk << targetSlice << zMnts.join(",") << fsType << tmp.setNum(zpoolSize) << zOpts << tmpPass;
  sysFinalDiskLayout << fileSystem;

  // If installing to a specific GPT slice, we can't create a 2nd swap partition
  if ( swapsize > 0 ) {
    // Now add swap space 
    fileSystem.clear();
    fileSystem << targetDisk << targetSlice << "SWAP.eli" << "SWAP.eli" << tmp.setNum(swapsize) << "" << "";
    sysFinalDiskLayout << fileSystem;
  }

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

	  if ( copyList.at(i).at(3) == "ZFS" || copyList.at(i).at(3) == "ZFS.eli" ) {
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
        && copyList.at(i).at(2) == "SWAP.eli" ) {

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

void wizardDisk::setRestoreMode()
{
  restoreMode=true;
}

void wizardDisk::slotUEFIClicked()
{
  radioMBR->setEnabled(! radioUEFI->isChecked());
  if ( radioUEFI->isChecked() )
    radioGPT->setChecked(true);
}
