#ifndef WIFICONFIGWIDGETBASE_H
#define WIFICONFIGWIDGETBASE_H

#include <qfile.h>
#include <qmessagebox.h>
#include <qdialog.h>
#include <qwidget.h>
#include <pcbsd-netif.h>
#include <pcbsd-utils.h>
#include <pcbsd-ui.h>
#include "wifiselectiondialog.h"
#include "wepconfig.h"
#include "dialogwpapersonal.h"
#include "ui_wificonfigwidgetbase.h"
#include <QProcess>


// Setup our encryption type defines
#define NO_ENCRYPTION 0
#define WEP_ENCRYPTION 1
#define WPA_ENCRYPTION 2
#define WPAE_ENCRYPTION 3

#define KEYWPAEAP 0
#define KEY8021X 1

// Setup WPAE Phase2 defines
#define KEYPHASE2MD5 0
#define KEYPHASE2MSCHAPV2 1
#define KEYPHASE2GTC 2
#define KEYPHASE2OTP 3
#define KEYPHASE2PAP 4
#define KEYPHASE2CHAP 5
#define KEYPHASE2MSCHAP 6

class wificonfigwidgetbase : public QWidget, private Ui::wificonfigwidgetbase
{
        Q_OBJECT

public:
        wificonfigwidgetbase() : QWidget()
        {
          setupUi(this);
	  WPAONLY=false;
        }

    void setDevice( QString Device );
    void showInfo();
    // Are we just editing WPA supplicant? 
    void setModeWPAOnly();


public slots:

private slots:
    void slotClose();
    void slotApply();
    void slotCheckDHCPBox();
    void slotAddHiddenProfile();
    void slotAddNewProfile();
    void slotEditProfile();
    void slotRemoveProfile();
    void slotRemoveProfileSSID(QString RemoveSSID);
    void slotAddNewProfileSSID(QString ssidc);
    void slotAddNewProfileOpen( QString SSID, bool isBSSID );
    void slotAddNewProfileWEP( QString SSID, bool isBSSID, QString newKey, int newIndex, bool hexkey );
    void slotAddNewProfileWPA( QString SSID, bool isBSSID, QString newKey );
    void slotAddNewProfileWPAE( QString SSID, bool isBSSID, int type, QString EAPIdent, QString CACert, QString ClientCert, QString PrivKeyFile, QString PrivKeyPass, int keyMgmt, int EAPPhase2 );
    void slotCheckDisabled();
    void slotOK();
    void slotMACClicked();
    void loadInfo();
    void slotFinishLoading();
    void slotCheckGlobalText();
    void slotRefreshSSIDList();
    void slotMoveUp();
    void slotMoveDown();
    void slotRescan();
    void slotWEPSave(QString newkey, int newIndex, bool hexkey );
    void slotWPAPSave(QString newkey);

private:
    QString getLineFromCommandOutput( QString command );
    QString getNetmaskForIdent( QString ident );
    QString getOutErrorsForIdent( QString ident );
    QString getOutPacketsForIdent( QString ident );
    QString getInErrorsForIdent( QString ident );
    QString getInPacketsForIdent( QString ident );
    QString getStatusForIdent( QString ident );
    QString getMacForIdent( QString ident );
    QString getIpForIdent( QString ident );
    QString getNameForIdent( QString ident );
    QString getIPv6ForIdent( QString ident );
    QString getGatewayForIdent( QString ident );
    QString getMediaForIdent( QString ident );
    QString getWifiParent( QString dev );
    void loadCountryCodes();
    void runCommand( QString Command );
    void updateWPASupp();
    QString DeviceName;
    QString Country;
    
    dialogWPAPersonal *dialogWPA;
    wepConfig *dialogWEP;
    wifiselectiondialog *wifiselect;
    QProcess *netifProc;

    // Lets define our arrays for the SSID profiles
    QString SSIDList[150];
    int SSIDEncType[150];
    bool BSSID[150];
    QString WEPKey[150];
    int WEPIndex[150];
    bool WEPHex[150];
    QString WPAPersonalKey[150];
    QString WPAEIdent[150];
    QString WPAECACert[150];
    QString WPAEClientCert[150];
    QString WPAEPrivKeyFile[150];
    QString WPAEPassword[150];
    int WPAEKeyMgmt[150];
    int WPAEPhase2[150];
    int WPAEType[150];
    bool usingLagg;
    bool WPAONLY;

signals:

} ;
#endif // WIFICONFIGWIDGETBASE_H

