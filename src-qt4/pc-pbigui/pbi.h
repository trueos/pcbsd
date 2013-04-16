
#ifndef PBI_H
#define PBI_H

#include "ui_pbi.h"
#include <qprocess.h>
#include <qwizard.h>
#include <qfile.h>
#include <qdir.h>
#include <qmessagebox.h>
#include <qtimer.h>
#include <qfiledialog.h>

class PBI : public QWizard, private Ui::PBI
{
	Q_OBJECT

public:
        PBI() : QWizard()
        {
          setupUi(this);
        }
    void checkCustomGraphics();
    QString getLineFromCommandOutput(QString command);

public slots:
    void ProgramInit( QString pbiFile );
    void PageChanged( int pageId );
private slots:
    void slotCheckExistingProgram();
    void AgreeButtonSlot();
    void FinishedInstallSlot();
    void readSettingsSlot();
    void setProgressSlot();
    void startInstallSlot();
    void reject();
    void accept();

private:

    // Init our meta-data for this PBI
    QString PBIFILE;
    QString ProgramName;
    QString ProgramVer;
    QString ProgramAuthor;
    QString ProgramWeb;
    QString ProgramArch;
    QString ProgramOS;
    QString ProgramSig;
    QString CreatorVer;
    QString ProgramDir;
    QString topBannerFile;
    QString sideBannerFile;
    QString iconFile;
    int TotalLinesTar;

    QProcess *pbiAdd;
    int OverWrote;
    bool ExtractStarted;
    bool inTar;
    bool inHash;

    QPixmap topbanner;
    void uninstall();
    void runCommand( QString Command );
    void checkSignature();


signals:

} ;
#endif // PBI_H
