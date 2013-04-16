
#ifndef __PFADDLG_HPP

#define __PFADDLG_HPP

#include "ui_adddlg.h"
#include "rule.hpp"
#include <qwidget.h>
#include <qobject.h>

class PFAddDlg : public QDialog, private Ui::AddDlg
{
    Q_OBJECT
    
private:

    rule *_rule;
    
    void fillData ( void );
    
public:

    PFAddDlg() : QDialog()
    {
      setupUi(this);
    }

    ~PFAddDlg ( void );
    
    inline rule *rules ( void ) const
    {
        return _rule;
    }
    
    inline void rules ( rule *value )
    {
        _rule = value;
        fillData();
    }
    
public slots:

    void serviceClicked ( const QString& index );
    void okClicked ( void );
    void cancelClicked ( void );
    void init(void );
};

#endif
