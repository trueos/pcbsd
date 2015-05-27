#ifndef _PCBSD_FIRSTBOOT_SERVICES_H
#define _PCBSD_FIRSTBOOT_SERVICES_H

#include <QString>
#include <QStringList>
#include <QList>

#include <unistd.h>

class service{
public:
  service(){}
  ~service(){}
	  
  QString ID; //Unique ID/Name for this service (for finding in the lists)
  QString file; //Location of the service file (/usr/local/etc/rc.d/<something>)
  QString name; //translated name of the service
  QString description; //translated description of the service
  QStringList cmds; //Optional commands to run before enabling the service
  QStringList openPorts; //Which ports to open in the IPFW firewall (Example: "tcp 22")
  QStringList rcLines; //Lines to put into /etc/rc.conf (Example: sshd_enable="YES")
};


class Services{
public:
	static QList<service> getServiceList();

	static void enableService(service S);
};
#endif