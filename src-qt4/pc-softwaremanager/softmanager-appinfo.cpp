#include "softmanager-main.h"
#include <QDesktopServices>

void PBM::setupAppInfo()
{
    //InfoBar signals
       connect(AppAuthorLabel, SIGNAL( clicked() ), this, SLOT(infoBarAutorClicked() ) );
}



QMenu* PBM::generatePopup(PBI *item)
{
    popup = new QMenu();
    popup->setTitle(item->getName());

    // If this is in an associated repo show the view details option
    if ( item->getRepoID() != "-1" )
      popup->addAction( tr("View details"), this, SLOT(slotViewPBIClicked()));

    if ( ! item->isNewDownload() ) {
      popup->addSeparator();
      popup->addAction( tr("Install Desktop Icons"), this, SLOT(slotAddDesktopIcons()));
      popup->addAction( tr("Install Menu Icons"), this, SLOT(slotAddMenuIcons()));
      popup->addAction( tr("Install Menu Icons (All Users)"), this, SLOT(slotAddMenuIconsRoot()));
      popup->addAction( tr("Install PATH links"), this, SLOT(slotAddPATHLink()));
      popup->addAction( tr("Install PATH links (All Users)"), this, SLOT(slotAddPATHLinkRoot()));

      popup->addSeparator();
      popup->addAction( tr("Uninstall"), this, SLOT(RemoveButtonSlot()));
    } else {
      // If this is a new download, offer cancel option
      if ( item->isNewDownload() ) {
        popup->addAction( tr("Cancel Download"), this, SLOT(RemoveButtonSlot()));
      }
      // If the item failed to download, let us re-start it here
      if ( item->getDownloadFailed() ) {
        popup->addAction( tr("Retry Download"), this, SLOT(slotRetryPBIDownload()));
      }
  }
  return  popup;
}

void PBM::refreshAppInfo(PBI *item)
{
    PBI *curr_item = dynamic_cast<PBI*>(SoftwareListBox->currentItem());
    if (curr_item!= item)
        return;

    if (!item)
    {
        AppInfoStack->setVisible(false);
        return;
    }

    generatePopup(item);

    if (item->doDelete())
        {
            AppInfoStack->setVisible(false);
            return;
        }

        static QMenu* popup= 0;
        static PBI* lastItem = 0;

        AppInfoStack->setVisible(true);

        int curr_status = item->getWorkingStatus();
        if ( PBIINACTIVE == curr_status )
        {
            if (lastItem != item)
            {
                AppNameLabel->setText(item->getName());                
                int pTot = pbiAvail->count();
                int pbiNum=-1;
                QString PBIName = item->getName();
                QString PBIRepo = item->getRepoID();
                for( int i = 0; i < pTot; i++)
                {
                    if ( pbiAvail->getName(i).toLower().remove(" ") == PBIName.toLower().remove(" ") && pbiAvail->getRepo(i) == PBIRepo)
                    {
                        pbiNum = i;
                        break;
                    }
                }
                if (pbiNum>0)
                    AppLicenseLabel->setText(pbiAvail->getLic(pbiNum));
                else
                    AppLicenseLabel->setText(tr("Unknown"));
                lastItem = item;

                if (popup)
                    delete popup;
                popup = generatePopup(item);
                PBIMoreButton->setMenu(popup);
            }// if item changed

            AppVersionLabel->setText(item->getVersion());
            AppIcon->setPixmap(item->icon(0).pixmap(64, 64));
            bool isUpdate= item->getUpdateAvail() && ! item->isNewDownload();//item->getUpdateAvail();
            NewVersionLabel->setVisible(isUpdate);
            if (isUpdate)
                AppUpdateVersionLabel->setText(item->getDownloadVersion());
            AppUpdateVersionLabel->setVisible(isUpdate);
            pushUpdatePBIs->setVisible(isUpdate);
            AppAuthorLabel->setText(item->getAuthor());

            AppStateLabel->setText(item->getStatusText());

            AppInfoStack->setCurrentIndex(0);

        }// if state is PBIINACTIVE
        else if ( (item->getWorkingStatus() == PBINEWDOWNLOAD) || (item->getWorkingStatus() == PBIUPDATING) )
        {
            AppNameLabel2->setText(item->getName());
            AppIcon2->setPixmap(item->icon(0).pixmap(64, 64));
            PbiProgressBar->setMaximum(item->getProgressMax());
            PbiProgressBar->setValue(item->getProgressCurrent());
            ProgressTextLabel->setText(item->getProgressText());
            AppDownloadStateLabel->setText(item->getProgressCaption());
            AppInfoStack->setCurrentIndex(1);
        }
        else
        {
            AppInfoStack->setVisible(false);
        }
}

void PBM::resfreshAppStateText(PBI *item, QString text)
{
        item->setStatusText(text);
        refreshAppInfo(item);
        /*if (SoftwareListBox->currentItem() != item)
            return;
        int curr_status = item->getWorkingStatus();
        if ( PBIINACTIVE == curr_status )
        {
                AppInfoStack->setCurrentIndex(0);
                int pTot = pbiAvail->count();
                int pbiNum=-1;
                QString PBIName = item->getName();
                QString PBIRepo = item->getRepoID();
                for( int i = 0; i < pTot; i++)
                {
                        if ( pbiAvail->getName(i).toLower().remove(" ") == PBIName.toLower().remove(" ") && pbiAvail->getRepo(i) == PBIRepo)
                        {
                                pbiNum = i;
                                break;
                        }
                }
                if (pbiNum>0)
                    AppLicenseLabel->setText(pbiAvail->getLic(pbiNum));
                else
                    AppLicenseLabel->setText(tr("Unknown"));

                AppStateLabel->setText(text);

        }
        else
        {
            if ((PBINEWDOWNLOAD == curr_status) || (PBINEWDOWNLOAD == curr_status))
            {
                AppInfoStack->setCurrentIndex(1);
                PbiProgressBar->setMaximum(item->getProgressMax());
                PbiProgressBar->setValue(item->getProgressCurrent());
                ProgressTextLabel->setText(item->getProgressText());
            }
            else
            {
                AppInfoStack->setCurrentIndex(0);
                AppStateLabel->setText(text);
            }
        }*/
}
/*
void PBM::refreshAppProcessState(PBI *item, int state)
{
    if (!item)
        return;
    item->setProgressStatus(state);
    if (PBIPREPARING == state)
    {
        item->setActiveProgress(0, 0);
    }
}
*/
void PBM::reportAppProgress(PBI *item, int current, int total,  QString caption, QString progress_text)
{
    if (!item)
        return;
    item->setActiveProgress(current, total, caption, progress_text);

    if (SoftwareListBox->currentItem() != item)
        return;
    AppInfoStack->setCurrentIndex(1);
    PbiProgressBar->setMinimum(0);
    PbiProgressBar->setMaximum(total);
    PbiProgressBar->setValue(current);
    AppDownloadStateLabel->setText(caption);
    ProgressTextLabel->setText(progress_text);
}

void PBM::infoBarAutorClicked()
{
    PBI *item = dynamic_cast<PBI*>(SoftwareListBox->currentItem());
    if (!item)
    {
        return;
    }
    QDesktopServices::openUrl(QUrl(item->getWebsite()));
}

