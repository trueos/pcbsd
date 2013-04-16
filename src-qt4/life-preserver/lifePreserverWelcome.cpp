/***************************************************************************
 *   Copyright (C) 2006 - 2011 PC-BSD Software  			   *
 *   kris@pcbsd.org 							   *
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
 &nimize*                                                                         *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/
/* QT4 */
#include <QPixmap>

/* Local includes */
#include "ui_lifePreserverWelcome.h"
#include "lifePreserverWelcome.h"
#include "ui_lifePreserverWizard.h"
#include "lifePreserverWizard.h"
#include "externals.h"

void lifePreserverWelcome::ProgramInit()
{ 
   setWindowIcon(QIcon(QString::fromUtf8(":/images/images/lifepreserver.png")));

   connect(pushWizard, SIGNAL( clicked() ), this, SLOT(slotPushWizardClicked() ) );
}

void lifePreserverWelcome::slotPushWizardClicked()
{
   lpw = new lifePreserverWizard();
   lpw->widgetInit();
   lpw->setWindowModality( Qt::ApplicationModal );
   connect(lpw, SIGNAL( added() ), this, SLOT(slotWizardDone() ) );
   lpw->show();
}

void lifePreserverWelcome::slotWizardDone()
{
   hide();
   // See if we need to start a backup now
   QString cronscript = LIFEPRESERVERPATH + "/scripts/check-backups.sh &";
   system(cronscript.toLatin1());
   m->loadPreservers();
   m->show();
}

void lifePreserverWelcome::closeEvent(QCloseEvent *event)
{
   if(isVisible() )
     hide();
   event->ignore();
}
