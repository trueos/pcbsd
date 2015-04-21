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
        netKey(QWidget *parent = 0, QString security="") : QDialog(parent)
        {
          setupUi(this);
	  //Make sure it is centered on the parent
	  QPoint ctr = parent->geometry().center();
	  this->move(ctr.x() - (this->width()/2), ctr.y() - (this->height()/2) );
	  //Now list the current security type if possible
	  if(security.isEmpty()){
	    label_sectitle->setVisible(false);
	    label_security->setVisible(false);
	  }else{
	    label_security->setText(security);
	  }
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

