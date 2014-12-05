#ifndef _LP_CONTAINERS_H
#define _LP_CONTAINERS_H

#include <QHash>
#include <QStringList>
#include <QString>


class LPDataset{
public:
	LPDataset(){}
	~LPDataset(){}

	//Information needed on each dataset
	QString finishedStatus, runningStatus, errorStatus;
	QString latestSnapshot;
	QStringList harddisks;
	QStringList harddiskStatus;
	QString poolStatus;
	QString repHost;
	QHash<QString,QStringList> subsetHash; //<subset, snapshot list> (complete dataset name should be <ds><subset>)
	QHash<QString, QString> snapComment; //<snapshot, comment>
	
	//Simplification functions for getting info from the hash
	QStringList subsets(){ return QStringList(subsetHash.keys()); }
	QStringList snapshots(QString subset){ 
	  if(subsetHash.contains(subset)){
	    return subsetHash[subset];
	  }else{
	    return QStringList();
	  }
	}
	QStringList allSnapshots(){ return QStringList(snapComment.keys()); }
	QString snapshotComment(QString snap){
	  if(snapComment.contains(snap)){
	    return snapComment[snap];
	  }else{
	    return "";
	  }
	}
};

#endif
