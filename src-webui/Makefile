#############################################################################
# Makefile for building: src-sh
#############################################################################

first: make_default
MAKEFILE      = Makefile
DEL_FILE      = rm -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
COPY          = cp -f
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -R
INSTALL_FILE  = $(COPY_FILE)
INSTALL_PROGRAM = $(COPY_FILE)
INSTALL_DIR   = $(COPY_DIR)
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
PREFIX?= $(STAGEDIR)/usr/local
SUBTARGETS    =  \
		sub-appweb
		
sub-appweb: appweb/$(MAKEFILE) FORCE
	cd appweb/ && $(MAKE) -f $(MAKEFILE)
sub-appweb-make_default: appweb/$(MAKEFILE) FORCE
	cd appweb/ && $(MAKE) -f $(MAKEFILE) 
sub-appweb-make_first: appweb/$(MAKEFILE) FORCE
	cd appweb/ && $(MAKE) -f $(MAKEFILE) first
sub-appweb-all: appweb/$(MAKEFILE) FORCE
	cd appweb/ && $(MAKE) -f $(MAKEFILE) all
sub-appweb-clean: appweb/$(MAKEFILE) FORCE
	cd appweb/ && $(MAKE) -f $(MAKEFILE) clean
sub-appweb-install_subtargets: appweb/$(MAKEFILE) FORCE
	cd appweb/ && $(MAKE) -f $(MAKEFILE) install		

make_default: sub-appweb-make_default \
	FORCE

make_first: sub-appweb-make_first \
	FORCE

all:	sub-appweb-all \
	FORCE

clean: sub-appweb-clean \
	FORCE

install_subtargets: sub-appweb-install_subtargets \
	FORCE

install: install_subtargets FORCE

FORCE:

