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
		sub-pcbsdutils
		
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

make_default: sub-libsh-make_default \
	sub-lpreserver-make_default \
	sub-pcbsdutils-make_default \
	FORCE

make_first: sub-libsh-make_first \
	sub-lpreserver-make_first \
	sub-pcbsdutils-make_first \
	FORCE

all:	sub-libsh-all \
	sub-lpreserver-all \
	sub-pcbsdutils-all \
	FORCE

clean: sub-libsh-clean \
	sub-lpreserver-clean \
	sub-pcbsdutils-clean \
	FORCE

install_subtargets: sub-libsh-install_subtargets \
	sub-lpreserver-install_subtargets \
	sub-pcbsdutils-install_subtargets \
	FORCE

install: install_subtargets FORCE

FORCE:

