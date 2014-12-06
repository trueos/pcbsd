/* PCDM Login Manager:
*  Written by Ken Moore (ken@pcbsd.org) 2012/2013
*  Copyright(c) 2013 by the PC-BSD Project
*  Available under the 3-clause BSD license
*/

/*
Sub-classed fancy widget for selecting a desktop environment
*/

#include "fancySwitcher.h"

FancySwitcher::FancySwitcher(QWidget* parent,bool horizontal) : QWidget(parent)
{
  //Set the default values for the current item
  idL.clear(); icL.clear();
  item=-1; oldItem=0; //default values for the current/old item number
  frames=-1; //smoothest animation possible (1 pixel at a time)
  //Save whether it is a horizontal or vertical display
  isHorizontal = horizontal;
  //Set the default size for the images
  iconSize = 64; //default all icons in the viewer to 64x64 (can be changed later)
  //Create the Grid layout
  QGridLayout* layout = new QGridLayout();
  //Add the buttons to the grid (back is always left/up)
  pushForward = new QPushButton;
  pushBack = new QPushButton;
  pushForward->setIconSize(QSize(iconSize/2,iconSize/2));
  pushBack->setIconSize(QSize(iconSize/2,iconSize/2)); 
  
  textLabel = new QLabel;
  textLabel->setAlignment(Qt::AlignCenter);

  iconViewer = new QGraphicsView();
  iconViewer->setInteractive(false);
  iconViewer->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  iconViewer->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  iconViewer->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  if(isHorizontal){
    iconViewer->setFixedSize(2.5*iconSize,iconSize+2);
    pushBack->setIcon(QIcon(":images/left.png"));
    pushBack->setShortcut(tr("Alt+Left"));
    pushForward->setIcon(QIcon(":images/right.png"));
    pushForward->setShortcut(tr("Alt+Right"));
    layout->addWidget(pushBack,1,1);
    layout->addWidget(iconViewer,1,2);
    layout->addWidget(pushForward,1,3);
    layout->addWidget(textLabel,2,1,1,3);
  }else{ //vertical
    iconViewer->setFixedSize(iconSize+2,2.5*iconSize);
    pushBack->setIcon(QIcon(":images/up.png"));
    pushBack->setShortcut(tr("Alt+Up"));
    pushForward->setIcon(QIcon(":images/down.png"));
    pushForward->setShortcut(tr("Alt+Down"));
    layout->addWidget(pushBack,1,1);
    layout->addWidget(iconViewer,2,1);
    layout->addWidget(textLabel,3,1);
    layout->addWidget(pushForward,4,1);
  }
  //set this layout as the layout for the FancySwitcher
  this->setLayout(layout);
  //connect the buttons to their appropriate slots
  connect(pushBack,SIGNAL(clicked()),this,SLOT(slotPushBack()));
  connect(pushForward,SIGNAL(clicked()),this,SLOT(slotPushForward()));

  //Create the default scene for the iconViewer
  updateIconViewer();
  //Enable/disable the forward/back buttons as appropriate
  checkButtons();
}

FancySwitcher::~FancySwitcher(){
}

void FancySwitcher::updateIconViewer(){
  //detect the base dimensions
  int xsize,ysize;
  if(isHorizontal){
    xsize = (idL.length()+1+(idL.length()-1)*5.0/4)*iconSize;
    ysize = iconSize;
  }else{ //vertical
    xsize = iconSize;
    ysize = (idL.length()+1+(idL.length()-1)*5.0/4)*iconSize;
  }
  //qDebug() << "scene dimensions:" << xsize << ysize;
  //Create the scene with the proper dimensions
  scene = new QGraphicsScene(0,0,xsize,ysize);
  //Fill the scene with the given images
  int offset = iconSize*3/4; //initial offset ( (viewer width-iconsize)/2 )
  for(int i=0; i<idL.length(); i++){
    //qDebug() << "offset" << offset;
    if(isHorizontal){
      icL[i]->setOffset(offset,0);
    }else{
      icL[i]->setOffset(0,offset);
    }
    scene->addItem(icL[i]);
    offset = offset + 5*iconSize/4; //have a 1/4 iconsize spacing between icons
  }
  //set the iconViewer to display the scene
  iconViewer->setScene(scene);
  displayCurrentItem(false);

}

void FancySwitcher::displayCurrentItem(bool showAnimation){
  //Check that the new item is valid
  //if( (item >= idL.length()) || (item < 0) ){item = oldItem; return;} //reset to the previous value and don't change the display

  if(item == -1){
    iconViewer->centerOn(0,0);
    iconViewer->show();
    return;
  }
  //Get the current/old display pixel
  int cPixel = (1 + oldItem*5.0/4)*iconSize;
  //Get the new/desired display pixel
  int tPixel = (1 + item*5.0/4)*iconSize;  
  //detect if we are moving back/forward
  bool moveForward = true;
  if(tPixel < cPixel){ moveForward  = false; }
  if(tPixel == cPixel){ showAnimation = false; } //same item, skip the animation loop and just refresh
  //qDebug() << "displayItem:" << item << oldItem << cPixel << tPixel << moveForward;
  //Show the scrolling animation if desired
  if(showAnimation && (frames!=0)){
    int pixchange = 1;
    if(frames > 0){ pixchange=abs(tPixel-cPixel)/frames; } //draw fewer frames per animation
  	  	  
    if(moveForward){
      while(cPixel < (tPixel-pixchange+1)){
        cPixel += pixchange;
	if(isHorizontal){
      	  iconViewer->centerOn(cPixel,0);
   	}else{
      	  iconViewer->centerOn(0,cPixel);
    	}
        iconViewer->show();
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 22);
      }
    }else{
      while(cPixel > (tPixel+pixchange-1)){
        cPixel -= pixchange;
	if(isHorizontal){
      	  iconViewer->centerOn(cPixel,0);
   	}else{
      	  iconViewer->centerOn(0,cPixel);
    	}
        iconViewer->show();
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 22);
      }
    }
    iconViewer->centerOn(icL[item]);
    iconViewer->show();
  }else{
    //Just center on the desired item (skip animation)
    iconViewer->centerOn(icL[item]);
    iconViewer->show();
  }

  //Save the (now) current item as the old item for animation purposes later
  oldItem = item;
  textLabel->setText(idL[item]); //set the info text 
}

void FancySwitcher::checkButtons(){
  //Sanity check the current item value
  if(item > (idL.length()-1)){ item = idL.length()-1; }
  if(item < -1){ item = -1; }
  //Enable/Disable the pushbuttons as appropriate
  if(item == (idL.length()-1)){ pushForward->setEnabled(false); }
  else{ pushForward->setEnabled(true); }
  if(item < 1){ pushBack->setEnabled(false); }
  else{ pushBack->setEnabled(true); };
}

void FancySwitcher::slotPushBack(){
  item--;
  if(item < 0){ item = 0; } //cannot go lower than the first item
  displayCurrentItem(true); //show animation of changing items
  checkButtons();
  emit itemChanged();
  emit itemChanged(idL[item]);
}

void FancySwitcher::slotPushForward(){
  item++;
  if(item >= idL.length()){ item = idL.length()-1; } //cannot go higher than the last item
  displayCurrentItem(true);
  checkButtons();
  emit itemChanged();
  emit itemChanged(idL[item]);
}

//-----------------------------
//     PUBLIC FUNCTIONS
//-----------------------------
void FancySwitcher::addItem(QString id, QString icon, QString tooltip){
  if(!QFile::exists(icon)){
    qDebug() << "FancySwitcher: invalid icon file" << icon;
    return;
  }
  if(idL.indexOf(id)==-1){
    idL << id;
    icL << new QGraphicsPixmapItem(QPixmap(icon).scaled(iconSize,iconSize));
    if(!tooltip.isEmpty()){
      int index = idL.indexOf(id);
      icL[index]->setToolTip(tooltip);
    }
  }
  if(item==-1){item = 0;} //There is now an item that can be selected
  updateIconViewer();
  checkButtons();
}

void FancySwitcher::addItems(QStringList idList, QStringList iconList, QStringList tooltipList){
  //Check that all the lists are the same length
  if(!( (idList.length()==iconList.length()) && (idList.length()==tooltipList.length()) )){
    qDebug() << "FancySwitcher: item lists are not equal length";
    return;
  }
  //Check each item and add it to the list if applicable
  for(int i=0; i<idList.length(); i++){
    if(!QFile::exists(iconList[i])){
      qDebug() << "FancySwitcher: invalid icon file" << iconList[i];
      return;
    }else{
      if(idL.indexOf(idList[i].toLower()) == -1){
        idL << idList[i];
        icL << new QGraphicsPixmapItem(QPixmap(iconList[i]).scaled(iconSize,iconSize));
	if(!tooltipList[i].isEmpty()){
          int index = idL.indexOf(idList[i]);
          icL[index]->setToolTip(tooltipList[i]);
	}
      }
    }
  }
  if(item==-1){item = 0;} //There is now an item to be selected
  updateIconViewer();
  checkButtons();
}

void FancySwitcher::removeItem(QString id){
  int index = idL.indexOf(id.toLower());
  if(index == -1){
    qDebug() << "FancySwitcher: Item does not exist -" <<id;
    return;
  }else{
    idL.removeAt(index);
    icL.removeAt(index);
  }
  updateIconViewer();
  checkButtons();
}

void FancySwitcher::removeItems(QStringList idList){
  for(int i=0; i<idList.length(); i++){
    int index = idL.indexOf(idList[i].toLower());
    if(index == -1){
      qDebug() << "FancySwitcher: Item does not exist -" <<idList[i];
      return;
    }else{
      idL.removeAt(index);
      icL.removeAt(index);
    }
  }
  updateIconViewer();
  checkButtons();
}

void FancySwitcher::removeAllItems(){
  if( idL.isEmpty() ){ return; } //do nothing if nothing to change
  //clear the internal lists
  idL.clear();
  icL.clear();
  item = -1;
  //now refresh the display
  updateIconViewer();
  checkButtons();
}

QString FancySwitcher::currentItem(){
  QString id = idL[item];
  return id;
}

void FancySwitcher::setCurrentItem(QString id){
  int index = idL.indexOf(id);
  if(index == -1){
    qDebug() << "FancySwitcher: Item does not exist -" << id;
    return;
  }else{
    item = index;
    displayCurrentItem(false); //do not show animation
    checkButtons();
  }
}

void FancySwitcher::setNumberAnimationFrames(QString numframes){
  if(numframes.toLower()=="smooth"){frames=-1; return;}  //auto-detect max
  if(numframes.toLower()=="instant"){frames=0; return;}   //don't use animation
  bool ok;
  int num = numframes.toInt(&ok);
  if(!ok){qDebug() << "FancySwitcher: invalid input for number of animation frames. Must be \"smooth\", \"instant\", or an integer > 0."; return; }
  if(num < 2){ frames=0; } //1 frame = instant
  else{ frames=num; } //don't worry about huge numbers, min pixel animation will always be 1 at a time
  
}

void FancySwitcher::setIconSize(int xysize){
  iconSize = xysize; //change the saved icon size
  //Update the pushbuttons icons
  pushBack->setIconSize(QSize(iconSize/2,iconSize/2));
  pushForward->setIconSize(QSize(iconSize/2,iconSize/2));
  //Update the iconViewer sizes
  if(isHorizontal){
    iconViewer->setFixedSize(2.5*iconSize,iconSize+2);
  }else{
    iconViewer->setFixedSize(iconSize+2,2.5*iconSize);
  }
  updateIconViewer();
}

void FancySwitcher::changeButtonIcon(QString button, QString iconFile){
  if(!QFile::exists(iconFile)){ qDebug() << "FancySwitcher: invalid image file"<<iconFile; return; }
  if(button.toLower() == "back"){ pushBack->setIcon(QIcon(iconFile)); }
  else if(button.toLower() == "forward"){ pushForward->setIcon(QIcon(iconFile)); }
  else{ 
    qDebug() << "FancySwitcher: Cannot change the icon for button" << button << " - valid buttons are \"forward\" and \"back\""; 
  }
}

void FancySwitcher::setStyleSheet(QString style){
  //Propagate the style sheet to both the buttons and the iconViewer
  pushForward->setStyleSheet(style);
  pushBack->setStyleSheet(style);
  iconViewer->setStyleSheet("QGraphicsView { "+style+" } "); //make sure the tooltips are not affected
  textLabel->setStyleSheet(style);
}

