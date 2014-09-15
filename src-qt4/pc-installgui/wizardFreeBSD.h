
#ifndef WIZARDFBSD_H
#define WIZARDFBSD_H

#include "ui_wizardFreeBSD.h"
#include <qstring.h>

class wizardFreeBSD : public QWizard, private Ui::wizardFreeBSD
{
	Q_OBJECT

public:
        wizardFreeBSD() : QWizard()
        {
          setupUi(this);
        }
    void programInit(bool);

public slots:

protected:

private slots:
    virtual void accept();
    void slotClose();
    void slotCheckComplete();
    void slotSuggestUsername();
    void slotChangedNic();

private:
    bool validatePage();
    enum { Page_Intro, Page_Root, Page_User, Page_Host, Page_AppCafe, Page_Optional };

signals:
    void saved(QString, QString, QString, QString, QString, QString, bool, bool, bool, QStringList, QStringList);

} ;
#endif // WIZARDFBSD_H
