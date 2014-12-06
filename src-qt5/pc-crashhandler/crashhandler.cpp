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
#include "crashhandler.h"
#include <qtextstream.h>


void CrashHandler::setup( QString name, QString stdout, QString stderr )
{
    //Alter the dialog to display our program name, placeholder is # so just replace that.
    text->setText(tr("PC-BSD encountered an error while running:") + " <b>" + name + "</b><br><br>" \
                  + tr("For more information on this error, click Details.") );

    //Read files
    QFile file(stdout);
    if ( file.open( QIODevice::ReadOnly ) ) {
	QTextStream stream( &file );
	stream.setCodec("UTF-8");
	QString line;
	while ( !stream.atEnd() ) {
	    line = stream.readLine(); // line of text excluding '\n'
	    stdoutTxt += (line + "\n");
	}
	file.close();
	file.remove();
    }
    else {
	stdoutTxt = "Error: Unable to open " + stderr + "!";
    }
    QFile file2(stderr);
    if ( file2.open( QIODevice::ReadOnly ) ) {
	QTextStream stream( &file2 );
	stream.setCodec("UTF-8");
	QString line;
	while ( !stream.atEnd() ) {
	    line = stream.readLine(); // line of text excluding '\n'
	    stderrTxt += (line + "\n");
	}
	file2.close();
	file2.remove();
    }
    else {
	stderrTxt = "Error: Unable to open " + stdout + "!";
    }
    
    details.populate(stdoutTxt, stderrTxt);
}


void CrashHandler::showDetails()
{
    details.show();
}
