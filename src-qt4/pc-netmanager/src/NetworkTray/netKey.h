#ifndef NETKEY_H
#define NETKEY_H

#include <qfile.h>
#include <QMessageBox>
#include <QDialog>
#include "ui_netKey.h"

class netKey : public QDialog, private Ui::netKey
{
        Q_OBJECT

public:
        netKey() : QDialog()
        {
          setupUi(this);
        }

   void setSSID( QString SSID );

public slots:

private slots:
   void slotCancel();
   void slotApply();
   void slotShowKey();

private:

signals:
    void saved(QString, QString);

} ;
#endif // NETKEY_H

