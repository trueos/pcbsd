#ifndef DIALOGNEWPOOL_H
#define DIALOGNEWPOOL_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "zmanagerwindow.h"

namespace Ui {
class DialogNewPool;
}






class DialogNewPool : public QDialog
{
    Q_OBJECT
    
public:

    enum DiskType {
        DISK_STRIPE=1,
        DISK_MIRROR,
        DISK_RAIDZ,
        DISK_RAIDZ2,
        DISK_RAIDZ3,
        DISK_CACHE,
        DISK_LOG,
        DISK_SPARE
    };


    int Fixednumdisks;
    bool Fixedtype;
    int Vdevcount;

    QStringList getVdevList();
    QString getRaidType();
    QString getName();

    void setDevices(QList<vdev_t> *disks);
    void setTitle(QString name);
    void setName(QString name);
    void setType(int disktype);
    void setNumDisks(int ndisks);



    explicit DialogNewPool(QWidget *parent = 0);
    ~DialogNewPool();


private slots:
    void on_vdevList_itemClicked(QTreeWidgetItem *item, int column);

    void on_vdevList_itemChanged(QTreeWidgetItem *item, int column);


private:

    virtual void done(int r);

    Ui::DialogNewPool *ui;



};

#endif // DIALOGNEWPOOL_H
