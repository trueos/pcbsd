#include "dialogmount.h"
#include "ui_dialogmount.h"
#include <QFileDialog>
DialogMount::DialogMount(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMount)
{
    ui->setupUi(this);
}

DialogMount::~DialogMount()
{
    delete ui;
}


void DialogMount::setDevice(vdev_t *device)
{
    ui->labelDevice->setText(device->Name);


}

void DialogMount::on_browseButton_clicked()
{
    QFileDialog dlg(this,tr("Select Directory"),"/mnt");
    dlg.setFileMode(QFileDialog::Directory);
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setOption(QFileDialog::ShowDirsOnly,true);

    int result=dlg.exec();

    if(result) {
        QStringList a=dlg.selectedFiles();

        if(a.count()>0) ui->mountLocation->setText(a.at(0));

    }

}

QString DialogMount::getMountLocation()
{
    return ui->mountLocation->text();
};
