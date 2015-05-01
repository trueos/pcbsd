#ifndef _LIFE_PRESERVER_ISCSI_SETUP_WIZARD_H
#define _LIFE_PRESERVER_ISCSI_SETUP_WIZARD_H

#include <QWizard>
#include <QWidget>
#include <QString>

namespace Ui{
	class LPISCSIWizard;
};

class LPISCSIWizard : public QWizard{
	Q_OBJECT
public:
	LPISCSIWizard(QWidget *parent = 0, QString zpool = "");
	~LPISCSIWizard();

	bool success;
	
private:
	Ui::LPISCSIWizard *ui;
	bool saveAttempted;
	QString randList;

private slots:
	void CheckPage(); //This for linking any items which are changed by user interaction
	void enableReplication(); //Actually save the settings to the system

	//UI/user interaction
	void FindGELIKeyFile();
	void RandomizeEncKey(); //generate random encryption key
	void GenerateKeyFile(); //Create the special "key file" for the new settings
	void FindLPSfile(); //have the user look for and select a LPS file
};

#endif
