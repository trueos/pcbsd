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

void widgetKeyboard::programInit(QStringList kModel, QStringList kLayouts)
{
  connect(pushClose, SIGNAL(clicked()), this, SLOT(slotClose()));
  connect(pushApply, SIGNAL(clicked()), this, SLOT(slotApply()));
  keyboardModels = kModel;
  keyboardLayouts = kLayouts;
  connectKeyboardSlots();
}

void widgetKeyboard::slotClose()
{
  close();
}

void widgetKeyboard::slotApply()
{
  slotUpdateKbOnSys();
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

void widgetKeyboard::slotUpdateKbOnSys()
{
  QString model, layout, variant;

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
  
  Scripts::Backend::changeKbMap(model, layout, variant);
  emit changedLayout(model, layout, variant);
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
	// Find the "us" key layout as the default
	for ( int i = 0; i < listKbLayouts->count(); i++ )
		if ( listKbLayouts->item(i)->text().indexOf("(us)") != -1 )
    			listKbLayouts->setCurrentRow(i);


    	comboBoxKeyboardModel->setCurrentIndex(2);
}
