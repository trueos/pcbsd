
#include "pfmanagerdlg.hpp"
//#include "rule.hpp"
//#include "packetfilter.hpp"
//#include "interfaces.hpp"
//#include "pfaddlg.hpp"
//#include "taggedlistviewitem.hpp"
#include <qcheckbox.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <QTreeWidgetItem>
#include <pcbsd-utils.h>
#include <QProcess>
#include <QFile>
#include <QTextStream>

PFManagerDlg::~PFManagerDlg(void)
{

}

void PFManagerDlg::enableClicked ( void )
{
    bool checked = cbEnable->isChecked();

    if (firewallRunning)
    {
        // Just modify something, if our state has changed
        if ( checked )
        {
            //_firewall.enable();
	    pcbsd::Utils::setConfFileValue( "/etc/rc.conf", "firewall_enable=", "firewall_enable=\"YES\"", -1);
   	    system("/etc/rc.d/ipfw start");
        }
        else
        { // disable firewall
            // Ask again if he's really sure...
            QString question = tr("This will turn off the firewall completely.\n" \
                                  "Are you sure?");

            if ( QMessageBox::question(this, tr("Question"), question,
                                       QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
            {
		system("/etc/rc.d/ipfw stop");
		pcbsd::Utils::setConfFileValue( "/etc/rc.conf", "firewall_enable=", "firewall_enable=\"NO\"", -1);
            } else {
    		cbEnable->setChecked(true);
	    }
        }
    }
}

void PFManagerDlg::startClicked ( void )
{
    system("/etc/rc.d/ipfw start");
    refreshStatus();
}

void PFManagerDlg::stopClicked ( void )
{
    system("/etc/rc.d/ipfw stop");
    refreshStatus();
}

void PFManagerDlg::restartClicked ( void )
{
    system("/etc/rc.d/ipfw restart");
    refreshStatus();
    if (firewallRunning)
      QMessageBox::information(this, tr("Success"), tr("Restarted successfuly!"));
    else
      QMessageBox::warning(this, tr("Failure"), tr("Failed to restart!"));
}

void PFManagerDlg::addClicked ( void )
{
  //Get the options listed and save them into the file/data
  QString type = combo_porttype->currentText();
  QString port = spin_portnum->cleanText();
  openports << port+"::::"+type;
  tree_openports->addTopLevelItem( new QTreeWidgetItem(QStringList() << port << type) );
  SaveOpenPorts();
}


void PFManagerDlg::deleteClicked ( void )
{
    QList<QTreeWidgetItem*> sel = tree_openports->selectedItems();
    for(int i=0; i<sel.length(); i++){
      openports.removeAll( sel[i]->text(0)+"::::"+sel[i]->text(1) );
    }
    //Now update the UI list (need to be careful about multiple TreeWidgetItems per row - safer this way
    tree_openports->clear();
    for(int i=0; i<openports.length(); i++){
      tree_openports->addTopLevelItem( new QTreeWidgetItem( openports[i].split("::::") ) );
    }
    //Now save the list to file
    SaveOpenPorts();
}

void PFManagerDlg::restoreClicked ( void )
{
    //move the files out of the way
    system("mv /etc/ipfw.rules /etc/ipfw.rules.previous");
    system("mv /etc/ipfw.openports /etc/ipfw.openports.previous");
    //refresh/restart the rules files
    system("sh /usr/local/share/pcbsd/scripts/reset-firewall");

    QMessageBox::information(this, tr("Restored."), tr("Config file successfuly restored."));
}    

void PFManagerDlg::refreshStatus(void)
{
    //First check if the firewall is running
    firewallRunning = false;
    QProcess proc;
    proc.start("sysctl net.inet.ip.fw.enable");
    if(proc.waitForFinished() || proc.canReadLine()){
	if (proc.canReadLine()){
	    QString line = proc.readLine();
	    if(line.section(":",1,1).simplified().toInt() ==1) { firewallRunning = true; }
	}
    }
    //Enable/disable the UI elements
    if (firewallRunning)
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
    UpdatePortButtons();
}

void PFManagerDlg::UpdatePortButtons(){
  pbDelete->setEnabled( !tree_openports->selectedItems().isEmpty() );
  pbAdd->setEnabled( !openports.contains( combo_porttype->currentText()+"::::"+spin_portnum->cleanText() ) );	
}

void PFManagerDlg::LoadOpenPorts(){
  openports.clear();
  tree_openports->clear();
  QFile file("/etc/ipfw.openports");
  if( file.open(QIODevice::ReadOnly) ){
    QTextStream in(&file);
    while( !in.atEnd() ){
      QString line = in.readLine();
      if(line.startsWith("#") || line.simplified().isEmpty()){ continue; }
      //File format: "<type> <port>" (nice and simple)
      openports << line.section(" ",1,1)+"::::"+line.section(" ",0,0);
    }
    file.close();
  }
  openports.sort(); //order them in ascending port order
  //Now update the UI list
  for(int i=0; i<openports.length(); i++){
    tree_openports->addTopLevelItem( new QTreeWidgetItem( openports[i].split("::::") ) );
  }
}

void PFManagerDlg::SaveOpenPorts(){
  //Convert to file format
  openports.sort(); //make sure they are still sorted by port
  QStringList fileout;
  for(int i=0; i<openports.length(); i++){
    fileout << openports[i].section("::::",1,1)+" "+openports[i].section("::::",0,0);
  }
  //Always make sure that the file always ends with a newline
  if(!fileout.isEmpty()){ fileout << ""; }
  //Save to file
  QFile file("/etc/ipfw.openports");
  if( file.open(QIODevice::WriteOnly | QIODevice::Truncate) ){
    QTextStream out(&file);
    out << fileout.join("\n");
    file.close();
  }
  //Re-load/start rules (just in case - it is a smart script)
  if(firewallRunning){ system("sh /usr/local/share/pcbsd/scripts/reset-firewall"); }
}