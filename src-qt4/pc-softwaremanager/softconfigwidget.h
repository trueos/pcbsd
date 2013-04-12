
#ifndef SOFTCONFGWIDGET_H
#define SOFTCONFIGWIDGET_H

#include "ui_softconfigwidget.h"
#include "../config.h"
#include "classes.h"

class softwareConfigWidget : public QDialog, private Ui::softwareConfigWidget
{
	Q_OBJECT

public:
        softwareConfigWidget() : QDialog()
        {
          setupUi(this);
        }

	void programInit(QString, QString);
	void loadRepos(Repo tmpRepos, int editRepo);

public slots:

protected:

private slots:
        bool sanityCheckSettings();
        void slotAddClicked();
        void slotMoveDownClicked();
        void slotMoveUpClicked();
        void slotRemClicked();
        void slotCancelClicked();
        void slotSaveClicked();

private:
	Repo pbiRepos;
	int curRepo;
	void displayRepos();
        void loadSettings();
        void saveSettings();
	QString wardenDir;
	QString wardenIP;

signals:
        void saved();
} ;
#endif // SOFTCONFIGWIDGET_H
