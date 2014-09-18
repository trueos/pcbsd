#ifndef NETMAN_H
#define NETMAN_H

#define IFCONFIG QString("/sbin/ifconfig")

#include <qdialog.h>
#include <QListWidgetItem>
#include <QProcess>
#include "ui_networkman.h"
#include <pcbsd-netif.h>
#include <pcbsd-utils.h>
#include <pcbsd-dialogAPsetup.h>

class NetworkMan : public QDialog, private Ui::NetworkMan
{
        Q_OBJECT

public:
        NetworkMan() : QDialog()
        {
          setupUi(this);
        }

    void setNotRoot();

public slots:
    void Init();
    void setInstallerMode();

private slots:
    void detectDev();
    void DevSelectionChanged();
    void PropertiesSlot();
    void restartNetwork();
    void refreshDevices();
    void slotSysTray();
    void slotTimerRefresh();
    void slotListRightClick( const QPoint &pos );
    void slotDisableDevice();
    void slotSetupAP();
    void slotDisableAP();
    void slotEnableDevice();
    void slotShowInfoConfig();
    void slotDoubleClick( QListWidgetItem * item );
    void loadGlobals();
    void slotSave();
    void slotClose();
    void slotCheckGlobalText();
    void slotProxyUserChecked();
    void slotPublicDNS1Clicked();
    void slotPublicDNS2Clicked();
    void slotIPV6DNS1Clicked();
    void slotIPV6DNS2Clicked();
    void slotSingleInstance();


private:
    bool InstallerMode;
    QString Devs[100];
    QString DevsParent[100];
    QString DevsIP[100];
    QString DevsStatus[100];
    QString DevsMAC[100];
    QProcess *GetDevList;
    int workingDev;
    QProcess *GetDevName;
    QString DevsName[100];
    QProcess *StartDHCP;
    QProcess *KillDHClient;
    QString DevsNetmask[100];
    QString DevsType[100];
    QProcess *ConfigDevice;
    QProcess *ConfigSys;
    QString username;
    int firstUpdate;
    QMenu *popup;
    QString DevsUp[100];
    bool Changed;
    QColor colourWhite;
    QColor colourRed;
    int FreeBSDMajor;
    void checkFreeBSDVer();
    QString getLineFromCommandOutput( QString command );
    QString getNameForIdent( QString ident );
    QString getIpForIdent( QString ident );
    QString getMacForIdent( QString ident );
    QString getStatusForIdent( QString ident );
    bool checkValue( QString File, QString Key, QString Value );
    bool checkWifiWlans( QString Key );
    void runCommand( QString command );
    QString getNetmaskForIdent( QString ident );
    QString getTypeForIdent( QString ident );
    QString getUpStatusForIdent( QString ident );
    QString getNextAvailWlan();
    QString getWifiParent(QString dev);
    void loadProxyConfig();
    void saveProxyConfig();

    QProcess *ExeProc;

signals:
	void changed(bool);

} ;
#endif // NETPROPERTIES_H

