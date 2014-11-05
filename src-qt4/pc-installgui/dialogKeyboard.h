
#ifndef DIALOGKEY_H
#define DIALOGKEY_H

#include "ui_dialogKeyboard.h"
#include <qstring.h>

class widgetKeyboard : public QWidget, private Ui::widgetKeyboard
{
	Q_OBJECT

public:
        widgetKeyboard() : QWidget()
        {
          setupUi(this);
        }

    void programInit(QStringList, QStringList, QString, QString, QString);

public slots:

protected:

private slots:
    void slotClose();
    void slotApply();
    void slotCurrentKbLayoutChanged(int row);
    void slotUpdateKbOnSys(bool reset = false);
    void slotCurrentKbVariantChanged(int row);
    void slotSelectedKbItemChanged();


private:
    void connectKeyboardSlots();
    void setKbVariants(const QString &kbLayout);
    void setKbDefaults();
    QStringList savedKeyVariants;
    QStringList keyboardModels;
    QStringList keyboardLayouts;
    QString cKeyModel, cKeyLayout, cKeyVarient; //currently set (before dialog opened)

signals:
    void saved(QString, QString, QString);

} ;
#endif // DIALOGKEY_H
