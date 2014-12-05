#ifndef DIALOGCONFIRM_H
#define DIALOGCONFIRM_H

#include <qfile.h>
#include <qmessagebox.h>
#include <qdialog.h>
#include "ui_dialogconfirm.h"

class dialogConfirm : public QDialog, private Ui::dialogConfirm
{
        Q_OBJECT

public:
        dialogConfirm() : QDialog()
        {
          setupUi(this);
        }

    void ProgramInit(QString txt, bool ac, bool yesno);

public slots:

private slots:
    void slotYes();
    void slotNo();
    void checkTime();

private:
    int timeLeft;

signals:

} ;
#endif // DIALOGCONFIRM_H

