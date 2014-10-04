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

RUN_DEPENDS=	pc-metapkgmanager:${PORTSDIR}/sysutils/pcbsd-utils \
		appcafe-setpass:${PORTSDIR}/sysutils/pcbsd-appweb \
		beadm:${PORTSDIR}/sysutils/beadm-devel \
		sudo:${PORTSDIR}/security/sudo \
		bash:${PORTSDIR}/shells/bash \
		zsh:${PORTSDIR}/shells/zsh \
		rsync:${PORTSDIR}/net/rsync \
		smartd:${PORTSDIR}/sysutils/smartmontools \
		jailme:${PORTSDIR}/sysutils/jailme \
		screen:${PORTSDIR}/sysutils/screen \
		nano:${PORTSDIR}/editors/nano \
		mosh:${PORTSDIR}/net/mosh \
		zfs-stats:${PORTSDIR}/sysutils/zfs-stats \
		ataidle>=0:${PORTSDIR}/sysutils/ataidle \
		panicmail>=0:${PORTSDIR}/sysutils/panicmail \
		grub-mkconfig:${PORTSDIR}/sysutils/grub2-pcbsd

NO_BUILD=	yes

do-install:
	${DO_NADA}

.include <bsd.port.mk>
