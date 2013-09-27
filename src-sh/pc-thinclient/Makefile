#############################################################################
# Makefile for building: pc-xdgutil
#############################################################################

PREFIX?= /usr/local

AR            = ar cqs
RANLIB        = 
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
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)$(PREFIX)/bin/ || $(MKDIR) $(INSTALL_ROOT)$(PREFIX)/bin/ 
	-$(INSTALL_FILE) pc-thinclient $(INSTALL_ROOT)$(PREFIX)/bin/


uninstall_scripts:  FORCE
	-$(DEL_FILE) -r $(INSTALL_ROOT)$(PREFIX)/bin/pc-thinclient
	-$(DEL_DIR) $(INSTALL_ROOT)$(PREFIX)/bin/ 


install_dochmod: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)$(PREFIX)/bin/ || $(MKDIR) $(INSTALL_ROOT)$(PREFIX)/bin/ 
	chmod 755 $(PREFIX)/bin/pc-thinclient


install_ddirs: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)$(PREFIX)/share/pcbsd/pc-thinclient/ || $(MKDIR) $(INSTALL_ROOT)$(PREFIX)/share/pcbsd/pc-thinclient/ 
	-$(INSTALL_DIR) resources $(INSTALL_ROOT)$(PREFIX)/share/pcbsd/pc-thinclient/

uninstall_ddirs:  FORCE
	-$(DEL_FILE) -r $(INSTALL_ROOT)$(PREFIX)/share/pcbsd/pc-thinclient/resources
	-$(DEL_DIR) $(INSTALL_ROOT)$(PREFIX)/share/pcbsd/pc-thinclient/ 

install:  install_scripts install_dochmod install_ddirs  FORCE

uninstall: uninstall_scripts uninstall_ddirs  FORCE

FORCE:

