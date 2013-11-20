# New ports collection makefile for:	pcbsd-meta-xfce
#
# $FreeBSD$
#

PORTNAME=	pcbsd-meta-xfce
PORTVERSION=	CHGVERSION
CATEGORIES=	misc
MASTER_SITES=	# empty
DISTFILES=	# none

MAINTAINER=	kris@pcbsd.org
COMMENT=	The meta-port pcbsd-meta-xfce

RUN_DEPENDS=	xfce>=0:${PORTSDIR}/x11-wm/xfce4 \
		midori>=0:${PORTSDIR}/www/midori \
		xfburn>=0:${PORTSDIR}/sysutils/xfburn \
		ristretto>=0:${PORTSDIR}/graphics/ristretto \
		xfce4-wm-themes>=0:${PORTSDIR}/misc/xfce4-wm-themes \
		xfce4-power-manager>=0:${PORTSDIR}/sysutils/xfce4-power-manager \
		thunar-archive-plugin>=0:${PORTSDIR}/archivers/thunar-archive-plugin \

NO_BUILD=	yes

do-install:
	${DO_NADA}

.include <bsd.port.mk>
