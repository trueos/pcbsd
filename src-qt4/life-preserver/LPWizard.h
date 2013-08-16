#ifndef _LP_WIZARD_H
#define _LP_WIZARD_H

#include <QWizard>
#include <QString>
#include <QDebug>

namespace Ui{
  class LPWizard;	
}

class LPWizard : public QWizard{
	Q_OBJECT
public:
	explicit LPWizard(QWidget*);
	~LPWizard();

	//Input dataset
	void setDataset(QString);
	//Output variables
	bool cancelled, enableReplication;
	int localTime, totalSnapshots, remotePort, remoteTime;
	QString remoteHost, remoteUser, remoteDataset;

private:
	Ui::LPWizard *ui;

private slots:
	void slotFinished();
	void slotCancelled();
};

#endif
