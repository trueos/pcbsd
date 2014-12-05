
#include "rcconfig.hpp"
#include <qfile.h>
#include <qtextstream.h>
#include <iostream>
#include <QRegExp>
#include <QTextStream>

RcConfig::RcConfig ( void )
    : _modified(false)
{
}

RcConfig::~RcConfig ( void )
{
}

RcConfig* RcConfig::fromDefault ( void )
{
    return RcConfig::fromFile("/etc/rc.conf");
}

RcConfig* RcConfig::fromFile ( const QString& filename )
{
    RcConfig* inst = new RcConfig;
    QFile file(filename);
    
    if ( file.open(QIODevice::ReadOnly) )
    { // It went good
        QString line;
           
        while ( !file.atEnd() )
        {
            line = file.readLine();
            // Push back the lines of the file
            inst->_lines.push_back(line);
        }
        inst->_file = filename;
    }
    else
    { // Error to report
        throw QString(tr("Error opening the specified rc.conf file."));
    }
    return inst;
}

// It may be a code doubling... but providing const/non const
// is necessary to improve overall performance
RcConfig::qstr_vector_it RcConfig::findLine ( const QRegExp& rx )
{
    qstr_vector_revit it = _lines.rbegin();
    qstr_vector_revit last = _lines.rend();
    
    while ( it != last )
    {
        QString line = *it++;
        
        if ( rx.exactMatch(line) )
        {
            return it.base();
        }
    }
    return _lines.end();
}

RcConfig::qstr_vector_cit RcConfig::findLine ( const QRegExp& rx ) const
{
    qstr_vector_crevit it = _lines.rbegin();
    qstr_vector_crevit last = _lines.rend();
    
    // Reverse lookup: We need to find the LAST line
    // since the last line is the one with the effect
    while ( it != last )
    {
        QString line = *it++;
        
        if ( rx.exactMatch(line) )
        {
            return it.base();
        }
    }
    return _lines.end();
}

void RcConfig::save ( void )
{
    QFile file(_file);
    
    // Try to write file to disk
    // 
    if ( _modified )
    { // We only save if the _modified flag is true
        if ( file.open(QIODevice::WriteOnly) )
        {
            QTextStream stream(&file);
            
            for ( qstr_vector_cit it = _lines.begin();
                it != _lines.end();
                it++ )
            { // Write line by line...
                stream << (*it) << "\n";
            }
            _modified = false;
        }
    }
}

bool RcConfig::isEnabled ( const QString& service ) const
{
    const QString sregex = service + "_enable=\"[a-zA-Z]+\"\\s*";
    const QRegExp regex(sregex);
    qstr_vector_cit found;

    // PF is enabled by default in /etc/rc.conf.pcbsd
    bool enabled = true;
    
    found = findLine(regex);
    if ( found != _lines.end() )
    { // We have found one... let's see if it's enabled
        QString line = (*found);
        
        if (line.indexOf("\"yes\"") == -1 && line.indexOf("\"YES\"") == -1 )
        	enabled = false;
	else
        	enabled = true;
    }
    return enabled;
}

void RcConfig::enable ( const QString& service, const QString& enable )
{
    const QString sregex = service + "_enable=\"[a-zA-Z]*\"\\s*";
    const QString senable = service + "_enable=\"" + enable + "\"";
    const QRegExp regex(sregex);
    qstr_vector_it found;
    
    found = RcConfig::findLine(regex);
    if ( found != _lines.end() )
    { // Let's replace the old
        QString line = *found;
        if ( line.indexOf("\"" + enable + "\"") != -1 )
        { // but only if it does not have the current state
            _lines.erase(found);
            _lines.insert(found, senable); 
            _modified = true;
        }
    }
    else
    { // Append the needed lines...        
        _lines.push_back(senable);
        _modified = true;
    }
    // And save changes
    save();
}

void RcConfig::enable ( const QString& service )
{
    enable(service, "YES");
}

void RcConfig::disable ( const QString& service )
{
    enable(service, "NO");
}

void RcConfig::addParameter ( const QString& service, const QString& param, const QString& value )
{
    const QString sxline = service + "_" + param + "=\"[^\"]*\"\\s*";
    const QString sxvalue = service + "_" + param + "=\"" + value + "\"\\s*";
    QString newline = service + "_" + param + "=\"" + value + "\"";
    const QRegExp xline(sxline);
    const QRegExp xvalue(sxvalue);
    qstr_vector_it found;
    
    found = findLine(xline);
    if ( found != _lines.end() )
    { // We have such a line
        QString line = *found;
        
        if ( !xvalue.exactMatch(line) )
        { // Let's modify it...
            _lines.erase(found);
            _lines.insert(found, newline);
            _modified = true;
        }
    }
    else
    { // We do not have such a line, therefore we append it
        _lines.push_back(newline);
        _modified = true;
    }
    // And save the changes (if there are some)
    save();
}

bool RcConfig::getParameter ( const QString& service, const QString& param, QString& retvalue ) const
{
    const QString sregex = service + "_" + param + "=\"([^\"]*)\"\\s*";
    QRegExp regex(sregex);    
    qstr_vector_cit it;
    bool success = false;
    
    it = findLine(regex);
    if ( it != _lines.end() )
    {
        QString line = *it;
        int pos = 0;
        // Fetch the value
        pos = regex.indexIn(line);
        if ( pos > -1 )
        { // Retrieve capture
            retvalue = regex.cap(1);
            success = true;
        }
    }
    // nothing found
    return success;
}

bool RcConfig::exists ( const QString& service ) const
{
    const QString sregex = service + "_enable=\"[a-zA-Z]*\"\\s*";
    const QRegExp regex(sregex);
    qstr_vector_cit it;
    
    return ( (it = findLine(regex)) != _lines.end() );
}

bool RcConfig::exists ( const QString& service, const QString& param ) const
{
    const QString sregex = service + "_" + param + "=\"[^\"]*\"\\s*";
    const QRegExp regex(sregex);
    qstr_vector_cit it;
    
    return ( (it = findLine(regex)) != _lines.end() );
}

