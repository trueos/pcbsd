#ifndef _PCBSD_BOOT_MANAGER_QUICK_GRUB_ENTRIES_H
#define _PCBSD_BOOT_MANAGER_QUICK_GRUB_ENTRIES_H

#include <QObject>
#include <QList>
#include <QString>

struct GEntry{
  QString name, ID, icon;
};

class GrubEntries{
public:
	static QList<GEntry> listAll(){
	  QList<GEntry> out;
	  GEntry GE;
		GE.name = QObject::tr("Windows");
		GE.ID = "WINDOWS";
		GE.icon = ":/icons/windows.png";
	    out << GE;
	    //Add more entries here as necessary 
	    //   (don't forget the associated entry for the new ID below!)
		
	  return out;
	}
	
	static QString CreateEntry(QString ID, QString HD, QString Part, QString customName = ""){
	  //Note: HD is the hard drive number (0+) and Part is the partition number (1+)
	  QString entry;
	  if(ID=="WINDOWS"){
	    if(customName.isEmpty()){ customName = "Windows (ada"+HD+"s"+Part+")"; }
	    entry = "menuentry \'"+customName+"\' {\n\
  set root=(hd"+HD+","+Part+")\n\
  chainloader +1\n\
}";
	  }
	  return entry;
	}
};
#endif