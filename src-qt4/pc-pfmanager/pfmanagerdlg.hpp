
#ifndef __PFMANAGER_DLG_H

#define __PFMANAGER_DLG_H

#include "ui_mainwindow.h"
#include <qdialog.h>
#include "packetfilter.hpp"

class PFManagerDlg : public QDialog, private Ui::MainDialog
{
    Q_OBJECT

public:

    PFManagerDlg () : QDialog()
    {
       setupUi(this);
    // Enabled
    connect(cbEnable, SIGNAL(clicked()),
           this, SLOT(enableClicked()));
    // Buttons
    QPushButton::connect(pbStart, SIGNAL(clicked()),
                        this, SLOT(startClicked()));
    // Stop button
    QPushButton::connect(pbStop, SIGNAL(clicked()),
                        this, SLOT(stopClicked()));
    // Restart button
    QPushButton::connect(pbRestart, SIGNAL(clicked()),
                        this, SLOT(restartClicked()));
    // Add a new exception
    QPushButton::connect(pbAdd, SIGNAL(clicked()),
                         this, SLOT(addClicked()));
    // Edit a current one
    QPushButton::connect(pbEdit, SIGNAL(clicked()),
                         this, SLOT(editClicked()));
    // Delete an entry
    QPushButton::connect(pbDelete, SIGNAL(clicked()),
                         this, SLOT(deleteClicked()));
    // Delete stuff
    QPushButton::connect(pbRestore, SIGNAL(clicked()),
                         this, SLOT(restoreClicked()));

    //Show if pf is running or not
    refreshStatus();

    }

    // Create the _firewall object
    PacketFilter _firewall;

    ~PFManagerDlg ( void );

public slots:

    void enableClicked ( void );
    void startClicked ( void );
    void stopClicked ( void );
    void restartClicked ( void );
    void addClicked ( void );
    void editClicked ( void );
    void deleteClicked ( void );
    void restoreClicked ( void );

    void save ( void );
    void load ( void );
    
    void refreshList ( void );
    void refreshStatus(void);
};

#endif
