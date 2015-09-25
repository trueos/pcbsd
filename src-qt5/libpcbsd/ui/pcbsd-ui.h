/***************************************************************************
 *   Copyright (C) 2013 by Yuriy Momotyuk                  				   *
 *   yurkis@gmail.com                                                      *
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
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/

#ifndef PCBSDUI_H
#define PCBSDUI_H

#include <QApplication>
#include <QStringList>
#include <QTreeWidgetItem>
#include <QProgressDialog>
#include <QTranslator>
#include <QMessageBox>
#include <QMenu>
#include <QProcess>
#include <QFile>
#include <QDebug>

#include "../../config.h"

namespace pcbsd
{

class UI
{
   public:
    static QStringList quickUserInputBox( QString title, QStringList inputList );
};

}

//== Old Qt4 Code ==
/*
class PCApplication: public QApplication
{
    Q_OBJECT

public:
    PCApplication(int &argc, char **argv, bool GUIenabled = true);
    PCApplication(int &argc, char **argv, Type type);
    PCApplication(int &argc, char **argv, const QString& appname);

    bool loadTranslations(QString appname);
};

class PCSingleApplication: public QtSingleApplication
{
    Q_OBJECT

public:
    PCSingleApplication(int &argc, char **argv, bool GUIenabled = true);
    PCSingleApplication(const QString &id, int &argc, char **argv);
    PCSingleApplication(int &argc, char **argv, Type type);

    PCSingleApplication(int &argc, char **argv, const QString& appname);

    bool loadTranslations(QString appname);

protected:

    virtual bool onSecondInstance(QStringList args) {(void)args;return true;}

};*/

class dialogWPAEnterprise;

class dialogWPAE:public QObject {
  Q_OBJECT

public:
  dialogWPAE();
  void setVariables( int type, QString EAPIdent, QString AnonIdent, QString CACert, QString ClientCert, QString PrivKeyFile, QString Password, int keyMgmt, int EAPPhase2 );
  void show();
  void exec();

private:
  dialogWPAEnterprise *d_wpae;

private slots:
  void slotSigInt(int, QString, QString, QString, QString, QString, QString, int, int);

signals:
  void saved(int, QString, QString, QString, QString, QString, QString, int, int);

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

#endif // PCBSDUI_H
