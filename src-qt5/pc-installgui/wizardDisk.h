
#ifndef WIZDISK_H
#define WIZDISK_H

#include <QMenu>
#include "ui_wizardDisk.h"
#include <qstring.h>
#include "dialogFSSize.h"
#include "backend.h"

class wizardDisk : public QWizard, private Ui::wizardDisk
{
	Q_OBJECT

public:
        wizardDisk() : QWizard()
        {
          setupUi(this);
        }

    void programInit();
    void setRestoreMode();
    virtual int nextId() const;

public slots:

protected:

private slots:
    virtual void accept();
    void slotClose();
    void slotCheckComplete();
    void slotChangedDisk();
    void slotTreeDiskChanged();
    void slotResizeFS();
    void slotRemoveFS();
    void slotAddFS();
    void slotSaveFSResize(int);
    void slotTreeMountsRightClick();
    void slotTerminal();
    void slotSwapSize();
    void slotUEFIClicked();
 
    // QMenu slots
    void slotZCMON();
    void slotZCMOFF();
    void slotZCMNOAUTO();
    void slotZDDOFF();
    void slotZDDON();
    void slotZDDVERIFY();
    void slotZCmpON();
    void slotZCmpOFF();
    void slotZCmpLZ4();
    void slotZCmpLZJB();
    void slotZCmpGZIP();
    void slotZCmpZLE();
    void slotZChkON();
    void slotZChkOFF();
    void slotZATON();
    void slotZATOFF();
    void slotZEXON();
    void slotZEXOFF();
    void slotZSUIDON();
    void slotZSUIDOFF();

private:
    void populateDiskInfo();
    void populateDiskTree();
    void populateZFSDisks();
    void populateZFSDisks2();
    void generateDiskLayout();
    void generateCustomDiskLayout();
    void generateConfirmationText();
    void toggleZFSOpt(QString);
    bool validatePage();
    bool checkDiskSpace();
     int getDiskSliceSize();
     int prevID;
     int systemMemory;
     int swapsize;
    QList<QStringList> sysDisks; // Our lists which contains disk info
    QList<QStringList> sysPartitions; // Our lists which contains partition info
    QList<QStringList> sysFinalDiskLayout; // The final disk layout
    QString addingMount;

    QMenu *popup;
    QMenu *popupCmp;
    QMenu *popupDD;
    QMenu *popupCM;
    QMenu *popupAT;
    QMenu *popupCH;
    QMenu *popupNE;
    dialogFSSize *rFS;
    bool restoreMode;
    bool efiMode;
    enum { Page_Intro, Page_BasicDisk, Page_ZFS, Page_ZFS2, Page_Enc, Page_Mounts, Page_Expert, Page_Confirmation };

signals:
    void saved(QList<QStringList>, QString, QString, QString, bool, QString);

} ;
#endif // WIZDISK_H
