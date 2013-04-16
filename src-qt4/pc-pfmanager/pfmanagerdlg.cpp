
#include "pfmanagerdlg.hpp"
#include "rule.hpp"
#include "packetfilter.hpp"
#include "interfaces.hpp"
#include "pfaddlg.hpp"
#include "taggedlistviewitem.hpp"
#include <qcheckbox.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <QTreeWidgetItem>
#include <pcbsd-utils.h>

PFManagerDlg::~PFManagerDlg(void)
{

}

void PFManagerDlg::enableClicked ( void )
{
    bool checked = cbEnable->isChecked();

    if (checked != _firewall.isEnabled())
    {
        // Just modify something, if our state has changed
        if ( checked )
        {
            _firewall.enable();
	    pcbsd::Utils::setConfFileValue( "/etc/rc.conf", "pf_enable", "pf_enable=\"YES\"", -1);
   	    system("/etc/rc.d/pf start");
        }
        else
        { // disable firewall
            // Ask again if he's really sure...
            QString question = tr("This will turn off the firewall completely.\n" \
                                  "Are you sure?");

            if ( QMessageBox::question(this, tr("Question"), question,
                                       QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
            {
		system("/etc/rc.d/pf stop");
		pcbsd::Utils::setConfFileValue( "/etc/rc.conf", "pf_enable", "pf_enable=\"NO\"", -1);
            } else {
    		cbEnable->setChecked(true);
	    }
        }
    }
}

void PFManagerDlg::startClicked ( void )
{
    _firewall.start();
    refreshStatus();
}

void PFManagerDlg::stopClicked ( void )
{
    _firewall.stop();
    refreshStatus();
}

void PFManagerDlg::restartClicked ( void )
{
    _firewall.restart();
    refreshStatus();
    if (_firewall.isRunning())
      QMessageBox::information(this, tr("Success"), tr("Restarted successfuly!"));
    else
      QMessageBox::warning(this, tr("Failure"), tr("Failed to restart!"));
}

// WARNING: Make sure you only store TaggedListViewItem
//
void PFManagerDlg::refreshList ( void )
{
    lvExceptions->clear();
       
    for ( size_t i = 0; i < _firewall.rules().size(); i++ )
    {
        TaggedListViewItem *item = 0;
        const rule& srv = _firewall.rules()[i];
        
        item = new TaggedListViewItem(lvExceptions);
        
        item->setText(0, srv.name());
        item->setText(1, srv.port());
        item->setText(2, srv.policy());
        item->setText(3, srv.device());
        // Save the item we had, so we can savely
        // return it later
        item->tag(i);
    }
}

void PFManagerDlg::addClicked ( void )
{
    PFAddDlg *dlg = new PFAddDlg();
    dlg->init();
    
    if ( dlg->exec() == QDialog::Accepted )
    {
        _firewall.add_rule(* dlg->rules());
        // Save content
        _firewall.save();
        // Refresh the list
        refreshList();
    }
}

void PFManagerDlg::editClicked ( void )
{
    QTreeWidgetItem *cur = 0;
    
    cur = lvExceptions->currentItem();
    if (cur != 0)
    {
        PFAddDlg *dlg = new PFAddDlg();
        dlg->init();
        size_t tag = 0;
        rule *rule = 0;
        
        // A little bit hackerish though
        // But way more efficient than searching the list of the proper
        // item.
        tag = reinterpret_cast<TaggedListViewItem*>(cur)->tag();
        rule = &_firewall.rules()[tag];
        // Show
        dlg->rules(rule);
        if ( dlg->exec() == QDialog::Accepted )
        { // Refresh the list
            _firewall.save();
            refreshList();
        }
    }
}

void PFManagerDlg::deleteClicked ( void )
{
    QTreeWidgetItem *cur = 0;
    
    cur = lvExceptions->currentItem();
    if (cur != 0)
    {
        size_t tag = 0;
        
        tag = reinterpret_cast<TaggedListViewItem*>(cur)->tag();
        // Delete this item 
        _firewall.rules().erase(_firewall.rules().begin()+tag);   
        // And repaint the list
        refreshList();
        // Save the file
        _firewall.save();
    }
}

void PFManagerDlg::restoreClicked ( void )
{
    _firewall.restore();
    QMessageBox::information(this, tr("Restored."), tr("Config file successfuly restored."));
}

void PFManagerDlg::save ( void )
{
}

void PFManagerDlg::load ( void )
{
    // Make a backup
    _firewall.backup();
    (void)interfaces::getInstance();
    // load config file
    _firewall.init();
    // Check if it is enabled
    cbEnable->setChecked(_firewall.isEnabled());
    // Fill list with services
    refreshList();
}	    

void PFManagerDlg::refreshStatus(void)
{
    if (_firewall.isRunning())
    {
	pbStart->setEnabled(false);
	pbStop->setEnabled(true);
	pbRestart->setEnabled(true);
    }
    else
    {
	pbStart->setEnabled(true);
	pbStop->setEnabled(false);
	pbRestart->setEnabled(false);
    }
}
