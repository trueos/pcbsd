
#ifndef DIALOGCONFIG_H
#define DIALOGCONFIG_H

#include "ui_dialogConfig.h"
#include <QDebug>
#include <qstring.h>

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

protected:

private slots:
    void slotCancel();
    void slotOK();

private:

signals:
    void ok();

} ;
#endif // DIALOGCONFIG_H
