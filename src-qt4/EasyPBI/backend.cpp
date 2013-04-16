#include "backend.h"

QIcon Backend::icon(QString icon){
  icon = icon.toLower();
  //Qt embedded resources (http://www.qtcentre.org/wiki/index.php?title=Embedded_resources)
  QString iconPath = ":/trolltech/styles/commonstyle/images/";
  if(icon == "new"){ iconPath.append("newdirectory-128.png"); }
  else if(icon=="load"){ iconPath.append("diropen-128.png"); }
  else if(icon=="refresh"){ iconPath.append("refresh-32.png"); }
  else if(icon=="delete"){ iconPath.append("standardbutton-delete-128.png"); }
  else if(icon=="left"){ iconPath.append("left-128.png"); }
  else if(icon=="right"){ iconPath.append("right-128.png"); }
  else if(icon=="up"){ iconPath.append("up-128.png"); }
  else if(icon=="down"){ iconPath.append("down-128.png"); }
  else if(icon=="save"){ iconPath.append("standardbutton-save-32.png"); }
  else if(icon=="help"){ iconPath.append("standardbutton-help-32.png"); }
  else if(icon=="open"){ iconPath.append("standardbutton-open-32.png"); }
  else if(icon=="close"){ iconPath.append("standardbutton-close-32.png"); }
  else if(icon=="file"){ iconPath.append("file-32.png"); }
  else if(icon=="trash"){ iconPath.append("trash-32.png"); }
  else if(icon=="clear"){ iconPath.append("standardbutton-clear-128.png"); }
  else if(icon=="start"){ iconPath.append("media-play-32.png"); }
  else if(icon=="stop"){ iconPath.append("media-stop-32.png"); }
  //EasyPBI embedded resources (EasyPBI.qrc)
  else if(icon=="easypbi"){ iconPath = ":/png/icons/EasyPBIicon.png"; }
  else if(icon=="freebsd"){ iconPath = ":/png/icons/freebsd.png"; }
  else if(icon=="pcbsd"){iconPath = ":/png/icons/pcbsd.png"; }
  else if(icon=="config"){iconPath = ":/png/icons/config.png"; }
  //onebit iconset icons (http://www.iconfinder.com/search/?q=iconset%3Aonebit)
  else if(icon=="plus"){iconPath = ":/png/icons/plus.png"; }
  else if(icon=="minus"){iconPath = ":/png/icons/minus.png"; }
  else{ iconPath.clear(); }
	
  return QIcon(iconPath);
}
