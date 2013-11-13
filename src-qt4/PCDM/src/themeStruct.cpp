/* PCDM Login Manager:
*  Written by Ken Moore (ken@pcbsd.org) 2012/2013
*  Copyright(c) 2013 by the PC-BSD Project
*  Available under the 3-clause BSD license
*/

#include "themeStruct.h"


void ThemeStruct::loadThemeFile(QString filePath){
  qDebug() << "Loading PCDM Theme File:" << filePath;
  //Create the required Items:
  itemNames.clear();
  itemNames << "background" << "header" << "user" << "password" << "login" << "desktop" << "system" << "locale" << "keyboard" << "vkeyboard" << "toolbar" << "nextde" << "previousde";
  items.clear();
  for(int i=0; i<itemNames.length(); i++){ 
    ThemeItem it; 
    items << it; 
  }
  //Get the directory the current theme file is in
  int li = filePath.lastIndexOf("/");
  QString currDir = filePath;
  if(li > 0){ currDir.truncate(li); }
  else{ currDir.clear(); }
  if(!currDir.endsWith("/")){ currDir.append("/"); }
  //Now initialize variables for the file search
  QString iconFileDir;
  int numspacers = 0;
  
  QFile file(filePath);
  if( !file.open(QIODevice::ReadOnly | QIODevice::Text)){ return; }
  QTextStream in(&file);
  while( !in.atEnd() ){
    QString line = in.readLine().simplified();
    if( line.isEmpty() || line.startsWith("#") ){}//ignore empty and comment lines
    else if(line.startsWith("APP_STYLESHEET_START")){ 
      //save all the following lines as the stylesheet
      QStringList styleLines;
      while( !line.startsWith("APP_STYLESHEET_END") && !in.atEnd() ){
        line = in.readLine().simplified();
	styleLines << line;
      }
      //Now compress into a single string and save it
      applicationStyleSheet = styleLines.join(" ");
    }else{
      //Read off individual values for each item
	    
      //Remove comments at the end of lines
      line = line.section("#",0,0).simplified();
      //Get the important bits from the line
      QString prop = line.section("=",0,0).simplified(); //property
      QString val = line.section("=",1,1).simplified();  //value
      QString itemName = prop.section("_",0,0).toLower(); //item name
      QString itemCat = prop.section("_",1,3).simplified();  //item category for value
      int index = itemNames.indexOf( itemName ); //index for this item
      //qDebug() << "File Item:" << prop << val << itemName << itemCat << index;
      //Now save this item as appropriate
      if(prop == "IMAGE_DIR" && !val.isEmpty()){ //default directory for icons
	if(!val.endsWith("/")){val.append("/");}
	iconFileDir = val; 
        if(iconFileDir.startsWith("./")){ iconFileDir.replace(0,2,currDir); }
      }else if( prop == "ADDSPACER" ){ //create a spacer item
	itemNames << "spacer"+QString::number(numspacers);
	numspacers++;
	ThemeItem it;
	items << it;
	index = items.length() -1; //last item in the list
	//read whether it is vertical
	items[index].isVertical = (val.section("::",0,0).toLower() == "vertical");
	//Now get the location
	val = val.section("::",1,1).remove("[").remove("]").simplified();
	//qDebug() << "ADDSPACER:" << line << val;
	items[index].x1 = val.section(",",0,0).section("-",0,0).toInt();
	items[index].x2 = -1;
	items[index].y1 = val.section(",",1,1).section("-",0,0).toInt();
	items[index].y2 = -1;
      }else if( (index != -1) && !val.isEmpty() ){
	//Save the value to the structure based upon the type of value it is
	if(itemCat == "IMAGE"){
	    if(val.startsWith("/")){ items[index].icon = val; }
	    else{ items[index].icon = iconFileDir + val; }
	    
	}else if(itemCat == "IMAGE_SIZE"){
	    items[index].iconSize = QSize( val.section("x",0,0).toInt(), val.section("x",1,1).toInt() );
		
	}else if(prop == "TOOLBAR_LOCATION"){
	    //special instructions for this item location
	    items[index].value = val; //should be [bottom | top | left | right]
	}else if(prop == "TOOLBAR_STYLE"){
	    //special instructions for this item
	    items[index].icon = val; //should be [icononly | textonly | textbesideicon | textundericon ]
	}else if(itemCat == "LOCATION"){
	   val =  val.remove("[").remove("]").simplified(); //remove the brackets
	    QString x = val.section(",",0,0);
	    QString y = val.section(",",1,1);
	    if(x.indexOf("-") == -1){ items[index].x1 = x.toInt(); items[index].x2 = -1;}
	    else{ items[index].x1 = x.section("-",0,0).toInt(); items[index].x2 = x.section("-",1,1).toInt(); }
	    if(y.indexOf("-") == -1){ items[index].y1 = y.toInt(); items[index].y2 = -1;}
	    else{ items[index].y1 = y.section("-",0,0).toInt(); items[index].y2 = y.section("-",1,1).toInt(); }
	}else if(itemCat == "ORIENTATION"){
	    items[index].isVertical = (val.toLower() == "vertical"); 
	    if(val.toLower()=="simple"){ items[index].value = "simple"; }
	}else if(itemCat == "DISABLE"){
	    items[index].enabled = (val.toLower() != "true");
	}
      }
    } // end if line is not a comment or empty
  } // end reading lines from file
  file.close();
}

ThemeItem ThemeStruct::exportItem(QString item){
  int index = itemNames.indexOf(item);
  ThemeItem TI;
  if( index == -1 ){ 
    qDebug() << "ThemeStruct: Invalid export item:"<<item;
  }else{
    //Now format the output
    TI = items[index];
  }
  return TI;
}

void ThemeStruct::importItem(QString item, ThemeItem TI){
  int index = itemNames.indexOf(item);
  if( index == -1 ){ 
    qDebug() << "ThemeStruct: Invalid import item:"<<item;
  }else{
    items[index] = TI;
  }
}

bool ThemeStruct::validItem(QString item){
  int index = itemNames.indexOf(item);
  bool ret = FALSE;
  if( index == -1 ){ 
    qDebug() << "ThemeStruct: Invalid item:"<<item;
  }else{
    bool chk = items[index].enabled && items[index].value.isEmpty() && items[index].icon.isEmpty() && ( (items[index].x1==-1) || (items[index].y1==-1) );
    ret = !chk; //true if the checks do not catch an empty item
  }
  return ret;
}

QStringList ThemeStruct::invalidItems(){
  //Scan all the current Items and make a list of all the invalid ones
   // This makes it easy to auto-populate these invalid items with defaults
  QStringList inv;
  for(int i=0; i<itemNames.length(); i++){
    if( !validItem(itemNames[i]) ){ inv << itemNames[i]; }
  }
  return inv;
}

bool ThemeStruct::itemIsEnabled(QString item){
  int index = itemNames.indexOf(item);
  bool ret = FALSE;
  if( index == -1 ){ 
    qDebug() << "ThemeStruct: Invalid item:"<<item;
  }else{
    ret = items[index].enabled;
  }
  return ret;
}

bool ThemeStruct::itemIsVertical(QString item){
  int index = itemNames.indexOf(item);
  bool ret = FALSE;
  if( index == -1 ){ 
    qDebug() << "ThemeStruct: Invalid item:"<<item;
  }else{
    ret = items[index].isVertical;  
  }
  return ret;
}

QString ThemeStruct::itemValue(QString item){
  int index = itemNames.indexOf(item);
  QString ret;
  if( index == -1 ){ 
    qDebug() << "ThemeStruct: Invalid item:"<<item;
    ret = "";
  }else{
    ret = items[index].value;
  }
  return ret;
}

QString ThemeStruct::itemIcon(QString item){
  int index = itemNames.indexOf(item);
  QString ret;
  if( index == -1 ){ 
    qDebug() << "ThemeStruct: Invalid item:"<<item;
    ret = "";
  }else{
    ret = items[index].icon;
  }  
  //qDebug() << "Theme:" << item << "Icon:" << ret;
  return ret;
}

QSize ThemeStruct::itemIconSize(QString item){
  int index = itemNames.indexOf(item);
  QSize ret;
  if( index == -1 ){ 
    qDebug() << "ThemeStruct: Invalid item:"<<item;
  }else{
    ret = items[index].iconSize;
  }
  //qDebug() << "Theme:" << item << "Size:" << ret;
  return ret;
}

int ThemeStruct::itemLocation(QString item, QString variable){
  int index = itemNames.indexOf(item);
  int ret = 0;
  if( index == -1 ){ 
    qDebug() << "ThemeStruct: Invalid item:"<<item;
    return ret;
  }else{
    variable = variable.toLower();
    if(variable == "row"){ ret = items[index].x1; }
    else if(variable == "col"){ ret = items[index].y1; }
    else if(variable == "rowspan"){ 
      int span = items[index].x2 - items[index].x1;
      if(span < 0){ span = 0; } //Make sure it is always positive
      ret = span +1; //add 1 for the end point
    }else if(variable == "colspan"){
      int span = items[index].y2 - items[index].y1;
      if(span < 0){ span = 0; } //Make sure it is always positive
      ret = span +1; //add 1 for the end point
    }
  }
  //qDebug() << "Theme:" << item << variable << ret;
  return ret; //catch for errors;
}

QString ThemeStruct::styleSheet(){
  return applicationStyleSheet;
}

QStringList ThemeStruct::getSpacers(){
  QStringList spc;
  QString isvert;
  for(int i=0; i<itemNames.length(); i++){
    if( itemNames[i].startsWith("spacer") ){
      if(items[i].isVertical){ isvert="true"; }
      else{ isvert="false"; }
      spc << isvert + "::" + QString::number(items[i].x1) +"::"+ QString::number(items[i].y1);
    }
  }
  //qDebug() << "Spacers:" << spc;
  return spc;
}
