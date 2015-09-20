#include "pcbsd-ui.h"

#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>

#include <qtranslator.h>
#include <qlocale.h>
#include <QFile>

#include "dialogMetaProgress.h"
#include "dialogwpaenterprise.h"
#include "dialogInfoBox.h"

///////////////////////////////////////////////////////////////////////////////
QStringList pcbsd::UI::quickUserInputBox(QString title, QStringList inputList){
  //inputList:=  list of labels, one entry for each user input required
  //returns:=  list of user inputs - 1 per label in inputList, element number corresponds to input labels

  //Create the dialog
  QDialog *inputDialog = new QDialog;
  inputDialog->setModal(true);
  inputDialog->setWindowTitle(title);
  //Add a label and editbox for each row (one per input in list)
  QList<QLineEdit *> iboxes;
  QFormLayout *layout = new QFormLayout;
  for(int i=0; i<inputList.length(); i++){
    //Create the widgets
    QLabel *label = new QLabel(inputList[i]);
    iboxes << new QLineEdit;
    //add them to the layout
    layout->addRow(label,iboxes[i]);
  }
  //Add the "Done" button to the bottom
  QPushButton *pushdone = new QPushButton("Done");
  const QString blank = "";
  layout->addRow(blank,pushdone);
  inputDialog->setLayout(layout);
  QObject::connect(pushdone,SIGNAL(clicked()),inputDialog,SLOT(accept()));

  //Initialize output list
  QStringList outputList;

  //Start the dialog
  if( inputDialog->exec() == QDialog::Accepted ){ //If the dialog was not cancelled
    //Retrieve the user inputs and return them for each edit box created (in order)
    for(int i=0; i<iboxes.length(); i++){
      outputList << iboxes[i]->text();
    }
  }

  delete inputDialog; //delete the dialog - we are done with it
  return outputList; //Ouput will be empty if an error or if box cancelled
}

// PLMPL class for dialogInfoBox
dialogInfo::dialogInfo() {
  d_p = new dialogInfoBox;
}

void dialogInfo::programInit(QString title)
{
  d_p->programInit(title);
}

void dialogInfo::setInfoText(QString text)
{
  d_p->setInfoText(text);
}

void dialogInfo::show()
{
  d_p->show();
}

// PLMPL class for metaProgressDialog
//////////////////////////////////////////
metaProgressDialog::metaProgressDialog() {
  d_p = new dialogMetaProgress;
  d_p->setWindowModality(Qt::ApplicationModal);
  d_p->connect( (QObject*)d_p, SIGNAL(canceled()), (QObject*)this, SLOT(slotCancel()));
}

void metaProgressDialog::programInit(QString desc) {
  d_p->programInit(desc);
}

void metaProgressDialog::setTotalDesc(QString desc) {
  d_p->setTotalDesc(desc);
}

void metaProgressDialog::setSubDesc(QString desc) {
  d_p->setSubDesc(desc);
}

void metaProgressDialog::setTotalRange(int min, int max) {
  d_p->setTotalRange(min, max);
}

void metaProgressDialog::setTotalVal(int val) {
  d_p->setTotalVal(val);
}

int metaProgressDialog::getTotalVal() {
  return d_p->getTotalVal();
}

void metaProgressDialog::setSubRange(int min, int max) {
  d_p->setSubRange(min, max);
}

void metaProgressDialog::setSubVal(int val) {
  d_p->setSubVal(val);
}

int metaProgressDialog::getSubVal() {
  return d_p->getSubVal();
}

int metaProgressDialog::getDLVal() {
  return d_p->getDLVal();
}

void metaProgressDialog::setDLVisible(bool vis) {
  d_p->setDLVisible(vis);
}

void metaProgressDialog::setDLVal(int val) {
  d_p->setDLVal(val);
}

void metaProgressDialog::setDLRange(int min, int max) {
  d_p->setDLRange(min, max);
}

void metaProgressDialog::setDLDesc(QString desc) {
  d_p->setDLDesc(desc);
}

void metaProgressDialog::show() {
  d_p->show();
}

void metaProgressDialog::close() {
  d_p->close();
}

void metaProgressDialog::hide() {
  d_p->hide();
}

void metaProgressDialog::slotCancel() {
  emit canceled();
}


// PLMPL class for dialogwpaenterprise
dialogWPAE::dialogWPAE() {
  d_wpae = new dialogWPAEnterprise;
  d_wpae->init();
}

void dialogWPAE::show() {
  QObject::connect( (QObject*)d_wpae, SIGNAL( saved(int, QString, QString, QString, QString, QString, QString, int, int) ), (QObject*)this, SLOT( slotSigInt(int, QString, QString, QString, QString, QString, QString, int, int) ) );
  d_wpae->show();
}

void dialogWPAE::exec() {
  QObject::connect( (QObject*)d_wpae, SIGNAL( saved(int, QString, QString, QString, QString, QString, QString, int, int) ), (QObject*)this, SLOT( slotSigInt(int, QString, QString, QString, QString, QString, QString, int, int) ) );
  d_wpae->exec();
}

void dialogWPAE::setVariables( int type, QString EAPIdent, QString AnonIdent, QString CACert, QString ClientCert, QString PrivKeyFile, QString Password, int keyMgmt, int EAPPhase2 )
{
  d_wpae->setVariables(type, EAPIdent, AnonIdent, CACert, ClientCert, PrivKeyFile, Password, keyMgmt, EAPPhase2 );
}

void dialogWPAE::slotSigInt(int type, QString EAPIdent, QString AnonIdent, QString CACert, QString CliCert, QString PrivKeyFile, QString Pass, int kMgmt, int EAPPhase2)
{
  emit saved(type, EAPIdent, AnonIdent, CACert, CliCert, PrivKeyFile, Pass, kMgmt, EAPPhase2);
}

//=== Old Qt4 Code ===
/*
///////////////////////////////////////////////////////////////////////////////
static bool initTranslator(QTranslator& translator, QString appname)
{
    appname = appname.trimmed();
    appname = appname.mid(appname.lastIndexOf("/"));

    //try to load translation
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/pcbsd/i18n/" + appname +"_" + langCode + ".qm" ) )
          langCode.truncate(langCode.indexOf("_"));
    translator.load( appname +"_" + langCode, PREFIX + "/share/pcbsd/i18n/" );
    return true;
}


///////////////////////////////////////////////////////////////////////////////
PCApplication::PCApplication(int &argc, char **argv, bool GUIenabled)
    :QApplication(argc, argv, GUIenabled)
{
    loadTranslations(argv[0]);
}

///////////////////////////////////////////////////////////////////////////////
PCApplication::PCApplication(int &argc, char **argv, Type type)
    :QApplication(argc, argv, type)
{
    loadTranslations(argv[0]);
}

///////////////////////////////////////////////////////////////////////////////
PCApplication::PCApplication(int &argc, char **argv, const QString& appname)
    :QApplication(argc, argv)
{
    loadTranslations(appname);
}

///////////////////////////////////////////////////////////////////////////////
bool PCApplication::loadTranslations(QString appname)
{
    //try to load translation
    QTranslator translator;
    if (initTranslator(translator, appname))
    {
        installTranslator( &translator );
        return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
PCSingleApplication::PCSingleApplication(int &argc, char **argv, bool GUIenabled)
    :QtSingleApplication(argc, argv, GUIenabled)
{
    loadTranslations(argv[0]);
}

///////////////////////////////////////////////////////////////////////////////
PCSingleApplication::PCSingleApplication(const QString &id, int &argc, char **argv)
    :QtSingleApplication(id, argc, argv)
{
    loadTranslations(argv[0]);
}

///////////////////////////////////////////////////////////////////////////////
PCSingleApplication::PCSingleApplication(int &argc, char **argv, Type type)
    :QtSingleApplication(argc, argv, type)
{
    loadTranslations(argv[0]);
}

///////////////////////////////////////////////////////////////////////////////
PCSingleApplication::PCSingleApplication(int &argc, char **argv, const QString& appname)
    :QtSingleApplication(argc, argv)
{
    loadTranslations(appname);
}

///////////////////////////////////////////////////////////////////////////////
bool PCSingleApplication::loadTranslations(QString appname)
{
    //try to load translation
    QTranslator translator;
    if (initTranslator(translator, appname))
    {
        installTranslator( &translator );
        return true;
    }
    return false;
}*/
