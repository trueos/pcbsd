#ifndef EOLDIALOG_H
#define EOLDIALOG_H

#include <QDialog>

namespace Ui {
class EOLDialog;
}

class EOLDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EOLDialog(QWidget *parent = 0);
    ~EOLDialog();

public slots:
    void setupDialog();

private:
    Ui::EOLDialog *ui;

    bool check();
};

#endif // EOLDIALOG_H
