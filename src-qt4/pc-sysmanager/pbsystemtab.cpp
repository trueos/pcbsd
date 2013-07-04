/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
#include <qstring.h>
#include <pcbsd-utils.h>
#include <qfiledialog.h>
#include <QTextStream>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QProgressDialog>
#include <QDebug>
#include "../config.h"

#include "pbsystemtab.h"

void PBSystemTab::ProgramInit()
{
    //Grab the username
    username = QString::fromLocal8Bit(getenv("LOGNAME"));
    
    // Set the Uname on the General Tab
    CheckUname();
    // Set the PC-BSD Version on the General Tab
    CheckPBVer();

    // See if we need to setup a proxy
    checkProxy();
    
    //Get & Set CPU Type
    labelCPU->setText(pcbsd::Utils::sysctl("hw.model").simplified());
    
    //Get & Set RAM
    labelMemory->setText(pcbsd::Utils::bytesToHumanReadable(pcbsd::Utils::sysctlAsInt("hw.physmem")));
    
    // Read any kernel settings
    LoadKernSettings();

    // Read any rc.conf settings
    //loadServSettings();

    //Load boot screen data
    loadBootData();

    // Load the mirror configuration
    loadMirrorConfig();
  
    // Connect our various buttons
    connect(buttonGenerate, SIGNAL(clicked()), this, SLOT(startGenerateSheet()) );
    connect(showBootCheck, SIGNAL(clicked()), this, SIGNAL(changed()));
    connect(splashSelect, SIGNAL(activated(int)), this, SIGNAL(changed()));
    connect(pushRefreshList, SIGNAL( clicked() ), this, SLOT( slotUpdateMirrorList() ) );
    connect(pushMirrorSave, SIGNAL( clicked() ), this, SLOT( slotMirrorSave() ) );
    connect(pushMiscSave, SIGNAL( clicked() ), this, SLOT( slotMiscSave() ) );

    connect(fetchSourceBut, SIGNAL( clicked() ), this, SLOT( fetchSourcePressed() ) );
    connect(fetchPortsBut, SIGNAL( clicked() ), this, SLOT( fetchPortsPressed() ) );

    // Connect the mirror radio buttons, so we can enable disable objects based on status
    connect(radioAutoMirror, SIGNAL( clicked() ), this, SLOT( slotCheckMirrorRadio() ) );
    connect(radioSelectMirror, SIGNAL( clicked() ), this, SLOT( slotCheckMirrorRadio() ) );
    connect(radioCustomMirror, SIGNAL( clicked() ), this, SLOT( slotCheckMirrorRadio() ) );

}

void PBSystemTab::loadMirrorConfig()
{

  QSettings settings("PCBSD");
  QString tmp;

  // Get the current mirror
  currentMirror = pcbsd::Utils::getMasterMirror();

  // Load our array of mirrors
  comboMirrorList->clear();
  bool foundMirror = false;
  bool usingMirrorList = false;
  QString MirrorName, MirrorURL;

  int size = settings.beginReadArray("/PC-BSD/SoftwareManager/mirrorList");
  for (int i = 0; i < size; ++i) {
     foundMirror = true;
     settings.setArrayIndex(i);
     MirrorName = settings.value("MirrorName").toString();
     MirrorURL = settings.value("MirrorURL").toString();
     comboMirrorList->addItem( MirrorName );
     mirrorNames[i] = MirrorName;
     mirrorURLs[i] = MirrorURL;
     if ( currentMirror == MirrorURL ) {
       comboMirrorList->setCurrentIndex(i);
       radioSelectMirror->setChecked(true);
       usingMirrorList = true;
     }
  }
                
  settings.endArray();

  // If our mirror list is empty, start a refresh of it now
  if (!foundMirror)
     slotUpdateMirrorList();
  
  // Check if we found a mirror in the PC-BSD list, otherwise default to custom mirror
  if (!usingMirrorList) {
      if ( currentMirror.isEmpty() )
        radioAutoMirror->setChecked(true);
      else
        radioCustomMirror->setChecked(true);
      lineCustomMirror->setText(currentMirror);
  }

  slotCheckMirrorRadio();
}

void PBSystemTab::CheckUname()
{
	QString prog = "uname";
        QStringList args;  
	args << "-rm"; 
		
	GetUname = new QProcess( this );
	connect( GetUname, SIGNAL(readyReadStandardOutput()), this, SLOT(ReadUname()) );
	GetUname->start(prog, args);
}


void PBSystemTab::CheckPBVer()
{
 
 QString pcVer="";
 QStringList out = pcbsd::Utils::runShellCommand(QString("pkg info -f pcbsd-base"));
 if (out.size()) {
   for (int i=0; i<out.size(); i++)
   {
     if (out[i].contains("Version"))
     {
         pcVer = out[i];
         pcVer.replace("Version        : ","");
         labelPCBSD->setText(pcVer);
         break;
     }
   }
 }


 Arch = getLineFromCommandOutput("uname -m");
 
}


void PBSystemTab::ReadUname()
{    
    if (GetUname->canReadLine() )
    {
        QString uname = GetUname->readLine().simplified();
        labelBASEVer->setText( uname );
        Version= uname.section(" ",0,0);
    }
}


// Ask user where to save text file, then start the generation
void PBSystemTab::startGenerateSheet()
{
    SheetFileName="";
        SheetFileName = QFileDialog::getSaveFileName(
                    this,
                    "save file dialog",
                    "/home/" + username + "/Desktop",
                    "Text File (*.txt)");
	
      if ( ! SheetFileName.isEmpty() )
     {
	  if ( SheetFileName.indexOf(".txt", 0) == -1)
	  {
	      SheetFileName.append(".txt");
	  }
	if ( QFile::exists(SheetFileName ) )
	{
	    int answer = QMessageBox::warning( this, "Overwrite File", "Overwrite " + SheetFileName + "?", "&Yes", "&No", QString::null, 1, 1 );
                   if ( answer == 0 ) {
	      CreateSheetFile();		
	    }
	    
	} else {
	    CreateSheetFile();  
               }
     }
	

}


void PBSystemTab::CreateSheetFile()
{
    
    	SheetGenScript = new QProcess( this );
	QString prog = PREFIX + "/share/pcbsd/scripts/GenDiagSheet.sh";
	QStringList args;
	args << SheetFileName;
	connect( SheetGenScript, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finishedSheet()) );
		
	SheetGenScript->start(prog, args);
}


void PBSystemTab::finishedSheet()
{
	ViewSheet = new QProcess( this );
	QString prog = "kedit";
	QStringList args;
	args << SheetFileName;
	ViewSheet->start(prog, args);
}


void PBSystemTab::saveKernScreen()
{
     if ( checkForceIbus->isChecked() )
     		pcbsd::Utils::setConfFileValue(PREFIX + "/share/pcbsd/xstartup/enable-ibus.sh", "FORCEIBUS", "FORCEIBUS=\"YES\"", 1);
     else
     		pcbsd::Utils::setConfFileValue(PREFIX + "/share/pcbsd/xstartup/enable-ibus.sh", "FORCEIBUS", "FORCEIBUS=\"NO\"", 1);

}



void PBSystemTab::showRebootRequired()
{
    QMessageBox::information( this, "PC-BSD Notification",
    "Settings Updated!\n You must reboot for changes to take effect!" );
}


// Read the loader.conf, and load any settings we need
void PBSystemTab::LoadKernSettings()
{
    if ( checkValue("/boot/loader.conf", "splash_pcx_load=", "YES" ) ) { showBootCheck->setChecked(true); }
    else { showBootCheck->setChecked(false); }
}

// Checks the file for a string KEY, and sees if its set to VALUE
bool PBSystemTab::checkValue( QString File, QString Key, QString Value )
{
    QString theValue = pcbsd::Utils::getConfFileValue(File, Key);
    if (theValue == Value) {
	return TRUE;
    }
    return FALSE;
    
}

void PBSystemTab::fetchSourcePressed()
{
    portsnapUI = new PortsnapProgress();
    portsnapUI->init(false, Version);
    portsnapUI->show();
}

void PBSystemTab::fetchPortsPressed()
{
    portsnapUI = new PortsnapProgress();
    portsnapUI->init(true, Version);
    portsnapUI->show();
}

void PBSystemTab::customSplashPressed()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Select Splash"), "/home/" + username, "*.pcx");
    if (file == "") { return; }
    
    QImage image(file, "PCX");
    
    if (image.isNull())
    {
	QMessageBox::warning(this, tr("Invalid Image"), tr("The file supplied does not appear to be a valid PCX image."));
	return;
    }
    if ((image.width() > 1024) || (image.height() > 768) || (image.depth() > 8))
    {
	QMessageBox::warning(this, tr("Invalid Image"), tr("The splash screen image cannot be any larger than 1024x768, or any greater than 8-bit (256 colours) depth."));
	return;
    }

    system("cp '" + file.toAscii() + "' /boot/loading-screen.pcx");
    splashSelect->clear();
    loadBootData();
    splashSelect->setCurrentIndex(0);
}


void PBSystemTab::miscSavePressed()
{
    //Change registry entry
    QSettings settings;
    QString code = codeMap[splashSelect->currentText()];
    settings.setValue("/PCBSD/splash-screen", code);
    
    //Move files about
    system("cp " + PREFIX.toAscii() + "/share/pcbsd/splash-screens/loading-screen-" + code.toAscii() + ".pcx /boot/loading-screen.pcx");
    system("chmod 644 /boot/loading-screen.pcx");
    
    if ( showBootCheck->isChecked() )
    {
	pcbsd::Utils::setConfFileValue("/boot/loader.conf", "splash_pcx_load=", "splash_pcx_load=\"YES\"" );
	pcbsd::Utils::setConfFileValue("/boot/loader.conf", "vesa_load=", "vesa_load=\"YES\"" );
	pcbsd::Utils::setConfFileValue("/boot/loader.conf", "bitmap_load=", "bitmap_load=\"YES\"" );
	pcbsd::Utils::setConfFileValue("/boot/loader.conf", "bitmap_name=", "bitmap_name=\"/boot/loading-screen.pcx\"" );
     } else {
	pcbsd::Utils::setConfFileValue("/boot/loader.conf", "splash_pcx_load=", "splash_pcx_load=\"NO\"" );
	pcbsd::Utils::setConfFileValue("/boot/loader.conf", "vesa_load=", "vesa_load=\"NO\"" );
	pcbsd::Utils::setConfFileValue("/boot/loader.conf", "bitmap_load=", "bitmap_load=\"NO\"" );
     }
}


void PBSystemTab::showSplashChecked(int newState)
{
    if (newState == 0)
    {
	splashSelect->setEnabled(false);
	customBut->setEnabled(false);
    }
    else
    {
	splashSelect->setEnabled(true);
	customBut->setEnabled(true);
    }
}


void PBSystemTab::loadBootData()
{
    //Trawl screens directory
    QDir screens = QDir(PREFIX + "/share/pcbsd/splash-screens/");
    if (! screens.exists())
    {
        QMessageBox::critical( 0, tr("Refresh Mirrors"), PREFIX + "/share/pcbsd/splash-screens/ " + tr("doesn't exist!"), QMessageBox::Ok );
	return;
    }
    screens.setFilter(QDir::Files);
    QStringList screensList = screens.entryList();
    
    QRegExp screenMatcher = QRegExp("^loading-screen-(([a-z]|[A-Z]|_)+)\\.pcx$");
    QStringList langList = QStringList();
    QLocale langResolver;
    codeMap = QMap<QString, QString>();
    bool customPresent = false;
    for (QStringList::Iterator it = screensList.begin(); it != screensList.end(); it++)
    {
	int pos  = screenMatcher.indexIn(*it);
	if (pos > -1)
	{
	    QString code = screenMatcher.cap(1);
	    if (code == "custom") { customPresent = true; }
	    else
	    { 
                QLocale lang(code);
		QString langName = langResolver.languageToString(lang.language());
                if ( langName.isEmpty() )
                {
		    langName = "Splash Screen: " + code;
                }
		langList += langName;
		codeMap[langName] = code;
	    }
	}
    }
    
    
    //Populate select box
    langList.sort();
    splashSelect->addItems(langList);

    int indexMod = 0;
    
    if (customPresent)
    {
	codeMap[tr("Custom")] = "custom";
	splashSelect->addItem(tr("Custom"), 0);
	indexMod = 1;
    }
    
    //Load current, get info from registry - /PCBSD/splash-screen
    QSettings settings;
    QString selectedLang = settings.value("/PCBSD/splash-screen", langResolver.language()).toString();
    
    if (selectedLang == "custom") { splashSelect->setCurrentIndex(0); }
    else
    {
	//If we don't have a splash for the user's language, default to english
	QLocale tlocale(selectedLang);
        QString testLangName = langResolver.languageToString(tlocale.language());
	if (codeMap[testLangName] == "") selectedLang = "en";

	QLocale tlocale2(selectedLang);
	selectedLang = langResolver.languageToString(tlocale2.language());
        if ( selectedLang.isEmpty() )
        {
	     selectedLang = "Splash Screen: " + selectedLang;
        }

	splashSelect->setCurrentIndex(langList.indexOf(selectedLang) + indexMod);
    }
    
    if ( checkValue("/boot/loader.conf", "splash_pcx_load=", "YES" ) )
        	showBootCheck->setChecked(TRUE);

    if ( pcbsd::Utils::getConfFileValue(QString(PREFIX + "/share/pcbsd/xstartup/enable-ibus.sh"), QString("FORCEIBUS=") ) == QString("YES"))
		checkForceIbus->setChecked(TRUE);
    else
		checkForceIbus->setChecked(FALSE);
}

void PBSystemTab::slotUpdateMirrorList() {
  // Fetch from here http://updates.pcbsd.org/mirror-list.php
  QString URL, LFILE;
  URL= "http://updates.pcbsd.org/mirror-list.php";

  mirrorJob = new QNetworkAccessManager(this);
  mirrorJobBuf.clear();

  QNetworkRequest netRequest;
  netRequest.setUrl(QUrl(URL));
  mirrorReply = mirrorJob->get(netRequest);
   
  connect(mirrorJob, SIGNAL(finished(QNetworkReply *)), this, SLOT(slotRefreshMirrorDone() ) );
  connect(mirrorReply, SIGNAL(readyRead()), this, SLOT(slotGetMirrorData() ) );
}

void PBSystemTab::slotGetMirrorData() {
        //while (mirrorReply->canReadLine() )
        //        mirrorJobBuf << mirrorReply->readLine().simplified();

        mirrorJobBuf = mirrorJobBuf + mirrorReply->readAll();
}

void PBSystemTab::slotRefreshMirrorDone() {
   QString MirrorName, MirrorURL, tmp;


   if (mirrorReply->error() != QNetworkReply::NoError)
   {
      QMessageBox::critical( 0, tr("Refresh Mirrors"), tr("Error fetching mirror list! Check your internet connection, or try again later."), QMessageBox::Ok );
        return;
   }

   QStringList mirrorList;
   mirrorList = mirrorJobBuf.split("\n");
   qDebug() << "Avail Mirrors:" << mirrorList;
   // Clear the combo box  
   comboMirrorList->clear();
   int i = 0;

   QString line;
   for ( int z = 0; z < mirrorList.size(); z++ ) {
     line = mirrorList.at(z);

     tmp = line;
     tmp.truncate(line.indexOf("=") -1 );
     MirrorName = tmp;
     MirrorURL = line.remove(0, line.indexOf("=") + 1);
     mirrorNames[i] = MirrorName;
     mirrorURLs[i] = MirrorURL.simplified();
     comboMirrorList->addItem( MirrorName );

     if ( currentMirror == MirrorURL )
        comboMirrorList->setCurrentIndex(i);

     i++;
   }

   // Close the array
   mirrorNames[i] = "";
   mirrorURLs[i] = "";

   QSettings settings("PCBSD");
   settings.beginWriteArray("/PC-BSD/SoftwareManager/mirrorList");
   i = 0;
   while ( !mirrorNames[i].isEmpty() )
   {
      settings.setArrayIndex(i);
      settings.setValue("MirrorName", mirrorNames[i]);
      settings.setValue("MirrorURL", mirrorURLs[i].simplified());
      i++;
   }

   settings.endArray(); 
}


void PBSystemTab::slotCheckMirrorRadio() {

    comboMirrorList->setEnabled(false);
    pushRefreshList->setEnabled(false);
    lineCustomMirror->setEnabled(false);
    if( radioSelectMirror->isChecked() )
    {
      comboMirrorList->setEnabled(true);
      pushRefreshList->setEnabled(true);
    }
    if( radioCustomMirror->isChecked() )
      lineCustomMirror->setEnabled(true);
}

void PBSystemTab::slotMiscSave() {
    miscSavePressed();
    saveKernScreen();
}

void PBSystemTab::slotMirrorSave() {
    saveMirrorConfig();
}

void PBSystemTab::saveMirrorConfig() {
    if( ! sanityCheckSettings() )
	return;


    if ( ! radioSelectMirror->isChecked() ) {
       pcbsd::Utils::setMasterMirror(lineCustomMirror->text());
    } else if (radioAutoMirror->isChecked() ) {
       pcbsd::Utils::setMasterMirror("");
    } else {
       // Check if we have a mirror selected and save it
       QString curMirror;
       int i = 0;
       while ( !mirrorNames[i].isEmpty() )
       {
          if ( comboMirrorList->currentText() == mirrorNames[i]) {
            curMirror = mirrorURLs[i];
	    break;
	  }
          i++;
       }
	
       pcbsd::Utils::setMasterMirror(curMirror);
    }
}

void PBSystemTab::slotClose() {
    close();
}

bool PBSystemTab::sanityCheckSettings()
{
  if (radioCustomMirror->isChecked() ) {
    if( lineCustomMirror->text().indexOf("http://") != 0 && lineCustomMirror->text().indexOf("ftp://") != 0 )
    {
      tabWidget->setCurrentIndex(1);
      QMessageBox::critical( 0, tr("Config Error"), tr("Error: Your custom mirror needs to begin with ftp:// or http://"), QMessageBox::Ok );
      return false;
    }
  }

  return true;
}


QString PBSystemTab::getLineFromCommandOutput( QString cmd )
{
        FILE *file = popen(cmd.toLatin1(),"r");
    
        char buffer[100];
 
        QString line = "";
        char firstChar;

        if ((firstChar = fgetc(file)) != -1){
                line += firstChar;
                line += fgets(buffer,100,file);
        }
        pclose(file);
        return line.simplified();
}

void PBSystemTab::changeOpenTab(int tab)
{
   tabWidget->setCurrentIndex(tab);
}

void PBSystemTab::checkProxy()
{
  bool ok;
  int port;

  // If no proxy set
  if ( pcbsd::Utils::getProxyURL().isEmpty() )
    return;

  QNetworkProxy proxy;
  if ( pcbsd::Utils::getProxyType() == "SOCKS5" )
    proxy.setType(QNetworkProxy::Socks5Proxy);
  else
    proxy.setType(QNetworkProxy::HttpProxy);

  proxy.setHostName(pcbsd::Utils::getProxyURL());

  port = pcbsd::Utils::getProxyPort().toInt(&ok);
  if ( ! pcbsd::Utils::getProxyPort().isEmpty() && ok )
    proxy.setPort(port);
  if ( ! pcbsd::Utils::getProxyUser().isEmpty() )
    proxy.setUser(pcbsd::Utils::getProxyUser());
  if ( ! pcbsd::Utils::getProxyPass().isEmpty() )
    proxy.setPassword(pcbsd::Utils::getProxyPass());

  QNetworkProxy::setApplicationProxy(proxy);
}
