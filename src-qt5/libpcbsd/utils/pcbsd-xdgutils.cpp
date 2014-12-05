#include "pcbsd-xdgutils.h"

QList<XDGFile> XDGUtils::allApplications(bool showhidden, bool showinvalid){
  //Get the list of places to look for applications based on the current path settings
  QStringList paths = QString( getenv("XDG_DATA_DIRS") ).split(":");
  paths.prepend(QDir::homePath()+"/.local/share"); //make sure the user's home-dir is searched too
  QStringList found; //to make sure we don't get duplicate files
  QList<XDGFile> output;
  QDir dchk;
  for(int i=0; i<paths.length(); i++){
    if(dchk.cd(paths[i]+"/applications") ){
      QStringList files = dchk.entryList(QStringList() << "*.desktop", QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
      for(int f=0; f<files.length(); f++){
        if(found.contains(files[f])){ continue; } //skip this file - duplicate from earlier
	XDGFile tmp;
	  tmp.LoadDesktopFile(dchk.absoluteFilePath(files[f]));
	bool valid = true;
	if(!showhidden){ valid = !tmp.isHidden(); }
	if(valid && !showinvalid){ valid = tmp.isValid(); }
	if(valid){
	  output << tmp;
	  found << files[f];
	}
      }//end loop over files
    }
  }//end loop over paths
  return output;
}

QList<XDGFile> XDGUtils::filterAppsByCategory(QString cat, QList<XDGFile> apps){
  QList<XDGFile> out;
  for(int i=0; i<apps.length(); i++){
    if( apps[i].Categories().contains(cat) ){ out << apps[i]; }
  }
  return out;
}

QList<XDGFile> XDGUtils::sortAppsByName(QList<XDGFile> apps){
  //Sort the list by name of the application
  QHash<QString, XDGFile> sorter;
  for(int i=0; i<apps.length(); i++){
    sorter.insert(apps[i].Name(), apps[i]);	  
  }
  QStringList keys = sorter.keys();
  keys.sort();
  //Re-assemble the output list
  QList<XDGFile> out;
  for(int i=0; i<keys.length(); i++){
    out << sorter[keys[i]];
  }
  return out;	
}