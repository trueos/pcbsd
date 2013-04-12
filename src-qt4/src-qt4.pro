TEMPLATE = subdirs

SUBDIRS+= libpcbsd \
	 about-gui \
	 life-preserver \
	 pc-adsldap \
	 pc-bluetoothmanager \
	 pc-bluetoothtray \
	 pc-controlpanel \
	 pc-crashhandler \
	 pc-cupscfg \
	 pc-firstbootgui \
	 pc-gdmconf \
	 pc-mounttray \
	 pc-netmanager \
	 pc-pbigui \
	 pc-pfmanager \
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
	 warden-gui \
	 qsu \
         EasyPBI \ 
         PCDM

doins.path=/usr/local/share/pcbsd/
doins.extra=cd xtrafiles && sh install.sh
                
INSTALLS += doins

