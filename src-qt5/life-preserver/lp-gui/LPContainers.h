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
	QString zpool;
	QString finishedStatus, runningStatus, errorStatus;
	QString latestSnapshot;
	QStringList harddisks;
	QStringList harddiskStatus;
	QString poolStatus;
	QStringList repHost;
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

class LPRepHost : private QStringList{
public:
	LPRepHost() : QStringList(){
	  //Always 5 elements
	  for(int i=0; i<5; i++){ this->append(""); }
	  //pre-define the numbers
	  this->replace(3,"22"); //port
	  this->replace(4, "-1"); //freq
	}
	~LPRepHost(){}
		
	//INTERNAL FORMAT 
	// [0] host, [1] user, [2] dataset, [3] port, [4] freq
		
	//Getters
	QString host(){ return this->at(0); }
	QString user(){ return this->at(1); }
	QString dataset(){ return this->at(2); }
	int port(){ return this->at(3).toInt(); }
	int freq(){ return this->at(4).toInt(); }
	
	//Setters
	void setHost(QString txt){ this->replace(0,txt); }
	void setUser(QString txt){ this->replace(1,txt); }
	void setDataset(QString txt){ this->replace(2,txt); }
	void setPort(int num){ this->replace(3,QString::number(num)); }
	void setFreq(int num){ this->replace(4,QString::number(num)); }
	
};
#endif
