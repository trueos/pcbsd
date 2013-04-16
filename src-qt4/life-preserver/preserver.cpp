/***************************************************************************
 *   Copyright (C) 2009 by Kris Moore   *
 *   kris@pcbsd.org   *
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
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/

#include "preserver.h"
#include <qobject.h>
#include <QDir>
#include <QTextStream>
#include <QFile>

#include "externals.h"

PRESERVER::PRESERVER(QString name) : QObject()
{
  connectHost = name;

  // Create our new Preserver Directory
  QDir newPreserverDir;
  newPreserverDir.setPath(LIFEPRESERVERSDIR + "/" + connectHost);
  if ( ! newPreserverDir.exists() )
  {
     newPreserverDir.mkpath(newPreserverDir.path());
  }

}

PRESERVER::~PRESERVER()
{
}

// Function which sets the backup to run 
void PRESERVER::setCron(QString frequency)
{
  removeCronTab();
  addCronTab();

  QFile File( LIFEPRESERVERSDIR + "/" + connectHost + "/frequency" );
  if ( File.open( QIODevice::WriteOnly ) )
  {
    QTextStream streamout( &File );
    streamout << frequency;
    File.close();
  }

}

void PRESERVER::addCronTab()
{
  // Add line to rescan for backups every hour
  QFile cronFile( "/etc/crontab" );
  if ( cronFile.open( QIODevice::WriteOnly | QIODevice::Append) )
  {
    QTextStream streamout( &cronFile );
    streamout << "5     *        *       *       *" \
    << "        root    " << LIFEPRESERVERPATH + "/scripts/check-backups.sh\n";
    cronFile.close();
  }

  // Restart cron now
  system("/etc/rc.d/cron restart");

}

// Function which removes this preservers settings from the app
void PRESERVER::remove()
{
  if ( ! connectHost.isEmpty() )
  {
    QString cmd = "rm -rf '" + LIFEPRESERVERSDIR + "/" + connectHost + "'";
    system(cmd.toLatin1()); 
  }

}

// Function which starts the target backup
void PRESERVER::startBackup()
{
  QString cmd = LIFEPRESERVERPATH + "/scripts/start-backup.sh " + connectHost + " &";
  system(cmd.toLatin1());
}

// Function which stops the target backup
void PRESERVER::stopBackup()
{
  QString cmd = LIFEPRESERVERPATH + "/scripts/kill-backup.sh " + connectHost + "";
  system(cmd.toLatin1());
}

// Function which removes a cron entry for this backup
void PRESERVER::removeCronTab()
{
   QStringList updatedCron;

   QFile cronFile( "/etc/crontab" );
   if ( cronFile.open( QIODevice::ReadOnly ) )
   {
     QTextStream stream( &cronFile );
     QString line;
     while ( !stream.atEnd() ) {
       line = stream.readLine(); // line of text excluding '\n'
       // Check if we have a match for this backup
       if ( line.indexOf(LIFEPRESERVERPATH + "/scripts/check-backups.sh") == -1  ) {
          updatedCron << line;
       }
     }
     cronFile.close();
   }

   // Save the updated config
   if ( cronFile.open( QIODevice::WriteOnly ) )
   {
     QTextStream streamout( &cronFile );
     for (int i = 0; i < updatedCron.size(); ++i)
     {
        streamout << updatedCron.at(i) << "\n";
     }
     cronFile.close();
   }

  // Restart cron now
  system("/etc/rc.d/cron restart");

}

void PRESERVER::setFailedCleanup(bool enabled)
{
  setConfValue("cleanfailed", enabled);
}

void PRESERVER::setHost(QString userHost)
{
  setConfValue("host", userHost);
}

void PRESERVER::setType(QString type)
{
  setConfValue("type", type);
}

void PRESERVER::setNumBackups(int num)
{
  setConfValue("keepbackups", num);
}

void PRESERVER::setPort(int port)
{
  setConfValue("port", port);
}

QString PRESERVER::getPreserverString()
{
  return connectHost;
}

QString PRESERVER::getHost()
{
  return getConfValue("host").toString();
}

bool PRESERVER::getFailedCleanup()
{
  return getConfValue("cleanfailed").toBool();
}

int PRESERVER::getNumBackups()
{
  return getConfValue("keepbackups").toInt();
}

int PRESERVER::getPort()
{
  return getConfValue("port").toInt();
}

// Function which returns the remote backup dir
QString PRESERVER::getRemoteDir()
{
   QString result = "life-preserver";

   // Read in the result file
   QFile File( LIFEPRESERVERSDIR + "/" + connectHost + "/remote-dir" );
   if ( File.open( QIODevice::ReadOnly ) )
   {
     QTextStream stream( &File );
     QString line;
     while ( !stream.atEnd() ) {
       line = stream.readLine(); // line of text excluding '\n'
       // Return the first line
       return line;
     }
     File.close();
   }
  return result;
}

// Function which sets the remote backup dir
void PRESERVER::setRemoteDir(QString remoteDir)
{
   // Read in the result file
   QFile File( LIFEPRESERVERSDIR + "/" + connectHost + "/remote-dir" );
   if ( File.open( QIODevice::WriteOnly ) )
   {
     QTextStream streamout( &File );
     streamout << remoteDir;
     File.close();
   }
}

// Function which returns the remote backup dir
QStringList PRESERVER::getUserExcludes()
{
   QStringList excludes;
   // Read in the result file
   QFile File( LIFEPRESERVERSDIR + "/" + connectHost + "/exclude-list" );
   if ( File.open( QIODevice::ReadOnly ) )
   {
     QTextStream stream( &File );
     QString line;
     while ( !stream.atEnd() ) {
       line = stream.readLine(); // line of text excluding '\n'
       excludes << line;
     }
     File.close();
   }

  return excludes;
}

// Function which returns the include list
QStringList PRESERVER::getUserIncludes()
{
   QStringList includes;
   // Read in the result file
   QFile File( LIFEPRESERVERSDIR + "/" + connectHost + "/include-list" );
   if ( File.open( QIODevice::ReadOnly ) )
   {
     QTextStream stream( &File );
     QString line;
     while ( !stream.atEnd() ) {
       line = stream.readLine(); // line of text excluding '\n'
       includes << line;
     }
     File.close();
   }

  return includes;
}


// Function which returns the remote backup dir
void PRESERVER::setUserExcludes(QStringList excludes)
{
   // Save the updated config
   QFile File( LIFEPRESERVERSDIR + "/" + connectHost + "/exclude-list" );
   if ( File.open( QIODevice::WriteOnly ) )
   {
     QTextStream streamout( &File );
     for (int i = 0; i < excludes.size(); ++i)
        streamout << excludes.at(i) << "\n";
     File.close();
   }
}

// Function which sets the include list
void PRESERVER::setUserIncludes(QStringList includes)
{
   // Save the updated config
   QFile File( LIFEPRESERVERSDIR + "/" + connectHost + "/include-list" );
   if ( File.open( QIODevice::WriteOnly ) )
   {
     QTextStream streamout( &File );
     for (int i = 0; i < includes.size(); ++i)
        streamout << includes.at(i) << "\n";
     File.close();
   }
}

// Function which returns the last backup status + date
QString PRESERVER::getLastBackup()
{
   QString result = tr("No Previous Backup");

   // Read in the result file
   QFile pidFile( LIFEPRESERVERSDIR + "/" + connectHost + "/last-result" );
   if ( pidFile.open( QIODevice::ReadOnly ) )
   {
     QTextStream stream( &pidFile );
     QString line;
     while ( !stream.atEnd() ) {
       line = stream.readLine(); // line of text excluding '\n'
       // Return the first line
       return line;
     }
     pidFile.close();
   }

  return result;
}


// Function which returns the cron time for internal use
QString PRESERVER::getCronSettings()
{
   QString result = "disabled";

   // Read in the result file
   QFile fFile( LIFEPRESERVERSDIR + "/" + connectHost + "/frequency" );
   if ( fFile.open( QIODevice::ReadOnly ) )
   {
     QTextStream stream( &fFile );
     QString line;
     while ( !stream.atEnd() ) {
       line = stream.readLine(); // line of text excluding '\n'
       // Return the first line
       return line;
     }
     fFile.close();
   }
         
  return result;
}

// Function which returns the cron status of this backup for end user display
QString PRESERVER::getFrequency()
{
   QString result = "disabled";
 
   // Read in the result file
   QFile fFile( LIFEPRESERVERSDIR + "/" + connectHost + "/frequency" );
   if ( fFile.open( QIODevice::ReadOnly ) )
   {
     QTextStream stream( &fFile );
     QString line;
     while ( !stream.atEnd() ) {
       line = stream.readLine(); // line of text excluding '\n'
       // Return the first line
       return line;
     }
     fFile.close();
   }

  return result;
}

// Function which returns the PID of the current backup, or -1 if none is running
int PRESERVER::getPID()
{
   bool ok;
   int pid = -1;

   // Read in the pid file
   QFile pidFile( LIFEPRESERVERSDIR + "/" + connectHost + "/preserver.pid" );
   if ( pidFile.open( QIODevice::ReadOnly ) )
   {
     QTextStream stream( &pidFile );
     QString line;
     while ( !stream.atEnd() ) {
       line = stream.readLine(); // line of text excluding '\n'
       pid = line.toInt(&ok);
       break;
     }
     pidFile.close();
   }
  
  if ( pid != -1 )
  {
    // Check if this PID is alive
    QString cmd, tmp;
    tmp.setNum(pid);
    cmd = "ps -p " + tmp ;
    if ( system(cmd.toLatin1()) != 0 )
    {
       pidFile.remove();
       pid = -1;
    }
  }
  return pid;
}

QVariant PRESERVER::getConfValue(QString key)
{
   QString value;

   // Read in the old config and change the target key
   QFile confFile( LIFEPRESERVERSDIR + "/" + connectHost + "/settings.conf" );
   if ( confFile.open( QIODevice::ReadOnly ) )
   {
     QTextStream stream( &confFile );
     QString line;
     while ( !stream.atEnd() ) {
       line = stream.readLine(); // line of text excluding '\n'
       // Check if we are on a profile line
       if ( line.indexOf(key + ": ") == 0  ) {
          value = line.replace(key + ": ", "");
          return value;
       }
     }
     confFile.close();
   }

  return value;
}

// Function which returns a list of available backups to restore from
QString PRESERVER::listServerBackups()
{
  QString command;
  command = LIFEPRESERVERPATH + "/scripts/list-backup-rsync.sh " + connectHost;
  
  FILE *file = popen(command.toLatin1(),"r");

  char buffer[10000];

  QString line = "";
  char firstChar;

  if ((firstChar = fgetc(file)) != -1){
     line += firstChar;
     line += fgets(buffer,10000,file);
  }
  pclose(file);

  return line;
}

void PRESERVER::setConfValue(QString key, QVariant value)
{
   bool found;
   found = false;
   QStringList updatedConf;

   // Create our new Preserver Directory
   QDir newPreserverDir;
   newPreserverDir.setPath(LIFEPRESERVERSDIR + "/" + connectHost);
   if ( ! newPreserverDir.exists() )
      newPreserverDir.mkpath(newPreserverDir.path());

   // Read in the old config and change the target key
   QFile confFile( newPreserverDir.path() + "/settings.conf" );
   if ( confFile.open( QIODevice::ReadOnly ) )
   {
     QTextStream stream( &confFile );
     QString line;
     while ( !stream.atEnd() ) {
       line = stream.readLine(); // line of text excluding '\n'
       // Check if we are on a profile line
       if ( line.indexOf(key + ": ") == 0  ) {
          updatedConf << key + ": " + value.toString();
          found = true;
       } else {
          updatedConf << line;
       }
     }
     confFile.close();
   }

   // If we didn't find this key, add it in
   if (!found)
     updatedConf << key + ": " + value.toString();

   // Save the updated config
   if ( confFile.open( QIODevice::WriteOnly ) ) 
   {
     QTextStream streamout( &confFile );
     for (int i = 0; i < updatedConf.size(); ++i) 
        streamout << updatedConf.at(i) << "\n";
     confFile.close();
   }

}

