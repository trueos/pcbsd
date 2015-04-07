#ifndef _LP_CONFIG_H
#define _LP_CONFIG_H

#include <QDialog>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>

#include "LPBackend.h"
#include "LPGUtils.h"

namespace Ui{
	class LPConfig;
}

class LPConfig : public QDialog{
	Q_OBJECT
public:
	LPConfig(QWidget* parent = 0);
	~LPConfig();

	void loadDataset(QString, bool, bool);

	//Output variables
	bool localChanged, scrubChanged, remoteChanged, isReplicated, isScrubSched;
	int localSchedule, localSnapshots, remotePort, remoteFreq, scrubTime, scrubDay;
	QString remoteHost, remoteUser, remoteDataset, scrubSchedule;

private:
	Ui::LPConfig *ui;
	void loadDatasetConfiguration(QString, bool, bool);
	void checkForChanges();
	void setLocalKeepNumber();

private slots:
	void slotApplyChanges();
	void slotCancelConfig();
	void on_combo_local_schedule_currentIndexChanged(int);
	void on_combo_scrub_schedule_currentIndexChanged(int);
	void on_combo_remote_schedule_currentIndexChanged(int index);
	void autoDetectReplicationTargets();
};

#endif
