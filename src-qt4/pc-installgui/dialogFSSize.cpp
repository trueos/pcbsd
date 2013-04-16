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
#include "dialogFSSize.h"

void dialogFSSize::programInit(QString desc, int curSize, int availSize, int minSize)
{
  connect(horizontalSizeSlider, SIGNAL(sliderMoved(int)), this, SLOT(slotSliderChangedValue(int)));
  connect(spinSize, SIGNAL(valueChanged(int)), this, SLOT(slotSpinBoxChanged(int)));
  connect(pushClose, SIGNAL(clicked()), this, SLOT(slotClose()));
  connect(pushSave, SIGNAL(clicked()), this, SLOT(slotSave()));
  horizontalSizeSlider->setMinimum(minSize);
  horizontalSizeSlider->setMaximum(availSize);
  horizontalSizeSlider->setValue(curSize);
  spinSize->setRange(minSize, availSize);
  spinSize->setValue(curSize);
  labelDesc->setText(desc);
}

void dialogFSSize::slotClose()
{
  close();
}

void dialogFSSize::slotSave()
{
  emit saved(spinSize->value());
  close();
}

// function which updates the spinbox when the user drags the slider
void dialogFSSize::slotSliderChangedValue(int newVal)
{
  spinSize->setValue(newVal);
}
    
// function which updates the spinbox when the user drags the slider
void dialogFSSize::slotSpinBoxChanged(int newVal)
{
  horizontalSizeSlider->setValue(newVal);
}
