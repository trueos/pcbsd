#ifndef DETAILS_H
#define DETAILS_H

#include <qfile.h>
#include <qmessagebox.h>
#include <qdialog.h>
#include "ui_details.h"

class Details : public QDialog, private Ui::Details
{
        Q_OBJECT

public:
        Details() : QDialog()
        {
          setupUi(this);
        }

    void populate( QString stdout, QString stderr );

public slots:

private slots:
    void saveOutput();

private:

signals:

} ;
#endif // DETAILS_H

