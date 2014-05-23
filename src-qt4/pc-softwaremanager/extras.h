/***************************************************************************
 *   Copyright (C) 2011 - iXsystems				 	  *
 *   kris@pcbsd.org  *
 *   tim@pcbsd.org   *
 *   ken@pcbsd.org   *
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
#ifndef _APPCAFE_EXTRAS_H
#define _APPCAFE_EXTRAS_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QProcessEnvironment>
#include <QProcess>
#include <QCoreApplication>

class Extras{

public:
  static bool checkUser(){
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString logname;
    if(env.contains("LOGNAME")){ logname = env.value("LOGNAME"); }
    else if(env.contains("USERNAME")){ logname = env.value("USERNAME"); }
    else if(env.contains("USER")){ logname = env.value("USER"); }
    bool ok = (logname.toLower()=="root");
    return ok;
  }
  
  static QString bytesToHumanReadable(float bytes){
    QStringList suffix;
    suffix << "B" << "KB" << "MB" << "GB" << "TB" << "PB";
    int i = 0;
    while((bytes >= 1000) && ( i < (suffix.size() - 1 )) ){
	bytes = bytes / 1024;  i++;
    }
    QString result = QString::number(bytes, 'f', 0);
    result += suffix[i];
    return result;
  }
  
  static QString getLineFromCommandOutput( QString command ){
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
  
 static QString getSystemArch(){
    return getLineFromCommandOutput("uname -m").simplified();
 }
 
 static QString getRegularUser(){
    QString user = QString( getenv("SUDO_USER") );
    if(user.isEmpty()){ user = QString( getenv("LOGNAME") ); }
    if(user.toLower()=="root"){ user.clear(); }
    return user;
 }
 
 static QStringList readFile( QString filePath ){
   QFile file(filePath);
   QStringList output;
   if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
     QTextStream in(&file);
     while(!in.atEnd()){
       QString line = in.readLine();
       if(!line.isEmpty()){ output << line; }
     }
     file.close();
   }
   return output;
 }
 
 static bool writeFile( QString filePath, QStringList contents ){
   QFile file(filePath);
   bool ok = FALSE;
   if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
     QTextStream out(&file);
     for(int i=0; i<contents.length(); i++){
       out << contents[i] + "\n";	     
     }
     file.close();
     ok = TRUE;
   }
   return ok;
 }
 
 static QString nameToID(QString name){
   QString out = name.toLower();
   out.remove(" ");
   out.remove("\t");
   out = out.simplified();
   return out;
 }
 
 static bool newerDateTime(QString check, QString standard){
   //Returns true if the first input is a later date/time than the second
   double chkNum = check.remove(" ").toDouble();
   double stdNum = standard.remove(" ").toDouble();
   return (chkNum > stdNum);
 }
 
 static QString datetimeToString(QString datetime){
   //datetime format: "YYYYMMDD HHMMSS"
   //converts a database date/time string to human readable form
   QString date = datetime.section(" ",0,0,QString::SectionSkipEmpty);
   //date format: YYYYMMDD
   QString year = date.left(4);
   QString day = QString::number(date.right(2).toInt());
   date.chop(2); // remove the day
   QString month = QString::number(date.right(2).toInt());
   
   QString output = month+"/"+day+"/"+year;
   return output;
 }
 
 static QString sizeKToDisplay(QString sizeK){
   double num = sizeK.toDouble();
   QStringList lab; lab << "KB" << "MB" << "GB" << "TB" << "PB";
   int i=0;
   while( (i<lab.length()) && (num > 1024) ){
     num=num/1024; i++;	   
   }
   //Round to 2 decimel places
   num = int(num*100)/100.0;
   QString output = QString::number(num)+" "+lab[i];
   //qDebug() << "Size calculation:" << sizeK << output;
   return output;
 }
 
static QStringList getCmdOutput(QString cmd, QString dir = ""){
  QProcess *proc = new QProcess;
  proc->setProcessChannelMode(QProcess::MergedChannels);
  if( !dir.isEmpty() && QFile::exists(dir) ){ proc->setWorkingDirectory(dir); }
  proc->start(cmd);
  while(!proc->waitForFinished(300)){
    QCoreApplication::processEvents();
  }
  QStringList out = QString(proc->readAllStandardOutput()).split("\n");
  if(out.length()>0){
    if(out[out.length()-1].isEmpty()){ out.removeAt(out.length()-1); } //remove any extra line at the end
  }
  delete proc;	
  return out;
}
 
};

#endif
