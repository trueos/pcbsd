/***************************************************************************
 *   Copyright (C) 2008 by Tim McCormick 				   *
 *   tim@pcbsd.org   						  	   *
 *                                                                         *
 *   Permission is hereby granted, free of charge, to any person obtaining *
 *   a copy of this software and associated documentation files (the       *
 *   "Software"), to deal in the Software without restriction, including   *
 *   without limitation the rights to use, copy, modify, merge, publish,   *
 *   distribute, sublicense, and/or sell copies of the Software, and to    *
 *   permit persons to whom the Software is furnished to do so, subject to *
 *   the following conditions:                                             *
 *                                                                         *
 *   The above copyright notice and this permission notice shall be        *
 *   included in all copies or substantial portions of the Software.       *
 *                                                                         *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. *
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/

#include <sys/types.h>
#include <sys/sysctl.h>

#include <QRegExp>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QCoreApplication>
#include <QDebug>

#include <QObject>


#include "pcbsd-netif.h"
#include "pcbsd-utils.h"


#include "../../config.h"

using namespace pcbsd;

// The default pbi.conf file
QString PBI_ETCCONF(PREFIX + "/etc/pbi.conf");
QString PCBSD_ETCCONF(PREFIX + "/etc/pcbsd.conf");

QString Utils::getLineFromCommandOutput( QString cmd )
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

QString Utils::sysctl(QString sysctl)
{
   char result[1000];
   size_t len = sizeof(result);
   sysctlbyname(sysctl.toLocal8Bit(), result, &len, NULL, 0);
   result[len] = '\0';
   return QString(result);
}

long long Utils::sysctlAsInt(QString sysctl)
{
   long long result = 0;
   size_t len = sizeof(result);
   sysctlbyname(sysctl.toLocal8Bit(), &result, &len, NULL, 0);
   return result;
}

QString Utils::bytesToHumanReadable(long long bytes)
{
   float num = bytes;
   QStringList list;
   list << "KB" << "MB" << "GB" << "TB";

   QStringListIterator i(list);
   QString unit("bytes");

   while(num >= 1024.0 && i.hasNext())
   {
     unit = i.next();
     num /= 1024.0;
   }
   return QString().setNum(num,'f',2)+" "+unit;
}


// Function to get a value from a specific key in the pbi.conf file
QString Utils::getValFromPBIConf(QString key) {
	return getValFromPCConf(PBI_ETCCONF, key);
}

// Function to get a value from a specific key in the pcbsd.conf file
QString Utils::getValFromPCBSDConf(QString key) {
	return getValFromPCConf(PCBSD_ETCCONF, key);
}

QString Utils::getValFromPCConf(QString conf, QString key) {
  
  // Load from conf the requested key
  QFile confFile(conf);
  if ( confFile.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &confFile );
        stream.setCodec("UTF-8");
        QString line;
        while ( !stream.atEnd() ) {
            line = stream.readLine().simplified();
	    if ( line.indexOf(key + ": ") == 0 ) {
   		confFile.close();
		return line.replace(key + ": ", "");
	    }

        }
   confFile.close();
  }

  return QString();
}

QString Utils::getValFromSHFile(QString envFile, QString envVal)
{
  QFile confFile(envFile);
  if ( ! confFile.open( QIODevice::ReadOnly ) )
        return "";

  QTextStream stream( &confFile );
  QString line;
  while ( !stream.atEnd() ) {
     line = stream.readLine();
     if ( line.indexOf(envVal + "=") == 0 ) {
	line.replace(envVal + "=", "");

	// Remove the ' or " from variable
	if ( line.indexOf("'") == 0 ) {
	   line = line.section("'", 1, 1);
	   line = line.section("'", 0, 0);
	} 
	if ( line.indexOf('"') == 0 ) {
	   line = line.section('"', 1, 1);
	   line = line.section('"', 0, 0);
	}

        confFile.close();
	return line.simplified();
     }
  }
  confFile.close();

  return QString();
}

QString Utils::getValFromCSHFile(QString envFile, QString envVal)
{
  QFile confFile(envFile);
  if ( ! confFile.open( QIODevice::ReadOnly ) )
        return "";

  QTextStream stream( &confFile );
  QString line;
  while ( !stream.atEnd() ) {
     line = stream.readLine();
     if ( line.indexOf("setenv " + envVal + " ") == 0 ) {
	line.replace("setenv " + envVal + " ", "");

	// Remove the ' or " from variable
	if ( line.indexOf("'") == 0 ) {
	   line = line.section("'", 1, 1);
	   line = line.section("'", 0, 0);
	}
	if ( line.indexOf('"') == 0 ) {
	   line = line.section('"', 1, 1);
	   line = line.section('"', 0, 0);
	}

        confFile.close();
	return line;
     }
  }
  confFile.close();

  return QString();
}

bool Utils::setValSHFile(QString envFile, QString envName, QString envVal)
{
	return setValSHFile(envFile, envName, envVal, QString('"'));
}

bool Utils::setValSHFile(QString envFile, QString envName, QString envVal, QString quote)
{
  QFile confFile(envFile);
  if ( ! confFile.open( QIODevice::ReadOnly ) )
        return false;

  QStringList shConf;
  bool added = false;

  QTextStream stream( &confFile );
  QString line;
  while ( !stream.atEnd() ) {
     line = stream.readLine();
     if ( line.indexOf(envName + "=") == 0 ) {
	if ( ! envVal.isEmpty() && ! added )
	  shConf << envName + "=" + quote + envVal + quote + " ; export " + envName;
	added = true;
     } else {
	shConf << line;
     }
     
  }
  confFile.close();

  if (! added && ! envVal.isEmpty() )
    shConf << envName + "=" + quote + envVal + quote + " ; export " + envName;

  if ( confFile.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &confFile );
        for (int i = 0; i < shConf.count(); ++i)
                stream << shConf.at(i) << "\n";
        confFile.close();
        return true;
  }

  return false;
}

bool Utils::setValCSHFile(QString envFile, QString envName, QString envVal)
{
  QFile confFile(envFile);
  if ( ! confFile.open( QIODevice::ReadOnly ) )
        return false;

  QStringList shConf;
  bool added = false;

  QTextStream stream( &confFile );
  QString line;
  while ( !stream.atEnd() ) {
     line = stream.readLine();
     if ( line.indexOf("setenv " + envName + " ") == 0 ) {
	if ( ! envVal.isEmpty() && ! added )
	  shConf << "setenv " + envName + " '" + envVal + "'";
	added = true;
     } else {
	shConf << line;
     }
     
  }
  confFile.close();

  if (! added && ! envVal.isEmpty() )
    shConf << "setenv "  + envName + " '" + envVal + "'";

  if ( confFile.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &confFile );
        for (int i = 0; i < shConf.count(); ++i)
                stream << shConf.at(i) << "\n";
        confFile.close();
        return true;
  }

  return false;
}

QString Utils::getProxyURL() {
	QString val = getValFromSHFile("/etc/profile", "HTTP_PROXY");
	if ( val.isEmpty() )
	   return QString();
	return val.section(":", 0, 0);
}

QString Utils::getProxyUser() {
	QString val = getValFromSHFile("/etc/profile", "HTTP_PROXY_AUTH");
	if ( val.isEmpty() )
	   return QString();
	return val.section(":", 2, 2);
}

QString Utils::getProxyPass() {
	QString val = getValFromSHFile("/etc/profile", "HTTP_PROXY_AUTH");
	if ( val.isEmpty() )
	   return QString();
	return val.section(":", 3, 3);
}

QString Utils::getProxyType() {
	QString val = getValFromSHFile("/etc/profile", "HTTP_PROXY_AUTH");
	if ( val.isEmpty() )
	   return QString();
	return val.section(":", 0, 0);
}

QString Utils::getProxyPort() {
	QString val = getValFromSHFile("/etc/profile", "HTTP_PROXY");
	if ( val.isEmpty() )
	   return QString();
	return val.section(":", 1, 1);
}

QString Utils::getMasterMirror() {
	return getValFromPCBSDConf("PCBSD_MIRROR");
}


bool Utils::setValPBIConf(QString key, QString val)
{
	return setValPCConf(PBI_ETCCONF, key, val);
}

bool Utils::setValPCBSDConf(QString key, QString val)
{
	return setValPCConf(PCBSD_ETCCONF, key, val);
}

bool Utils::setValPCConf(QString conf, QString key, QString val)
{
  bool savedLine, added;
  QStringList pbiConf;

  // Read PBI_ETCCONF into memory
  QFile confFile(conf);
  if ( ! confFile.open( QIODevice::ReadOnly ) )
	return false;

  QTextStream stream( &confFile );
  QString line;
  added = false;

  while ( !stream.atEnd() ) {
   	savedLine = false;
        line = stream.readLine();

        if ( line.indexOf(key + ": ") == 0 ) {
          if ( ! val.isEmpty() )
             pbiConf << key + ": " + val;
             added = true;
	     savedLine = true;
          }

          if (!savedLine )
            pbiConf << line;
       }

  confFile.close();

  if (! added && ! val.isEmpty() )
    pbiConf << key + ": " + val;

  if ( confFile.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &confFile );
	for (int i = 0; i < pbiConf.count(); ++i)
		stream << pbiConf.at(i) << "\n";
	confFile.close();
	return true;
  }
	
  return false;
}

bool Utils::setProxyAddress(QString val) {
   setValCSHFile(QString("/etc/csh.cshrc"), QString("HTTP_PROXY"), val);
   setValCSHFile(QString("/etc/csh.cshrc"), QString("HTTPS_PROXY"), val);
   setValCSHFile(QString("/etc/csh.cshrc"), QString("NO_PROXY"), "127.0.0.1,localhost");
   setValCSHFile(QString("/etc/csh.cshrc"), QString("http_proxy"), val);
   setValCSHFile(QString("/etc/csh.cshrc"), QString("https_proxy"), val);
   setValCSHFile(QString("/etc/csh.cshrc"), QString("no_proxy"), "127.0.0.1,localhost");
   setValSHFile(QString("/etc/profile"), QString("HTTP_PROXY"), val, "'");
   setValSHFile(QString("/etc/profile"), QString("HTTPS_PROXY"), val, "'");
   setValSHFile(QString("/etc/profile"), QString("NO_PROXY"), "127.0.0.1,localhost");
   setValSHFile(QString("/etc/profile"), QString("http_proxy"), val, "'");
   setValSHFile(QString("/etc/profile"), QString("https_proxy"), val, "'");
   setValSHFile(QString("/etc/profile"), QString("no_proxy"), "127.0.0.1,localhost");
   return true;
}

bool Utils::setProxyAuth(QString val) {
   setValCSHFile(QString("/etc/csh.cshrc"), QString("HTTP_PROXY_AUTH"), val);
   return setValSHFile(QString("/etc/profile"), QString("HTTP_PROXY_AUTH"), val, "'");
}

bool Utils::setMasterMirror(QString val) {
   return setValPCBSDConf("PCBSD_MIRROR", val);
}

QString Utils::getConfFileValue( QString oFile, QString Key )
{
   return getConfFileValue(oFile, Key, 1);
}

QString Utils::getConfFileValue( QString oFile, QString Key, int occur )
{
	int found = 1;
    
    	QFile file( oFile );
    	if ( ! file.open( QIODevice::ReadOnly ) ) {
		return QString();
	}

       	QTextStream stream( &file );
       	QString line;
       	while ( !stream.atEnd() ) {
            	line = stream.readLine(); // line of text excluding '\n'
		line = line.section("#",0,0).trimmed(); //remove any comments
		if(line.isEmpty()){ continue; }
		int index = line.indexOf(Key, 0);
                //qDebug() << "Line:" << line << index;
                // If the KEY is not found at the start of the line, continue processing 
		if(index!=0)
			continue;
		
	    	if ( found == occur) {
 			line.remove(line.indexOf(Key, 0), Key.length());
    
    			// Remove any quotes
   			if ( line.indexOf('"') == 0 )
				line = line.remove(0, 1);
    
    			if ( line.indexOf('"') != -1  )
				line.truncate(line.indexOf('"'));

			file.close();
			
    			return line;
    		} else {
       			found++;  
    		}
        }

	file.close();
	return QString();
}

QString Utils::getConfFileValue( QString oFile, QString Key, QString ValRx, int occur )
{
	int found = 1;
    
    	QFile file( oFile );
    	if ( ! file.open( QIODevice::ReadOnly ) ) {
		return QString();
	}

	QString rxStr ( Key );
	rxStr.append( ValRx );
	QRegExp rx(rxStr);
       	QTextStream stream( &file );
       	QString line;
       	while ( !stream.atEnd() ) {
            	line = stream.readLine(); // line of text excluding '\n'
            
                // If the KEY is not found in the line, continue processing 
		if ( line.trimmed().indexOf("#", 0) == 0 || line.indexOf(rx, 0) == -1 || line.indexOf(rx, 0) > 0)
			continue;

	    	if ( found == occur ) {
 			line.remove(line.indexOf(Key, 0), Key.length());
    
    			// Remove any quotes
   			if ( line.indexOf('"') == 0 )
				line = line.remove(0, 1);
    
    			if ( line.indexOf('"') != -1  )
				line.truncate(line.indexOf('"'));

			file.close();
    			return line;
    		} else {
       			found++;  
    		}
        }

	file.close();
	return QString();
}

bool Utils::setConfFileValue( QString oFile, QString oldKey, QString newKey )
{
	return setConfFileValue(oFile, oldKey, newKey, -1);
}

bool Utils::setConfFileValue( QString oFile, QString oldKey, QString newKey, int occur )
{
    	// Lets the dev save a value into a specified config file. 
	// The occur value tells which occurance of "oldKey" to replace
    	// If occur is set to -1, it will remove any duplicates of "oldKey"
    
	//copy the original file to create a temporary file for editing
	QString oFileTmp = oFile + ".tmp";
	QString cmd = "cp "+oFile+" "+oFileTmp;
	runShellCommand(cmd);
	
	//Continue evaluating the temporary file
    	QStringList SavedFile;
    	int found = 1;

    	// Load the old file, find the oldKey, remove it and replace with newKey
    	QFile file( oFileTmp );
	if ( ! file.open( QIODevice::ReadOnly ) )
		return false;

        QTextStream stream( &file );
        QString line;
        while ( !stream.atEnd() ) {
        	line = stream.readLine(); // line of text excluding '\n'
            
		// Key is not found at all
		if ( line.indexOf(oldKey, 0) == -1 ) {
	        	SavedFile << line ;
			continue;
		}

		// Found the key, but it is commented out, so don't worry about this line
		if ( line.trimmed().indexOf("#", 0) == 0 ) {
	         	SavedFile << line ;
			continue;
		}

		// If the KEY is found, and we are just on wrong occurance, save it and continue to search  
		if ( occur != -1 && found != occur ) {
	         	SavedFile << line ;
			found++;
			continue;
		}

            	// If the KEY is found in the line and this matches the occurance that must be processed
            	if ( ! newKey.isEmpty() && found == occur )
           	{
	         	SavedFile << newKey ;
			newKey.clear();
	    		found++;
			continue;
            	} 

             	// If the KEY is found and we just want one occurance of the key
            	if ( occur == -1 && ! newKey.isEmpty() ) {
	         	SavedFile << newKey ;
			newKey.clear();
			found++;	
			continue;
		}

        }

   	file.close();

	// Didn't find the key? Write it!
	if ( ! newKey.isEmpty() )
	    SavedFile << newKey;
	
    
    	// Save the new file
    	QFile fileout( oFileTmp );
    	if ( ! fileout.open( QIODevice::WriteOnly ) )
		return false;

  	QTextStream streamout( &fileout );
	for (int i = 0; i < SavedFile.size(); ++i)
          	streamout << SavedFile.at(i) << "\n";

       	fileout.close();

	//Have the temporary file with new changes overwrite the original file
	cmd = "mv "+oFileTmp+" "+oFile;
	runShellCommand(cmd);	
	
	return true;
    
}

QStringList Utils::runShellCommand( QString command ){
  //Just a simple overload for backwards compatibility
  bool junk = false;
  return runShellCommand(command,junk);	
}

//Run a shell command (return a list of lines and an optional success flag)
QStringList Utils::runShellCommand( QString command , bool& success)
{
 //split the command string with individual commands seperated by a ";" (if any)
 QStringList cmdl = command.split(";");
 QString outstr;
 //run each individual command
 bool ok = true;
 for(int i=0;i<cmdl.length() && ok;i++){ 
   QProcess p;  
   //Make sure we use the system environment to properly read system variables, etc.
   p.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
   //Merge the output channels to retrieve all output possible
   p.setProcessChannelMode(QProcess::MergedChannels);   
   p.start(cmdl[i]);
   while(p.state()==QProcess::Starting || p.state() == QProcess::Running){
     p.waitForFinished(200);
     QCoreApplication::processEvents();
   }
   QString tmp = p.readAllStandardOutput();
   outstr.append(tmp);
   ok = (p.exitCode()==0);
 }
 if(outstr.endsWith("\n")){outstr.chop(1);} //remove the newline at the end 
 QStringList out = outstr.split("\n");
  //if(success!=0){
    //Also return this optional output
    success = ok;
   //}
 //qDebug() << command;
 //for(int j=0; j<out.length();j++){ qDebug() << out[j];}
 return out;
}

//Run a shell command and return only lines with a specific search term
//Example command: ls | grep X -> runShellCommandSearch("ls","X")
QString Utils::runShellCommandSearch( QString command, QString sch)
{
 QStringList L = listShellCommandSearch(command,sch);
 QString outstr = L.join("\n"); //combine the remaining lines into a single string
 return outstr;
}

QString Utils::readTextFile(QString filepath){
  //Note: this is good for small-ish text files due to storing the entire contents of the file in memory
  // If you have a massive text file to read, it is better to run/parse the contents actively, line-by-line
  QFile file(filepath);
  if(!file.exists()){ return ""; } //not existant
  QString out;
  if(file.open(QFile::ReadOnly | QFile::Text)){
    QTextStream stream(&file);
    out = stream.readAll();
    file.close();
  }
  return out;
}

bool Utils::writeTextFile(QString filepath, QString contents, bool replace){
  QFile file(filepath);
  if(file.exists() && !replace){ return false; } // cannot overwrite
  if(!contents.endsWith("\n")){ contents.append("\n"); } //always end with a newline
  bool ok = false;
  if(file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
    QTextStream stream(&file);
    stream << contents;
    file.close();
    ok = true;
  }
  return ok;
}

// Function which displays a info box and restarts networking
void Utils::restartNetworking()
{
    
   QMessageBox infoBox;
   infoBox.setWindowModality(Qt::ApplicationModal);
   infoBox.setWindowTitle(QObject::tr("Restarting network..."));
   infoBox.setInformativeText(QObject::tr("Network is restarting, please wait..."));
   infoBox.setStandardButtons(QMessageBox::NoButton);
   infoBox.show();

   QProcess cmd;
   cmd.start(QString("/etc/rc.d/netif"), QStringList() << "restart" );
   while ( cmd.state() != QProcess::NotRunning ) {
       cmd.waitForFinished(100);
       QCoreApplication::processEvents();
   }

   // Set the gateway device name
   QString route = getConfFileValue("/etc/rc.conf", "defaultrouter=", 1);
   if ( ! route.isEmpty() ) {
     infoBox.setInformativeText(QObject::tr("Setting default route..."));
     cmd.start(QString("route"), QStringList() << "delete" << "default" );
     while ( cmd.state() != QProcess::NotRunning ) {
         cmd.waitForFinished(100);
         QCoreApplication::processEvents();
     }

     cmd.start(QString("route"), QStringList() << "add" << "default" << route );
     while ( cmd.state() != QProcess::NotRunning ) {
         cmd.waitForFinished(100);
         QCoreApplication::processEvents();
     }
   }

   // Check for any devices to run DHCP on
   QStringList ifs = NetworkInterface::getInterfaces();
   for ( QStringList::Iterator it = ifs.begin(); it != ifs.end(); ++it )
   {
       QString dev = *it;

       // Devices we can safely skip
       if (dev.indexOf("lo") == 0 
           || dev.indexOf("fwe") == 0 
           || dev.indexOf("ipfw") == 0
           || dev.indexOf("plip") == 0
           || dev.indexOf("pfsync") == 0
           || dev.indexOf("pflog") == 0
           || dev.indexOf("usbus") == 0
           || dev.indexOf("vboxnet") == 0
           || dev.indexOf("tun") == 0)
	  continue;

	// Check if this device has DHCP enabled
	if ( Utils::getConfFileValue( "/etc/rc.conf", "ifconfig_" + dev + "=", 1 ).indexOf("DHCP") != -1 )
	{
	   qDebug() << "Running DHCP on " << dev;
           infoBox.setInformativeText(QObject::tr("Running DHCP..."));
     	   cmd.start(QString("/etc/rc.d/dhclient"), QStringList() << "start" << dev );
           while ( cmd.state() != QProcess::NotRunning ) {
             cmd.waitForFinished(100);
             QCoreApplication::processEvents();
           }
	}
   }

   infoBox.close();
}

void Utils::runInTerminal(QString command, QString windowTitle)
{
    QStringList out = Utils::runShellCommand("de-info");
    QString terminal_app="xterm";
    QString terminal_title="-T";
    QString terminal_comm="-e";
    bool separateArgs=false;

    for(int i=0; i<out.size(); i++)
    {
        QString str = out.at(i).trimmed();        
        if (str.indexOf("Terminal:")>=0)
        {
            terminal_app = str.replace("Terminal:","");
            continue;
        }
        if(str.indexOf("Terminal title switch:")>=0)
        {
            terminal_title = str.replace("Terminal title switch:","");
            continue;
        }
        if (str.indexOf("Terminal command switch:")>=0)
        {
            terminal_comm = str.replace("Terminal command switch:","");
            continue;
        }
        if (str.indexOf("Terminal command separate args:")>=0)
        {
            separateArgs = (str.replace("Terminal command separate args:","").trimmed() == QString("YES"));
        }
    }//for all strings
    if (!terminal_app.length())
        return;    

    QStringList args;
    QStringList comm;
    if (separateArgs)
        comm=command.split(" ");
    else
        comm<<command.trimmed();

    if (windowTitle.length())
    {
        args<<terminal_title.trimmed();
        args<<windowTitle.trimmed();
    }
    args<<terminal_comm.trimmed()<<comm;

    qDebug() << terminal_app.trimmed() << args;

    QProcess::startDetached(terminal_app.trimmed(), args);
}

void Utils::openInFileManager(QString location)
{
    //TODO: Process quotes for location
    QString exec = Utils::runShellCommand("de-info -fileman").at(0);
    exec = exec.replace("%s", location);
    QProcess::startDetached(exec);
}

QStringList Utils::listShellCommandSearch( QString command, QString sch)
{
  QStringList L = Utils::runShellCommand(command);
  QStringList schList = sch.split(" "); //make a list of the words to search for in a single line
  for(int i=0; i<schList.size(); i++){
    L = L.filter(schList[i]);
  }
  return L;
}

bool Utils::validateIPV6( QString IP )
{
  bool ok;

  if ( IP.count(":") < 2)
     return false;

  // Validate the V6 blocks
  for ( int i = 0; i <= 7; ++i )
  {
     if ( IP.section(":", i, i).isEmpty() )
       continue;

     // Check if valid hex
     IP.section(":", i,i).toInt(&ok, 16);
     if (!ok)
        return false;
  }

  return true;
}

bool Utils::validateIPV4( QString IP )
{
  int num;
  bool ok;

  if ( IP.indexOf(".") == -1 )
     return false;

  // Make sure this is a number after the . is taken out
  QString tmp;
  tmp = IP;
  tmp.replace(".", "");
  tmp.toLong(&ok);
  if (!ok)
     return false;
    
  if ( IP.count(".") != 3)
     return false;

  // Validate the V4 blocks
  for ( int i = 0; i <= 3; ++i )
  {
     if ( IP.section(".", i, i).isEmpty() )
        return false;

     if ( IP.section(".", i, i).size() > 3 )
        return false;

     num = IP.section(".", i,i).toInt(&ok);
     if (!ok)
        return false;

     if ( num > 255 || num < 0)
        return false;
  }
    
  return true;
}

#define TRY_GET_VALUE_STR(val_name, field)\
if (Str.contains(val_name)){\
                            Entry.field = Str.replace(val_name,"").trimmed();\
                                          continue;}\

#define TRY_GET_VALUE_BOOL(val_name, field, true_val)\
if (Str.contains(val_name)){\
                            Entry.field = Str.replace(val_name,"").trimmed().toLower() == QString(true_val).toLower();\
                                          continue;}\


static int de_info(bool isCurrent, QVector<DesktopEnvironmentInfo>& retVal)
{
    static const char* const DE_NAME = "DE name:";
    static const char* const DE_ACTIVE = "Current DE:";
    static const char* const DE_XDG = "XDG compatible:";
    static const char* const DE_INSTALLED = "Installed:";
    static const char* const DE_SUDO = "Sudo command:";
    static const char* const DE_FILE_MANAGER = "File manager:";
    static const char* const DE_TERMINAL = "Terminal:";
    static const char* const DE_CONFIG_APP = "Configuration app:";

    retVal.clear();

    QStringList flags;
    if (!isCurrent)
        flags<<"-i";

    QProcess* deinfo = new QProcess();
    deinfo->setProcessChannelMode(QProcess::MergedChannels);
    deinfo->start(QString("/usr/local/bin/de-info"), flags);
    deinfo->waitForFinished(-1);
    DesktopEnvironmentInfo Entry;
    QString Str;

    while ( deinfo->canReadLine() )
    {
        Str = deinfo->readLine().simplified();
        if (Str.contains(DE_NAME))
        {
            if (Entry.Name.length())
            {
                retVal.push_back(Entry);
                Entry = DesktopEnvironmentInfo();
            }
            Entry.Name = Str.replace(DE_NAME,"").trimmed();
            continue;
        }//if found 'DE name'

        TRY_GET_VALUE_BOOL(DE_ACTIVE, isActive, "yes");
        TRY_GET_VALUE_BOOL(DE_INSTALLED, isInstalled, "yes");
        TRY_GET_VALUE_BOOL(DE_XDG, isXDG, "yes");
        TRY_GET_VALUE_STR(DE_SUDO, SudoCommand);
        TRY_GET_VALUE_STR(DE_FILE_MANAGER, FileManager);
        TRY_GET_VALUE_STR(DE_TERMINAL, TerminalCommand);
        TRY_GET_VALUE_STR(DE_CONFIG_APP, ConfigurationApplication);
        //TODO: another fields
    }//while process output reading

    if (Entry.Name.length())
        retVal.push_back(Entry);

    return retVal.size();
}


#undef TRY_GET_VALUE_STR
#undef TRY_GET_VALUE_BOOL

QVector<DesktopEnvironmentInfo> Utils::installedDesktops()
{
    QVector<DesktopEnvironmentInfo> retVal;
    de_info(false, retVal);
    return retVal;
}

DesktopEnvironmentInfo Utils::currentDesktop()
{
    QVector<DesktopEnvironmentInfo> script_result;
    if (de_info(true, script_result) != 0)
    {
        return script_result[0];
    }
    return DesktopEnvironmentInfo();
}

bool Utils::canLogout()
{
    // de-logout calls pnly one time for each application.
    // you can use this function without performance worry
    static bool wasCalledAlready = false;
    static bool storedVal = false;
    if (wasCalledAlready)
    {
        return storedVal;
    }

    QProcess* delogout = new QProcess();
    delogout->start(QString("/usr/local/bin/de-logout"), QStringList()<<"-check");
    delogout->waitForFinished(-1);

    wasCalledAlready = true;
    storedVal = (delogout->exitCode() == 0);
    return storedVal;
}

bool Utils::logout()
{
    QProcess* delogout = new QProcess();
    delogout->start(QString("/usr/local/bin/de-logout"));
    delogout->waitForFinished(-1);

    return (delogout->exitCode() == 0);
}


