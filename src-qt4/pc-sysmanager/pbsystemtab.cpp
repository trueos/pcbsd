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
    //LoadKernSettings();

    // Read any rc.conf settings
    //loadServSettings();

    //Load boot screen data
    loadBootData();

    // Load the default package set value
    if ( pcbsd::Utils::getValFromPCBSDConf("PACKAGE_SET") == "EDGE" ) {
       radioEdge->setChecked(true);
       radioProduction->setChecked(false);
    } else {
       radioProduction->setChecked(true);
       radioEdge->setChecked(false);
    }

    // Connect our various buttons
    connect(buttonGenerate, SIGNAL(clicked()), this, SLOT(startGenerateSheet()) );
    //connect(showBootCheck, SIGNAL(clicked()), this, SIGNAL(changed()));

    connect(pushMiscSave, SIGNAL( clicked() ), this, SLOT( slotMiscSave() ) );

    connect(fetchSourceBut, SIGNAL( clicked() ), this, SLOT( fetchSourcePressed() ) );
    connect(fetchPortsBut, SIGNAL( clicked() ), this, SLOT( fetchPortsPressed() ) );
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
/*void PBSystemTab::LoadKernSettings()
{
    if ( checkValue("/boot/loader.conf", "splash_pcx_load=", "YES" ) ) { showBootCheck->setChecked(true); }
    else { showBootCheck->setChecked(false); }
}
*/
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


void PBSystemTab::loadBootData()
{
    /*//Trawl screens directory
    QDir screens = QDir(PREFIX + "/share/pcbsd/splash-screens/");
    if (! screens.exists())
    {
        QMessageBox::critical( 0, tr("File not found!"), PREFIX + "/share/pcbsd/splash-screens/ " + tr("doesn't exist!"), QMessageBox::Ok );
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
*/
    if ( pcbsd::Utils::getConfFileValue(QString(PREFIX + "/share/pcbsd/xstartup/enable-ibus.sh"), QString("FORCEIBUS=") ) == QString("YES"))
		checkForceIbus->setChecked(TRUE);
    else
		checkForceIbus->setChecked(FALSE);
}

void PBSystemTab::slotMiscSave() {
    saveKernScreen();

    // Save package set
    if ( radioProduction->isChecked() )
      pcbsd::Utils::setValPCBSDConf("PACKAGE_SET", "PRODUCTION");
    else
      pcbsd::Utils::setValPCBSDConf("PACKAGE_SET", "EDGE");

    // Extract the ports overlay to grab a new package set repo config
    QProcess::execute(QString("pc-extractoverlay"), QStringList() << "ports");
}

void PBSystemTab::slotClose() {
    close();
}

bool PBSystemTab::sanityCheckSettings()
{
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
