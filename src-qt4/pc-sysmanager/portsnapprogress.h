#ifndef PORTSNAPPROGRESS_H
#define PORTSNAPPROGRESS_H

#include <qfile.h>
#include <qmessagebox.h>
#include <qdialog.h>
#include <QProcess>
#include "ui_portsnapprogress.h"

class PortsnapProgress : public QDialog, private Ui::PortsnapProgress
{
        Q_OBJECT

public:
        PortsnapProgress() : QDialog()
        {
          setupUi(this);
        }

    void init(bool, QString);

public slots:

private slots:
    void parseFetch();
    void cancel();
    void fetchDone();
    void parseUpdate();
    void updateDone();

private:
    void startPorts();
    void startSource(QString);
    QProcess *portsnap;
    QString workdir;
    QString portsdir;
    QString displayString;
    int numberSteps;
    void finish();
    bool doPorts;

signals:

} ;
#endif // PORTSNAPPROGRESS_H

