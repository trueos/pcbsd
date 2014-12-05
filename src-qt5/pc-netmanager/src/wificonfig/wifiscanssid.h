#ifndef WIFISCANSSID_H
#define WIFISCANSSID_H

#define PROGPATH QString("/usr/local/share/pcbsd/pc-netmanager")

#include <qfile.h>
#include <qmessagebox.h>
#include <qdialog.h>
#include "ui_wifiscanssid.h"

class wifiscanssid : public QDialog, private Ui::wifiscanssid
{
        Q_OBJECT

public:
        wifiscanssid() : QDialog()
        {
          setupUi(this);
        }

   void init(QString device);
   void scanWifi();


public slots:

private slots:
    void slotCancel();
    void slotConnect();
    void slotRescan();

private:
    QString DeviceName;
    QString getLineFromCommandOutput( QString command );
    QString getSignalStrengthForIdent( QString ifoutput );

signals:
   void saved(QString);

} ;
#endif // WIFISELECTIONDIALOG_H

