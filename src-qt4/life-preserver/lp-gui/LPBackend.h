#ifndef _LP_BACKEND_H
#define _LP_BACKEND_H

#include <QProcess>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>

//Class of static functions for using the "lpreserver" backend
class LPBackend{

public:	
	//Informational
	static QStringList listPossibleDatasets(); //list all possible datasets on the system
	static QStringList listDatasets(); //list all current lifepreserver datasets
	static QStringList listDatasetSubsets(QString dataset); //list all subsets of the main dataset
	static QStringList listSnapshots(QString dsmountpoint); //list all snapshots for a particular dataset mountpoint
	static QStringList listLPSnapshots(QString dataset); //list all snapshots created by life preserver
	static QStringList listReplicationTargets(); //list all datasets with replication enabled
	static QStringList listCurrentStatus(); //list the current snapshot/replication status
	//Dataset Management
	static bool setupDataset(QString dataset, int time, int numToKeep); //add or configure dataset
	static bool removeDataset(QString dataset);
	static bool datasetInfo(QString dataset, int& time, int& numToKeep); //get current settings for a dataset
	//Snapshop Management
	static void newSnapshot(QString dataset, QString snapshotname);
	static bool removeSnapshot(QString dataset, QString snapshot);
	static bool revertSnapshot(QString dataset, QString snapshot); //revert to given snapshot
	//Replication Management
	static bool setupReplication(QString dataset, QString remotehost, QString user, int port, QString remotedataset, int time);
	static bool removeReplication(QString dataset);
	static bool replicationInfo(QString dataset, QString& remotehost, QString& user, int& port, QString& remotedataset, int& time);
	//SSH Key Management
	static bool setupSSHKey(QString remoteHost, QString remoteUser, int remotePort);
	static QStringList findValidUSBDevices();
	static bool copySSHKey(QString mountPath, QString localHost);
	//USB Device Management
	static QStringList listDevices();
	static bool isMounted(QString device);
	static bool unmountDevice(QString device);
	//Zpool disk Management
	static bool attachDisk(QString pool, QString disk);
	static bool detachDisk(QString pool, QString disk);
	static bool setDiskOnline(QString pool, QString disk);
	static bool setDiskOffline(QString pool, QString disk);
	//General utility functions
	static QStringList getCmdOutput(QString);
	static int runCmd(QString);	

};
#endif
