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
#ifndef _APPCAFE_LARGE_WIDGET_H
#define _APPCAFE_LARGE_WIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QLabel>
#include <QGridLayout>
#include <QAction>
#include <QString>
#include <QIcon>

class LargeItemWidget : public QWidget{
	Q_OBJECT
	
  private:
    QString uniqueAppID; //should be something unique like <name>-<version>-<arch>	
    
  signals:
    void appClicked(QString);
    
  private slots:
    void sendSignal(){ emit appClicked(uniqueAppID); }
    
  public:
    LargeItemWidget(QString appID, QString name, QString description, QString icon) : QWidget(){
      QGridLayout *layout = new QGridLayout;
      //Create the toolbutton
      QToolButton *button = new QToolButton();
        button->setIconSize(QSize(34,34));
          QAction *act = new QAction( QIcon(icon), "", 0);
        button->setDefaultAction(act);
        button->setToolButtonStyle(Qt::ToolButtonIconOnly);

      //Create the labels
      QLabel *appName = new QLabel("<b>"+name+"</b>");
      QLabel *appDesc = new QLabel(description);
        appDesc->setWordWrap(TRUE);
      //Add the items to the widget
      layout->addWidget(button,0,0,2,1);
      layout->addWidget(appName,0,1);
      layout->addWidget(appDesc,1,1);
      this->setLayout(layout);
      //Save the app identifier
      uniqueAppID=appID;
      connect(button,SIGNAL(triggered(QAction*)), this, SLOT(sendSignal()) );
    }
    virtual ~LargeItemWidget(){}
 
  	  
};

#endif
