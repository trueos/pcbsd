#ifndef PKGVULDIALOG_H
#define PKGVULDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QVector>
#include <QTreeWidgetItem>

namespace Ui {
class PkgVulDialog;
}

class PkgVulDialog : public QDialog
{
    Q_OBJECT

protected:
    typedef struct _SVulInfo
    {
        QString mPkgGenericName;
        QString mPkgName;
        QString mPkgVersion;
        QString mMessage;
        QStringList mCVEList;
        QString mWWW;
    }SVulInfo;

public:
    explicit PkgVulDialog(QWidget *parent = 0);
    ~PkgVulDialog();

    int execDialog();

private slots:

    void on_vulList_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_moreInfoBtn_clicked();

private:
    Ui::PkgVulDialog *ui;

    QVector<SVulInfo> mVulVector;

    void beginAudit();
    void fillUI();
};

#endif // PKGVULDIALOG_H
