#############################################################################
# Makefile for building: pc-xdgutil
#############################################################################

AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/local/bin/qmake-qt4
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -R
STRIP         = 
INSTALL_FILE  = $(COPY_FILE)
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = $(COPY_FILE)
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

first: all

all:

install_scripts: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)/usr/local/bin/ || $(MKDIR) $(INSTALL_ROOT)/usr/local/bin/ 
	-$(INSTALL_FILE) pc-xdgutil $(INSTALL_ROOT)/usr/local/bin/


uninstall_scripts:  FORCE
	-$(DEL_FILE) -r $(INSTALL_ROOT)/usr/local/bin/pc-xdgutil
	-$(DEL_DIR) $(INSTALL_ROOT)/usr/local/bin/ 


install_dochmod: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)/usr/local/bin/ || $(MKDIR) $(INSTALL_ROOT)/usr/local/bin/ 
	chmod 755 /usr/local/bin/pc-xdgutil


install_ddirs: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-xdgutil/ || $(MKDIR) $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-xdgutil/ 
	-$(INSTALL_DIR) desktop-dirs $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-xdgutil/


uninstall_ddirs:  FORCE
	-$(DEL_FILE) -r $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-xdgutil/desktop-dirs
	-$(DEL_DIR) $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-xdgutil/ 


install_xdgmenu: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-xdgutil/ || $(MKDIR) $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-xdgutil/ 
	-$(INSTALL_FILE) menus/applications.menu $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-xdgutil/
	-$(INSTALL_FILE) menus/xfce-applications.menu $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-xdgutil/


uninstall_xdgmenu:  FORCE
	-$(DEL_FILE) -r $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-xdgutil/applications.menu 
	 -$(DEL_FILE) -r $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-xdgutil/xfce-applications.menu
	-$(DEL_DIR) $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-xdgutil/ 

install:  install_scripts install_dochmod install_ddirs install_xdgmenu  FORCE

uninstall: uninstall_scripts uninstall_ddirs uninstall_xdgmenu   FORCE

FORCE:

