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
    explicit NewModuleDialog(QWidget *parent = 0);
    virtual ~NewModuleDialog();

    bool isAccepted;
    void setDefaultIconFile(QString);
    QString moduleIcon, moduleData;


private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_push_port_clicked();
    void on_push_icon_clicked();

private:
    Ui::NewModuleDialog *ui;

};

#endif
