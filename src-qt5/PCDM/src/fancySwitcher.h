/* PCDM Login Manager:
*  Written by Ken Moore (ken@pcbsd.org) 2012/2013
*  Copyright(c) 2013 by the TrueOS Project
*  Available under the 3-clause BSD license
*/

/*
 Sub-classed widget for a fancy item selector
*/

#ifndef FANCY_SWITCHER_H
#define FANCY_SWITCHER_H

#include <QWidget>
#include <QFile>
#include <QCoreApplication>
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QGridLayout>
#include <QPixmap>
#include <QDebug>
#include <QLabel>

class FancySwitcher : public QWidget
{
	Q_OBJECT

  public:
	FancySwitcher(QWidget* parent = 0, bool horizontal = true);
	~FancySwitcher();
	
	//Add item(s) to the widget (icon must be a valid image file path)
	void addItem(QString id, QString icon, QString tooltip = "");
	void addItems(QStringList idList, QStringList iconList, QStringList tooltipList);
	//Remove item(s) from the widget (given the item(s) identifier(s))
	void removeItem(QString id);
	void removeItems(QStringList idList);
        void removeAllItems();
	//Get the currently selected item (returns the item identifier)
	QString currentItem();
	//Set the currently selected item (by identifier)
	void setCurrentItem(QString id);
	//Set x-y size (in pixels) to scale all images (best before adding items)
	void setIconSize(int xysize);
	//Manually set the "back" (up/left) and "forward" (down/right) button icons
	void changeButtonIcon(QString button, QString iconFile);
	//Change the style sheet for all widget items (see QtStyle options for more)
	void setStyleSheet(QString style);
        //Change the "smoothness" of the animation by the number of frames to draw (default is smoothest)
        void setNumberAnimationFrames(QString numframes); // [ smooth | instant | int > 0 ]
        
  private:
	QPushButton* pushBack;
	QPushButton* pushForward;
	QGraphicsView* iconViewer;
	QGraphicsScene* scene;
	QStringList idL;
	QList<QGraphicsPixmapItem*> icL;
	QLabel* textLabel;
	int item, oldItem, frames;
	bool isHorizontal;
	int iconSize;

	void updateIconViewer();
	void displayCurrentItem(bool);
	void checkButtons();

  private slots:
	void slotPushBack();
  	void slotPushForward();
	
  signals:
	//Emits these signals whenever the selection is changed (forward/back clicked)
	void itemChanged();
  	void itemChanged(QString);

};
#endif
