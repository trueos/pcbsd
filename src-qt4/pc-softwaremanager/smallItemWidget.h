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
#ifndef _APPCAFE_SMALL_WIDGET_H
#define _APPCAFE_SMALL_WIDGET_H

#include <QToolButton>
#include <QAction>
#include <QString>
#include <QIcon>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QSize>

class SmallItemWidget : public QWidget{
	Q_OBJECT
	
  private:
    QString uniqueAppID;	
    
  signals:
    void appClicked(QString);
    
  private slots:
    void sendSignal(){ emit appClicked(uniqueAppID); }
    
  public:
    SmallItemWidget(QString appID, QString name, QString icon, QString version) : QWidget(){
      //Create the items
      
      QToolButton* button = new QToolButton();
        button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	button->setFixedSize(QSize(64,64));
        button->setIconSize(QSize(22,22));
      QAction* act = new QAction(QIcon(icon),"",this);
	name = this->fontMetrics().elidedText(name, Qt::ElideRight ,60);
        if(version.isEmpty()){ act->setText(name); }
        else{ 
	  version = this->fontMetrics().elidedText(version, Qt::ElideRight ,60);
	  act->setText(name+"\n"+version); 
	}
        button->setDefaultAction(act);
      //QLabel* label = new QLabel();
        //label->setWordWrap(TRUE);
        //label->setText(version);
      //Now add them to the widget
      QVBoxLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(1,1,1,1);
        layout->addWidget(button);
        //layout->addWidget(label);
      this->setLayout(layout);
      //Save the app identifier and connect the signal/slot
      uniqueAppID=appID;
      connect(button,SIGNAL(triggered(QAction*)), this, SLOT(sendSignal()) );
    }
    virtual ~SmallItemWidget(){}
 
  	  
};

#endif
