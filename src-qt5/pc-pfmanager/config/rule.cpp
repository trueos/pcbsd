
#include "rule.hpp"

rule::rule ( void )
{
}

rule::~rule ( void )
{
}

QString rule::to_string ( void ) const
{
    QString scheme;
    QString line;

    if ( _policy == "pass" )
      scheme = "%1 %2 quick on %3 proto %4 from any to (%5) port %6 keep state";
    else
      scheme = "%1 %2 quick on %3 proto %4 from any to (%5) port %6";
    
    line = scheme.arg(_policy)      // policy (block|pass)
                 .arg(_direction)   // direction (in|out)
                 .arg(_device)      // device
                 .arg(_protocol)    // protocol udp or tcp
                 .arg(_device)      // device
                 .arg(_port);       // port number
    return line;
}
