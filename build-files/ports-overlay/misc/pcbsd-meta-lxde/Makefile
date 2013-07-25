# New ports collection makefile for:	pcbsd-meta-lxde
#
# $FreeBSD$
#

PORTNAME=	pcbsd-meta-lxde
PORTVERSION=	CHGVERSION
CATEGORIES=	misc
MASTER_SITES=	# empty
DISTFILES=	# none

MAINTAINER=	kris@pcbsd.org
COMMENT=	The meta-port pcbsd-meta-lxde

RUN_DEPENDS=	lxde-meta>=0:${PORTSDIR}/x11/lxde-meta \
		gvfs>=0:${PORTSDIR}/devel/gvfs \
		xarchiver>=0:${PORTSDIR}/archivers/xarchiver \
		obkey>=0:${PORTSDIR}/x11-wm/obkey \
		leafpad>=0:${PORTSDIR}/editors/leafpad \
		lxappearance>=0:${PORTSDIR}/x11-themes/lxappearance \
		lxmenu-data>=0:${PORTSDIR}/x11/lxmenu-data

NO_BUILD=	yes

do-install:
	${DO_NADA}

.include <bsd.port.mk>
