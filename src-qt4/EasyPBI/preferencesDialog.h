#ifndef _PREFERENCES_DIALOG_H
#define _PREFERENCES_DIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include "ui_preferencesDialog.h"
#include "backend.h"
#include "config.h"


namespace Ui {
    class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = 0);
    virtual ~PreferencesDialog();

private slots:
    void resetGUI();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    // PBI Settings Tab
    void on_push_pbioutdir_clicked();
    void on_push_sigfile_clicked();
    void on_push_addpkg_clicked();
    void on_push_rmpkg_clicked();
    void on_push_clearcache_clicked();
    void on_check_signpbi_stateChanged(int);
    void on_check_usepkgcache_stateChanged(int);
    // Paths Tab
    void on_push_moduledir_clicked();
    void on_push_icondir_clicked();
    void on_push_defaulticon_clicked();
    //Utilities Tab
    void on_push_suutility_clicked();
    void on_push_pbicreate_clicked();
    void on_push_makeport_clicked();
    void on_push_resetutils_clicked();
    void on_push_portsdir_clicked();

private:
    Ui::PreferencesDialog *ui;
    Config *settings;

};

#endif
