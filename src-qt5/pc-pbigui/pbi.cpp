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
/****************************************************************************
*     Name: pc-pbigui
*  Version: 1.0 
*   Author: Kris Moore
*  License: BSD
*     Date: 8-02-10
*****************************************************************************/

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <qtextstream.h>
#include <qdir.h>
#include <qstring.h>
#include <qlist.h>
#include "pbi.h"
#include "ui_pbi.h"
#include <QDebug>
using namespace std;


void PBI::ProgramInit( QString pbiFile )
{
    // Confirm the PBI file specified exists
    PBIFILE = pbiFile;
    QFile pbiTest( PBIFILE );
    if ( ! pbiTest.exists() ) {
      QMessageBox::critical( this, tr("Error!"), tr("Cannot locate PBI file:") + " " + PBIFILE );
      exit(-1);
	
    }
    
    // Connect the page changed slot to proper signal
    connect( this, SIGNAL(currentIdChanged(int)), this, SLOT(PageChanged(int)) );
    connect( LicenseCheckBox, SIGNAL( clicked() ), this, SLOT(AgreeButtonSlot() ) );

    // Lets the app know if the file-extraction has begun
    ExtractStarted = false;
    TotalLinesTar = 0;
    
    // Set the fixed size
    setFixedSize(500,315);
    setMaximumWidth(500);
    setContentsMargins(0,0,0,0);

    OverWrote = 0;

    // Read in this programs configuration data
     readSettingsSlot();

     // Slot to check if this Program is already installed
     slotCheckExistingProgram();    

     // Check if we have any custom graphics for this PBI 
     checkCustomGraphics();
  
     // Check the digital signature on this file
     checkSignature();
     
}

// Function which checks the ProgList if we already have this app installed
void PBI::slotCheckExistingProgram(void)
{
  QString line;
  bool foundProgram = false;

  if (foundProgram) {
    switch( QMessageBox::warning( this, tr("Warning!"), tr("The program") + " (" + ProgramName + ") " + tr("is already installed. Overwrite it?"), tr("OK"), tr("Cancel"), 0, 0, 1 ) ) 
    {
      case 0: // The user clicked the OK button
      break;

      case 1: // The user clicked the cancel button
         exit(1);
      break;
    }


  }
}

void PBI::PageChanged( int pageId )
{
    // If no page, don't process
    if ( pageId == -1 )
       return;

    // Make sure we set our wizard style
    setWizardStyle(QWizard::ModernStyle);

    if ( pageId == 0 ) {
	button(QWizard::NextButton)->setEnabled(TRUE);
        QPixmap pixmap1;
	pixmap1.load(":/leftside.png");
        setPixmap(QWizard::WatermarkPixmap, pixmap1);

        QPixmap pixmap2;
        setPixmap(QWizard::BannerPixmap, pixmap2);

        if (QFile::exists( sideBannerFile ) ) {
	 pixmap1.load(sideBannerFile);
         setPixmap(QWizard::WatermarkPixmap, pixmap1);
        }
    } else {
        QPixmap pixmap1;
        setPixmap(QWizard::WatermarkPixmap, pixmap1);
        page(pageId)->setSubTitle(" ");
        setPixmap(QWizard::BannerPixmap, topbanner);
    }
    
    if ( pageId == 1 ) {
	 button(QWizard::NextButton)->setEnabled(FALSE);
         AgreeButtonSlot();
    }

    if ( pageId == 2 ) {
	button(QWizard::BackButton)->setEnabled(FALSE);
	button(QWizard::NextButton)->setEnabled(FALSE);
	button(QWizard::FinishButton)->setEnabled(FALSE);
        startInstallSlot();
    }
    
    if ( pageId == 3 ) {
	button(QWizard::BackButton)->setEnabled(FALSE);
	button(QWizard::BackButton)->setHidden(TRUE);
	button(QWizard::FinishButton)->setEnabled(TRUE);
	button(QWizard::CancelButton)->setHidden(TRUE);
    }
    
}


void PBI::readSettingsSlot()
{
    QString line, tmp;
 
    QProcess pbiConfig;
    pbiConfig.setProcessChannelMode(QProcess::MergedChannels);
    pbiConfig.start("pbi_add", QStringList() << "-g" << "-i" << PBIFILE);
    if (! pbiConfig.waitForFinished(-1) ) {
      QMessageBox::critical( this, tr("Error!"), tr("Error while running pbi_add!"));
      exit(-1);
    }

    while ( pbiConfig.canReadLine() ) {
       line = pbiConfig.readLine(); // line of text excluding '\n'
   
       // Check if this app needs root
       if (line.indexOf("RootInstall: ") == 0 ) {
 	 tmp = line.replace("RootInstall: ", "").simplified();
	 if ( tmp == "YES" ) {
           // Check we are running as root
           int uid = getuid();
           if (uid != 0 ) {
             QMessageBox::critical( this, tr("Error!"), tr("Error, This application must be installed as root!"));
	     exit(-1);
           }
         }
       }
     
    
       if ( line.indexOf("Name:" ) == 0)
 	 ProgramName = line.replace("Name: ", "").simplified();

       if ( line.indexOf("Version:") == 0)
 	  ProgramVer = line.replace("Version: ", "").simplified();

       if ( line.indexOf("Prefix:") == 0)
 	  ProgramDir = line.replace("Prefix: ", "").simplified();

       if ( line.indexOf("Author:") == 0)
	  ProgramAuthor = line.replace("Author: ", "").simplified();

       if ( line.indexOf("Website:") == 0)
	  ProgramWeb = line.replace("Website: ", "").simplified();

       if ( line.indexOf("Arch:") == 0)
	  ProgramArch = line.replace("Arch: ", "").simplified();

       if ( line.indexOf("FbsdVer:") == 0)
	  ProgramOS = line.replace("FbsdVer: ", "").simplified();

       if ( line.indexOf("CreatorVer:") == 0)
	  CreatorVer = line.replace("CreatorVer: ", "").simplified();

       if ( line.indexOf("Signature:") == 0)
	  ProgramSig = line.replace("Signature: ", "").simplified();

       if ( line.indexOf("ArchiveCount:") == 0) {
	  line = line.replace("ArchiveCount: ", "").simplified();
          TotalLinesTar = line.toInt();
       }
	     
       if ( line.indexOf("TopBanner:") == 0)
	  topBannerFile = line.replace("TopBanner: ", "").simplified();

       if ( line.indexOf("SideBanner:") == 0)
	  sideBannerFile = line.replace("SideBanner: ", "").simplified();

       if ( line.indexOf("Icon:") == 0)
	  iconFile = line.replace("Icon: ", "").simplified();

    }

    // Make sure the required vars are populated
    if ( ProgramName.isEmpty() || ProgramVer.isEmpty() || ProgramAuthor.isEmpty() \
      || ProgramArch.isEmpty() || ProgramOS.isEmpty() || CreatorVer.isEmpty() \
      || ProgramSig.isEmpty() \
      ) {
      QMessageBox::critical( this, tr("Error!"), tr("Cannot load meta data from this PBI. File corrupt / invalid.") + " " + PBIFILE );
      exit(-1);
    }
	
	
    // Set the menu labels
    WelcomeLabel->setText(tr("Click Next to begin the installation."));
    QFont font("Sans Serif", 16, QFont::Bold);
    ProgramLabel->setText( ProgramName +  " " + ProgramVer );
    ProgramLabel->setFont(font);
    AuthorLabel->setText(ProgramAuthor);
    WebsiteLabel->setText(ProgramWeb);
    setWindowTitle("PBI: " + ProgramName + " " + ProgramVer );	
	

    // Check for license
    QProcess pbiLic;
    pbiLic.setProcessChannelMode(QProcess::MergedChannels);
    pbiLic.start("pbi_add", QStringList() << "-l" << PBIFILE);
    if (! pbiLic.waitForFinished(-1) ) {
      QMessageBox::critical( this, tr("Error!"), tr("Error while running pbi_add!"));
      exit(-1);
    }
    bool foundLic = false;

    while ( pbiLic.canReadLine() ) {
       line = pbiLic.readLine(); // line of text excluding '\n'
 
       if ( foundLic ) 
         LicenseTextEdit->append(line.simplified() + "\n" ); 

       if ( line.indexOf("LICENSE:") == 0)
	 foundLic=true;
    }
   
    if (!foundLic)
      removePage(1); 
    else 
      LicenseTextEdit->moveCursor(QTextCursor::Start);
}


void PBI::AgreeButtonSlot()
{
    if ( LicenseCheckBox->isChecked() )
	button(QWizard::NextButton)->setEnabled(TRUE);
    else
	button(QWizard::NextButton)->setEnabled(FALSE);
}

void PBI::FinishedInstallSlot()
{    
   if ( pbiAdd->exitCode() != 0 ) {
      QMessageBox::critical( this, tr("Error!"), tr("The pbi_add command returned an error, the source file may be corrupt or invalid.") );
      exit(-1);
   }

   installProgress->setValue(installProgress->maximum());
   StatusLabel->setText(tr("Installation Finished!"));
   progressTextDialog->setText("");
   next();
}


void PBI::setProgressSlot()
{
    QString tmp;
    QString value;
    bool ok;

    while (pbiAdd->canReadLine() )
    {	 
         // Read the line, sanitize it for display
	 tmp = pbiAdd->readLine();
	 tmp = tmp.simplified();

	 // TAR PROGRESS
	 if ( tmp.indexOf("Extracting to:") == 0) {
	   inTar = true;
           installProgress->setMaximum(TotalLinesTar);
	   continue;
	 }
	
	 if ( inTar ) {

	   // Done with tar, show activity and update dialog
	   if ( tmp.indexOf("Extraction Finished!") == 0 ) {
		inTar = false;
                progressTextDialog->setText(tr("Creating menu icons and links..."));	
	        installProgress->setValue(0);
	        installProgress->setMaximum(0);
		continue;
           }

	   // Reading tar lines, move the progress bar forward
	   if ( tmp.indexOf("x " ) == 0)
	     value = tmp.replace("x ", "");    
	   value.truncate(48);
	   progressTextDialog->setText(value);
	   // Set the steps +1
	   installProgress->setValue(installProgress->value() + 1);
	   continue;
	 } // End TAR Progress


	 // HASH Progress
	 if ( tmp.indexOf("HASHCOUNT:") == 0 ) {
		inHash = true;
	  	value = tmp.replace("HASHCOUNT: ", "").simplified();
		value.toInt(&ok);
		if ( ok )
	        	installProgress->setMaximum(value.toInt(&ok));
	        installProgress->setValue(0);
                progressTextDialog->setText(tr("Merging Files..."));	
		continue;
	 }
	 if ( inHash ) {
	   if ( tmp.indexOf("HASHDONE") == 0 ) {
		inHash = false;
                progressTextDialog->setText(tr("Registering installation and cleanup..."));	
	        installProgress->setValue(0);
	        installProgress->setMaximum(0);
		continue;
           }
	   installProgress->setValue(installProgress->value() + 1);
	   continue;
	 }
	 // End of HASH Checks
	 
    }

}



void PBI::runCommand( QString Command )
{
      FILE *file = popen(Command.toLatin1(),"r"); 
      pclose(file);
}


void PBI::startInstallSlot()
{
       progressTextDialog->setText(tr("Preparing to install..."));	
       
       inTar = false;
       inHash = false;
       installProgress->setMaximum(0);
       installProgress->setValue(0);
        
       // Start the extraction process	
       //StatusLabel->setText(tr("Installing") + " " + ProgramName);	
       StatusLabel->setText("");	

       ExtractStarted = true;
       pbiAdd = new QProcess(this);
       QStringList arguments;
       arguments << "-f" << "-v" << "--licagree" << "--no-checksig" << PBIFILE;

       connect( pbiAdd, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(FinishedInstallSlot() ) ); 
       connect( pbiAdd, SIGNAL(readyRead()), this, SLOT(setProgressSlot() ) );
       pbiAdd->setProcessChannelMode(QProcess::MergedChannels);
       pbiAdd->start(QString("pbi_add"), arguments);
}

void PBI::checkCustomGraphics()
{
     // Load the default images from resource file
     QPixmap pixmap1;
     pixmap1.load(":/leftside.png");
     setPixmap(QWizard::WatermarkPixmap, pixmap1);

     topbanner.load(":/header.png");

     QPixmap pixmap3;
     pixmap3.load(":/application.png");
     setWindowIcon(pixmap3);

     // Check for custom files now
     if (QFile::exists( sideBannerFile ) ) {
	 pixmap1.load(sideBannerFile);
         setPixmap(QWizard::WatermarkPixmap, pixmap1);
     }

     if ( QFile::exists( topBannerFile ) )
	topbanner.load(topBannerFile);

     if ( QFile::exists( iconFile ) ) {
	 pixmap3.load(iconFile);
	 setWindowIcon(pixmap3);
     }
     
}

void PBI::reject()
{

    switch( QMessageBox::warning( this, tr("Cancel Confirmation"), tr("Are you sure you wish to cancel?"), tr("Yes"), tr("No"), 0, 0, 1 ) ) {
	     case 0: // The user clicked the Yes again button or pressed Enter
		 uninstall();
		 exit(1);
		 break;
	     case 1: // The user clicked the No Button
		 break;
	     }

}

void PBI::uninstall()
{
    if (ExtractStarted) 
	pbiAdd->kill();
}

void PBI::accept()
{
    exit(0);
}

QString PBI::getLineFromCommandOutput( QString command )
{

        FILE *file = popen(command.toLatin1(),"r");

        char buffer[100];

        QString line = "";
        char firstChar;

        if ((firstChar = fgetc(file)) != -1){
                line += firstChar;
                line += fgets(buffer,100,file);
        }


        pclose(file);


        return line;
    
}

void PBI::checkSignature()
{
	// If sig is good, no changes needed
	if ( ProgramSig == "Verified" )
		return;

	if ( ProgramSig == "Not Signed" ) {
		labelSig->setText(tr("No digital signature"));
		labelSig->setStyleSheet("color: rgb(255, 0, 0);");
        	QPixmap pixmap;
		pixmap.load(":/no-sign.png");
        	labelSigPic->setPixmap(pixmap);
		switch( QMessageBox::warning( this, tr("Warning!"), tr("This application has no digital signature. Installing applications from unknown sources can be dangerous. Continue with the install? "), tr("OK"), tr("Cancel"), 0, 0, 1 ) ) 
    		{
      			case 0: // The user clicked the OK button
			break;

			case 1: // The user clicked the cancel button
         		exit(1);
      			break;
    		}
		return;
	}
	if ( ProgramSig == "Bad" ) {
		labelSig->setText(tr("Invalid digital signature"));
		labelSig->setStyleSheet("color: rgb(255, 0, 0);");
        	QPixmap pixmap;
		pixmap.load(":/invalid-sign.png");
        	labelSigPic->setPixmap(pixmap);
		switch( QMessageBox::warning( this, tr("Warning!"), tr("This application has a bad or unknown digital signature. Installing applications from unknown sources can be dangerous. Continue with the install? "), tr("OK"), tr("Cancel"), 0, 0, 1 ) ) 
    		{
      			case 0: // The user clicked the OK button
			break;

			case 1: // The user clicked the cancel button
         		exit(1);
      			break;
    		}
		return;
	}
}
