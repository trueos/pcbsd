#ifndef JAILSDIALOG_H
#define JAILSDIALOG_H

#include <QDialog>
#include "jailsbackend.h"

namespace Ui {
class JailsDialog;
}

class JailsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit JailsDialog(QWidget *parent = 0);
    ~JailsDialog();

    int execDialog(CJailsBackend* pBackend);
    
private slots:
    void on_okBtn_clicked();

private:
    Ui::JailsDialog *ui;

    CJailsBackend* mpJail;
};

#endif // JAILSDIALOG_H
