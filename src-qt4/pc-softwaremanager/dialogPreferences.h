
#ifndef DIALOGPREF_H
#define DIALOGPREF_H

#include "ui_dialogPreferences.h"
#include "../config.h"

class dialogPreferences : public QDialog, private Ui::dialogPreferences
{
	Q_OBJECT

public:
        dialogPreferences() : QDialog()
        {
          setupUi(this);
        }

	void programInit();

public slots:

protected:

private slots:
        void slotCancelClicked();
        void slotSaveClicked();
private:

signals:
        void saved();
} ;
#endif // DIALOGPREF_H
