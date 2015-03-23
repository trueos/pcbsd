
#ifndef __SERVICE_HPP

#define __SERVICE_HPP

#include <qstring.h>

class rule
{
private:

    QString _name;
    QString _direction;
    QString _device;
    QString _port;
    QString _policy;
    QString _protocol;
    
public:

    rule ( void );
    ~rule ( void );
    
    inline const QString& name ( void ) const
    {
        return _name;
    }
    
    inline void name ( const QString& value )
    {
        _name = value;
    }
    
    inline const QString& device ( void ) const
    {
        return _device;
    }
    
    inline void device ( const QString& value )
    {
        _device = value;
    }
    
    inline const QString& direction ( void ) const
    {
        return _direction;
    }
    
    inline void direction ( const QString& value )
    {
        _direction = value;
    }
    
    inline const QString& port ( void ) const
    {
        return _port;
    }
    
    inline void port ( const QString& value )
    {
        _port = value;
    }
    
    inline const QString& policy ( void ) const
    {
        return _policy;
    }
    
    inline void policy ( const QString& value )
    {
        _policy = value;
    }
    
    inline const QString& protocol ( void ) const
    {
        return _protocol;
    }
    
    inline void protocol ( const QString& value )
    {
        _protocol = value;
    }
    
    QString to_string ( void ) const;
};

#endif
