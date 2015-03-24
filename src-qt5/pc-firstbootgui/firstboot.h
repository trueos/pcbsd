#ifndef INSTALLER_H
#define INSTALLER_H

#include <QWidget>
#include <QMessageBox>
#include <QFile>
#include <QProcess>
#include <QTranslator>

#include "ui_firstboot.h"
#include "dialogHelp.h"
#include "dialogKeyboard.h"
#include "dialogInfoBox.h"
#include "netKey.h"

#include "backend.h"

class Installer : public QMainWindow, private Ui::Installer
{
    Q_OBJECT

public:
    Installer(QWidget *parent = 0);
    ~Installer();
    void changeLang(QString code);
    void initInstall();

private slots:
    void slotBack();
    void slotHelp();
    void slotNext();
    void slotFinished();
    void slotChangeLanguage();

    // Start xvkbd
    void slotPushVirtKeyboard();

    // Keyboard selector
    void slotPushKeyLayout();
    void slotKeyLayoutUpdated(QString mod, QString lay, QString var);
    
    // Root PW validator
    void slotCheckRootPW();

    // User validators
    void slotCheckUser();
    void slotSuggestUsername();
    
    // Let us check the hostname
    void slotCheckHost();
    
    // Start a scan of the network
    void slotScanNetwork();

    // Add new wifi
    void slotAddNewWifi();
    void slotQuickConnect(QString key,QString SSID);

    // Select a PersonaCrypt device
    void slotGetPCDevice();
    
    // Set the current audio device
    void slotSetAudioDev();
    
    // Update the audio volume label
    void slotAudioVolumeChanged();
    
    // Play the test audio clip
    void slotPlayAudioTest();
    
private:
    void proceed(bool);
    bool haveWifi;

    // Get network pass
    void addNetworkProfile(QString ssid);

    // Apply the settings
    void saveSettings(); 

    QStringList languages;

    // Help Stuff
    dialogHelp *pcHelp;

    // Network key dialog
    netKey *dialogNetKey;

    // Infobox 
    dialogInfoBox *dIB;
    QMenu *popup;

    // Keyboard Widget
    widgetKeyboard *wKey;
    QStringList keyModels;
    QStringList keyLayouts;
    QString kbMod;
    QString kbLay;
    QString kbVar;

    QTranslator *translator;

protected:
    void closeEvent(QCloseEvent *event);

};

#endif // INSTALLER_H
