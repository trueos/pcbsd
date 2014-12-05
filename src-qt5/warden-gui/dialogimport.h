
#ifndef DIALOGIMPORT_H
#define DIALOGIMPORT_H

#include "ui_dialogimport.h"

class dialogImport : public QDialog, private Ui::dialogImport
{
	Q_OBJECT

public:
        dialogImport() : QDialog()
        {
          setupUi(this);
        }

     void programInit();

public slots:

private slots:
     void slotButtonCancel();
     void slotButtonOk();
     void checkLineText( const QString &text );

private:

signals:
     void import(const QString &, const QString &, const QString &);
} ;
#endif // DIALOGIMPORT_H
