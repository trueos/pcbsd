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
		sub-about \
		sub-pbreg \
		sub-pcad \
		sub-pcsu \
		sub-pcsys \
		sub-pcex \
		sub-pcxt \
		sub-pcrunx \
		sub-pcup \
		sub-pcfbsd \
		sub-pcfirst \
		sub-pbi \
		sub-pbi10 \
		sub-install \
		sub-deinfo \
		sub-meta \
		sub-warden \
		sub-xdgutil \
		sub-pccheckx
		
sub-pccheckx: pc-checkxdisplay/$(MAKEFILE) FORCE
	cd pc-checkxdisplay/ && $(MAKE) -f $(MAKEFILE)
sub-pccheckx-make_default: pc-checkxdisplay/$(MAKEFILE) FORCE
	cd pc-checkxdisplay/ && $(MAKE) -f $(MAKEFILE) 
sub-pccheckx-make_first: pc-checkxdisplay/$(MAKEFILE) FORCE
	cd pc-checkxdisplay/ && $(MAKE) -f $(MAKEFILE) first
sub-pccheckx-all: pc-checkxdisplay/$(MAKEFILE) FORCE
	cd pc-checkxdisplay/ && $(MAKE) -f $(MAKEFILE) all
sub-pccheckx-clean: pc-checkxdisplay/$(MAKEFILE) FORCE
	cd pc-checkxdisplay/ && $(MAKE) -f $(MAKEFILE) clean
sub-pccheckx-install_subtargets: pc-checkxdisplay/$(MAKEFILE) FORCE
	cd pc-checkxdisplay/ && $(MAKE) -f $(MAKEFILE) install		

sub-deinfo: pc-deinfo/$(MAKEFILE) FORCE
	cd de-info/ && $(MAKE) -f $(MAKEFILE)
sub-deinfo-make_default: de-info/$(MAKEFILE) FORCE
	cd de-info/ && $(MAKE) -f $(MAKEFILE) 
sub-deinfo-make_first: de-info/$(MAKEFILE) FORCE
	cd de-info/ && $(MAKE) -f $(MAKEFILE) first
sub-deinfo-all: de-info/$(MAKEFILE) FORCE
	cd de-info/ && $(MAKE) -f $(MAKEFILE) all
sub-deinfo-clean: de-info/$(MAKEFILE) FORCE
	cd de-info/ && $(MAKE) -f $(MAKEFILE) clean
sub-deinfo-install_subtargets: de-info/$(MAKEFILE) FORCE
	cd de-info/ && $(MAKE) -f $(MAKEFILE) install

sub-xdgutil: pc-xdgutil/$(MAKEFILE) FORCE
	cd pc-xdgutil/ && $(MAKE) -f $(MAKEFILE)
sub-xdgutil-make_default: pc-xdgutil/$(MAKEFILE) FORCE
	cd pc-xdgutil/ && $(MAKE) -f $(MAKEFILE) 
sub-xdgutil-make_first: pc-xdgutil/$(MAKEFILE) FORCE
	cd pc-xdgutil/ && $(MAKE) -f $(MAKEFILE) first
sub-xdgutil-all: pc-xdgutil/$(MAKEFILE) FORCE
	cd pc-xdgutil/ && $(MAKE) -f $(MAKEFILE) all
sub-xdgutil-clean: pc-xdgutil/$(MAKEFILE) FORCE
	cd pc-xdgutil/ && $(MAKE) -f $(MAKEFILE) clean
sub-xdgutil-install_subtargets: pc-xdgutil/$(MAKEFILE) FORCE
	cd pc-xdgutil/ && $(MAKE) -f $(MAKEFILE) install

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

sub-about: about/$(MAKEFILE) FORCE
	cd about/ && $(MAKE) -f $(MAKEFILE)
sub-about-make_default: about/$(MAKEFILE) FORCE
	cd about/ && $(MAKE) -f $(MAKEFILE) 
sub-about-make_first: about/$(MAKEFILE) FORCE
	cd about/ && $(MAKE) -f $(MAKEFILE) first
sub-about-all: about/$(MAKEFILE) FORCE
	cd about/ && $(MAKE) -f $(MAKEFILE) all
sub-about-clean: about/$(MAKEFILE) FORCE
	cd about/ && $(MAKE) -f $(MAKEFILE) clean
sub-about-install_subtargets: about/$(MAKEFILE) FORCE
	cd about/ && $(MAKE) -f $(MAKEFILE) install

sub-pbreg: pbreg/$(MAKEFILE) FORCE
	cd pbreg/ && $(MAKE) -f $(MAKEFILE)
sub-pbreg-make_default: pbreg/$(MAKEFILE) FORCE
	cd pbreg/ && $(MAKE) -f $(MAKEFILE) 
sub-pbreg-make_first: pbreg/$(MAKEFILE) FORCE
	cd pbreg/ && $(MAKE) -f $(MAKEFILE) first
sub-pbreg-all: pbreg/$(MAKEFILE) FORCE
	cd pbreg/ && $(MAKE) -f $(MAKEFILE) all
sub-pbreg-clean: pbreg/$(MAKEFILE) FORCE
	cd pbreg/ && $(MAKE) -f $(MAKEFILE) clean
sub-pbreg-install_subtargets: pbreg/$(MAKEFILE) FORCE
	cd pbreg/ && $(MAKE) -f $(MAKEFILE) install

sub-pcxt: xtrafiles/$(MAKEFILE) FORCE
	cd xtrafiles/ && $(MAKE) -f $(MAKEFILE)
sub-pcxt-make_default: xtrafiles/$(MAKEFILE) FORCE
	cd xtrafiles/ && $(MAKE) -f $(MAKEFILE) 
sub-pcxt-make_first: xtrafiles/$(MAKEFILE) FORCE
	cd xtrafiles/ && $(MAKE) -f $(MAKEFILE) first
sub-pcxt-all: xtrafiles/$(MAKEFILE) FORCE
	cd xtrafiles/ && $(MAKE) -f $(MAKEFILE) all
sub-pcxt-clean: xtrafiles/$(MAKEFILE) FORCE
	cd xtrafiles/ && $(MAKE) -f $(MAKEFILE) clean
sub-pcxt-install_subtargets: xtrafiles/$(MAKEFILE) FORCE
	cd xtrafiles/ && $(MAKE) -f $(MAKEFILE) && $(MAKE) -f $(MAKEFILE) install

sub-pcex: pc-extractoverlay/$(MAKEFILE) FORCE
	cd pc-extractoverlay/ && $(MAKE) -f $(MAKEFILE)
sub-pcex-make_default: pc-extractoverlay/$(MAKEFILE) FORCE
	cd pc-extractoverlay/ && $(MAKE) -f $(MAKEFILE) 
sub-pcex-make_first: pc-extractoverlay/$(MAKEFILE) FORCE
	cd pc-extractoverlay/ && $(MAKE) -f $(MAKEFILE) first
sub-pcex-all: pc-extractoverlay/$(MAKEFILE) FORCE
	cd pc-extractoverlay/ && $(MAKE) -f $(MAKEFILE) all
sub-pcex-clean: pc-extractoverlay/$(MAKEFILE) FORCE
	cd pc-extractoverlay/ && $(MAKE) -f $(MAKEFILE) clean
sub-pcex-install_subtargets: pc-extractoverlay/$(MAKEFILE) FORCE
	cd pc-extractoverlay/ && $(MAKE) -f $(MAKEFILE) && $(MAKE) -f $(MAKEFILE) install

sub-pcad: pc-adctl/$(MAKEFILE) FORCE
	cd pc-adctl/ && $(MAKE) -f $(MAKEFILE)
sub-pcad-make_default: pc-adctl/$(MAKEFILE) FORCE
	cd pc-adctl/ && $(MAKE) -f $(MAKEFILE) 
sub-pcad-make_first: pc-adctl/$(MAKEFILE) FORCE
	cd pc-adctl/ && $(MAKE) -f $(MAKEFILE) first
sub-pcad-all: pc-adctl/$(MAKEFILE) FORCE
	cd pc-adctl/ && $(MAKE) -f $(MAKEFILE) all
sub-pcad-clean: pc-adctl/$(MAKEFILE) FORCE
	cd pc-adctl/ && $(MAKE) -f $(MAKEFILE) clean
sub-pcad-install_subtargets: pc-adctl/$(MAKEFILE) FORCE
	cd pc-adctl/ && $(MAKE) -f $(MAKEFILE) && $(MAKE) -f $(MAKEFILE) install

sub-pcsys: pc-sysinstall/$(MAKEFILE) FORCE
	cd pc-sysinstall/ && $(MAKE) -f $(MAKEFILE)
sub-pcsys-make_default: pc-sysinstall/$(MAKEFILE) FORCE
	cd pc-sysinstall/ && $(MAKE) -f $(MAKEFILE) 
sub-pcsys-make_first: pc-sysinstall/$(MAKEFILE) FORCE
	cd pc-sysinstall/ && $(MAKE) -f $(MAKEFILE) first
sub-pcsys-all: pc-sysinstall/$(MAKEFILE) FORCE
	cd pc-sysinstall/ && $(MAKE) -f $(MAKEFILE) all
sub-pcsys-clean: pc-sysinstall/$(MAKEFILE) FORCE
	cd pc-sysinstall/ && $(MAKE) -f $(MAKEFILE) clean
sub-pcsys-install_subtargets: pc-sysinstall/$(MAKEFILE) FORCE
	cd pc-sysinstall/ && $(MAKE) -f $(MAKEFILE) install

sub-pcsu: pc-su/$(MAKEFILE) FORCE
	cd pc-su/ && $(MAKE) -f $(MAKEFILE)
sub-pcsu-make_default: pc-su/$(MAKEFILE) FORCE
	cd pc-su/ && $(MAKE) -f $(MAKEFILE) 
sub-pcsu-make_first: pc-su/$(MAKEFILE) FORCE
	cd pc-su/ && $(MAKE) -f $(MAKEFILE) first
sub-pcsu-all: pc-su/$(MAKEFILE) FORCE
	cd pc-su/ && $(MAKE) -f $(MAKEFILE) all
sub-pcsu-clean: pc-su/$(MAKEFILE) FORCE
	cd pc-su/ && $(MAKE) -f $(MAKEFILE) clean
sub-pcsu-install_subtargets: pc-su/$(MAKEFILE) FORCE
	cd pc-su/ && $(MAKE) -f $(MAKEFILE) install

sub-pcup: pc-updatemanager/$(MAKEFILE) FORCE
	cd pc-updatemanager/ && $(MAKE) -f $(MAKEFILE)
sub-pcup-make_default: pc-updatemanager/$(MAKEFILE) FORCE
	cd pc-updatemanager/ && $(MAKE) -f $(MAKEFILE) 
sub-pcup-make_first: pc-updatemanager/$(MAKEFILE) FORCE
	cd pc-updatemanager/ && $(MAKE) -f $(MAKEFILE) first
sub-pcup-all: pc-updatemanager/$(MAKEFILE) FORCE
	cd pc-updatemanager/ && $(MAKE) -f $(MAKEFILE) all
sub-pcup-clean: pc-updatemanager/$(MAKEFILE) FORCE
	cd pc-updatemanager/ && $(MAKE) -f $(MAKEFILE) clean
sub-pcup-install_subtargets: pc-updatemanager/$(MAKEFILE) FORCE
	cd pc-updatemanager/ && $(MAKE) -f $(MAKEFILE) install

sub-pcfbsd: pc-fbsdupdatecheck/$(MAKEFILE) FORCE
	cd pc-fbsdupdatecheck/ && $(MAKE) -f $(MAKEFILE)
sub-pcfbsd-make_default: pc-fbsdupdatecheck/$(MAKEFILE) FORCE
	cd pc-fbsdupdatecheck/ && $(MAKE) -f $(MAKEFILE) 
sub-pcfbsd-make_first: pc-fbsdupdatecheck/$(MAKEFILE) FORCE
	cd pc-fbsdupdatecheck/ && $(MAKE) -f $(MAKEFILE) first
sub-pcfbsd-all: pc-fbsdupdatecheck/$(MAKEFILE) FORCE
	cd pc-fbsdupdatecheck/ && $(MAKE) -f $(MAKEFILE) all
sub-pcfbsd-clean: pc-fbsdupdatecheck/$(MAKEFILE) FORCE
	cd pc-fbsdupdatecheck/ && $(MAKE) -f $(MAKEFILE) clean
sub-pcfbsd-install_subtargets: pc-fbsdupdatecheck/$(MAKEFILE) FORCE
	cd pc-fbsdupdatecheck/ && $(MAKE) -f $(MAKEFILE) install

sub-pcfirst: pc-firstgui/$(MAKEFILE) FORCE
	cd pc-firstgui/ && $(MAKE) -f $(MAKEFILE)
sub-pcfirst-make_default: pc-firstgui/$(MAKEFILE) FORCE
	cd pc-firstgui/ && $(MAKE) -f $(MAKEFILE) 
sub-pcfirst-make_first: pc-firstgui/$(MAKEFILE) FORCE
	cd pc-firstgui/ && $(MAKE) -f $(MAKEFILE) first
sub-pcfirst-all: pc-firstgui/$(MAKEFILE) FORCE
	cd pc-firstgui/ && $(MAKE) -f $(MAKEFILE) all
sub-pcfirst-clean: pc-firstgui/$(MAKEFILE) FORCE
	cd pc-firstgui/ && $(MAKE) -f $(MAKEFILE) clean
sub-pcfirst-install_subtargets: pc-firstgui/$(MAKEFILE) FORCE
	cd pc-firstgui/ && $(MAKE) -f $(MAKEFILE) install

sub-pcrunx: pc-runxgui/$(MAKEFILE) FORCE
	cd pc-runxgui/ && $(MAKE) -f $(MAKEFILE)
sub-pcrunx-make_default: pc-runxgui/$(MAKEFILE) FORCE
	cd pc-runxgui/ && $(MAKE) -f $(MAKEFILE) 
sub-pcrunx-make_first: pc-runxgui/$(MAKEFILE) FORCE
	cd pc-runxgui/ && $(MAKE) -f $(MAKEFILE) first
sub-pcrunx-all: pc-runxgui/$(MAKEFILE) FORCE
	cd pc-runxgui/ && $(MAKE) -f $(MAKEFILE) all
sub-pcrunx-clean: pc-runxgui/$(MAKEFILE) FORCE
	cd pc-runxgui/ && $(MAKE) -f $(MAKEFILE) clean
sub-pcrunx-install_subtargets: pc-runxgui/$(MAKEFILE) FORCE
	cd pc-runxgui/ && $(MAKE) -f $(MAKEFILE) install

sub-install: pc-installdialog/$(MAKEFILE) FORCE
	cd pc-installdialog/ && $(MAKE) -f $(MAKEFILE)
sub-install-make_default: pc-installdialog/$(MAKEFILE) FORCE
	cd pc-installdialog/ && $(MAKE) -f $(MAKEFILE) 
sub-install-make_first: pc-installdialog/$(MAKEFILE) FORCE
	cd pc-installdialog/ && $(MAKE) -f $(MAKEFILE) first
sub-install-all: pc-installdialog/$(MAKEFILE) FORCE
	cd pc-installdialog/ && $(MAKE) -f $(MAKEFILE) all
sub-install-clean: pc-installdialog/$(MAKEFILE) FORCE
	cd pc-installdialog/ && $(MAKE) -f $(MAKEFILE) clean
sub-install-install_subtargets: pc-installdialog/$(MAKEFILE) FORCE
	cd pc-installdialog/ && $(MAKE) -f $(MAKEFILE) install

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

sub-meta: pc-metapkgmanager/$(MAKEFILE) FORCE
	cd pc-metapkgmanager/ && $(MAKE) -f $(MAKEFILE)
sub-meta-make_default: pc-metapkgmanager/$(MAKEFILE) FORCE
	cd pc-metapkgmanager/ && $(MAKE) -f $(MAKEFILE) 
sub-meta-make_first: pc-metapkgmanager/$(MAKEFILE) FORCE
	cd pc-metapkgmanager/ && $(MAKE) -f $(MAKEFILE) first
sub-meta-all: pc-metapkgmanager/$(MAKEFILE) FORCE
	cd pc-metapkgmanager/ && $(MAKE) -f $(MAKEFILE) all
sub-meta-clean: pc-metapkgmanager/$(MAKEFILE) FORCE
	cd pbi-metapkgmanager/ && $(MAKE) -f $(MAKEFILE) clean
sub-meta-install_subtargets: pc-metapkgmanager/$(MAKEFILE) FORCE
	cd pc-metapkgmanager/ && $(MAKE) -f $(MAKEFILE) install

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

sub-pcthinclient: pc-thinclient/$(MAKEFILE) FORCE
	cd pc-thinclient/ && $(MAKE) -f $(MAKEFILE)
sub-pcthinclient-make_default: pc-thinclient/$(MAKEFILE) FORCE
	cd pc-thinclient/ && $(MAKE) -f $(MAKEFILE) 
sub-pcthinclient-make_first: pc-thinclient/$(MAKEFILE) FORCE
	cd pc-thinclient/ && $(MAKE) -f $(MAKEFILE) first
sub-pcthinclient-all: pc-thinclient/$(MAKEFILE) FORCE
	cd pc-thinclient/ && $(MAKE) -f $(MAKEFILE) all
sub-pcthinclient-clean: pc-thinclient/$(MAKEFILE) FORCE
	cd pc-thinclient/ && $(MAKE) -f $(MAKEFILE) clean
sub-pcthinclient-install_subtargets: pc-thinclient/$(MAKEFILE) FORCE
	cd pc-thinclient/ && $(MAKE) -f $(MAKEFILE) install

make_default: sub-libsh-make_default \
	sub-lpreserver-make_default \
	sub-pbi-make_default \
	sub-pbi10-make_default \
	sub-about-make_default \
	sub-install-make_default \
	sub-meta-make_default \
	sub-warden-make_default \
	sub-pcrunx-make_default \
	sub-pcup-make_default \
	sub-pcfbsd-make_default \
	sub-pcfirst-make_default \
	sub-pbreg-make_default \
	sub-pcad-make_default \
	sub-pcsu-make_default \
	sub-pcsys-make_default \
	sub-pcex-make_default \
	sub-pcxt-make_default \
	sub-deinfo-make_default \
	sub-pcthinclient-make_default \
	sub-xdgutil-make_default \
	sub-pccheckx-make_default \
	FORCE

make_first: sub-libsh-make_first \
	sub-lpreserver-make_first \
	sub-pbi-make_first \
	sub-pbi10-make_first \
	sub-about-make_first \
	sub-install-make_first \
	sub-meta-make_first \
	sub-warden-make_first \
	sub-pcrunx-make_first \
	sub-pcup-make_first \
	sub-pcfbsd-make_first \
	sub-pcfirst-make_first \
	sub-pcsu-make_first \
	sub-pcsys-make_first \
	sub-pcex-make_first \
	sub-pcxt-make_first \
	sub-pbreg-make_first \
	sub-pcad-make_first \
	sub-deinfo-make_first \
	sub-pcthinclient-make_first \
	sub-xdgutil-make_first \
	sub-pccheckx-make_first \
	FORCE

all:	sub-libsh-all \
	sub-lpreserver-all \
	sub-pbi-all \
	sub-pbi10-all \
	sub-about-all \
	sub-install-all \
	sub-meta-all \
	sub-warden-all \
	sub-pcrunx-all \
	sub-pcup-all \
	sub-pcfbsd-all \
	sub-pcfirst-all \
	sub-pcsu-all \
	sub-pcsys-all \
	sub-pcex-all \
	sub-pcxt-all \
	sub-pbreg-all \
	sub-pcad-all \
	sub-deinfo-all \
	sub-pcthinclient-all \
	sub-xdgutil-all \
	sub-pccheckx-all \
	FORCE

clean: sub-pbi-clean \
	sub-pbi10-clean \
	sub-libsh-clean \
	sub-lpreserver-clean \
	sub-about-clean \
	sub-install-clean \
	sub-meta-clean \
	sub-warden-clean \
	sub-pcrunx-clean \
	sub-pcup-clean \
	sub-pcfbsd-clean \
	sub-pcfirst-clean \
	sub-pcsu-clean \
	sub-pcsys-clean \
	sub-pcex-clean \
	sub-pcxt-clean \
	sub-pbreg-clean \
	sub-pcad-clean \
	sub-deinfo-clean \
	sub-pcthinclient-clean \
	sub-xdgutil-clean \
	sub-pccheckx-clean \
	FORCE

install_subtargets: sub-libsh-install_subtargets \
	sub-pbi-install_subtargets \
	sub-pbi10-install_subtargets \
	sub-lpreserver-install_subtargets \
	sub-about-install_subtargets \
	sub-install-install_subtargets \
	sub-meta-install_subtargets \
	sub-warden-install_subtargets \
	sub-pcrunx-install_subtargets \
	sub-pcup-install_subtargets \
	sub-pcfbsd-install_subtargets \
	sub-pcfirst-install_subtargets \
	sub-pcsu-install_subtargets \
	sub-pcsys-install_subtargets \
	sub-pcex-install_subtargets \
	sub-pcxt-install_subtargets \
	sub-pbreg-install_subtargets \
	sub-pcad-install_subtargets \
	sub-deinfo-install_subtargets \
	sub-pcthinclient-install_subtargets \
	sub-xdgutil-install_subtargets \
	sub-pccheckx-install_subtargets \
	FORCE

install: install_subtargets FORCE

FORCE:

