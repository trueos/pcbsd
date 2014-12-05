
#ifndef __SERVICES_HPP

#define __SERVICES_HPP

#include <qobject.h>
#include <qstring.h>
#include <map>

class services : public QObject
{
    Q_OBJECT
    
private:

    static const QString _service_file;
    static services *_instance;
    QString _file;
    std::map<QString, QString> _items;
    
    services ( void );
    ~services ( void );
    
    void load ( void );
    
public:

    typedef std::map<QString, QString>::const_iterator items_cit;
    typedef std::map<QString, QString>::iterator items_it;

    inline static services *getInstance ( void )
    {
        if ( _instance == 0 )
        { // Create a new instance of none exists
            _instance = new services;
            _instance->load();
        }
        return _instance;
    }
    
    QString lookUp ( const QString& port, const QString& def = tr("Unknown") );
    
    inline std::map<QString, QString>& items ( void )
    {
        return _items;
    }
};

#endif
