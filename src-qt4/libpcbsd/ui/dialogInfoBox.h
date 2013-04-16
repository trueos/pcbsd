
#ifndef DIALOGINFO_H
#define DIALOGINFO_H

#include "ui_dialogInfoBox.h"
#include <QDebug>
#include <QFile>
#include <QTranslator>
#include <qstring.h>

class dialogInfoBox : public QDialog, private Ui::dialogInfoBox
{
	Q_OBJECT

public:
        dialogInfoBox() : QDialog()
        {
          QTranslator translator;
          QLocale mylocale;
          QString langCode = mylocale.name();
          if ( ! QFile::exists( "/usr/local/share/lifePreserver/i18n/libpcbsd_" + langCode + ".qm" ) )
            langCode.truncate(langCode.indexOf("_"));
          translator.load( QString("libpcbsd_") + langCode, "/usr/local/share/lifePreserver/i18n/" );
          QCoreApplication::installTranslator( &translator );
          qDebug() << "Locale:" << langCode;

          setupUi(this);
        }

    void programInit(QString title);
    void setInfoText( QString text );


public slots:

protected:

private slots:
    void slotClose();

private:

signals:

} ;
#endif // DIALOGINFO_H
