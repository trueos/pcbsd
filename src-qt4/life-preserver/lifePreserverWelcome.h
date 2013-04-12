#ifndef LIFEPWEL_H
#define LIFEPWEL_H

/* QT4 */
#include <QDialog>

/* Local Includes */
#include "ui_lifePreserverWelcome.h"
#include "ui_lifePreserverWizard.h"
#include "lifePreserverWizard.h"

class lifePreserverWelcome : public QDialog, private Ui::lifePreserverWelcome
{
        Q_OBJECT

public:
        lifePreserverWelcome() : QDialog()
        {
          setupUi(this);
        }

   void ProgramInit();


public slots:

protected:
   void closeEvent(QCloseEvent *event);

private slots:
   void slotPushWizardClicked();
   void slotWizardDone();

private:
   lifePreserverWizard *lpw;

signals:

} ;
#endif // LIFEPWEL_H

