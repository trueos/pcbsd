
#ifndef WORKINGDIALOG_H
#define WORKINGDIALOG_H

#include "ui_dialogworking.h"

class dialogWorking : public QDialog, private Ui::dialogWorking
{
	Q_OBJECT

public:
        dialogWorking() : QDialog()
        {
          setupUi(this);
        }

    void programInit();
    void setDialogTitle( QString &title );
    void setDialogText( QString &text );

public slots:

private slots:
    void closeEvent( QCloseEvent *e );

private:

signals:
} ;
#endif // WORKINGDIALOG_H
