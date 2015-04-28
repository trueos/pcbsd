#ifndef DIALOGPARTITION_H
#define DIALOGPARTITION_H

#include <QDialog>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "zmanagerwindow.h"

namespace Ui {
class DialogPartition;
}




class DialogPartition : public QDialog
{
    friend class ZManagerWindow;
    Q_OBJECT
    vdev_t *device;
    int granularity;
    int dontupdate;
public:

    explicit DialogPartition(QWidget *parent = 0);
    ~DialogPartition();

    void setDevice(vdev_t * _device);

    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);

    long long getStartSector();
    unsigned long long getSectorCount();
    bool needAlign();
    QString getPartType();
    bool isnewfsChecked();
    QString getnewFSType();
    
private slots:
    void on_SizeSelect_currentIndexChanged(int index);

    void on_sizeSlider_valueChanged(int value);



    void on_devLayout_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_PartType_currentIndexChanged(int index);

    void on_SizeText_textEdited(const QString &arg1);

private:
    Ui::DialogPartition *ui;
};

#endif // DIALOGPARTITION_H
