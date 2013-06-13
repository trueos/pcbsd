
#ifndef WIZARDCREATEJAIL_H
#define WIZARDCREATEJAIL_H

#include "ui_wizardNewJail.h"

class wizardNewJail : public QWizard, private Ui::wizardNewJail
{
	Q_OBJECT

public:
        wizardNewJail() : QWizard()
        {
          setupUi(this);
        }

     void programInit();
     void setHostIPUsed(QStringList, QStringList);
     virtual int nextId() const;

public slots:

private slots:
    virtual void accept();
    void slotClose();
    void slotCheckComplete();
    void slotCheckChecks();
    void slotSelectLinuxScript();

private:
    void loadTemplates();
    bool validatePage();
    QStringList usedHosts, usedIP;
    enum { Page_IP, Page_Type, Page_Root, Page_Linux, Page_Opts };

signals:
     void create(const QString &, const QString &, const QString &, bool, bool, const QString &,  bool, bool, bool, bool, const QString &, const QString &);
} ;
#endif // WIZARDCREATEJAIL_H
