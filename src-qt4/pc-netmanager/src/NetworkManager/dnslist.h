#ifndef DNSLIST_H
#define DNSLIST_H

#include <QDialog>

namespace Ui {
class DNSList;
}

class DNSList : public QDialog
{
    Q_OBJECT
    
public:
    explicit DNSList(QWidget *parent = 0);
    ~DNSList();

    typedef enum EIPType
    {
        eIPV4=0,
        eIPV6
    };

    int getIP(EIPType type, QString& ip);
    
private:
    Ui::DNSList *ui;
};

#endif // DNSLIST_H
