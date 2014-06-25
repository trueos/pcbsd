#include "dnslist.h"
#include "ui_dnslist.h"

#include "pcbsd-utils.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

/////////////////////////////////////////////////////////////////////////////////
// HARDCODED DNS SERVERS LIST
/////////////////////////////////////////////////////////////////////////////////
DNSList::SDNSEntry default_servers_v4[] =
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

const int default_servers_v4_size = sizeof(default_servers_v4)/sizeof(DNSList::SDNSEntry);

DNSList::SDNSEntry default_servers_v6[] =
{
    {"2620:0:ccc::2", "OpenDNS", ""},
    {"2620:0:ccd::2", "OpenDNS", ""},
    {"2001:4860:4860::8888", "Google", ""},
    {"2001:4860:4860::8844", "Google", ""},
};

const int default_servers_v6_size = sizeof(default_servers_v6)/sizeof(DNSList::SDNSEntry);

const char* const dnslist_conf_file = "/usr/local/share/pcbsd/conf/pubdns.conf";

/////////////////////////////////////////////////////////////////////////////////

DNSList::DNSList(QWidget *parent, EIPType type) :
    QDialog(parent),
    ui(new Ui::DNSList)
{
    ui->setupUi(this);
    success = false;

    readConfFile();
    //Set default values in no file or no entries in file
    if (!servers_v4.size())
    {
        for (unsigned int i=0; i<default_servers_v4_size; i++)
            servers_v4.push_back(default_servers_v4[i]);
    }
    if (!servers_v6.size())
    {
        for (unsigned int i=0; i<default_servers_v6_size; i++)
            servers_v6.push_back(default_servers_v6[i]);
    }

    if (eIPV4 == type)
    {
        for (int i=0; i<servers_v4.size(); i++)
        {
            QTreeWidgetItem* item = new QTreeWidgetItem();
            item->setText(0, servers_v4[i].mIP);
            item->setText(1, servers_v4[i].mProvider);
            item->setText(2, servers_v4[i].mLocation);
            ui->list->addTopLevelItem(item);
        }
    }else{ //ipv6
        for (int i=0; i<servers_v6.size(); i++)
        {
            QTreeWidgetItem* item = new QTreeWidgetItem();
            item->setText(0, servers_v6[i].mIP);
            item->setText(1, servers_v6[i].mProvider);
            item->setText(2, servers_v6[i].mLocation);
            ui->list->addTopLevelItem(item);
        }
    }
    //ui->list->setRowCount(servers_size+1);    
    ui->list->setColumnWidth(0, 200);
}

/////////////////////////////////////////////////////////////////////////////////
DNSList::~DNSList()
{
    delete ui;
}

/////////////////////////////////////////////////////////////////////////////////
bool DNSList::readConfFile()
{
    QFile file(dnslist_conf_file);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"no file";
        return false;
    }

    QTextStream tst(&file);
    QString line;
    QStringList fields;
    while(!tst.atEnd())
    {
        SDNSEntry entry;

        line = tst.readLine().trimmed();

        qDebug()<<line;
        //Skip empty lines
        if (!line.length())
            continue;
        //Skip comments
        if (line.indexOf("#") == 0)
            continue;

        fields = line.split(";");
        if (fields.size() < 1)
            continue;

        //Read fields
        entry.mIP = fields[0].trimmed();

        if (fields.size()>1)
        {
            entry.mProvider = fields[1].trimmed();
        }

        if (fields.size()>2)
        {
            entry.mLocation = fields[2].trimmed();
        }

        //check type of IP address
        if (pcbsd::Utils::validateIPV4(entry.mIP))
        {
            servers_v4.push_back(entry);
        }
        else
        if (pcbsd::Utils::validateIPV6(entry.mIP))
        {
            servers_v6.push_back(entry);
        }
    }//read all file lines
    return true;
}

/////////////////////////////////////////////////////////////////////////////////
void DNSList::on_buttonBox_accepted(){
  QTreeWidgetItem *it = ui->list->currentItem();
  if(it == 0){
    success = false;
    ipout.clear();
  }else{
    success = true;
    ipout = it->text(0);
  }
  this->close();
}

void DNSList::on_buttonBox_rejected(){
  success = false;
  ipout.clear();
  this->close();
}
