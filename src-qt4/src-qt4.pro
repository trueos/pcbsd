TEMPLATE = subdirs

SUBDIRS+= libpcbsd \
	 about-gui \
	 life-preserver \
	 pc-adsldap \
	 pc-bluetoothmanager \
	 pc-bluetoothtray \
	 pc-bootconfig \
	 pc-bugreport \
	 pc-controlpanel \
	 pc-crashhandler \
	 pc-cupscfg \
	 pc-firstbootgui \
	 pc-gdmconf \
	 pc-mounttray \
	 pc-netmanager \
	 pc-pbigui \
	 pc-pfmanager \
	 pc-pkgmanager \
	 pc-servicemanager \
	 pc-softwaremanager \
	 pc-sounddetect \
	 pc-soundconfig \
	 pc-installgui \
	 pc-sysmanager \
	 pc-systemupdatertray \
	 pc-updategui \
	 pc-usermanager \
	 pc-welcome \
	 pc-xgui \
	 pc-zmanager \
	 warden-gui \
	 qsudo \
         EasyPBI \ 
         PCDM

doins.path=/
doins.extra=cd xtrafiles && sh install.sh $(INSTALL_ROOT)/usr/local
                
INSTALLS += doins

