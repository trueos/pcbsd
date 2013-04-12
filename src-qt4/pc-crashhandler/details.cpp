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
#include "details.h"
#include <qtextstream.h>
#include <qfiledialog.h>


void Details::populate( QString stdout, QString stderr )
{
    stdoutBox->setText(stdout);
    stderrBox->setText(stderr);
}


void Details::saveOutput()
{
    QString saveLocation = QFileDialog::getSaveFileName(this, tr("Save File"),
                            "/home",
                            tr("Diagnostic Text (*.txt)"));
    
    if (saveLocation == "") {
	return;
    }
    
    if (QFile::exists(saveLocation)) {
	int result = QMessageBox::question(this, tr("Overwrite File?"), tr("File already exists, would you like to overwrite it?"), tr("&Yes"), tr("&No"), QString::null, 0, 1);
	if (result == 1) {
	    return;
	}
    }
    
    //Write the file
    QFile file(saveLocation);
    if ( file.open( QIODevice::WriteOnly ) ) {
	QTextStream stream( &file );
	stream.setCodec("UTF-8");
	stream << "Output logged from stdout\n";
	stream << "=========================\n\n";
	stream << stdoutBox->toPlainText() << "\n\n";
	stream << "Output logged from stderr\n";
	stream << "=========================\n\n";
	stream << stderrBox->toPlainText();
	file.close();
    }
}
