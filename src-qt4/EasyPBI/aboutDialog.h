#ifndef ABOUT_DIALOG_H
#define ABOUT_DIALOG_H

#include <QDialog>
#include <QString>
#include <QFile>
#include "ui_aboutDialog.h"


namespace Ui {
    class aboutDialog;
}

class aboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit aboutDialog(QWidget *parent = 0, QString version = "");
    virtual ~aboutDialog();

private slots:
    void on_buttonBox_accepted();
    void slotTypeChanged();

private:
    Ui::aboutDialog *ui;

};

#endif
