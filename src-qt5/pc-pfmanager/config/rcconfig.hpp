
#ifndef __RC_CONFIG

#define __RC_CONFIG

#include <qobject.h>
#include <qstring.h>
#include <qregexp.h>
#include <vector>

/*
 * This class provides basic manipulation of the
 * rc.conf file. It aims to reserve original file as
 * good as possible, by allowing enabling/disabling of
 * services and adding service depended parameters with values.
 * Notice: It will automatically save the file to disk 
 * after a modification of the config state stored internally
 * happened. Since the class does not check if the
 * configuration file has been modified externally it may
 * overrides external settings which where added to the file
 * manually after the opening.
 * It is wise to use this class just in time.
 */     
class RcConfig : public QObject
{
    Q_OBJECT
    
private:

    
    typedef std::vector<QString> qstr_vector;
    typedef std::vector<QString>::iterator qstr_vector_it;
    typedef std::vector<QString>::const_iterator qstr_vector_cit;
    typedef std::vector<QString>::reverse_iterator qstr_vector_revit;
    typedef std::vector<QString>::const_reverse_iterator qstr_vector_crevit;
    
    qstr_vector _lines;
    
    QString _file;
    bool _modified;
    
    qstr_vector_it findLine ( const QRegExp& regexp );
    qstr_vector_cit findLine ( const QRegExp& regexp ) const;
    
    void save ( void );    
    void enable ( const QString& service, const QString& enable );

public:

    RcConfig ( void );
    ~RcConfig ( void );

    static RcConfig* fromDefault ( void );
    static RcConfig* fromFile ( const QString& file );
    
    void enable ( const QString& service );    
    bool isEnabled ( const QString& service ) const;
    void disable ( const QString& service );
    void addParameter ( const QString& service, const QString& param, const QString& value );
    bool exists ( const QString& service ) const;
    bool exists ( const QString& service, const QString& param ) const;
    bool getParameter ( const QString& service, const QString& param, QString& retvalue ) const;
};

#endif
