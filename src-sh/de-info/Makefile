#############################################################################
# Makefile for building: pc-metapkgmanager
#############################################################################

####### Compiler, tools and options
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

all: Makefile $(TARGET)

check: first

install_scripts: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)$(PREFIX)/bin/ || $(MKDIR) $(INSTALL_ROOT)$(PREFIX)/bin/ 
	-$(INSTALL_FILE) de-info $(INSTALL_ROOT)$(PREFIX)/bin/

uninstall_scripts:  FORCE
	-$(DEL_FILE) -r $(INSTALL_ROOT)$(PREFIX)/bin/de-info
	-$(DEL_DIR) $(INSTALL_ROOT)$(PREFIX)/bin/ 

install_dochmod: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)$(PREFIX)/bin/ || $(MKDIR) $(INSTALL_ROOT)$(PREFIX)/bin/ 
	chmod 755 $(PREFIX)/bin/de-info

install_ddirs: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)$(PREFIX)/share/pcbsd/de-profiles/ || $(MKDIR) $(INSTALL_ROOT)$(PREFIX)/share/pcbsd/de-profiles/ 
	-$(INSTALL_DIR) de-profiles $(INSTALL_ROOT)$(PREFIX)/share/pcbsd

uninstall_ddirs:  FORCE
	-$(DEL_DIR) $(INSTALL_ROOT)$(PREFIX)/share/pcbsd/de-profiles/ 

install:  install_scripts install_dochmod install_ddirs FORCE

uninstall: uninstall_scripts uninstall_ddirs  FORCE

FORCE:

