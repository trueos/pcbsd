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
	-$(INSTALL_FILE) pc-metapkgmanager $(INSTALL_ROOT)$(PREFIX)/bin/
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)$(PREFIX)/share/pcbsd/ || $(MKDIR) $(INSTALL_ROOT)$(PREFIX)/share/pcbsd/
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)$(PREFIX)/share/pcbsd/metaset/ || rm -rf $(PREFIX)/share/pcbsd/metaset
	cp -r metaset $(PREFIX)/share/pcbsd/
	cd $(PREFIX)/bin/ && ln -f pc-metapkgmanager pc-pkg

uninstall_scripts:  FORCE
	-$(DEL_FILE) -r $(INSTALL_ROOT)$(PREFIX)/bin/pc-metapkgmanager
	-$(DEL_DIR) $(INSTALL_ROOT)$(PREFIX)/bin/ 

install_dochmod: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)$(PREFIX)/bin/ || $(MKDIR) $(INSTALL_ROOT)$(PREFIX)/bin/ 
	chmod 755 $(PREFIX)/bin/pc-metapkgmanager

install:  install_scripts install_dochmod  FORCE

uninstall: uninstall_scripts   FORCE

FORCE:

