#ifndef LOGVIEWDIALOG_H
#define LOGVIEWDIALOG_H

#include <QDialog>

#include "updatecontroller.h"

namespace Ui {
class LogViewDialog;
}

class LogViewDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit LogViewDialog(QWidget *parent = 0);
    ~LogViewDialog();

    void showLog(CAbstractUpdateController* controller);
    
private slots:
    void on_cclippboardBtn_clicked();

    void on_logView_copyAvailable(bool b);

    void on_sfileBtn_clicked();

private:
    Ui::LogViewDialog *ui;
};

#endif // LOGVIEWDIALOG_H
