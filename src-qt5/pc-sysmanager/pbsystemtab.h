#ifndef PBSYSTEMTAB_H
#define PBSYSTEMTAB_H

#include <QDialog>
#include <QFile>
#include <QMenu>
#include <QMessageBox>
//#include <QNetworkAccessManager>
//#include <QNetworkReply>
#include <QProcess>
#include <QProgressDialog>
#include <QTextStream>
#include <QTimer>
#include <QCloseEvent>

#include <pcbsd-utils.h>
#include <pcbsd-ui.h>
#include "ui_pbsystemtab.h"
#include "cmdDialog.h"
#include "../config.h"


/* Change this to switch the default patch tmpdir */
//const QString PATCHTMPDIR_DEFAULT( PREFIX + "/tmp" );
//#define SYSTRIGGER "/tmp/.sysupdatetraytrigger"

/* The Update STATUS Flags */
//#define SYSTEM_UP2DATE 0
//#define SYSTEM_UPDATE_AVAIL 1
//#define SYSTEM_CHECKING4UPDATES 4
//#define SYSTEM_UPDATING 5
//#define CHECK_FAILED 6

// Set our defines for the type of mirror the user is running
//#define AUTOMIRROR      0
//#define SELECTMIRROR    1
//#define CUSTOMMIRROR    2



class PBSystemTab : public QDialog, private Ui::PBSystemTab
{
        Q_OBJECT

public:
        PBSystemTab() : QDialog()
        {
          setupUi(this);
        }

    QMap <QString, QString> codeMap;
    bool checkValue( QString File, QString Key, QString Value );

public slots:
    void ProgramInit();
    void changeOpenTab(int tab);

private slots:

    void startGenerateSheet();
    void finishedSheet();
    void fetchSourcePressed();
    void fetchPortsPressed();
    void loadMiscData();
    void slotMiscSave();
    void slotClose();
    
private:
    //Diagnostic Sheet generation
    QProcess *SheetGenScript;
    QString SheetFileName;

    //Ports/Source fetching
    CMDDialog *cmdDlg;

    //Internal variables
    QString username, Version, Arch;


    void CheckUname();
    void CheckPBVer();
    void CreateSheetFile();
    void showRebootRequired();

protected:
	void closeEvent(QCloseEvent *event);

signals:
  void changed();

} ;
#endif // PBSYSTEMTAB_H
