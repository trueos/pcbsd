/*
Basic XDG compatibility framework for reading/using *.desktop files
Author: Ken Moore <ken@pcbsd.org> 3/25/14
*/
#ifndef _PCBSD_XDG_FILE_H
#define _PCBSD_XDG_FILE_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QProcess>
#include <QDebug>

#include <unistd.h>
#include <stdlib.h>

class XDGFile{
private:
	QString filepath, name, lname, gname, lgname, comment, lcomment, exec, tryexec,  cats, icon, runpath;
	bool hidden, terminal, nodisplay;

	QStringList quickRead();

public:
	XDGFile();
	~XDGFile();

	void LoadDesktopFile(QString filePath);

	//===================
	// == Standard Information ==
	//===================
	 // -- File Loaded --
	QString File(){ //return the absolute path to the currently loaded file
	  return filepath;
	}
	 // -- Name --
	QString Name(){ //will return localized name if there is one
	  if(lname.isEmpty()){ return name; }
	  else{ return lname; }
	}
	QString RawName(){
	  return name;
	}
	// -- Generic Name --
	QString GenericName(){ //will return localized generic name if there is one
	  if(lgname.isEmpty()){ return gname; }
	  else{ return lgname; }
	}
	QString RawGenericName(){
	  return gname;
	}
	// -- Comment --
	QString Comment(){ //will return localized comment if there is one
	  if(lcomment.isEmpty()){ return comment; }
	  else{ return lcomment; }
	}
	// -- Icon --
	QString Icon(){
	  return icon;
	}
	// -- Exec Line --
	QString RawExec(){
	  return exec;
	}
	// -- Path Line --
	QString Path(){
	  return runpath;
	}
	// -- TryExec Line --
	QString RawTryExec(){
	  return tryexec;
	}
	// -- Categories --
	QStringList Categories(){
	  return cats.split(";");
	}
	
	//===================
	// == Info Simplifications ==
	//===================
	
	// -- Check the validity of the application shortcut --
	bool isValid(){
	  //Get the binary to look for
	  QString chk = tryexec;
	  if(chk.isEmpty()){
	    chk = exec.section(" ",0,0, QString::SectionSkipEmpty);
	  }
	  if( chk.isEmpty() || exec.isEmpty() ){ return false; } //no exec/tryexec given
	  QFileInfo info(chk);
	  if(info.exists()){ return true; } //absolute path given (or local relative)
	  //Relative path given, check all PATH 's
	  QStringList paths = QString( getenv("PATH") ).split(":");
	  for(int i=0; i<paths.length(); i++){
	    if( QFile::exists(paths[i]+"/"+chk) ){ return true; } //binary found
	  }
	  //Not found - invalid application
	  return false;
	}
	
	// -- Check whether app is hidden --
	bool isHidden(){
	  return (hidden || nodisplay);
	}	
	
	// -- Get the full command that should be run --
	QString Exec(){
	  QString cmd = exec;
	  if(terminal){ cmd.prepend("xterm -e "); } //start this app in an xterm
	  cmd = cmd.section(" %",0,0); //cut off the file designations from the end
	  return cmd;
	}
	
	//==================
	// == Quick File Launch ==
	//==================
	void startDetached(){
	  QProcess::startDetached( this->Exec() );
	}
};

#endif