
#ifndef DIALOGHELP_H
#define DIALOGHELP_H

#include "ui_dialogHelp.h"

class dialogHelp : public QDialog, private Ui::dialogHelp
{
	Q_OBJECT

public:
        dialogHelp() : QDialog()
        {
          setupUi(this);
        }

    void dialogInit(QString helpText);

private slots:
    void slotClose();

} ;
#endif // DIALOGHELP_H
