/***************************************************************************
 *   Copyright (C) 2008 by Tim McCormick   *
 *   tim@pcbsd.org   *
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
#include <QCoreApplication>
#include <QDebug>
#include <QDialog>
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include <QProcess>
#include <QProgressDialog>
#include <QMenu>
#include <QMessageBox>
#include <QTranslator>
#include <QTreeWidget>
#include <QWidget>

 
#ifndef _PCBSD_UTILS_H_
#define _PCBSD_UTILS_H_

class dialogWPAEnterprise;

class dialogWPAE:public QObject {
  Q_OBJECT

public:
  dialogWPAE();
  void setVariables( int type, QString EAPIdent, QString CACert, QString ClientCert, QString PrivKeyFile, QString Password, int keyMgmt );
  void show();
  void exec();

private:
  dialogWPAEnterprise *d_wpae;

private slots:
  void slotSigInt(int, QString, QString, QString, QString, QString, int);

signals:
  void saved(int, QString, QString, QString, QString, QString, int);

};

class dialogInfoBox; 
class dialogInfo {

public:
  dialogInfo();
  void programInit(QString title);
  void setInfoText(QString text);
  void show();

private:
  dialogInfoBox *d_p;
};

class dialogMetaProgress;
class metaProgressDialog:public QObject {

  Q_OBJECT

public:
  metaProgressDialog();
  void programInit(QString desc);
  void setTotalDesc(QString desc);
  void setSubDesc(QString desc);
  void setTotalRange(int min, int max);
  void setTotalVal(int val);
  int  getTotalVal();
  void setSubRange(int min, int max);
  void setSubVal(int val);
  int  getSubVal();
  void setDLVisible(bool);
  void setDLDesc(QString desc);
  void setDLRange(int min, int max);
  void setDLVal(int val);
  int  getDLVal();
  void show();
  void close();
  void hide();

private slots:
  void slotCancel();

private:
  dialogMetaProgress *d_p;

signals:
  void canceled();

};


class Utils
{
public:
   static QString sysctl(QString sysctl);
   static long long sysctlAsInt(QString sysctl);
   static QString bytesToHumanReadable(long long bytes);
   static QString getValFromPBIConf(QString key);
   static QString getValFromPCBSDConf(QString key);
   static QString getValFromPCConf(QString conf, QString key);
   static QString getProxyURL();
   static QString getProxyUser();
   static QString getProxyPass();
   static QString getProxyType();
   static QString getProxyPort();
   static QString getMasterMirror();
   static bool setValPBIConf(QString key, QString val);
   static bool setValSHFile(QString, QString, QString);
   static bool setValCSHFile(QString, QString, QString);
   static bool setValPCBSDConf(QString key, QString val);
   static bool setValPCConf(QString conf, QString key, QString val);
   static bool setProxyAddress(QString val);
   static bool setProxyAuth(QString val);
   static bool setMasterMirror(QString val);
   static bool setConfFileValue( QString oFile, QString oldKey, QString newKey );
   static bool setConfFileValue( QString oFile, QString oldKey, QString newKey, int occur );
   static QString getConfFileValue( QString oFile, QString Key );
   static QString getConfFileValue( QString oFile, QString Key, int occur );
   static QString getConfFileValue( QString oFile, QString Key, QString ValRx, int occur );
   static QStringList runShellCommand( QString command );
   static QString runShellCommandSearch( QString command, QString sch );
   static QStringList listShellCommandSearch( QString command, QString sch );
   static QStringList quickUserInputBox( QString title, QStringList inputList );
   static QString getLineFromCommandOutput( QString cmd );
   static QString getValFromSHFile(QString envFile, QString envVal);
   static QString getValFromCSHFile(QString envFile, QString envVal);
   static void restartNetworking();
   static void runInTerminal(QString command, QString windowTitle=QString());
   static void openInFileManager(QString location);
   static bool validateIPV6(QString);
   static bool validateIPV4(QString);

};

class metaWidget : public QWidget
{
        Q_OBJECT

public:
        metaWidget() : QWidget()
        {
	  QTranslator translator;
          QLocale mylocale;
          QString langCode = mylocale.name();
          if ( ! QFile::exists( "/usr/local/share/lifePreserver/i18n/libpcbsd_" + langCode + ".qm" ) )
            langCode.truncate(langCode.indexOf("_"));
          translator.load( QString("libpcbsd_") + langCode, "/usr/local/share/lifePreserver/i18n/" );
          QCoreApplication::installTranslator( &translator );
          qDebug() << "Locale:" << langCode;
        }

     void init(QString chroot);
     void setPackageSet(QString set);
     void stop();

public slots:

private slots:
   void slotMetaRightClick();
   void slotMetaViewPkgs();
   void slotDeskPkgsChanged(QTreeWidgetItem *aItem, int __unused);
   void slotApplyMetaChanges();
   void slotMetaAddDone();
   void slotMetaAddCanceled();
   void slotMetaAddRead(); 
   void slotMetaDelDone();
   void slotMetaDelCanceled();
   void slotMetaDelRead();
   void startMetaChanges();
   void slotFinishLoadingMetaPkgs();
   void slotGetPackageDataOutput();

private:
   QTreeWidget *treeMetaPkgs;
   QLabel *label_2;
   QPushButton *pushApplyMeta;
   QList<QStringList> metaPkgList;
   QStringList tmpMetaPkgList;
   QProcess *addMetaProc;
   QProcess *delMetaProc;
   QProcess *getMetaProc;
   QProgressDialog *delprogress;
   QProgressDialog *addprogress;
   QString addPkgs;
   QString delPkgs;
   QString pkgSource;
   QString Arch;
   QString chrootArg1;
   QString chrootArg2;
   bool stopped;
   bool canceled;
   QString PkgSet;

   QMenu *popup;
   metaProgressDialog *metaProgressDel;
   metaProgressDialog *metaProgressAdd;
   dialogInfo *dIB;
   

   void uncheckAllChildrenPkgs(QString parent);
   void checkAllChildrenPkgs(QString parent);
   bool allChildrenPkgsUnchecked(QString parent);
   bool allChildrenPkgsChecked(QString parent);
   QString getDelPkgs();
   QString getAddPkgs();
   bool haveMetaPkgChanges();
   bool haveAMetaDesktop();
   void saveMetaPkgs();
   void parseTmpMetaList();
   bool isMetaPkgInstalled(QString mPkg);
   void addTreeItems(QString parent);
   void populateMetaPkgs();
   QString getLineFromCommandOutput( QString cmd );

signals:

} ;

#endif
