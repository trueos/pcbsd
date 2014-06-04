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
    enum EIPType
    {
        eIPV4=0,
        eIPV6
    };	

    explicit DNSList(QWidget *parent = 0, EIPType type = eIPV4);
    ~DNSList();

    bool success; //for returning whether the settings should be applied
    QString ipout; //return value
    
private:
    Ui::DNSList *ui;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // DNSLIST_H
