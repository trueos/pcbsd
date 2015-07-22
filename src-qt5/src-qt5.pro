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
	 pc-fwmanager \
	 pc-servicemanager \
	 pc-softweb \
	 pc-hwdetect \
	 pc-installgui \
	 pc-pdfviewer \
	 pc-syskeyboard \
	 pc-sysmanager \
	 pc-systemupdatertray \
	 pc-updategui \
	 pc-usermanager \
	 pc-webkitviewer \
	 pc-welcome \
	 pc-xgui \
	 pc-zmanager \
	 qsudo \ 
	 EasyPBI \
         PCDM

doins.path=/
doins.extra=cd xtrafiles && sh install.sh $(INSTALL_ROOT)/usr/local
                
mkdocs.path=/
mkdocs.extra=cd docs && sh mkdocs.sh $(INSTALL_ROOT)/usr/local

INSTALLS += doins mkdocs

