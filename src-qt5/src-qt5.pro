TEMPLATE = subdirs
CONFIG += recursive

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
         PCDM

doins.path=/
doins.extra=cd xtrafiles && sh install.sh $(INSTALL_ROOT)/usr/local
                
mkdocs.path=/
mkdocs.extra=cd docs && sh mkdocs.sh $(INSTALL_ROOT)/usr/local

INSTALLS += doins mkdocs

about-gui.depends = libpcbsd
life-preserver.depends = libpcbsd
pc-adsldap.depends = libpcbsd
pc-bootconfig.depends = libpcbsd
pc-bugreport.depends = libpcbsd
pc-controlpanel.depends = libpcbsd
pc-cupsconfig.depends = libpcbsd
pc-firstbootgui.depends = libpcbsd
pc-dmconf.depends = libpcbsd
pc-mixer.depends = libpcbsd
pc-mounttray.depends = libpcbsd
pc-netmanager.depends = libpcbsd
pc-fwmanager.depends = libpcbsd
pc-servicemanager.depends = libpcbsd
pc-softweb.depends = libpcbsd
pc-hwdetect.depends = libpcbsd
pc-installgui.depends = libpcbsd
pc-pdfviewer.depends = libpcbsd
pc-syskeyboard.depends = libpcbsd
pc-sysmanager.depends = libpcbsd
pc-systemupdatertray.depends = libpcbsd
pc-updategui.depends = libpcbsd
pc-usermanager.depends = libpcbsd
pc-webkitviewer.depends = libpcbsd
pc-xgui.depends = libpcbsd
pc-zmanager.depends = libpcbsd
EasyPBI.depends = libpcbsd
PCDM.depends = libpcbsd
