#ifndef DIALOGWPAPERSONAL_H
#define DIALOGWPAPERSONAL_H

#include <qfile.h>
#include <qmessagebox.h>
#include <qdialog.h>
#include "ui_dialogwpapersonal.h"

class dialogWPAPersonal : public QDialog, private Ui::dialogWPAPersonal
{
        Q_OBJECT

public:
        dialogWPAPersonal() : QDialog()
        {
          setupUi(this);
        }

        void setKey( QString Key );

public slots:

private slots:
    void slotClose();
    void slotShowKey();

private:

signals:
    void saved(QString);

} ;
#endif // DIALOGWPAENTERPRISE_H

