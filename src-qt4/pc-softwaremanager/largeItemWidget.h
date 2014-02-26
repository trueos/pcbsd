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
#include <QHBoxLayout>
#include <QPixmap>

class LargeItemWidget : public QWidget{
	Q_OBJECT
	
  private:
    QString uniqueAppID; //should be something unique like <name>-<version>-<arch>	
    QLabel *type;
  signals:
    void appClicked(QString);
    
  private slots:
    void sendSignal(){ emit appClicked(uniqueAppID); }
    
  public:
    LargeItemWidget(QString appID, QString name, QString description, QString icon) : QWidget(){

      //Create the toolbutton
      QToolButton *button = new QToolButton();
        button->setIconSize(QSize(34,34));
          QAction *act = new QAction( QIcon(icon), "", 0);
        button->setDefaultAction(act);
        button->setToolButtonStyle(Qt::ToolButtonIconOnly);

      //Create the labels
      QLabel *appName = new QLabel("<b>"+name+"</b>");
	    appName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
      QLabel *appDesc = new QLabel(description);
        appDesc->setWordWrap(TRUE);
      type = new QLabel();
	    //type->setScaledContents(true);
	    type->setVisible(false);
      QHBoxLayout *hl = new QHBoxLayout;
	    hl->addWidget(appName);
	    hl->addWidget(type);
      //Add the items to the widget
      QGridLayout *layout = new QGridLayout;
      layout->addWidget(button,0,0,2,1);
      layout->addLayout(hl,0,1);
      layout->addWidget(appDesc,1,1);
      this->setLayout(layout);
      
      //Save the app identifier
      uniqueAppID=appID;
      connect(button,SIGNAL(clicked()), this, SLOT(sendSignal()) );
    }
    virtual ~LargeItemWidget(){}
    
    void setType(QString typ){
      if(typ.toLower()=="graphical"){
        type->setPixmap(QPixmap(":icons/graphicalapp.png").scaled(16,16));
	type->setToolTip(tr("Graphical Application"));
        type->setVisible(true);
      }else if(typ.toLower()=="text"){
        type->setPixmap(QPixmap(":icons/textapp.png").scaled(16,16));
	type->setToolTip(tr("Command-line Application"));
        type->setVisible(true);	      
      }else if(typ.toLower()=="server"){
        type->setPixmap(QPixmap(":icons/serverapp.png").scaled(16,16));
	type->setToolTip(tr("Server Application"));
        type->setVisible(true);	      
      }else{
        type->setVisible(false);	      
      }
    }
    
  protected:
    void mousePressEvent(QMouseEvent *ev){ Q_UNUSED(ev); sendSignal(); }  
 
  	  
};

#endif
