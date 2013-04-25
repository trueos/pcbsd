
#ifndef DIALOGCONFIRM_H
#define DIALOGCONFIRM_H

#include "ui_dialogConfirm.h"
#include <QDebug>
#include <qstring.h>

class dialogConfirm : public QDialog, private Ui::dialogConfirm
{
	Q_OBJECT

public:
        dialogConfirm() : QDialog()
        {
          setupUi(this);
        }

    void programInit(QString title);
    void setInfoText( QString text );


public slots:

protected:

private slots:
    void slotCancel();
    void slotOK();

private:

signals:
    void ok();

} ;
#endif // DIALOGCONFIRM_H
