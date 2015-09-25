#ifndef WIFISELECTIONDIALOG_H
#define WIFISELECTIONDIALOG_H

#include <qfile.h>
#include <qmessagebox.h>
#include <qdialog.h>
#include "wepconfig.h"
#include "dialogwpapersonal.h"
#include "wifiscanssid.h"
#include "ui_wifiselectiondialog.h"
#include "pcbsd-utils.h"
#include "pcbsd-ui.h"

class wifiselectiondialog : public QDialog, private Ui::wifiselectiondialog
{
        Q_OBJECT

public:
        wifiselectiondialog() : QDialog()
        {
          setupUi(this);
        }

   void init(QString device);
   void scanWifi();
   void initEdit(QString selectedSSID, bool usingBSSID);
   void initEdit(QString selectedSSID, bool usingBSSID, QString editWEPKey, int editWEPIndex, bool editWEPHex);
   void initEdit(QString selectedSSID, bool usingBSSID, QString editWPAPersonalKey);
   void initEdit(QString selectedSSID, bool usingBSSID, int editWPAEType, QString editWPAEIdent, QString editWPAEAnonIdent, QString editWPAECACert, QString editWPAEClientCert, QString editWPAEPrivKeyFile, QString editWPAEPassword, int keyMgmt, int editPhase2);
   void setWPAOnly(bool);


public slots:

private slots:
    void slotCancel();
    void slotConnect();
    void slotOpenWifiScan();
    void slotSSIDChanged(QString newSSID);
    void slotConfigWEP();
    void slotWEPChanged( QString newKey, int newIndex, bool hexkey );
    void slotCheckSecurityRadio();
    void slotCheckWhichSecurity();
    void slotConfigWPAP();
    void slotConfigWPAE();
    void slotWPAPChanged( QString newKey );
    void slotWPAEChanged( int type, QString EAPIdent, QString AnonIdent, QString CACert, QString ClientCert, QString PrivKeyFile, QString PrivKeyPass, int keyMgmt, int EAPPhase2 );

private:
    QString DeviceName;
    QString SSID;
    QString EditingSSID;

    // Setup our dialogs
    wifiscanssid *dialogWifiscanssid;
    wepConfig *dialogWEP;
    dialogWPAPersonal *dialogWPAP;
    dialogWPAE *libWPAE;

    // WEP variables
    QString WEPKey;
    int WEPIndex;
    bool WEPHex;

    // WPA Personal Key variable
    QString WPAPersonalKey;

    // WPAE Variables
    int WPAEType;
    QString WPAEIdent;
    QString WPAEAnonIdent;
    QString WPAECACert;
    QString WPAEClientCert;
    QString WPAEPrivKeyFile;
    QString WPAEPassword;
    int WPAEPhase2;
    int WPAEKeyMgmt;

signals:
   void signalDeleteSSID(QString);
   void signalSavedOpen(QString, bool);
   void signalSavedWEP(QString, bool, QString, int, bool );
   void signalSavedWPA(QString, bool, QString);
   void signalSavedWPAE(QString, bool, int, QString, QString, QString, QString, QString, QString, int, int);

} ;
#endif // WIFISELECTIONDIALOG_H

