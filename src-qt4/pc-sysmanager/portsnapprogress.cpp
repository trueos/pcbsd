/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
#include "portsnapprogress.h"
#include <qtextstream.h>
#include <QProcess>
#include <QDebug>
#include <QDir>

void PortsnapProgress::init(bool ports, QString pcVer)
{
    doPorts = ports;

    if ( ports )
       startPorts();
    else
       startSource(pcVer);

}

void PortsnapProgress::startPorts()
{
    //Init vars
    workdir = "/var/db/portsnap";
    portsdir = "/usr/ports";

    QDir checkdir;
    if ( ! checkdir.exists(portsdir) )
      checkdir.mkdir(portsdir);

    displayString = "";
    numberSteps = 0;
    

    //Else, just use defaults defined earlier
    
    QStringList args;
    QString prog;
    prog = "/usr/local/share/pcbsd/scripts/portsnap-noterm.sh";
    args << "fetch";
    portsnap = new QProcess(this);
    portsnap->setProcessChannelMode(QProcess::MergedChannels);
    connect(portsnap, SIGNAL(readyReadStandardOutput()), this, SLOT(parseFetch()));
    connect(portsnap, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(fetchDone()));
    portsnap->start(prog, args);

}

void PortsnapProgress::startSource(QString pcVer)
{      
    label->setText(tr("Downloading FreeBSD sources..."));

    QString branch;
    // Figure out which to download
    if ( pcVer.indexOf("STABLE") )
       branch = "stable/" + pcVer.section("-", 0, 0).section(".", 0, 0); 
    else if ( pcVer.indexOf(".") != -1 )
       branch = "releng/" + pcVer.section("-", 0, 0); 
    else
       branch = "master";

    QStringList args;
    QString prog;
    prog = "git";
    args << "clone" << "https://github.com/pcbsd/freebsd.git" << "-b" << branch << "/usr/src";
    qDebug() << args;
    portsnap = new QProcess(this);
    portsnap->setProcessChannelMode(QProcess::MergedChannels);
    connect(portsnap, SIGNAL(readyReadStandardOutput()), this, SLOT(parseUpdate()));
    connect(portsnap, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(fetchDone()));
    portsnap->start(prog, args);
}


void PortsnapProgress::parseFetch()
{
  while (portsnap->canReadLine() )
  {
    QString output = portsnap->readLine().simplified();
    qDebug() << "Portsnap Fetch: " + output;
    taskProgressLbl->setText(displayString);
  }
}



void PortsnapProgress::finish()
{
    taskProgressLbl->setText("Finished!");
    cancelBut->setText("&Close");
}


void PortsnapProgress::cancel()
{
    if (cancelBut->text() == "&Cancel") {
	portsnap->kill();
    }
    this->close();
}


void PortsnapProgress::fetchDone()
{
    if (portsnap->exitStatus() != 0)
    {
	QMessageBox::critical(this, tr("Update Failed!"), tr("Unable to connect to server. Possible causes:\n- Your network is down\n- Target server is unresponsive."));
	this->close();
    }
    else
    {

        // If doing SVN, we can end now
        if ( ! doPorts ) {
          updateDone();
          return;
        }
	
        QStringList args;
        QString prog;
        prog = "/usr/local/share/pcbsd/scripts/portsnap-noterm.sh";
	// Check if we need to do a quick update or full extract
	if ( QFile::exists( portsdir + "Mk/bsd.port.mk") ) 
           args << "update" << "-p" << portsdir ;
	else
           args << "extract" << "update" << "-p" << portsdir ;
	portsnap = new QProcess(this);
        portsnap->setProcessChannelMode(QProcess::MergedChannels);
	connect(portsnap, SIGNAL(readyReadStandardOutput()), this, SLOT(parseUpdate()));
	connect(portsnap, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(updateDone()));
	portsnap->start(prog, args);
    }
}


void PortsnapProgress::parseUpdate()
{
  while( portsnap->canReadLine() )
  {
    QString output = portsnap->readLine().simplified();
    qDebug() << "Portsnap Update: " + output;
    taskProgressLbl->setText(output);
  }
}


void PortsnapProgress::updateDone()
{
    finish();
}

