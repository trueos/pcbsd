
#include "services.hpp"
#include <qfile.h>
#include <qtextstream.h>
#include <qstringlist.h>
#include <qregexp.h>
#include <QTextStream>

const QString services::_service_file = "/etc/services";
services *services::_instance = 0;

services::services ( void )
{
   load();
}

services::~services ( void )
{
}

void services::load ( void )
{
    QRegExp items("([a-zA-Z0-9\\-_\\.\\/]+)\\s+\\b(\\d+)\\b");
    int pos = 0;
    QStringList lst;
    QFile serv(_service_file);
    
    if ( serv.open(QIODevice::ReadOnly) )
    {
        QString line;
        
        while ( !serv.atEnd() )
        {
            line = serv.readLine();
            if ( line[0] != '#' )
            { // Try to leave out comments, should boost this thing
              // a little bit
                _file += line + '\n';
                
                pos = items.indexIn(line);
                if ( pos >= 0 )
                { // Find all items
                    lst = items.capturedTexts();
                    
                    if ( lst.count() >= 3 )
                    { // Read it there
                        _items[lst[1]] = lst[2];
                    }
                }
            }
        } 
    }
}

QString services::lookUp ( const QString& port, const QString& def )
{
    QString sregex;
    QRegExp regex;
    int find = 0;
    QStringList found;
    
    // Put our regex together
    sregex = QString("\\s+([a-zA-Z0-9\\-_\\.\\/]+)\\s+\\b%1\\b").arg(port);
    regex.setPattern(sregex);
    
    find = regex.indexIn(_file);
    if ( find != -1 )
    {
        found = regex.capturedTexts();
        if ( found.size() > 0 )
        {
            return found[1];
        }
    }
    return def;
}
