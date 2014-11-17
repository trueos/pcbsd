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
		sub-libsh \
		sub-lpreserver \
		sub-warden \
		sub-pcbsdutils \
		sub-pbi \
		sub-syscache
		
sub-pcbsdutils: pcbsd-utils/$(MAKEFILE) FORCE
	cd pcbsd-utils/ && $(MAKE) -f $(MAKEFILE)
sub-pcbsdutils-make_default: pcbsd-utils/$(MAKEFILE) FORCE
	cd pcbsd-utils/ && $(MAKE) -f $(MAKEFILE) 
sub-pcbsdutils-make_first: pcbsd-utils/$(MAKEFILE) FORCE
	cd pcbsd-utils/ && $(MAKE) -f $(MAKEFILE) first
sub-pcbsdutils-all: pcbsd-utils/$(MAKEFILE) FORCE
	cd pcbsd-utils/ && $(MAKE) -f $(MAKEFILE) all
sub-pcbsdutils-clean: pcbsd-utils/$(MAKEFILE) FORCE
	cd pcbsd-utils/ && $(MAKE) -f $(MAKEFILE) clean
sub-pcbsdutils-install_subtargets: pcbsd-utils/$(MAKEFILE) FORCE
	cd pcbsd-utils/ && $(MAKE) -f $(MAKEFILE) install		

sub-syscache: pc-syscache/$(MAKEFILE) FORCE
	cd syscache/ && $(MAKE) -f $(MAKEFILE)
sub-syscache-make_default: syscache/$(MAKEFILE) FORCE
	cd syscache/ && $(MAKE) -f $(MAKEFILE) 
sub-syscache-make_first: syscache/$(MAKEFILE) FORCE
	cd syscache/ && $(MAKE) -f $(MAKEFILE) first
sub-syscache-all: syscache/$(MAKEFILE) FORCE
	cd syscache/ && $(MAKE) -f $(MAKEFILE) all
sub-syscache-clean: syscache/$(MAKEFILE) FORCE
	cd syscache/ && $(MAKE) -f $(MAKEFILE) clean
sub-syscache-install_subtargets: syscache/$(MAKEFILE) FORCE
	cd syscache/ && $(MAKE) -f $(MAKEFILE) install		

sub-lpreserver: lpreserver/$(MAKEFILE) FORCE
	cd lpreserver/ && $(MAKE) -f $(MAKEFILE)
sub-lpreserver-make_default: lpreserver/$(MAKEFILE) FORCE
	cd lpreserver/ && $(MAKE) -f $(MAKEFILE) 
sub-lpreserver-make_first: lpreserver/$(MAKEFILE) FORCE
	cd lpreserver/ && $(MAKE) -f $(MAKEFILE) first
sub-lpreserver-all: lpreserver/$(MAKEFILE) FORCE
	cd lpreserver/ && $(MAKE) -f $(MAKEFILE) all
sub-lpreserver-clean: lpreserver/$(MAKEFILE) FORCE
	cd lpreserver/ && $(MAKE) -f $(MAKEFILE) clean
sub-lpreserver-install_subtargets: lpreserver/$(MAKEFILE) FORCE
	cd lpreserver/ && $(MAKE) -f $(MAKEFILE) install

sub-libsh: libsh/$(MAKEFILE) FORCE
	cd libsh/ && $(MAKE) -f $(MAKEFILE)
sub-libsh-make_default: libsh/$(MAKEFILE) FORCE
	cd libsh/ && $(MAKE) -f $(MAKEFILE) 
sub-libsh-make_first: libsh/$(MAKEFILE) FORCE
	cd libsh/ && $(MAKE) -f $(MAKEFILE) first
sub-libsh-all: libsh/$(MAKEFILE) FORCE
	cd libsh/ && $(MAKE) -f $(MAKEFILE) all
sub-libsh-clean: libsh/$(MAKEFILE) FORCE
	cd libsh/ && $(MAKE) -f $(MAKEFILE) clean
sub-libsh-install_subtargets: libsh/$(MAKEFILE) FORCE
	cd libsh/ && $(MAKE) -f $(MAKEFILE) install

sub-warden: warden/$(MAKEFILE) FORCE
	cd warden/ && $(MAKE) -f $(MAKEFILE)
sub-warden-make_default: warden/$(MAKEFILE) FORCE
	cd warden/ && $(MAKE) -f $(MAKEFILE) 
sub-warden-make_first: warden/$(MAKEFILE) FORCE
	cd warden/ && $(MAKE) -f $(MAKEFILE) first
sub-warden-all: warden/$(MAKEFILE) FORCE
	cd warden/ && $(MAKE) -f $(MAKEFILE) all
sub-warden-clean: warden/$(MAKEFILE) FORCE
	cd warden/ && $(MAKE) -f $(MAKEFILE) clean
sub-warden-install_subtargets: warden/$(MAKEFILE) FORCE
	cd warden/ && $(MAKE) -f $(MAKEFILE) install

sub-pbi: pbi-manager/$(MAKEFILE) FORCE
	cd pbi-manager/ && $(MAKE) -f $(MAKEFILE)
sub-pbi-make_default: pbi-manager/$(MAKEFILE) FORCE
	cd pbi-manager/ && $(MAKE) -f $(MAKEFILE) 
sub-pbi-make_first: pbi-manager/$(MAKEFILE) FORCE
	cd pbi-manager/ && $(MAKE) -f $(MAKEFILE) first
sub-pbi-all: pbi-manager/$(MAKEFILE) FORCE
	cd pbi-manager/ && $(MAKE) -f $(MAKEFILE) all
sub-pbi-clean: pbi-manager/$(MAKEFILE) FORCE
	cd pbi-manager/ && $(MAKE) -f $(MAKEFILE) clean
sub-pbi-install_subtargets: pbi-manager/$(MAKEFILE) FORCE
	cd pbi-manager/ && $(MAKE) -f $(MAKEFILE) install
	
make_default: sub-libsh-make_default \
	sub-pbi-make_default \
	sub-lpreserver-make_default \
	sub-warden-make_default \
	sub-syscache-make_default \
	sub-pcbsdutils-make_default \
	FORCE

make_first: sub-libsh-make_first \
	sub-pbi-make_first \
	sub-lpreserver-make_first \
	sub-warden-make_first \
	sub-syscache-make_first \
	sub-pcbsdutils-make_first \
	FORCE

all:	sub-libsh-all \
	sub-pbi-all \
	sub-lpreserver-all \
	sub-warden-all \
	sub-syscache-all \
	sub-pcbsdutils-all \
	FORCE

clean: sub-pbi-clean \
	sub-libsh-clean \
	sub-lpreserver-clean \
	sub-warden-clean \
	sub-pcbsdutils-clean \
	FORCE

install_subtargets: sub-libsh-install_subtargets \
	sub-pbi-install_subtargets \
	sub-lpreserver-install_subtargets \
	sub-warden-install_subtargets \
	sub-syscache-install_subtargets \
	sub-pcbsdutils-install_subtargets \
	FORCE

install: install_subtargets FORCE

FORCE:

