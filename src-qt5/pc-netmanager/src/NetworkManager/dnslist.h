#ifndef DNSLIST_H
#define DNSLIST_H

#include <QDialog>

#include <QVector>

namespace Ui {
class DNSList;
}

class DNSList : public QDialog
{
    Q_OBJECT
    
public:
    enum EIPType
    {
        eIPV4=0,
        eIPV6
    };	

    typedef struct _SDNSEntry
    {
        QString mIP;
        QString mProvider;
        QString mLocation;
    }SDNSEntry;

    explicit DNSList(QWidget *parent = 0, EIPType type = eIPV4);
    ~DNSList();

    bool success; //for returning whether the settings should be applied
    QString ipout; //return value
    
private:
    Ui::DNSList *ui;

    bool readConfFile();

    QVector<SDNSEntry> servers_v4;
    QVector<SDNSEntry> servers_v6;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // DNSLIST_H
