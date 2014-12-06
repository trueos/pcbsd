
#ifndef DIALOGCONFIG_H
#define DIALOGCONFIG_H

#include "ui_dialogconfig.h"

class dialogConfig : public QDialog, private Ui::dialogConfig
{
	Q_OBJECT

public:
        dialogConfig() : QDialog()
        {
          setupUi(this);
        }

    void programInit();

public slots:

private slots:
    void slotChangeJailDir();
    void slotChangeTMPDIR();
    void slotSave();
    void slotCancel();

private:
    void detectDev();
    void readConfig();
    QString NIC;
    QString TMPDIR;
    QString JailDir;

signals:
   void saved();
} ;
#endif // DIALOGCONFIG_H
