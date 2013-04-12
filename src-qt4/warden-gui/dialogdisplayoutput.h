
#ifndef DIALOGDISPLAYOUTPUT_H
#define DIALOGDISPLAYOUTPUT_H

#include "ui_dialogdisplayoutput.h"

class dialogDisplayOutput : public QDialog, private Ui::dialogDisplayOutput
{
	Q_OBJECT

public:
        dialogDisplayOutput() : QDialog()
        {
          setupUi(this);
        }

     void programInit( bool buttonOn );
     void setDialogCaption( const QString &text );
     void setDialogText( const QString &text );
     void appendDialogText( const QString &text );
     void setCloseHide( bool status );

public slots:
     void slotPushClose();

private slots:
     void closeEvent( QCloseEvent *e );

private:

signals:
} ;
#endif // DIALOGDISPLAYOUTPUT_H
