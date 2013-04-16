#ifndef pcADSLDAP_H
#define pcADSLDAP_H

#include <QDialog>
#include <QFile>
#include <QMessageBox>
#include <QStringList>

#include "pcbsd-utils.h"
#include "ui_adsldap.h"

class pcADSLDAP : public QDialog, private Ui::pcADSLDAP
{
        Q_OBJECT

public:
        pcADSLDAP() : QDialog()
        {
          setupUi(this);
        }

        void programInit();

public slots:

private slots:
   void slotClose();
   void slotSave();

private:
   void saveSettings();
   void loadSettings();
   void exportSettings();
   void exportAD();
   void exportLDAP();

signals:

} ;
#endif // pcADSLDAP_H

