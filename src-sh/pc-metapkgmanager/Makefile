#############################################################################
# Makefile for building: pc-metapkgmanager
#############################################################################

####### Compiler, tools and options

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
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)/usr/local/bin/ || $(MKDIR) $(INSTALL_ROOT)/usr/local/bin/ 
	-$(INSTALL_FILE) pc-metapkgmanager $(INSTALL_ROOT)/usr/local/bin/
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)/usr/local/share/pcbsd/ || $(MKDIR) $(INSTALL_ROOT)/usr/local/share/pcbsd/
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)/usr/local/share/pcbsd/metaset/ || rm -rf /usr/local/share/pcbsd/metaset
	cp -r metaset /usr/local/share/pcbsd/metaset
	cd /usr/local/bin/ && ln -f pc-metapkgmanager pc-pkg

uninstall_scripts:  FORCE
	-$(DEL_FILE) -r $(INSTALL_ROOT)/usr/local/bin/pc-metapkgmanager
	-$(DEL_DIR) $(INSTALL_ROOT)/usr/local/bin/ 

install_dochmod: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)/usr/local/bin/ || $(MKDIR) $(INSTALL_ROOT)/usr/local/bin/ 
	chmod 755 /usr/local/bin/pc-metapkgmanager

install:  install_scripts install_dochmod  FORCE

uninstall: uninstall_scripts   FORCE

FORCE:

