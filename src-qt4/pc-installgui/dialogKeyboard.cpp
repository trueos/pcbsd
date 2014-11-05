/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
#include "backend.h"
#include "dialogKeyboard.h"
#include "ui_dialogKeyboard.h"

void widgetKeyboard::programInit(QStringList kModel, QStringList kLayouts, QString cModel, QString cLayout, QString cVarient)
{
  connect(pushClose, SIGNAL(clicked()), this, SLOT(slotClose()));
  connect(pushApply, SIGNAL(clicked()), this, SLOT(slotApply()));
  keyboardModels = kModel;
  keyboardLayouts = kLayouts;
  cKeyModel = cModel;
  cKeyLayout = cLayout;
  cKeyVarient = cVarient;
  if(cKeyLayout.isEmpty()){ cKeyLayout = "us"; }
  connectKeyboardSlots();
}

void widgetKeyboard::slotClose()
{
  //Return back to original keyboard settings and close
  slotUpdateKbOnSys(true);
  close();
}

void widgetKeyboard::slotApply()
{
  //Apply and close
  slotUpdateKbOnSys();
  close();
}

void widgetKeyboard::connectKeyboardSlots()
{
  // load keyboards
  listKbLayouts->clear();
  listKbLayouts->addItems(keyboardLayouts);
  comboBoxKeyboardModel->addItems(keyboardModels);

  // Set the default keyboard stuff
  setKbDefaults();
  slotCurrentKbLayoutChanged(0);
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

void widgetKeyboard::slotUpdateKbOnSys(bool reset)
{
  QString model, layout, variant;
  if(reset){
    model = cKeyModel;
    layout = cKeyLayout;
    variant = cKeyVarient;
  }else{
    if ( comboBoxKeyboardModel->currentIndex() == -1 )
       return;

    if ( ! listKbLayouts->currentItem() )
       return;

    if ( ! listKbVariants->currentItem() )
       return;

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
  }
  Scripts::Backend::changeKbMap(model, layout, variant);
  emit saved(model, layout, variant);
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
    laycode = laycode.remove(0, laycode.indexOf("(") + 1);
    laycode.truncate(laycode.indexOf(")") );

    listKbVariants->addItem("<none>");
    listKbVariants->addItems(Scripts::Backend::keyVariants(laycode, savedKeyVariants));

    listKbVariants->setCurrentRow(0);
}

void widgetKeyboard::slotSelectedKbItemChanged()
{
    lineKbTest->clear();
}

// set the keyboard layout and variant defaults
void widgetKeyboard::setKbDefaults()
{
	// Find the current key layout as the default
	for ( int i = 0; i < listKbLayouts->count(); i++ )
		if ( listKbLayouts->item(i)->text().indexOf("("+cKeyLayout+")") != -1 )
    			listKbLayouts->setCurrentRow(i);

	int index = comboBoxKeyboardModel->findText(cKeyModel);
	if(index>=0){ comboBoxKeyboardModel->setCurrentIndex(index); }
}
