/* PCDM Login Manager:
*  Written by Ken Moore (ken@pcbsd.org) 2012/2013
*  Copyright(c) 2013 by the PC-BSD Project
*  Available under the 3-clause BSD license
*
* Note: This file initially copied from the graphical PC-BSD installer 
*/

#ifndef DIALOGLOC_H
#define DIALOGLOC_H

#include <QStringList>
#include <QDir>
#include "ui_dialogLocale.h"

namespace Ui {
	class widgetLocale;
}

class widgetLocale : public QWidget, private Ui::widgetLocale
{
	Q_OBJECT

public:
    explicit widgetLocale(QWidget *parent = 0);
    ~widgetLocale();

    void setCurrentLocale(QString);

public slots:

protected:

private slots:
    void slotClose();
    void slotApply();

private:
    Ui::widgetLocale *ui;
    QString initLocale; //initial locale (for resetting back when cancelled)
    QStringList locL,locCodeL;
    
    
    QStringList getLanguages();
    
signals:
  void languageChanged(QString);
  
} ;
#endif // DIALOGLOC_H
