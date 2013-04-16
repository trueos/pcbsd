
#ifndef DIALOGINFO_H
#define DIALOGINFO_H

#include "ui_dialogInfoBox.h"
#include <qstring.h>

class dialogInfoBox : public QDialog, private Ui::dialogInfoBox
{
	Q_OBJECT

public:
        dialogInfoBox() : QDialog()
        {
          setupUi(this);
        }

    void programInit(QString title);
    void setInfoText( QString text );


public slots:

protected:

private slots:
    void slotClose();

private:

signals:

} ;
#endif // DIALOGINFO_H
