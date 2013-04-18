
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
        void slotComboIPChanged();

private:
	void displayRepos();
        void loadPerms();
        void savePerms();
        void saveSettings();
        bool checkValidBlock(QString block, QString type);
        QString jailName;
        QString JailDir;
        QStringList IPv4Alias;
        QStringList IPv4AliasBridge;
        QStringList IPv6Alias;
        QStringList IPv6AliasBridge;
	QStringList jailFlags;

signals:
        void saved();
} ;
#endif // EDITIP_H
