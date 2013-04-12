#include "../config.h"

#ifndef PBM_H
#define PBM_H

#define PBITRIGGER "/tmp/.pbiupdatetraytrigger"

// Set our defines for the type of mirror the user is running
#define AUTOMIRROR      0
#define SELECTMIRROR    1
#define CUSTOMMIRROR    2

/* QT4 */
#include <QDebug>
#include <QDialog>
#include <QFile>
#include <QFileSystemWatcher>
#include <QListWidgetItem>
#include <QListIterator>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <QScrollArea>
#include <QTextBrowser>

/* Local Includes */
#include "classes.h"
#include "pbi.h"
#include "dialogPreferences.h"
#include "softconfigwidget.h"
#include "ui_softmanager-main.h"
#include "ui_softconfigwidget.h"

/* Change this to switch the default patch tmpdir */
const QString PATCHTMPDIR_DEFAULT( PREFIX + "/tmp" );

#define PBI_UPDATED 0
#define PBI_CHECKING4UPDATES 1
#define PBI_UPDATING 2

class PBM : public QMainWindow, private Ui::PBM
{
        Q_OBJECT

public:
        PBM() : QMainWindow()
        {
          setupUi(this);
        }

   void ProgramInit();
   void showInstalledTab();
   void setWardenMode(QString, QString);


public slots:
    void slotSingleInstance();
    
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void RemoveButtonSlot();
    void PopulateList();
    void checkRoot();

    // Slots for the PBI tab
    void slotAddDesktopIcons();
    void slotAddMenuIcons();
    void slotAddMenuIconsRoot();
    void slotAddPATHLink();
    void slotAddPATHLinkRoot();
    bool checkPBISelectedUpdates();
    void markAllPBIDirty();
    void markPBIForUpdate(PBI *item);
    void removeAllPBIDirty();
    void removeAllDeletedPBI();
    void slotDownloadPBIUpdate();
    void slotFinishedPBIInstall();
    void slotPBICheckUpdate();
    void slotPBIRightClicked();
    void slotPBISelectionChanged();
    void slotPBIUpdateCheckDone();
    void slotRestartAllFailedPBIDownloads();
    void slotRetryPBIDownload();
    void slotParsePBIOutput();
    void slotProcessPBIs();
    void slotReadPBIBuffer();
    void slotStartPBIUpgrades();
    void slotStartPBIUpdateCheck();
    void slotTabChanged(int);
    void slotViewPBIClicked();
    void slotUpdateAllClicked();

    // Slots for the scrollArea
    void slotHomeButtonClicked();
    void slotBackButtonClicked();

    // Configuration Slots
    void slotLoadSettings();
    void slotAddButtonClicked();
    void slotConfigButtonClicked();
    void slotRepoSaved();
    void slotQuitButtonClicked();

    // Software Browser
    void startSoftwareBrowser();
    void slotAuthorClicked(QString);
    void slotAutoUpdateClicked(bool);
    void slotCatClicked(QString);
    void slotDownloadClicked(QString);
    void slotPBIClicked(QString);
    void slotRepoChanged(QAction *);
    void slotSearchForKeyword(bool);
    void slotSearchAsTyped();
    void slotSearchClicked();
    void slotPrefButtonClicked();
    void slotLoadPref();

    void infoBarAutorClicked();

private:
    void checkGroupOperator();
    void RemovePBI( PBI * pbi );
    void addBrowseWidget(QWidget *bwid);
    bool checkPBILoaded(QString name);
    bool checkPBILoadedFuzzy(QString name);
    bool checkOkToClose();
    void installDesktopIcon( PBI * pbi );
    void installMenuIcon( PBI * pbi, bool asRoot );
    void installPATHLink( PBI * pbi, bool asRoot );
    void generateCopyLabel();
    PBI *getPBIFromName(QString name);
    void loadStatePBI();
    void purgeAllDeletedPBI();  
    void saveStatePBI();
    void scheduleNewPBIDownload(QString, int, bool);
    void setupMenuBar();
    void startPBIDirLoad();

    void setupAppInfo();
    QMenu* generatePopup(PBI *item);
    void refreshAppInfo(PBI *item);
    void resfreshAppStateText(PBI *item, QString text);
    void reportAppProgress(PBI *item, int current, int total, QString caption, QString progress_text=QString());

    bool parsePBIDownload(PBI *item, QString line);
    bool parsePBIInstall(PBI *item, QString line);

    QLabel* AllPBIsLabel;
    QLabel* InstalledPBIsLabel;

    // Function to return right pbi command to use
    QString getPBICmd(QString cmd);

    // PBI Browser 
    AvailPBI *pbiAvail;
    Category *pbiCats;
    Repo *pbiRepos;
    int curRepo; 
    QString curPageName;
    QString curPageRepo;
    QString curPageType;
    QString curPBIName;
    QStringList histType;
    QStringList histName;

    QStringList getLatestPBIs(int, QString);
    void displayRepos();
    void displayCategories();
    void displayCat(QString cat);
    void displayPBI(QString pbi, QString rID = QString());
    void displaySoftwareLoading();
    void clearSoftwareBrowser();
    QString getTranslation(QString, QString, QString, QString);
    void loadAvailPBIs();
    void loadCategories();
    void loadRepos();
    void loadTranslations();
    void setupRepoMenu();
    void updateHistory(QString type, QString name);
    QWidget* getLatestPBIWidget(int);
    // End PBI Browser
   
    QString getLineFromCommandOutput( QString command );
    QString bytesToHumanReadable( float bytes );
    int getDirSize( QString path );
    QProcess *InfoWindow;
    QProcess *RemoveScript;
    QProcess *CheckRoot;
    QString RealUserName;
    QFileSystemWatcher *fileWatcher;
    QMenu *popup;
    QMenu *repomenu;

    softwareConfigWidget *softConfig;
    int programstatus;
    int pbistatus;
    int workingPopupItem;

    long totalSeconds;
    long downloadSize;
    int totalSteps;
    int attemptedRedownload;
    int firstInstall;
    int firstDownload;
    bool afterDownload;
    
    // Global Config Variables
    int mirrorType;
    QString mirrorURL;
    QString customTmpDir;
    QString Version;
    QString Arch;
    QString Lang;
    QString wardenDir;
    QString wardenIP;

    QProcess *checkPBIProc;
    QProcess *upgradePBIProc;
    QProcess *installPBIDesktopIcon;
    QProcess *loadPBIs;
    QProcess *pbiJob;

    QString PBIBuffer;
    QTreeWidgetItemIterator *currentCheckingPBI;
    QTreeWidgetItemIterator *currentWorkingPBI;
    bool isWorkingOnPBI;
    bool autoUpdateSoftware;
    QList<QStringList> transCats;
    QList<QStringList> transApps;

    dialogPreferences *dialogPref;
    bool doMenuIcons;
    bool doDeskIcons;
    
signals:
    
} ;
#endif // PBM_H

