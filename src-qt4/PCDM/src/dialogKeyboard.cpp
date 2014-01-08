/* PCDM Login Manager:
*  Written by Ken Moore (ken@pcbsd.org) 2012/2013
*  Copyright(c) 2013 by the PC-BSD Project
*  Available under the 3-clause BSD license
*
* Note: This file initially copied from the graphical PC-BSD installer 
*/

#include "pcdm-backend.h"
#include "dialogKeyboard.h"
#include "ui_dialogKeyboard.h"

widgetKeyboard::widgetKeyboard(QWidget* parent) : QWidget(parent){
  setupUi(this);
}

void widgetKeyboard::programInit(QStringList kModel, QStringList kLayouts)
{
  connect(pushClose, SIGNAL(clicked()), this, SLOT(slotClose()));
  connect(pushApply, SIGNAL(clicked()), this, SLOT(slotApply()));
  keyboardModels = kModel;
  keyboardLayouts = kLayouts;
  //Load the current defaults
  QString junk;
  Backend::readDefaultSysEnvironment(junk, dModel, dLayout, dVariant);
  connectKeyboardSlots();
}

void widgetKeyboard::slotClose()
{
  //Close without changing the KBD layout
  Backend::changeKbMap(dModel, dLayout, dVariant); //reset to initial defaults
  close();
}

void widgetKeyboard::slotApply()
{
  //slotUpdateKbOnSys();
  Backend::changeKbMap(cModel, cLayout, cVariant); //use Current values
  Backend::saveDefaultSysEnvironment( QString(getenv("LANG")) , cModel, cLayout, cVariant);
  this->close();
}

void widgetKeyboard::connectKeyboardSlots()
{
  // load keyboards
  listKbLayouts->clear();
  listKbLayouts->addItems(keyboardLayouts);
  comboBoxKeyboardModel->addItems(keyboardModels);

  // Set the default keyboard stuff
  setKbDefaults();
  //slotCurrentKbLayoutChanged(0);
  groupKeyboard->setEnabled(true);


  // Connect all our slots
  connect(listKbLayouts, SIGNAL(currentRowChanged(int)), this, SLOT(slotCurrentKbLayoutChanged(int)));
  connect(listKbVariants,SIGNAL(currentRowChanged(int)), this, SLOT(slotCurrentKbVariantChanged(int)));
  connect(listKbLayouts, SIGNAL(itemSelectionChanged()), this, SLOT(slotSelectedKbItemChanged()));
  connect(listKbVariants,SIGNAL(itemSelectionChanged()), this, SLOT(slotSelectedKbItemChanged()));
  connect(comboBoxKeyboardModel,SIGNAL(currentIndexChanged(int)), this, SLOT(slotUpdateKbOnSys()));
}

void widgetKeyboard::slotCurrentKbLayoutChanged(int row)
{
    if (row != -1) {
        QString kbLayout = listKbLayouts->currentItem()->text();
        setKbVariants(kbLayout);
    }

   slotUpdateKbOnSys();
}

void widgetKeyboard::slotUpdateKbOnSys()
{
  QString model, layout, variant;
  //Backend::log("[DEBUG] Update Keyboard on system");
  if ( comboBoxKeyboardModel->currentIndex() == -1 ){
     Backend::log("[DEBUG] Invalid kbd model index");
     return;
  }else if ( ! listKbLayouts->currentItem() ){
     Backend::log("[DEBUG] Invalid kbd layout item");
     return;
  }else if ( ! listKbVariants->currentItem() ){
     Backend::log("[DEBUG] Invalid kbd variant item");
     return;
  }

  model = comboBoxKeyboardModel->currentText();
  model = model.remove(0, model.indexOf("- (") + 3 );
  model.truncate(model.size() -1 );

  layout = listKbLayouts->currentItem()->text();
  layout = layout.remove(0, layout.indexOf("- (") + 3 );
  layout.truncate(layout.size() -1 );

  variant = listKbVariants->currentItem()->text();
  if ( variant != "<none>" ) {
    variant = variant.remove(0, variant.indexOf("- (") + 3 );
    variant.truncate(variant.size() -1 );
  } else {
    variant = "";
  }
  //Backend::log("[DEBUG] Current KBD settings: "+model+", "+layout+", "+variant);
  Backend::changeKbMap(model, layout, variant);
  //Save the settings for later
  cModel = model; cLayout = layout; cVariant = variant;
}

void widgetKeyboard::slotCurrentKbVariantChanged(int row)
{
   if (row != -1)
      listKbVariants->currentItem()->text();

   slotUpdateKbOnSys();
}

void widgetKeyboard::setKbVariants(const QString &kbLayout)
{
    QString laycode = kbLayout;
    listKbVariants->clear();

    // Get the code we should search for
    laycode = laycode.section("- (",1,1).section(")",0,0);

    listKbVariants->addItem("<none>");
    listKbVariants->addItems(Backend::keyVariants(laycode, savedKeyVariants));

    listKbVariants->setCurrentRow(0);
}

void widgetKeyboard::slotSelectedKbItemChanged()
{
    lineKbTest->clear();
}

// set the keyboard layout and variant defaults
void widgetKeyboard::setKbDefaults()
{
  // Now update the GUI to reflect the current settings
  for ( int i = 0; i < listKbLayouts->count(); i++ ){
    if ( listKbLayouts->item(i)->text().contains("- ("+dLayout+")") ){
      listKbLayouts->setCurrentRow(i);
      break;
    }
  }
  
  for( int i=0; i<comboBoxKeyboardModel->count(); i++){
    if( comboBoxKeyboardModel->itemText(i).contains("- ("+dModel+")") ){
      comboBoxKeyboardModel->setCurrentIndex(i);
      break;
    }
  }
  //Now fill the list of layout variants
  listKbVariants->clear();
  listKbVariants->addItem("<none>");
  listKbVariants->addItems(Backend::keyVariants(dLayout, savedKeyVariants));
  //Now highlight the current variant
  if(dVariant.isEmpty()){ 
    listKbVariants->setCurrentRow(0); 
  }else{
    for(int i=1; i<listKbVariants->count(); i++){ //skip the first item (already accounted for)
      if(listKbVariants->item(i)->text().contains("- ("+dVariant+")") ){
	listKbVariants->setCurrentRow(i);
	break;
      }
    }
  }
}
