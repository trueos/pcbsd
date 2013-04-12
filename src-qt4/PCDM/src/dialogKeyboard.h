/* PCDM Login Manager:
*  Written by Ken Moore (ken@pcbsd.org) 2012/2013
*  Copyright(c) 2013 by the PC-BSD Project
*  Available under the 3-clause BSD license
*
* Note: This file initially copied from the graphical PC-BSD installer 
*/

#ifndef DIALOGKEY_H
#define DIALOGKEY_H

#include "ui_dialogKeyboard.h"
#include <qstring.h>

class widgetKeyboard : public QWidget, private Ui::widgetKeyboard
{
	Q_OBJECT

public:
    widgetKeyboard(QWidget* parent = 0);

    void programInit(QStringList, QStringList);

public slots:

protected:

private slots:
    void slotClose();
    void slotApply();
    void slotCurrentKbLayoutChanged(int row);
    void slotUpdateKbOnSys();
    void slotCurrentKbVariantChanged(int row);
    void slotSelectedKbItemChanged();


private:
    void connectKeyboardSlots();
    void setKbVariants(const QString &kbLayout);
    void setKbDefaults();
    QStringList savedKeyVariants;
    QStringList keyboardModels;
    QStringList keyboardLayouts;

signals:

} ;
#endif // DIALOGKEY_H
