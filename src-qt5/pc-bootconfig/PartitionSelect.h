#ifndef _PCBSD_BOOT_MANAGER_PARTITION_SELECT_H
#define _PCBSD_BOOT_MANAGER_PARTITION_SELECT_H

#include <QDialog>
#include <QString>

namespace Ui{
	class PartitionSelect;
};

class PartitionSelect : public QDialog{
	Q_OBJECT
public:
	PartitionSelect(QWidget *parent=0);
	~PartitionSelect();

	void setupGui(QString name);

	//output variables
	bool selected;
	QString HD, PART;

private:
	Ui::PartitionSelect *ui;
	QHash<QString, QStringList> DEVINFO;

	void GetDiskInfo(); //Fill the internal list of disk information

private slots:
	void on_push_cancel_clicked();
	void on_push_ok_clicked();

	void UpdateDisks();
	void UpdateParts();
	void CheckValidity();
};
#endif