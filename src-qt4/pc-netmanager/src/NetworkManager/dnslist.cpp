#include "dnslist.h"
#include "ui_dnslist.h"

typedef struct _SDNSEntry
{
    QString mIP;
    QString mProvider;
    QString mLocation;
}SDNSEntry;

/////////////////////////////////////////////////////////////////////////////////
// HARDCODED DNS SERVERS LIST
/////////////////////////////////////////////////////////////////////////////////
SDNSEntry servers_v4[] =
{
    {"208.67.222.222", "OpenDNS", ""},
    {"208.67.220.220", "OpenDNS", ""},
    {"8.8.8.8", "Google", "us"},
    {"8.8.4.4", "Google", "us"},
    {"202.83.95.227", "OpenNIC", "au"},
    {"178.63.26.173", "OpenNIC", "de"},
    {"127.110.120.30", "OpenNIC", "nz"},
    {"189.16.173.11", "OpenNIC", "uk"},
    {"69.164.208.50", "OpenNIC", "us"},
};

const int servers_v4_size = sizeof(servers_v4)/sizeof(SDNSEntry);

SDNSEntry servers_v6[] =
{
    {"2620:0:ccc::2", "OpenDNS", ""},
    {"2620:0:ccd::2", "OpenDNS", ""},
    {"2001:4860:4860::8888", "Google", ""},
    {"2001:4860:4860::8844", "Google", ""},
};

const int servers_v6_size = sizeof(servers_v6)/sizeof(SDNSEntry);

/////////////////////////////////////////////////////////////////////////////////

DNSList::DNSList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DNSList)
{
    ui->setupUi(this);

    //ui->list->setRowCount(servers_size+1);    
    ui->list->setColumnWidth(0, 200);
}

DNSList::~DNSList()
{
    delete ui;
}

int DNSList::getIP(EIPType type, QString& ip)
{
    if (eIPV4 == type)
    {
        for (unsigned int i=0; i<servers_v4_size; i++)
        {
            QTreeWidgetItem* item = new QTreeWidgetItem();
            item->setText(0, servers_v4[i].mIP);
            item->setText(1, servers_v4[i].mProvider);
            item->setText(2, servers_v4[i].mLocation);
            ui->list->addTopLevelItem(item);
        }
    }else{ //ipv6
        for (unsigned int i=0; i<servers_v6_size; i++)
        {
            QTreeWidgetItem* item = new QTreeWidgetItem();
            item->setText(0, servers_v6[i].mIP);
            item->setText(1, servers_v6[i].mProvider);
            item->setText(2, servers_v6[i].mLocation);
            ui->list->addTopLevelItem(item);
        }
    }

    int RetVal = exec();
    ip = ui->list->selectedItems()[0]->text(0);
    return RetVal;
}
