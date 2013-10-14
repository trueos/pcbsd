# New ports collection makefile for:	trueos-base
# Date created:				01 Feb 2013
# Whom:					Kris Moore <kmoore@FreeBSD.org>
#
# $FreeBSD$

PORTNAME=	trueos-base
PORTVERSION=	CHGVERSION
CATEGORIES=	misc
MASTER_SITES=	# Empty
DISTFILES=	# Empty

MAINTAINER=	kmoore@FreeBSD.org
COMMENT=	PC-BSD / TrueOS Command-Line Utilities

RUN_DEPENDS=    pkg-static:${PORTSDIR}/ports-mgmt/pkg \
		pc-metapkgmanager:${PORTSDIR}/sysutils/pcbsd-utils \
		rpm:${PORTSDIR}/archivers/rpm5 \
		xdg-desktop-icon:${PORTSDIR}/devel/xdg-utils \
		bsdstats>=0:${PORTSDIR}/sysutils/bsdstats \
		unrar:${PORTSDIR}/archivers/unrar \
		beadm:${PORTSDIR}/sysutils/beadm-devel \
		sudo:${PORTSDIR}/security/sudo \
		bash:${PORTSDIR}/shells/bash \
		zsh:${PORTSDIR}/shells/zsh \
		nano:${PORTSDIR}/editors/nano \
		vim:${PORTSDIR}/editors/vim-lite \
		rsync:${PORTSDIR}/net/rsync \
		svn:${PORTSDIR}/devel/subversion \
		mksquashfs:${PORTSDIR}/sysutils/squashfs-tools \
		debootstrap:${PORTSDIR}/sysutils/debootstrap \
		smartd:${PORTSDIR}/sysutils/smartmontools \
		jailme:${PORTSDIR}/sysutils/jailme \
		python:${PORTSDIR}/lang/python \
		screen:${PORTSDIR}/sysutils/screen \
		nano:${PORTSDIR}/editors/nano \
		mosh:${PORTSDIR}/net/mosh \
		zfs-stats:${PORTSDIR}/sysutils/zfs-stats \
		ataidle>=0:${PORTSDIR}/sysutils/ataidle \
		grub-mkconfig:${PORTSDIR}/sysutils/grub2 \
		grub2-efi>=0:${PORTSDIR}/sysutils/grub2-efi

NO_BUILD=	yes

do-install:
	${DO_NADA}

.include <bsd.port.mk>
