#ifndef SOUNDCONFIG_H
#define SOUNDCONFIG_H

#include <qdialog.h>
#include "ui_snddialog.h"

class sndDialog : public QDialog, private Ui::sndDialog
{
        Q_OBJECT

public:
        sndDialog() : QDialog()
        {
          setupUi(this);
        }

    void programInit( );

public slots:

private slots:
    void slotClose();
    void slotDiagnostic();
    void slotSave();
    void slotEnableApply();

private:
    void refreshDevices();
    int defaultUnit;

signals:

} ;
#endif // SOUNDCONFIG_H

