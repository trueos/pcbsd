
#ifndef DIALOGFSSIZE_H
#define DIALOGFSSIZE_H

#include "ui_dialogFSSize.h"
#include <qstring.h>

class dialogFSSize : public QDialog, private Ui::dialogFSSize
{
	Q_OBJECT

public:
      dialogFSSize() : QDialog()
      {
        setupUi(this);
      }

  void programInit(QString desc, int curSize, int availSize, int minSize);


public slots:

protected:

private slots:
  void slotClose();
  void slotSave();
  void slotSliderChangedValue(int newVal);
  void slotSpinBoxChanged(int newVal);

private:

signals:
    void saved(int);

} ;
#endif // DIALOGINFO_H
