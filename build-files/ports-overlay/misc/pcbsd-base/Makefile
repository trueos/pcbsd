# New ports collection makefile for:	pcbsd-base
# Date created:				01 Feb 2013
# Whom:					Kris Moore <kmoore@FreeBSD.org>
#
# $FreeBSD$

PORTNAME=	pcbsd-base
PORTVERSION=	CHGVERSION
CATEGORIES=	misc
MASTER_SITES=
DISTFILES=

MAINTAINER=	kmoore@FreeBSD.org
COMMENT=	PC-BSD QT4 Utilities

RUN_DEPENDS=	pc-updategui:${PORTSDIR}/sysutils/pcbsd-utils-qt4 \
		beadm:${PORTSDIR}/sysutils/beadm-devel \
		samba36>=0:${PORTSDIR}/net/samba36 \
		foomatic-db>=0:${PORTSDIR}/print/foomatic-db \
		foomatic-db-engine>=0:${PORTSDIR}/print/foomatic-db-engine \
		xorg>=0:${PORTSDIR}/x11/xorg \
		xf86-input-synaptics>=0:${PORTSDIR}/x11-drivers/xf86-input-synaptics \
		xf86-video-intel>=0:${PORTSDIR}/x11-drivers/xf86-video-intel \
		dejavu>=0:${PORTSDIR}/x11-fonts/dejavu \
		xv:${PORTSDIR}/graphics/xv \
		libvdpau>=0:${PORTSDIR}/multimedia/libvdpau \
		rdesktop:${PORTSDIR}/net/rdesktop \
		alsa-lib>=0:${PORTSDIR}/audio/alsa-lib \
		gconf2>=0:${PORTSDIR}/devel/gconf2 \
		nspluginwrapper:${PORTSDIR}/www/nspluginwrapper \
		linux_base-f10>=0:${PORTSDIR}/emulators/linux_base-f10 \
		linux-f10-flashplugin>=0:${PORTSDIR}/www/linux-f10-flashplugin11 \
		linux-f10-alsa-lib>=0:${PORTSDIR}/audio/linux-f10-alsa-lib \
		linux-f10-alsa-plugins-oss>=0:${PORTSDIR}/audio/linux-f10-alsa-plugins-oss \
		linux-f10-cups-libs>=0:${PORTSDIR}/print/linux-f10-cups-libs \
		fluxbox:${PORTSDIR}/x11-wm/fluxbox \
		fluxbox-tenr-styles-pack>=0:${PORTSDIR}/x11-themes/fluxbox-tenr-styles-pack \
		${LOCALBASE}/etc/rc.d/fail2ban:${PORTSDIR}/security/py-fail2ban \
		vorbis-tools>=0:${PORTSDIR}/audio/vorbis-tools \
		zip:${PORTSDIR}/archivers/zip \
		unzip:${PORTSDIR}/archivers/unzip \
		sudo:${PORTSDIR}/security/sudo \
		cdrdao:${PORTSDIR}/sysutils/cdrdao \
		growisofs:${PORTSDIR}/sysutils/dvd+rw-tools \
		ssh-askpass:${PORTSDIR}/security/openssh-askpass \
		vim:${PORTSDIR}/editors/vim \
		cups-smb-backend>=0:${PORTSDIR}/print/cups-smb-backend \
		cups-pdf>=0:${PORTSDIR}/print/cups-pdf \
		cups-bjnp>=0:${PORTSDIR}/print/cups-bjnp \
		splix>=0:${PORTSDIR}/print/splix \
		fusefs-ntfs>=0:${PORTSDIR}/sysutils/fusefs-ntfs \
		xdg-open:${PORTSDIR}/devel/xdg-utils \
		rsync:${PORTSDIR}/net/rsync \
		pwcview:${PORTSDIR}/multimedia/pwcview \
		mtools>=0:${PORTSDIR}/emulators/mtools \
		nss_mdns>=0:${PORTSDIR}/dns/nss_mdns \
		rar:${PORTSDIR}/archivers/rar \
		netatalk>=0:${PORTSDIR}/net/netatalk \
		cursor-jimmac-theme>=0:${PORTSDIR}/x11-themes/cursor-jimmac-theme \
		cuse4bsd-kmod>=0:${PORTSDIR}/multimedia/cuse4bsd-kmod \
		webcamd>=0:${PORTSDIR}/multimedia/webcamd \
		gutenprint>=0:${PORTSDIR}/print/gutenprint \
		gutenprint-cups>=0:${PORTSDIR}/print/gutenprint-cups \
		gutenprint-foomatic>=0:${PORTSDIR}/print/gutenprint-foomatic \
		system-config-printer:${PORTSDIR}/print/system-config-printer \
		mc:${PORTSDIR}/misc/mc \
		at-spi>=0:${PORTSDIR}/accessibility/at-spi \
		metacity:${PORTSDIR}/x11-wm/metacity \
		libgtop>=0:${PORTSDIR}/devel/libgtop \
		numlockx:${PORTSDIR}/x11/numlockx \
		swapexd:${PORTSDIR}/sysutils/swapexd \
		epdfview:${PORTSDIR}/graphics/epdfview \
		pciutils>=0:${PORTSDIR}/sysutils/pciutils \
		gpg:${PORTSDIR}/security/gnupg \
		idesk:${PORTSDIR}/x11/idesk \
		gtk2-qtcurve-theme>=0:${PORTSDIR}/x11-themes/qtcurve-gtk2 \
		qt4-style-qtcurve>=0:${PORTSDIR}/x11-themes/qtcurve-qt4 \
		webfonts>=0:${PORTSDIR}/x11-fonts/webfonts \
		gtk-theme-switch>=0:${PORTSDIR}/x11/gtk-theme-switch2 \
		xscreensaver-gnome>=0:${PORTSDIR}/x11/xscreensaver-gnome \
		foo2zjs>=0:${PORTSDIR}/print/foo2zjs \
		linux_dvbwrapper-kmod>=0:${PORTSDIR}/multimedia/linux_dvbwrapper-kmod \
		xvkbd:${PORTSDIR}/x11/xvkbd \
		volman>=0:${PORTSDIR}/sysutils/volman \
		mplayer:${PORTSDIR}/multimedia/mplayer \
		readline>=0:${PORTSDIR}/devel/readline \
		fribidi>=0:${PORTSDIR}/converters/fribidi \
		ja-font-vlgothic>=0:${PORTSDIR}/japanese/font-vlgothic \
		ibus-qt>=0:${PORTSDIR}/textproc/ibus-qt \
		zh-ibus-chewing>=0:${PORTSDIR}/chinese/ibus-chewing \
		zh-ibus-pinyin>=0:${PORTSDIR}/chinese/ibus-pinyin \
		ibus-m17n>=0:${PORTSDIR}/textproc/ibus-m17n \
		ibus-table>=0:${PORTSDIR}/textproc/ibus-table \
		fonts-indic>=0:${PORTSDIR}/x11-fonts/fonts-indic \
		ko-baekmukfonts-ttf>=0:${PORTSDIR}/korean/baekmukfonts-ttf \
		wqy>=0:${PORTSDIR}/x11-fonts/wqy \
		libdvdcss>=0:${PORTSDIR}/multimedia/libdvdcss \
		pam_ldap>=0:${PORTSDIR}/security/pam_ldap \
		pam_mkhomedir>=0:${PORTSDIR}/security/pam_mkhomedir \
		nss_ldap>=0:${PORTSDIR}/net/nss_ldap \
		rpm2cpio>=0:${PORTSDIR}/archivers/rpm2cpio \
		gsmartcontrol:${PORTSDIR}/sysutils/gsmartcontrol \
		xsane:${PORTSDIR}/graphics/xsane \
		emacs:${PORTSDIR}/editors/emacs \
		libaacs>=0:${PORTSDIR}/multimedia/libaacs \
		xbrightness>=0:${PORTSDIR}/x11/xbrightness \
		fusefs-ext4fuse>=0:${PORTSDIR}/sysutils/fusefs-ext4fuse \
		gstreamer-plugins-cdaudio>=0:${PORTSDIR}/audio/gstreamer-plugins-cdaudio \
		gstreamer-plugins-cdparanoia>=0:${PORTSDIR}/audio/gstreamer-plugins-cdparanoia \
		ar-aspell>=0:${PORTSDIR}/arabic/aspell \
		fr-aspell>=0:${PORTSDIR}/french/aspell \
		de-aspell>=0:${PORTSDIR}/german/aspell \
		iw-aspell>=0:${PORTSDIR}/hebrew/aspell \
		hu-aspell>=0:${PORTSDIR}/hungarian/aspell \
		pl-aspell>=0:${PORTSDIR}/polish/aspell \
		ru-aspell>=0:${PORTSDIR}/russian/aspell \
		uk-aspell>=0:${PORTSDIR}/ukrainian/aspell \
		vi-aspell>=0:${PORTSDIR}/vietnamese/aspell \
		mosh:${PORTSDIR}/net/mosh \
		virtualbox-ose>=0:${PORTSDIR}/emulators/virtualbox-ose \
		virtualbox-ose-kmod>=0:${PORTSDIR}/emulators/virtualbox-ose-kmod \
		x11vnc>=0:${PORTSDIR}/net/x11vnc \
		ataidle>=0:${PORTSDIR}/sysutils/ataidle \
		tk84>=0:${PORTSDIR}/x11-toolkits/tk84 \
		tk85>=0:${PORTSDIR}/x11-toolkits/tk85 \
		tk86>=0:${PORTSDIR}/x11-toolkits/tk86 \
		gdm>=0:${PORTSDIR}/x11/gdm \
		panicmail>=0:${PORTSDIR}/sysutils/panicmail \
		grub-mkconfig:${PORTSDIR}/sysutils/grub2 \
		grub2-efi>=0:${PORTSDIR}/sysutils/grub2-efi

NO_BUILD=	yes

do-install:
	${DO_NADA}

.include <bsd.port.mk>
