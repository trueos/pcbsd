/* PCDM Login Manager:
*  Written by Ken Moore (ken@pcbsd.org) 2012/2013
*  Copyright(c) 2013 by the PC-BSD Project
*  Available under the 3-clause BSD license
*
* Note: This file initially copied from the graphical PC-BSD installer 
*/

#include <QDebug>
#include <QFile>
#include "dialogLocale.h"
#include "ui_dialogLocale.h"

widgetLocale::widgetLocale(QWidget *parent) : QWidget(parent), ui(new Ui::widgetLocale)
{
  //Initialize the Interface
  ui->setupUi(this);
  
  //Connect the signals/slots
  connect(ui->pushCancel, SIGNAL(clicked()), this, SLOT(slotClose()) );
  connect(ui->pushApply, SIGNAL(clicked()), this, SLOT(slotApply()) );
  //Get all the available languages
  locCodeL = getLanguages(); //get language codes
  locL.clear();
  for(int i=0; i<locCodeL.length(); i++){
    QLocale locale(locCodeL[i]);
    locL << QLocale::languageToString(locale.language());
  }
  //Get the current language
  QLocale mylocale = QLocale::system();
  initLocale = mylocale.name(); //save this for later
  QString cLocale = QLocale::languageToString(mylocale.language()) + " ("+mylocale.name()+")";
  //qDebug() << "Default Locale:" << cLocale;
  //Setup the interface
  ui->lineCurrentLocale->setText(cLocale); //current language
  for(int i=0; i< locL.length(); i++){
    ui->localeListWidget->addItem(locL[i] + " ("+locCodeL[i]+")"); //Available languages
  }
  ui->localeListWidget->sortItems(); //Sort them by translated name
}

widgetLocale::~widgetLocale(){
}

void widgetLocale::setCurrentLocale(QString localeCode){
  QLocale locale(localeCode);
  //Change the widget Locale
  this->setLocale(locale);
  //Setup the interface  
  QString cLocale = QLocale::languageToString(locale.language()) + " ("+locale.name()+")";
  ui->lineCurrentLocale->setText(cLocale); //current language
  ui->retranslateUi(this);
}

void widgetLocale::slotClose()
{
  emit languageChanged(initLocale); //send the signal about the initial language we started with (just in case)
  close();
}

void widgetLocale::slotApply()
{
  // Figure out the language code
  QString langCode = ui->localeListWidget->currentItem()->text();
    
  // Grab the language code
  langCode.truncate(langCode.lastIndexOf(")"));
  langCode.remove(0, langCode.lastIndexOf("(") + 1); 

  emit languageChanged(langCode);
  close();
}

QStringList widgetLocale::getLanguages(){
  //Detect all the available localizations
  QString str = "/usr/local/share/pcbsd/i18n";
  QDir dir(str);
  QStringList stL = dir.entryList(QDir::Files);
  stL = stL.filter(".qm");  
  //Get the localization codes from the filenames
  for(int i=0; i<stL.length(); i++){
    stL[i] = stL[i].section("_",1,50).section(".qm",0,0).simplified();  
  }
  //make sure that en_US is always available
  stL.prepend("en_US");
  stL.removeDuplicates();
  stL.sort();
  return stL;
}

