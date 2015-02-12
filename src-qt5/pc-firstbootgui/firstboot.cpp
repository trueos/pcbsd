#include <QProcess>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QTemporaryFile>
#include <QCloseEvent>
#include <QInputDialog>
#include <pcbsd-netif.h>
#include <pcbsd-utils.h>

#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>

#include "backend.h"
#include "ui_firstboot.h"
#include "firstboot.h"
#include "helpText.h"

Installer::Installer(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    translator = new QTranslator();

    connect(backButton, SIGNAL(clicked()), this, SLOT(slotBack()));
    connect(nextButton, SIGNAL(clicked()), this, SLOT(slotNext()));

    connect(helpButton, SIGNAL(clicked()), this, SLOT(slotHelp()));
    connect(pushTouchKeyboard, SIGNAL(clicked()), this, SLOT(slotPushVirtKeyboard()));
    connect(pushChangeKeyLayout, SIGNAL(clicked()), this, SLOT(slotPushKeyLayout()));

    connect(lineHostname,SIGNAL(textChanged(const QString)),this,SLOT(slotCheckHost()));

    connect(lineRootPW, SIGNAL(textChanged ( const QString &)), this, SLOT(slotCheckRootPW()));
    connect(lineRootPW2, SIGNAL(textChanged ( const QString &)), this, SLOT(slotCheckRootPW()));

    connect(lineName,SIGNAL(textChanged(const QString)),this,SLOT(slotCheckUser()));
    connect(lineName,SIGNAL(editingFinished()),this,SLOT(slotSuggestUsername()));
    connect(lineUsername,SIGNAL(textChanged(const QString)),this,SLOT(slotCheckUser()));
    connect(linePW,SIGNAL(textChanged(const QString)),this,SLOT(slotCheckUser()));
    connect(linePW2,SIGNAL(textChanged(const QString)),this,SLOT(slotCheckUser()));
    connect(line_PCpass, SIGNAL(testChanged(const QString)), this, SLOT(slotCheckUser())) ;
    connect(line_PCpass_repeat, SIGNAL(testChanged(const QString)), this, SLOT(slotCheckUser())) ;
    connect(push_PC_device, SIGNAL(clicked()), this, SLOT(slotGetPCDevice()) );
    
    backButton->setText(tr("&Back"));
    nextButton->setText(tr("&Next"));


    // Load the keyboard info
    keyModels = Scripts::Backend::keyModels();
    keyLayouts = Scripts::Backend::keyLayouts();

    // If we have a saved keyboard layout from installation, use it first
    QString kD;
    if ( QFile::exists("/var/.wizardKeyboard") ) {
      QFile kFile("/var/.wizardKeyboard");
      if ( kFile.open(QIODevice::ReadOnly | QIODevice::Text) ) {
         kD = kFile.readLine().simplified();
         kFile.close();
         
         kbMod = kD.section(" ", 0, 0);
         kbLay = kD.section(" ", 1, 1);
         kbVar = kD.section(" ", 2, 2);
	 Scripts::Backend::changeKbMap(kbMod, kbLay, kbVar);
      } 
    }

    // Load the timezones
    comboBoxTimezone->clear();
    QString curZone = Scripts::Backend::guessTimezone();
    comboBoxTimezone->addItems(Scripts::Backend::timezones());
    if ( ! curZone.isEmpty() ) {
      int index = comboBoxTimezone->findText(curZone, Qt::MatchStartsWith);
      if (index != -1) {
         comboBoxTimezone->setCurrentIndex(index);
      } else {
        // Set America/New_York to default
        index = comboBoxTimezone->findText("America/New_York", Qt::MatchStartsWith);
        if (index != -1)
           comboBoxTimezone->setCurrentIndex(index);
      }
    } else {
      // Set America/New_York to default
      int index = comboBoxTimezone->findText("America/New_York", Qt::MatchStartsWith);
      if (index != -1)
         comboBoxTimezone->setCurrentIndex(index);
    }

    // Load the hostname
    lineHostname->setText(pcbsd::Utils::getConfFileValue("/etc/rc.conf", "hostname=", 1));

    // Start on the first screen
    installStackWidget->setCurrentIndex(0);
    backButton->setVisible(false);

    // Update the status bar
    // This makes the status text more "visible" instead of using the blue background
    statusBar()->setStyleSheet("background: white");
}

Installer::~Installer()
{
    //delete ui;
}

void Installer::slotSuggestUsername()
{
  if ( ! lineUsername->text().isEmpty() || lineName->text().isEmpty() )
    return;
  QString name;
  if ( lineName->text().indexOf(" ") != -1 ) {
    name = lineName->text().section(' ', 0, 0).toLower();
    name.truncate(1);
  }
  name = name + lineName->text().section(' ', -1, -1).toLower();
  lineUsername->setText(name);
  slotCheckUser();
}

void Installer::slotPushKeyLayout()
{
  wKey = new widgetKeyboard();
  wKey->programInit(keyModels, keyLayouts);
  wKey->setWindowModality(Qt::ApplicationModal);
  connect(wKey, SIGNAL(changedLayout(QString, QString, QString)), this, SLOT(slotKeyLayoutUpdated(QString, QString, QString)));
  wKey->show();
  wKey->raise();
}

// Start xvkbd
void Installer::slotPushVirtKeyboard()
{
   system("killall -9 xvkbd; xvkbd -compact &");
}

void Installer::initInstall()
{
    // load languages
    QString langCode;
    QString curLang;
    curLang = "en_US";
    bool foundLang = false;
    comboLanguage->clear();
    languages = Scripts::Backend::languages();
    if ( QFile::exists("/var/.wizardlang") ) {
      QFile lFile("/var/.wizardlang");
      if ( lFile.open(QIODevice::ReadOnly | QIODevice::Text) ) {
         curLang = lFile.readLine().simplified();
         lFile.close();
      } 
    } 

    qDebug() << curLang;

    // Now set languages
    for (int i=0; i < languages.count(); ++i) {
        QString languageStr = languages.at(i);
        QString language = languageStr.split("-").at(0);
        comboLanguage->addItem(language.trimmed());
    
        // Grab the language code
        langCode = languageStr;
        langCode.truncate(langCode.lastIndexOf(")"));
        langCode.remove(0, langCode.lastIndexOf("(") + 1);
        if ( curLang == langCode ) {
          comboLanguage->setCurrentIndex(i);
          foundLang = true;
        }
    }

    // If we found a language to default to
    if ( foundLang )
      slotChangeLanguage();

    connect(comboLanguage, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotChangeLanguage()));
}

void Installer::proceed(bool forward)
{
    int count = installStackWidget->count() - 1;
    int index = installStackWidget->currentIndex();

    index = forward ?
            (index == count ? count : index + 1) :
            (index == 0 ? 0 : index - 1);

    installStackWidget->setCurrentIndex(index);
}

// Check root pw
void Installer::slotCheckRootPW()
{
  nextButton->setEnabled(false);

  if ( lineRootPW->text().isEmpty() )
     return;
  if ( lineRootPW2->text().isEmpty() )
     return;
  if ( lineRootPW->text() != lineRootPW2->text() )
     return;
  // if we get this far, all the fields are filled in
  nextButton->setEnabled(true);
}

void Installer::slotCheckHost()
{
  QRegExp hostnameRegExp("^(([a-zA-Z0-9][a-zA-Z0-9-].*[a-zA-Z0-9])|([a-zA-Z0-9]+))$");
  nextButton->setEnabled(false);
  if (lineHostname->text().isEmpty())
  {
     lineHostname->setToolTip(tr("Please enter a hostname"));
     return;
  } 
  else if (hostnameRegExp.indexIn(lineHostname->text()) == -1)
  {
     lineHostname->setToolTip(tr("Hostname may only contain letters and numbers"));
     return;
  }
  nextButton->setEnabled(true);
}

void Installer::slotCheckUser()
{
  nextButton->setEnabled(false);
  if ( lineName->text().isEmpty() )
     return;
  if ( lineUsername->text().isEmpty() )
     return;
  if ( lineUsername->text().contains(" ") )
     return;
  else if( getpwnam(lineUsername->text().toUtf8()) != NULL )
     return;
  if ( linePW->text().isEmpty() )
     return;
  if ( linePW2->text().isEmpty() )
     return;
  if ( linePW->text() != linePW2->text() )
     return;
  if(group_usePC->isChecked()){
    if(line_PCpass->text().isEmpty() || line_PCpass_repeat->text().isEmpty())
       return;
    if(line_PCpass->text() != line_PCpass_repeat->text())
      return;
    if(push_PC_device->whatsThis().isEmpty())
      return;
    if( !QFile::exists("/dev/"+push_PC_device->whatsThis()) ){
      push_PC_device->setWhatsThis(""); //clear this for later
      push_PC_device->setText(tr("Select")); //reset back to initial text
      return;
    }
  }
  
  nextButton->setEnabled(true);
}

// Slot which is called when the Finish button is clicked
void Installer::slotFinished()
{
  qApp->quit();
}

void Installer::slotNext()
{
   QString tmp;
   backButton->setVisible(true);

   // Check rootPW match
   if ( installStackWidget->currentIndex() == 1)
     slotCheckRootPW();
   // Check user info
   if ( installStackWidget->currentIndex() == 2)
     slotCheckUser();

   // Check if we have a wireless device
   if ( installStackWidget->currentIndex() == 3) {
     if ( system("ifconfig wlan0") == 0 ) {
       haveWifi = true;
       QTimer::singleShot(50,this,SLOT(slotScanNetwork()));
       connect(pushButtonRescan, SIGNAL(clicked()), this, SLOT(slotScanNetwork()));
       connect(listWidgetWifi, SIGNAL(itemPressed(QListWidgetItem *)), this, SLOT(slotAddNewWifi()));
     } else {
       haveWifi = false;
     }
   }

   // If not doing a wireless connection
   if ( installStackWidget->currentIndex() == 3 && ! haveWifi) {
      installStackWidget->setCurrentIndex(5);
      // Save the settings
      saveSettings();
      nextButton->setText(tr("&Finish"));
      backButton->setVisible(false);
      nextButton->disconnect();
      connect(nextButton, SIGNAL(clicked()), this, SLOT(slotFinished()));
      return;
   }

   // Finished screen
   if ( installStackWidget->currentIndex() == 4 ) {
      // Save the settings
      saveSettings();
      nextButton->setText(tr("&Finish"));
      backButton->setVisible(false);
      nextButton->disconnect();
      connect(nextButton, SIGNAL(clicked()), this, SLOT(slotFinished()));
   }

   proceed(true);
}

void Installer::slotBack()
{
   nextButton->setEnabled(true);

   if ( installStackWidget->currentIndex() == 1 )
     backButton->setVisible(false);
   else
     backButton->setVisible(true);

   proceed(false);
}

void Installer::slotChangeLanguage()
{
    if ( comboLanguage->currentIndex() == -1 )
      return;

    // Figure out the language code
    QString langCode = languages.at(comboLanguage->currentIndex());
    
    // Grab the language code
    langCode.truncate(langCode.lastIndexOf(")"));
    langCode.remove(0, langCode.lastIndexOf("(") + 1); 

    // Check what directory our app is in
    QString appDir;
    if ( QFile::exists("/usr/local/bin/pc-sysinstaller") )
      appDir = "/usr/local/share/pcbsd";
    else
      appDir = QCoreApplication::applicationDirPath();
    
    //QTranslator *translator = new QTranslator();
    qDebug() << "Remove the translator";
    if ( ! translator->isEmpty() )
      QCoreApplication::removeTranslator(translator);
    
    if (translator->load( QString("FirstBoot_") + langCode, appDir + "/i18n/" )) {
      qDebug() << "Load new Translator" << langCode;
      QCoreApplication::installTranslator(translator);
      this->retranslateUi(this);
    }

}

void Installer::changeLang(QString code)
{
   // Change the language in the combobox with the current running one
   comboLanguage->disconnect();

   for (int i=0; i < languages.count(); ++i) {
      if ( languages.at(i).indexOf("(" + code + ")" ) != -1 ) {
        comboLanguage->setCurrentIndex(i); 
      }
   }

   connect(comboLanguage, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotChangeLanguage()));
}

void Installer::slotHelp()
{
	pcHelp = new dialogHelp();
	switch (installStackWidget->currentIndex()) {
	case 0:
		pcHelp->dialogInit(HELPTEXT0);
		break;
	case 1:
		pcHelp->dialogInit(HELPTEXT1);
		break;
	case 2:
		pcHelp->dialogInit(HELPTEXT2);
		break;
	case 3:
		pcHelp->dialogInit(HELPTEXT3);
		break;
	case 4:
		pcHelp->dialogInit(HELPTEXT4);
		break;
	case 5:
		pcHelp->dialogInit(HELPTEXT5);
		break;
	default:
		pcHelp->dialogInit("Missing help file...");
		break;
	}
	pcHelp->show();
}

void Installer::slotScanNetwork()
{
  QString strength, ssid, security, FileLoad;
  QStringList ifconfout, ifline;
  int foundItem = 0;

  // Clear the list box and disable the add button
  listWidgetWifi->clear();

  // Start the scan and get the output
  //ifconfout = pcbsd::Utils::runShellCommand("ifconfig -v wlan0 up list scan");
  ifconfout = pcbsd::Utils::runShellCommand("ifconfig -v wlan0 list scan");
  
  qDebug() << ifconfout;

  //display the info for each wifi access point
  for(int i=1; i<ifconfout.size(); i++){    //Skip the header line by starting at 1
    ifline = NetworkInterface::parseWifiScanLine(ifconfout[i],true); //get a single line
    //save the info for this wifi
    ssid = ifline[0];
    strength = ifline[4];
    //determine the icon based on if there is security encryption
    security = ifline[6]; //NetworkInterface::getWifiSecurity(ssid,DeviceName);
    if(security.contains("None")){
      FileLoad = ":/modules/images/object-unlocked.png";
    }else{
      FileLoad = ":/modules/images/object-locked.png";
    }
    //Add the wifi access point to the list
    listWidgetWifi->addItem(new QListWidgetItem(QIcon(FileLoad), ssid + " (signal: " +strength + ")") );
    foundItem = 1; //set the flag for wifi signals found
  }
   
  if ( foundItem == 1 )
    listWidgetWifi->setCurrentRow(-1);
}

void Installer::slotAddNewWifi()
{
  if ((listWidgetWifi->currentRow()) ==-1 )
     return;

  QString line = listWidgetWifi->item(listWidgetWifi->currentRow())->text();
  QString ssidc = line.section(" (",0,0,QString::SectionSkipEmpty);
  addNetworkProfile(ssidc);
}

void Installer::addNetworkProfile(QString ssid)
{
  //get the full SSID string
  QString dat = pcbsd::Utils::runShellCommandSearch("ifconfig -v wlan0 list scan",ssid);
  QStringList wdat = NetworkInterface::parseWifiScanLine(dat,true);
  QString SSID = wdat[0];
 
  //Get the Security Type
  QString sectype = wdat[6];
 
  if(sectype == "None"){
    //run the Quick-Connect slot without a key
    slotQuickConnect("",SSID);
  }else{
    //Open the dialog to prompt for the Network Security Key
    dialogNetKey = new netKey();
    //Insert the SSID into the dialog
    dialogNetKey->setSSID(SSID);
    //connect the signal from the dialog to the quick-connect slot
    connect(dialogNetKey,SIGNAL( saved(QString,QString) ),this,SLOT( slotQuickConnect(QString,QString) ) );
    //Activate the dialog
    dialogNetKey->exec();
  }
}

void Installer::slotQuickConnect(QString key,QString SSID){
 
  // Run the wifiQuickConnect function
  NetworkInterface::wifiQuickConnect(SSID,key,"wlan0");
 
  // Move to finish screen
  installStackWidget->setCurrentIndex(5);

  // Save the settings
  saveSettings();
  nextButton->setText(tr("&Finish"));
  backButton->setVisible(false);
  nextButton->disconnect();
  connect(nextButton, SIGNAL(clicked()), this, SLOT(slotFinished()));
}

void Installer::slotGetPCDevice(){
  //This will find any personacrypt capable devices
  QStringList devs = pcbsd::Utils::runShellCommand("personacrypt list -r");
  if(devs.isEmpty()){
    QMessageBox::warning(this, tr("No Devices Found"), tr("Please connect a removable device and try again") );
     return;
  }
  bool ok = false;
  QString device = QInputDialog::getItem(this, tr("Select Removable Device"), tr("Warning: Any existing data on the selected device will be deleted during the user creation process."), devs, 0, false, &ok);
  if(!ok || device.isEmpty()){ return; } //cancelled
  push_PC_device->setText(device.section(":",0,0));
  push_PC_device->setWhatsThis(device.section(":",0,0)); //save the device ID here for later use
  slotCheckUser(); //Update the UI
}

void Installer::saveSettings()
{
  // Check if we need to change the language
  if ( comboLanguage->currentIndex() != 0 ) {
    QString lang = languages.at(comboLanguage->currentIndex());
    // Grab the language code
    lang.truncate(lang.lastIndexOf(")"));
    lang.remove(0, lang.lastIndexOf("(") + 1);
    qDebug() << "Set Lang:" << lang;
    system("/usr/local/share/pcbsd/scripts/set-lang.sh " + lang.toLatin1());
  }

  // Set the timezone
  QString tz = comboBoxTimezone->currentText();
  if ( tz.indexOf(":") != -1 )
   tz = tz.section(":", 0, 0);
  system("cp /usr/share/zoneinfo/" + tz.toLatin1() + " /etc/localtime");

  // Using UTC bios?
  if ( checkUTC->isChecked() )
     system("rm /etc/wall_cmos_clock >/dev/null 2>/dev/null");
  else
     system("touch /etc/wall_cmos_clock");

  // Set the root PW
  QTemporaryFile rfile("/tmp/.XXXXXXXX");
  if ( rfile.open() ) {
    QTextStream stream( &rfile );
      stream << lineRootPW->text();
    rfile.close();
  }
  system("cat " + rfile.fileName().toLatin1() + " | pw usermod root -h 0 ");
  rfile.remove();

  // Create the new home-directory
  system("/usr/local/share/pcbsd/scripts/mkzfsdir.sh /usr/home/" + lineUsername->text().toLatin1() );

  // Create the new username
  QTemporaryFile ufile("/tmp/.XXXXXXXX");
  if ( ufile.open() ) {
    QTextStream stream( &ufile );
      stream << linePW->text();
    ufile.close();
  }
  QString userCmd = " | pw useradd -n \"" + lineUsername->text() + "\" -u "+spin_UID->cleanText()+" -c \"" + lineName->text() + "\" -h 0 -s \"/bin/csh\" -m -d \"/usr/home/" + lineUsername->text() + "\" -G \"wheel,operator\"";
  system("cat " + ufile.fileName().toLatin1() + userCmd.toLatin1());
  ufile.remove();

  // Sync after adding the user
  sync();
  
  //Create the personacrypt device (if selected)
  if(group_usePC->isChecked()){
    QTemporaryFile tmpfile("/tmp/.XXXXXXXXXXXXXXXXX");
    if( tmpfile.open() ){
      QTextStream ostream(&tmpfile);
	ostream << line_PCpass->text();
      tmpfile.close();
      QProcess::execute("personacrypt init \""+lineUsername->text()+"\" \""+tmpfile.fileName()+"\" "+push_PC_device->whatsThis());
    }
  } 

  // Enable Flash for the new user
  QProcess::execute("su", QStringList() << lineUsername->text() << "-c" << "/usr/local/bin/flashpluginctl on" );
  
  // Do we need to change the system hostname?
  if ( lineHostname->text() != pcbsd::Utils::getConfFileValue("/etc/rc.conf", "hostname=", 1) )
  {
      pcbsd::Utils::setConfFileValue("/etc/rc.conf", "hostname=", "hostname=\"" + lineHostname->text() + "\"", -1);
      pcbsd::Utils::setConfFileValue("/etc/hosts", "::1", "::1\t\t\tlocalhost localhost.localdomain " + lineHostname->text() + ".localhost " + lineHostname->text(), -1);
      pcbsd::Utils::setConfFileValue("/etc/hosts", "127.0.0.1", "127.0.0.1\t\tlocalhost localhost.localdomain " + lineHostname->text() + ".localhost " + lineHostname->text(), -1);

      // Now set the hostname on the system
      sethostname(lineHostname->text().toLatin1(), lineHostname->text().length());
  }


  // Save the PCDM default lang / inputs
  QString curLang;
  if ( comboLanguage->currentIndex() != -1 ) {
    // Figure out the language code
    curLang = languages.at(comboLanguage->currentIndex());
          
    // Grab the language code
    curLang.truncate(curLang.lastIndexOf(")"));
    curLang.remove(0, curLang.lastIndexOf("(") + 1);
  }

  system("mkdir -p /var/db/pcdm 2>/dev/null");
  QFile pcdmfile("/var/db/pcdm/defaultInputs");
  if (pcdmfile.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream out(&pcdmfile);
    out << "Lang=" << curLang << "\n";

    if ( kbMod.isEmpty() )
      out << "KeyModel=pc104\n";
    else
      out << "KeyModel=" << kbMod << "\n";

    if ( kbLay.isEmpty() )
      out << "KeyLayout=us\n";
    else
      out << "KeyLayout=" << kbLay << "\n";

    if ( kbVar.isEmpty() )
      out << "KeyVariant=";
    else
      out << "KeyVariant=" << kbVar;

    pcdmfile.close();
  } else {
    qDebug() << "Error opening /var/db/pcdm/defaultInputs";
  }
}

void Installer::slotKeyLayoutUpdated(QString mod, QString lay, QString var)
{
  kbMod = mod;
  kbLay = lay;
  kbVar = var;
}

void Installer::closeEvent(QCloseEvent *event)
{
   event->ignore();
}
