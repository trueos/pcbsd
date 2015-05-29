#ifndef _LP_BACKGROUND_WORKER_H
#define _LP_BACKGROUND_WORKER_H

#include <QObject>
#include <QApplication>

#include "LPContainers.h"
#include "LPGUtils.h"


class BackgroundWorker : public QObject{
	Q_OBJECT
public:
	BackgroundWorker(){}
	~BackgroundWorker(){}

signals:
	void SnapshotsLoaded();

public slots:
	//Kickoff processes with these slots
        // and then listen for the appropriate signals when finished
	void loadSnapshotInfo(LPDataset* DS){
	  static bool running = false;
	  if(running){ return; }
	  running = true;
	  LPGUtils::loadSnapshotInfo(DS);
	  emit SnapshotsLoaded();
	  QApplication::processEvents();
	  running = false;
	}
	
};

#endif
