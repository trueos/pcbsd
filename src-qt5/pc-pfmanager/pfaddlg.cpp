
#include "pfaddlg.hpp"
#include "services.hpp"
#include "interfaces.hpp"
#include <qcombobox.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <QDebug>
#include <QMessageBox>

void PFAddDlg::init(void)
{
    rule *rules = 0;

    // Well: The incoming thing must always be the first item
    // Otherwise it won't work
    cbDirection->addItem(tr("incoming"));
    cbDirection->addItem(tr("outgoing"));
    
    // Protocols...
    cbProtocol->addItem(tr("TCP"));
    cbProtocol->addItem(tr("UDP"));
    
    // Default check the Allow button
    rbAllow->setChecked(true);
    
    //First define the "Custom" service
    cbServices->addItem("Custom");
    // Add all services
    for ( services::items_cit it = services::getInstance()->items().begin(); 
          it != services::getInstance()->items().end(); it++ )
    {
        cbServices->addItem(it->first);
    }
    // Add all interfaces
    for ( interfaces::ifs_cit it = interfaces::getInstance()->ifs().begin();
          it != interfaces::getInstance()->ifs().end(); it++ )
    {
        cbInterfaces->addItem(*it);
    }
    
    // Rule
    _rule = rules;
    fillData();
    
    // Connect the signals/slots
    //QComboBox::connect(cbServices, SIGNAL(highlighted(const QString&)),
    //                   this, SLOT(serviceClicked(const QString&)));
    QComboBox::connect(cbServices, SIGNAL(currentIndexChanged(const QString&)),
                       this, SLOT(serviceClicked(const QString&)));
    // The buttons
    QPushButton::connect(pbOkay, SIGNAL(clicked()),
                         this, SLOT(okClicked()));
    // Cancel button
    QPushButton::connect(pbCancel, SIGNAL(clicked()),
                         this, SLOT(cancelClicked()));
}

PFAddDlg::~PFAddDlg ( void )
{
}

void PFAddDlg::fillData ( void )
{
    if ( _rule == 0 )
    { // The 'new' mode
        _rule = new rule;        
        // And do nothing... We will fill it up later
    }
    else
    { // The 'edit' mode
        qDebug() << _rule->to_string();
        int item = 0;
    
        cbServices->setItemText(cbServices->currentIndex(), _rule->name());
        txPort->setText(_rule->port());
        // Check the policy
        if ( _rule->policy().compare("block") == 0 )
        {
            rbBlock->setChecked(true);
            rbAllow->setChecked(false);
        }
        else
        { 
            rbBlock->setChecked(false);
            rbAllow->setChecked(true);
        }
        cbDirection->setCurrentIndex((_rule->direction().compare("in") == 0) ? 0 : 1);
        cbProtocol->setCurrentIndex((_rule->protocol().compare("tcp") == 0) ? 0 : 1);
        // Find the item
        // I haven't found any better solution than to iterate
        // over it on my own. listBox()->findItem() method returns
        // only a ListBoxItem object which is pretty useless here
        for (; item < cbInterfaces->count(); ++item )
        {
            if ( cbInterfaces->itemText(item).compare(_rule->device()) == 0 )
            {
                cbInterfaces->setCurrentIndex(item);
                break;
            }
        }
    }
}

void PFAddDlg::serviceClicked ( const QString& )
{
    if ( index >= 0 )
    {
        QString item = "";
        QString service = "";
        
        service = cbServices->currentText();
        item = services::getInstance()->items()[service];
    
        txPort->setText(item);
    }
}

void PFAddDlg::okClicked ( void )
{
    // Set the information
    _rule->port(txPort->text());
    _rule->name(cbServices->currentText());
    _rule->protocol(cbProtocol->currentText().toLower());
    _rule->policy( (rbBlock->isChecked() ? "block" : "pass") );
    _rule->direction( (cbDirection->currentIndex() == 0) ? "in" : "out" );
    _rule->device( cbInterfaces->currentText() );
    // Additional checking here
    accept();
}

void PFAddDlg::cancelClicked ( void )
{
    // Additional checking here
    reject();
}
