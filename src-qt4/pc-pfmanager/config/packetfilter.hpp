
#ifndef __PF_FIREWALL

#define __PF_FIREWALL

#include <qstring.h>
#include <vector>
#include "rule.hpp"

class PacketFilter
{
private:
    
    static const QString _service;
    static const QString _param_rules;
    static const QString _param_flags;
    static const QString _default_rules;
    
    QString _rulefile;
    
    std::vector<QString> _lines;
    std::vector<rule> _rules;
    
    
protected:

    bool parseLine ( const QString& line );
    void readConfig ( void );
    void saveConfig ( void );
    
public:
    
    typedef std::vector<rule>::iterator rules_it;
    typedef std::vector<rule>::const_iterator rules_cit;
    
    PacketFilter ( void );
    virtual ~PacketFilter ( void );
    
    void enable ( void );
    void disable ( void );
    bool isEnabled ( void ) const;
    void init ( void );
    void save ( void );

    void start ( void );
    void stop ( void );
    void restart ( void );
    bool isRunning(void);
    
    void add_rule ( const rule& rul );
    
    inline std::vector<rule>& rules ( void )
    {
        return _rules;
    }
    
    void backup ( void );
    void restore ( void );
};

#endif
