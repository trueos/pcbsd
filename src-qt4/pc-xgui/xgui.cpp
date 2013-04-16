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
#include "xgui.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include "../config.h"

const QString PROGDIR( PREFIX + "/share/pcbsd/xorg-gui" );
const QString XDRIVERDIR( PREFIX + "/lib/xorg/modules/drivers" );
#define TMPDIR QString("/tmp/.xgui")


void XGUI::ProgramInit()
{
    
    //gridLayout_4->setMargin(0);
    //gridLayout_4->setSpacing(0);
    
    // Load the screen resolutions
    loadResolutions();

    // Load the drivers
    loadDrivers();

    // Load the Depth
    loadDepth();

    // Load the Device Information
    loadDeviceInformation();
   
    // Disable the monitor advanced group box
    monitorChangedSlot();

    // Check for an unprobed monitor
    checkUnprobedMonitor(); 

    // Connect our Dual-head checkbox / slot
    connect(checkDualHead, SIGNAL( clicked() ), this, SLOT( dualChangedSlot() ) );
    connect(checkMonitorSync, SIGNAL( clicked() ), this, SLOT( monitorChangedSlot() ) );
    connect(pushApply, SIGNAL( clicked() ), this, SLOT( applySlot() ) );
    connect(pushClose, SIGNAL( clicked() ), this, SLOT( closeSlot() ) );

    // Check out dualChangedSlot to ensure we disable the box by default
    dualChangedSlot();
}


void XGUI::loadDeviceInformation()
{
    QString tmp;
    QString scriptdir = PROGDIR + "/scripts";

    // Get the card name and display it
    tmp = getLineFromCommandOutput(scriptdir + "/cardboard.sh");
    tmp.truncate(30);
    labelVideoCard->setText(tmp);

    // Get the Monitor Name and display it 
    tmp = getLineFromCommandOutput(scriptdir + "/monmodel.sh");
    if ( tmp == "Monitor Model" )
    {
       labelMonitor->setText(tr("Unknown Monitor"));
    } else {
       tmp.truncate(50);
       labelMonitor->setText(tmp);
    }


    // Get the detected driver, and make suggestion based upon it
    tmp = getLineFromCommandOutput(scriptdir + "/carddriver.sh");
    // Figure out the suggested driver for this card
    suggestDriver(tmp);

}

// Function which takes a supplied driver name, and makes suggestion for user
void XGUI::suggestDriver(QString driver)
{
   QString sugDriver, tmp;
   int z;
   QString nvdir = PROGDIR + "/nvidia-detect";

   // If we are using the "nv" driver, check which version of the nvidia-drivers are for this card
   if ( driver == "nv" )
   {
     tmp = getLineFromCommandOutput(nvdir + "/detect-nvidia.sh");
     // If we didn't get an unknown hit, set to the correct nvidia driver
     if ( tmp != "UNKNOWN" )
       driver = tmp;
     qDebug() << driver;
     qDebug() << tmp;
   }

   // Loop through the drivers, and mark the suggested one
   z = comboDriver->count();
   for ( int i = 0; i < z; i++)
   {
      tmp = comboDriver->itemText(i);

      // Set the x-suggested driver to the default first
      if ( driver == tmp ) {
        comboDriver->setCurrentIndex(i);
        sugDriver = tmp;
      }

      // Check if we are using an nvidia driver
      if ( driver == "nv" && tmp.indexOf("nvidia") != -1 ) {
        comboDriver->setCurrentIndex(i);
        sugDriver = tmp;
        break;
      } 
 
      // Check if we are using a driver that has a -3d-enable equivilant
      if (tmp.indexOf(driver + "-3d-enable") != -1 ) {
        comboDriver->setCurrentIndex(i);
        sugDriver = tmp;
        break;
      } 
   }  

   // We didn't find the suggested driver, lets default to vesa in that case
   if ( sugDriver.isEmpty() )
   {
     for ( int i = 0; i < z; i++)
     {
        if ( comboDriver->itemText(i) == "vesa" )
        {
          comboDriver->setCurrentIndex(i);
        }
         
     }

     sugDriver = "vesa";
   }

   //labelRecDriver->setText(tr("Suggested Driver:") + " <b>" + sugDriver + "</b>");
}

// Function which checks if we are using an unprobed monitor, warns the user
// and then enables the advanced tab
void XGUI::checkUnprobedMonitor()
{

  QFile test("/tmp/.unprobedMonitor");
  if (test.exists())
  {
     // Enable the advanced monitor horz / vert refresh rates
     checkMonitorSync->setChecked(true);
     monitorChangedSlot();

     QFile test2("/tmp/.warnedUnprobedMonitor");
     if ( ! test2.exists() )
     {
       system("touch /tmp/.warnedUnprobedMonitor");
       QMessageBox::warning(0, tr("Warning!"), tr("PC-BSD was unable to autodetect your monitor. You may need to specify this monitor's Horizontal and Vertical refresh rates in the 'Advanced' tab."));      
     }
     
  }

}

void XGUI::loadResolutions()
{
    QString line;
    int i=0;
    
    comboResolution->clear();

        QFile file( PROGDIR + "/settings/resolutions.txt" );
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
             while ( !stream.atEnd() ) {
               line = stream.readLine(); // line of text excluding '\n'
               comboResolution->addItem(line);	    
               comboDualRes->addItem(line);	    
               if (line == "1024x768" )
               {
			comboResolution->setCurrentIndex(i);
			comboDualRes->setCurrentIndex(i);
               }
          i++;
	}
	file.close();
    }
}


void XGUI::loadDrivers()
{
    int i = 0;
    QString line;
    
    comboDriver->clear();

    // Now populate the combo box
    QFile file( PROGDIR + "/settings/drivers.txt" );
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
             while ( !stream.atEnd() ) {
               line = stream.readLine(); // line of text excluding '\n'
	  	if ( isDriverAvailable(line) ) {
                  comboDriver->addItem(line);	    
	          if ( line == "vesa" )
	   	    comboDriver->setCurrentIndex(i);
		}
	       i++;
	}
	file.close();
    }
}


void XGUI::loadDepth()
{
    QString line;
    int i = 0;
    
    comboDepth->clear();

        QFile file( PROGDIR + "/settings/depth.txt" );
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
             while ( !stream.atEnd() ) {
               line = stream.readLine(); // line of text excluding '\n'
               comboDepth->addItem(line);	    
	       if ( line == "24bit" ) {
		  comboDepth->setCurrentIndex(i);
	       }
	 i++;

	}
	file.close();
    }
}



void XGUI::closeSlot()
{
	exit(1);
}


void XGUI::applySlot()
{
	QString tmp;

 	// Make sure the TMPDIR exists
	QDir ctmp(TMPDIR);
	if ( ! ctmp.exists() )
		ctmp.mkdir(TMPDIR);

	QFile file( TMPDIR + "/.xsettings.sh" );
    	if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
            stream << "#!/bin/sh\n";
            stream << "RES=\"" + comboResolution->currentText() + "\"; export RES\n";
            stream << "DRIVER=\"" + comboDriver->currentText() + "\"; export DRIVER\n";

	     	tmp = comboDepth->currentText();
		tmp.truncate(tmp.indexOf("bit"));
	    stream << "DEPTH=\"" + tmp + "\"; export DEPTH\n";
   	    if ( checkMonitorSync->isChecked() )
    	    {
  	    	stream << "HORIZSYNC=\"" +lineHorz->text() + "\"; export HORIZSYNC\n" ;
	    	stream << "VERTREFRESH=\"" +lineVertRefresh->text() + "\"; export VERTREFRESH\n" ;
	    } else {
		stream << "HORIZSYNC=\"OFF\"; export HORIZSYNC\n" ;
	    	stream << "VERTREFRESH=\"OFF\"; export VERTREFRESH\n" ;
	    }
            if ( checkDualHead->isChecked() )
            {
                stream << "DUALHEAD=\"YES\" ; export DUALHEAD\n";
                stream << "DUALRES=\"" + comboDualRes->currentText() + "\"; export DUALRES\n";
            }
	    file.close();
    	}


	QMessageBox::information( this, tr("Display Settings"),
    tr("The settings you have specified will now be tested.\nIf your screen does not appear, please wait and you will be returned to this screen.") );

	// Message box, letting user know that we will test this resolution
	exit(0);
}


void XGUI::monitorChangedSlot()
{

    if ( checkMonitorSync->isChecked() )
    {
	groupMonitor->setEnabled(TRUE);
    } else {
	groupMonitor->setEnabled(FALSE);
    }
    
}

void XGUI::dualChangedSlot()
{
    if ( checkDualHead->isChecked() )
    {
	groupDualHead->setEnabled(TRUE);
    } else {
	groupDualHead->setEnabled(FALSE);
    }
}

QString XGUI::getLineFromCommandOutput( QString command )
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

        return line.simplified();
}

// Script to check if a particular xorg driver is available on the system
bool XGUI::isDriverAvailable(QString driver)
{
	// Make sure to strip the -3d-enable flag off
	if ( driver.indexOf("-3d-enable") != -1 )
		driver.truncate(driver.indexOf("-3d-enable"));

	return QFile::exists(XDRIVERDIR + "/" + driver + "_drv.so");
}
