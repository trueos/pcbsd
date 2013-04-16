
#include "packetfilter.hpp"
#include "rule.hpp"
#include "services.hpp"
#include "rcconfig.hpp"
#include <qfile.h>
#include <qregexp.h>
#include <qstringlist.h>
#include <qtextstream.h>
#include <QProcess>
#include <QUrl>
#include <qmessagebox.h>
#include <QTextStream>
#include <QDebug>

#include <iostream>

PacketFilter::PacketFilter ( void )
{
    init();
}

PacketFilter::~PacketFilter ( void )
{
}

const QString PacketFilter::_service = "pf";
const QString PacketFilter::_param_rules = "rules";
const QString PacketFilter::_param_flags = "flags";
const QString PacketFilter::_default_rules = "/etc/pf.conf";

bool PacketFilter::parseLine ( const QString& line )
{
    QRegExp regex("(block|pass)\\s+(in|out)\\s+(quick on|on)\\s+(\\w+)\\s+proto\\s+(.*)\\s+from\\s+.*\\s+port\\s+(.*)\\s+");
    rule rul;
    int search = 0;
    bool edited = false;
    
    // Needed for non gready matches
    regex.setMinimal(true);
    regex.setPatternSyntax(QRegExp::RegExp2);
    search = regex.indexIn(line);
    if ( search != -1 )
    {
        QStringList capt = regex.capturedTexts();
        
        if ( capt.size() > 5 )
        { // We have enough data
            QString name;
        
	    //qDebug() << capt;
            rul.policy(capt[1]);
            rul.direction(capt[2]);
	    if ( capt[3] == "quick on" ) {
              rul.device(capt[4]);
              rul.protocol(capt[5]);
              rul.port(capt[6]);
	    } else {
              rul.device(capt[3]);
              rul.protocol(capt[4]);
              rul.port(capt[5]);
            }
            // Find out the service depending on the port
            name = services::getInstance()->lookUp(rul.port());
            rul.name(name);
            
            _rules.push_back(rul);
            
            edited = true;
        }
    }
    return edited;
}

void PacketFilter::add_rule ( const rule& value )
{
    _rules.push_back(value);
}

bool PacketFilter::isEnabled ( void ) const
{
    RcConfig *config = RcConfig::fromDefault();
    bool enabled;
    
    // Retrieve enabled
    enabled = config->isEnabled(_service);
    delete config;
    
    return enabled;
}

void PacketFilter::enable ( void )
{
    RcConfig *config = RcConfig::fromDefault();
    // Enable the rule
    config->enable(_service);
    // Set default parameters, like flags and config file
    if ( !config->exists(_service, _param_rules) )
    { // Enable default rules file if none exists
        config->addParameter(_service, _param_rules, _default_rules);
    }
    if ( !config->exists(_service, _param_flags) )
    { // Enable default flags if none exists
        config->addParameter(_service, _param_flags, "");
    }
    // Delete object
    delete config;
}

void PacketFilter::disable ( void )
{
    RcConfig *config = RcConfig::fromDefault();
    // Disable rule
    config->disable(_service);
    // Delete config object
    delete config;
}

void PacketFilter::init ( void )
{

    RcConfig *config = RcConfig::fromDefault();
    
    // Retrieve rules file
    if ( !config->getParameter(_service, _param_rules, _rulefile) )
    { // if there is none, set the default one
        config->addParameter(_service, _param_rules, _default_rules);
        _rulefile = _default_rules;
    }
    
    // Read rules file here
    readConfig();
    
    // Delete config
    delete config;
}

void PacketFilter::readConfig ( void )
{
    QFile file(_rulefile);
    
    _lines.clear();
    _rules.clear();
    
    if ( file.open(QIODevice::ReadOnly) )
    {
        QString line;
        
        while ( !file.atEnd() )
        {
            line = file.readLine();
            // Now check line if it tells us any
            // nice information about the rules
            if ( !parseLine(line) )
            {
                _lines.push_back(line);
            }
        }
    }
}

void PacketFilter::saveConfig ( void )
{
    QFile file(_rulefile);
    
    if ( file.open(QIODevice::WriteOnly) )
    {
        QTextStream stream(&file);
        
        for ( size_t i = 0; i < _lines.size(); ++i )
        { // Write the non parse files first
            stream << _lines[i] << "\n";
        }
        // And now write the rules out
        for ( size_t i = 0; i < _rules.size(); ++i )
        {
            const rule& cur = _rules[i];
            
            stream << cur.to_string() << "\n";
        }
        // Make sure it is flushed
        file.close();
    }
}

void PacketFilter::start ( void )
{
    QProcess proc;
    
    QStringList args;
    QString prog = "pfctl";
    
    args << "-e";
    proc.start(prog, args);
    proc.waitForFinished();
    
    // Refresh the config file
    args.clear();
    args << "-f";
    args << _rulefile;
    proc.start(prog, args);
    proc.waitForFinished();
}

void PacketFilter::stop ( void )
{
    QProcess proc;
    
    QStringList args;
    QString prog = "pfctl";
    // Disable the pf firewall
    args << "-d";
    proc.start(prog, args);
    proc.waitForFinished();
}

void PacketFilter::restart ( void )
{
    // That easy, isn't it?
    stop();
    start();
}

void PacketFilter::save ( void )
{
    // ADDME: Support for restoring
    saveConfig();
}

void PacketFilter::backup ( void )
{
    QString file = _rulefile;
    QString backup = _rulefile + "~";
    QFile oldFile(file);
    oldFile.copy(backup);
}

void PacketFilter::restore ( void )
{
    QString file = _rulefile;
    QString backup = _rulefile + "~";
    QFile oldFile(backup);
    oldFile.copy(file);
}

bool PacketFilter::isRunning(void)
{
    QProcess proc;
    
    QStringList args;
    QString prog = "pfctl";
    // Query pf
    args << "-s";
    args << "info";
    proc.start(prog, args);
    
    if (proc.waitForFinished() || proc.canReadLine())
    {
	if (proc.canReadLine())
	{
	    QString line = proc.readLine();
	    if (line.indexOf("Enabled") != -1) { return true; }
	}
    }
    
    return false;
    
}
