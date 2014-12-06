#ifndef NEW_MODULE_DIALOG_H
#define NEW_MODULE_DIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QDir>
#include <QMessageBox>
#include "ui_newModuleDialog.h"
#include "backend.h"
#include "pkgSelect.h"


namespace Ui {
    class NewModuleDialog;
}

class NewModuleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewModuleDialog(QWidget *parent = 0, QString portsdir = "");
    virtual ~NewModuleDialog();

    bool isAccepted;
    bool isPort;
    bool quickMake;
    void setDefaultIconFile(QString);
    QString moduleIcon, moduleData;


private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    //void on_push_port_clicked();
    void on_push_package_clicked();
    void on_push_icon_clicked();

private:
    Ui::NewModuleDialog *ui;
    QString portsDir;

};

#endif
