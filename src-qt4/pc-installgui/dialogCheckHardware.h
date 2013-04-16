
#ifndef DIALOGHW_H
#define DIALOGHW_H

#include "ui_dialogCheckHardware.h"
#include <QString>


class dialogCheckHardware : public QDialog, private Ui::dialogCheckHardware
{
	Q_OBJECT

public:
        dialogCheckHardware() : QDialog()
        {
          setupUi(this);
        }

    void programInit();


public slots:

protected:

private slots:
    void slotClose();

private:
    // Functions
    void getVideo();
    void getNetwork();
    void getSound();

signals:

} ;
#endif // DIALOGHW_H
