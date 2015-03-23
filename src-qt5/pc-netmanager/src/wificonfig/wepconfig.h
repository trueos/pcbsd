#ifndef WEPCONFIG_H
#define WEPCONFIG_H

#include <qfile.h>
#include <qmessagebox.h>
#include <qdialog.h>
#include "ui_wepconfig.h"

class wepConfig : public QDialog, private Ui::wepConfig
{
        Q_OBJECT

public:
        wepConfig() : QDialog()
        {
          setupUi(this);
	  //Make sure the plaintext key is default
	  radioHex->setChecked(false);
	  radioPlaintext->setChecked(true);
        }

  void setKey( QString Key, int index, bool wephex );

public slots:

private slots:
   void slotClose();
   void slotShowKey();

private:

signals:
    void saved(QString, int, bool);

} ;
#endif // WEPCONFIG_H

