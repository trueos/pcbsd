#ifndef BRANCHESDIALOG_H
#define BRANCHESDIALOG_H

#include <QDialog>

#include <QStringList>

namespace Ui {
class BranchesDialog;
}

class BranchesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BranchesDialog(QWidget *parent = 0);
    ~BranchesDialog();
    QString selectedBranch();

public slots:
    void setupDialog();

private:
    Ui::BranchesDialog *ui;

    QStringList mBranches;
    QString mCurrentBranch;

};

#endif // BRANCHESDIALOG_H
