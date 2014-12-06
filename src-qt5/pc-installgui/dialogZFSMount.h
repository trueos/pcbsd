
#ifndef DIALOGZFSMOUNT_H
#define DIALOGZFSMOUNT_H

#include "ui_dialogZFSMount.h"

class dialogZFSMount : public QDialog, private Ui::dialogZFSMount
{
	Q_OBJECT

public:
        dialogZFSMount() : QDialog()
        {
          setupUi(this);
        }

    void dialogInit();

public slots:

private slots:
   void slotPushCancel();
   void slotPushAdd();

private:

signals:
   void saved(QString);

} ;
#endif // DIALOGZFSMOUNT_H
