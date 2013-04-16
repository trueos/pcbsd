#ifndef DIALOGWPAENTERPRISE_H
#define DIALOGWPAENTERPRISE_H

#include <QDebug>
#include <QDialog>
#include <QFile>
#include <QMessageBox>
#include <QTranslator>
#include "ui_dialogwpaenterprise.h"

class dialogWPAEnterprise : public QDialog, private Ui::dialogWPAEnterprise
{
        Q_OBJECT

public:
        dialogWPAEnterprise() : QDialog()
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
	  setWindowModality(Qt::ApplicationModal);
        }

    void setVariables( int type, QString EAPIdent, QString CACert, QString ClientCert, QString PrivKeyFile, QString Password, int keyMgmt );



public slots:

private slots:
    void slotTypeChanged();
    void slotClose();
    void slotSelectCACert();
    void slotSelectClientCert();
    void slotSelectPrivateKeyFile();
    void slotShowKey();

private:

signals:
  void saved(int, QString, QString, QString, QString, QString, int);

} ;
#endif // DIALOGWPAENTERPRISE_H

