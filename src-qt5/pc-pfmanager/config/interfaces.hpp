
#ifndef __INTERFACES_H

#define __INTERFACES_H

#include <qstring.h>
#include <qprocess.h>
#include <vector>

class interfaces : public QObject
{
    Q_OBJECT

private:

    static interfaces *_instance;
    
    std::vector<QString> _ifs;
    
    interfaces ( void );
    ~interfaces ( void );
    
    void load ( void );
    
public:

    typedef std::vector<QString>::const_iterator ifs_cit;
    typedef std::vector<QString>::iterator ifs_it;

    inline static interfaces* getInstance ( void )
    {
        if ( _instance == 0 )
        {
            _instance = new interfaces;
            _instance->load();
        }
        return _instance;
    }
    
    inline const std::vector<QString>& ifs ( void ) const
    {
        return _ifs;
    }

};

#endif
