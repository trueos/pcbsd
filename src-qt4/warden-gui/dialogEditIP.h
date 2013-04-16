
#ifndef EDITIP_H
#define EDITIP_H

#include "ui_dialogEditIP.h"

class dialogEditIP : public QDialog, private Ui::dialogEditIP
{
	Q_OBJECT

public:
        dialogEditIP() : QDialog()
        {
          setupUi(this);
        }

	void programInit(QString);

public slots:

protected:

private slots:
        bool sanityCheckSettings();
        void slotAddClicked();
        void slotRemClicked();
        void slotCancelClicked();
        void slotSaveClicked();
        void slotCheckChecks();

private:
	void displayRepos();
        void saveSettings();
        bool checkValidBlock(QString block, QString type);
        QString jailName;
        QString JailDir;

signals:
        void saved();
} ;
#endif // EDITIP_H
