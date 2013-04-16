/***************************************************************************
 *   Copyright (C) 2006 - 2011 PC-BSD Software   *
 *   kris@pcbsd.org  *
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
/* QT4 */
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

/* Local includes */
#include "ui_lifePreserverUserExclude.h"
#include "lifePreserverUserExclude.h"
#include "preserver.h"
#include "externals.h"

void pUserExclude::widgetInit(QString preserver, bool mode)
{ 
   qDebug() << "Starting UserExcludes...";
   setWindowModality( Qt::WindowModal );

   // If true, editing excludes, otherwise editing include files
   excludeMode=mode;

   editPreserver = new PRESERVER(preserver);
   
   // Connect our slots for complete status checking
   connect(pushOk,SIGNAL(clicked()),this,SLOT(slotOK()));
   connect(pushCancel,SIGNAL(clicked()),this,SLOT(slotCancel()));
   connect(pushAdd,SIGNAL(clicked()),this,SLOT(slotAddExclude()));
   connect(pushRemove,SIGNAL(clicked()),this,SLOT(slotRemoveExclude()));
   connect(pushBrowse,SIGNAL(clicked()),this,SLOT(slotFindDirectory()));

   // Load the excludes
   QStringList curExcludes;
   if ( excludeMode ) {
      groupBox->setTitle(tr("Editing exclude list"));
      curExcludes = editPreserver->getUserExcludes();
   } else {
      groupBox->setTitle(tr("Editing include list"));
      curExcludes = editPreserver->getUserIncludes();
   }
   listExcludes->addItems(curExcludes);
}

void pUserExclude::slotAddExclude()
{
  QString newExclude = lineNewExclude->text();
  if( newExclude.isEmpty() ){ return; } //Do nothing if nothing given

  //Trim the file/wildcards off the end for the moment
  bool wild = FALSE;
  QString endfile;
  if(!QFile::exists(newExclude)){
    //file does not exists, probably a wildcard in the path
    wild=TRUE;
    endfile = newExclude.section("/",-1);  
    newExclude.remove(endfile);
  }

  //Check and replace symbolic links in the given path
  QDir dir(newExclude);
  QString canonical = dir.canonicalPath(); //removes symbolic links (if any)
  if(canonical != newExclude && !canonical.isEmpty()){
    qDebug() << "Symbolic links detected in the given path:"<< newExclude+endfile;
    qDebug() << " - Changing to real path:"<<canonical+"/"+endfile;

    newExclude = canonical;
  }

  //Add the file/wildcards on the end again
  if(wild){
    if(!newExclude.endsWith("/")){ newExclude.append("/"); }
    newExclude.append(endfile);
  }

  //Add it to the list
  listExcludes->addItem(newExclude);
  lineNewExclude->clear();
}

void pUserExclude::slotRemoveExclude()
{
  if ( ! listExcludes->currentItem() )
    return;

  listExcludes->takeItem(listExcludes->currentRow());
}

// Save the config to disk
void pUserExclude::savePreserver()
{
   QStringList excludes;
   for ( int z=0; z<listExcludes->count(); ++z)
      excludes << listExcludes->item(z)->text();
   if ( excludeMode )
     editPreserver->setUserExcludes(excludes);
   else
     editPreserver->setUserIncludes(excludes);
}

void pUserExclude::slotOK()
{
  savePreserver();
  close();
}

void pUserExclude::slotCancel()
{
  close();
}

void pUserExclude::slotFindDirectory()
{
  QString dirPath = QFileDialog::getExistingDirectory(this, tr("Select Directory"), QDir::homePath());
  if(dirPath.isEmpty()){return;} //action cancelled or closed
  lineNewExclude->setText(dirPath);
  
}

