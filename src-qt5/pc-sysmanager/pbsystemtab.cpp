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
    username = QString::fromLocal8Bit(getenv("SUDO_USER")); //since the app is always run as root with sudo
    if(username.isEmpty()){ username = QString::fromLocal8Bit(getenv("LOGNAME")); } //in case SUDO_USER is not set
    //qDebug() << "Username:" << username;
    // Set the Uname on the General Tab
    CheckUname();
    // Set the PC-BSD Version on the General Tab
    CheckPBVer();

    //Get & Set CPU Type
    labelCPU->setText(pcbsd::Utils::sysctl("hw.model").simplified());
    
    //Get & Set RAM
    labelMemory->setText(pcbsd::Utils::bytesToHumanReadable(pcbsd::Utils::sysctlAsInt("hw.physmem")));

    //Load Misc tab data
    loadMiscData();


    // Connect our various buttons
    connect(buttonGenerate, SIGNAL(clicked()), this, SLOT(startGenerateSheet()) );

    connect(pushMiscSave, SIGNAL( clicked() ), this, SLOT( slotMiscSave() ) );

    connect(fetchSourceBut, SIGNAL( clicked() ), this, SLOT( fetchSourcePressed() ) );
    connect(fetchPortsBut, SIGNAL( clicked() ), this, SLOT( fetchPortsPressed() ) );
    
    cmdDlg = new CMDDialog(this);
    cmdDlg->hide();
}

void PBSystemTab::CheckUname()
{
	QString line = Backend::shortCMD("","uname -rm").join("").simplified();
	labelBASEVer->setText( line );
	//Save the output version/architecture internally
        Version= line.section(" ",0,0);
	Arch = line.section(" ",1,1);
}


void PBSystemTab::CheckPBVer()
{
 
  //Get pcbsd-base version
  QString base = pcbsd::Utils::runShellCommand("pkg info -f pcbsd-base").filter("Version").join("").section(":",1,1).simplified();
  if(base.isEmpty()){ base ="UNKNOWN"; }
  label_pcbsdbase->setText(base);
  //Get pcbsd-utils version
  QString utils = pcbsd::Utils::runShellCommand("pkg info -f pcbsd-utils").filter("Version").join("").section(":",1,1).simplified();
  if(utils.isEmpty()){ utils ="UNKNOWN"; }
  label_pcbsdutils->setText(utils);
  //Get pcbsd-utils-qt4 version
  QString utilsqt4 = pcbsd::Utils::runShellCommand("pkg info -f pcbsd-utils-qt5").filter("Version").join("").section(":",1,1).simplified();
  if(utilsqt4.isEmpty()){ utilsqt4 ="UNKNOWN"; }
  label_pcbsdgutils->setText(utilsqt4);

}

// Ask user where to save text file, then start the generation
void PBSystemTab::startGenerateSheet()
{
    SheetFileName="";
        SheetFileName = QFileDialog::getSaveFileName(
                    this,
                    "save file dialog",
                    "/home/" + username + "/Desktop/systemDiagnostic.txt",
                    "Text File (*.txt)");
	
      if ( ! SheetFileName.isEmpty() ){
        if ( !SheetFileName.endsWith(".txt") ){ SheetFileName.append(".txt");  }
	if ( QFile::exists(SheetFileName ) ){
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
	qDebug() << "Generating System Diagnostic Sheet";
    	SheetGenScript = new QProcess( this );
	QString prog = PREFIX + "/share/pcbsd/scripts/GenDiagSheet.sh";
	QStringList args;
	args << SheetFileName;
	args << username;
	//qDebug() << "CMD: " << prog+" "+args.join(" ");
	connect( SheetGenScript, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finishedSheet()) );
	//Make sure the diagnostic generation script is executable
	QFileInfo info(prog);
	if(!info.isExecutable()){
	  QFile::setPermissions(prog, info.permissions() | QFile::ExeOther); //executable by anyone
	}
	SheetGenScript->start(prog, args);
}

void PBSystemTab::finishedSheet()
{
	int exitcode = SheetGenScript->exitCode();
	qDebug() << "Diagnostic Sheet Return Code:" << QString::number(exitcode);
	if(exitcode != 0 || !QFile::exists(SheetFileName) ){
	  QMessageBox::warning(this, tr("Error"), tr("Error Generating Diagnostic File"));
	}else{
	  QMessageBox::information(this, tr("Success"), QString(tr("Diagnostic File Created: %1")).arg(SheetFileName));
	}
		
}

void PBSystemTab::showRebootRequired()
{
    QMessageBox::information( this, "PC-BSD Notification",
    "Settings Updated!\n You must reboot for changes to take effect!" );
}

// Checks the file for a string KEY, and sees if its set to VALUE
bool PBSystemTab::checkValue( QString File, QString Key, QString Value )
{
    QString theValue = pcbsd::Utils::getConfFileValue(File, Key);
    if (theValue == Value) {
	return true;
    }
    return false;
    
}

void PBSystemTab::fetchSourcePressed()
{
    if(cmdDlg->isRunning() ){
      QMessageBox::warning(this, tr("Process Already Running"), tr("You already have a process running. Please wait for that one to finish first.") );
    }else{
      //Get the current version branch of the source tree
      QString version = pcbsd::Utils::runShellCommand("uname -r").join(" ");
      QString branch;
      if(version.contains("RELEASE")){ branch = "releng/"+version.section("-",0,0).simplified(); }
      else if(version.contains("STABLE")){ branch = "stable/"+version.section("-",0,0).section(".",0,0).simplified(); }
      else{ branch = "master"; } //CURRENT
      
      cmdDlg->start("source", branch); //Version not implemented yet
      cmdDlg->show();
    }
}

void PBSystemTab::fetchPortsPressed()
{
    if(cmdDlg->isRunning() ){
      QMessageBox::warning(this, tr("Process Already Running"), tr("You already have a process running. Please wait for that one to finish first.") );
    }else{
      cmdDlg->start("ports"); //Version not implemented yet
      cmdDlg->show();
    }
}


void PBSystemTab::loadMiscData()
{
    //Check if IBUS input is forced
    if ( pcbsd::Utils::getConfFileValue(QString(PREFIX + "/share/pcbsd/xstartup/enable-ibus.sh"), QString("FORCEIBUS=") ) == QString("YES")){
	checkForceIbus->setChecked(true);
    }else{
	checkForceIbus->setChecked(false);
    }
}

void PBSystemTab::slotMiscSave() {
     //Save IBUS setting
     if ( checkForceIbus->isChecked() ){
	pcbsd::Utils::setConfFileValue(PREFIX + "/share/pcbsd/xstartup/enable-ibus.sh", "FORCEIBUS", "FORCEIBUS=\"YES\"", 1);
     }else{
	pcbsd::Utils::setConfFileValue(PREFIX + "/share/pcbsd/xstartup/enable-ibus.sh", "FORCEIBUS", "FORCEIBUS=\"NO\"", 1);
     }
}

void PBSystemTab::slotClose() {
    close();
}

void PBSystemTab::changeOpenTab(int tab)
{
   tabWidget->setCurrentIndex(tab);
}

void PBSystemTab::closeEvent(QCloseEvent *event){
    if(cmdDlg->isRunning()){
      //Process Running - minimize the main window instead
      event->ignore();
      this->showMinimized();
    }
}
