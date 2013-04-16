
#ifndef WELCOMEWIZARD_H
#define WELCOMEWIZARD_H

#include "ui_welcomeWizard.h"
#include <QCheckBox>
#include <qstring.h>

class welcomeWizard : public QWizard, private Ui::welcomeWizard
{
	Q_OBJECT

public:
        welcomeWizard() : QWizard()
        {
          setupUi(this);
        }

    void programInit();

public slots:
    void slotCheckDisabled();

protected:

private slots:

private:
	QCheckBox *checkDisableWelcome;

signals:

} ;
#endif // WELCOMEWIZARD_H
