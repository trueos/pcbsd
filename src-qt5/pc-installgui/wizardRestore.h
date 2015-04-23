
#ifndef WIZARDRESTORE_H
#define WIZARDRESTORE_H

#include "ui_wizardRestore.h"
#include <qstring.h>

class wizardRestore : public QWizard, private Ui::wizardRestore
{
	Q_OBJECT

public:
        wizardRestore() : QWizard()
        {
          setupUi(this);
        }
    void programInit();
    virtual void initializePage(int);
    virtual int nextId() const;

public slots:

protected:

private slots:
    virtual void accept();
    void slotClose();
    void slotCheckComplete();
    void slotGetUSBISCSI();
    void slotGetISCSIFile();

private:
    bool getUSBAuth(bool);
    bool startPWAuth();
    bool getSysList();
    bool validatePage();
    enum { Page_Intro, Page_Host, Page_Auth, Page_System, Page_Finish };
    QStringList sysList;
    QString authKey;

signals:
    void saved(QStringList);

} ;
#endif // WIZARDRESTORE_H
