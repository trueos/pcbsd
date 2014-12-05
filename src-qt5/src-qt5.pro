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
	 pc-dmconf \
	 pc-mixer \
	 pc-mounttray \
	 pc-netmanager \
	 pc-pbigui \
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
	 pc-updatecenter \
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

