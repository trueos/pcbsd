
#ifndef DIALOGTEMPLATES_H
#define DIALOGTEMPLATES_H

#include "ui_dialogTemplates.h"
#include "dialogdisplayoutput.h"

class dialogTemplates : public QDialog, private Ui::dialogTemplates
{
	Q_OBJECT

public:
        dialogTemplates() : QDialog()
        {
          setupUi(this);
        }

    void programInit();

public slots:

private slots:
    void slotAdd();
    void slotRemove();
    void slotClose();

private:
    dialogDisplayOutput *dialogOutput;
    void loadTemps();

signals:
} ;
#endif // DIALOGTEMPLATES_H
