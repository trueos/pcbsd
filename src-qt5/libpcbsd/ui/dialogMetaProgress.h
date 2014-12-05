
#ifndef DIALOGMETAPROGRESS_H
#define DIALOGMETAPROGRESS_H

#include "ui_dialogMetaProgress.h"
#include <QFile>
#include <QDebug>
#include <QTranslator>
#include <qstring.h>

class dialogMetaProgress : public QDialog, private Ui::dialogMetaProgress
{
	Q_OBJECT

public:
        dialogMetaProgress() : QDialog()
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

    void programInit(QString desc);
    void setTotalDesc(QString desc);
    void setTotalRange(int min, int max);
    void setTotalVal(int val);
    int  getTotalVal();
    void setSubDesc(QString desc);
    void setSubRange(int min, int max);
    void setSubVal(int val);
    int  getSubVal();

    void setDLVisible(bool);
    void setDLDesc(QString desc);
    void setDLRange(int min, int max);
    void setDLVal(int val);
    int  getDLVal();

public slots:

protected:

private slots:

private:
    QString PkgSet;

protected:
   void closeEvent(QCloseEvent *event);

signals:
   void canceled();

} ;
#endif // DIALOGMETAPROGRESS_H
