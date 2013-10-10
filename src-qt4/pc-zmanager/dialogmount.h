#ifndef DIALOGMOUNT_H
#define DIALOGMOUNT_H

#include <QDialog>
#include "zmanagerwindow.h"

namespace Ui {
class DialogMount;
}

class DialogMount : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogMount(QWidget *parent = 0);
    ~DialogMount();
    
    void setDevice(vdev_t *device);
    QString getMountLocation();


private slots:
    void on_browseButton_clicked();

private:
    Ui::DialogMount *ui;
};

#endif // DIALOGMOUNT_H
