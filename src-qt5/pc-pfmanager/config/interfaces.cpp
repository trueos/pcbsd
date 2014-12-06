
#include "interfaces.hpp"
#include <qregexp.h>
#include <qstringlist.h>

interfaces *interfaces::_instance = 0;

interfaces::interfaces ( void )
{
}

interfaces::~interfaces ( void )
{
}

void interfaces::load ( void )
{
    FILE *file = 0;
    char buf[100] = {0};
    QString content;
    QRegExp ifs("(\\w+\\d+)\\:\\s+.*\\s+ether\\s+[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}.*");
    QStringList lst;
    int pos = 0;
    
    file = popen("ifconfig", "r");
    if ( file != 0 )
    {
        while ( !feof(file) )
        { // Read all input
            const char *ret = fgets(buf, 99, file);
//          if ( ret[99] != '\0' )
//          { // Possible buffer overflow
//              exit();
//          }
            content += ret;
        }
        ifs.setMinimal(true);
        // Parse input
        while ( (pos = ifs.indexIn(content, pos)) >= 0 )
        {
            QStringList lst = ifs.capturedTexts();
            QString device;
            
            if ( lst.size() > 0 )
            {
                device = lst[1];
                _ifs.push_back(device);
                pos += lst[1].length();
            }
            else ++pos;
        }
      pclose(file);
    }
}
