#ifndef PORTS_DIALOG_H
#define PORTS_DIALOG_H

#include <QDialog>
#include <QString>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QProcess>
#include <QProcessEnvironment>
#include <QMessageBox>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include "ui_portsDialog.h"


namespace Ui {
    class portsDialog;
}

class portsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit portsDialog(QWidget *parent = 0, QString localPortsDir = "", QString sucmd = "");
    virtual ~portsDialog();

private slots:
    void slotClose();
    void slotGetLocalPorts();
    void slotGetSystemPorts();
    void updateProgressBar(qint64,qint64);
    void updateStatusBar();
    void downloadFinished(QNetworkReply*);
    void extractLocalPorts();
    void processFinished(int,QProcess::ExitStatus);
    void resizeDialog();

private:
    Ui::portsDialog *ui;

    QString localPortsTree, systemPortsTree, suCMD;
    bool localPorts, systemPorts;
    void checkForPorts();
    QProcess *p;
};

#endif
