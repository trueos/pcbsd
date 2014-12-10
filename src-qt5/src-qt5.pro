TEMPLATE = subdirs

SUBDIRS+= libpcbsd \
	 about-gui \
	 life-preserver \
	 pc-adsldap \
	 pc-bootconfig \
	 pc-bugreport \
	 pc-controlpanel \
	 pc-cupscfg \
	 pc-firstbootgui \
	 pc-dmconf \
	 pc-mixer \
	 pc-mounttray \
	 pc-netmanager \
	 pc-pfmanager \
	 pc-servicemanager \
	 pc-softwaremanager \
	 pc-softweb \
	 pc-hwdetect \
	 pc-soundconfig \
	 pc-installgui \
	 pc-syskeyboard \
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

