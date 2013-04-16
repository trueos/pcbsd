# New ports collection makefile for:	pcbsd-meta-kde-artwork
#
# $FreeBSD$
#

PORTNAME=	pcbsd-meta-kde-artwork
PORTVERSION=	CHGVERSION
CATEGORIES=	misc
MASTER_SITES=	# empty
DISTFILES=	# none

MAINTAINER=	kris@pcbsd.org
COMMENT=	The meta-port pcbsd-meta-kde-artwork

RUN_DEPENDS=	kdeartwork>=0:${PORTSDIR}/x11-themes/kdeartwork4 \
		kde4-style-nitrogen>=0:${PORTSDIR}/x11-themes/kde4-style-nitrogen \
		kde4-style-polyester>=0:${PORTSDIR}/x11-themes/kde4-style-polyester \
		kde4-style-skulpture>=0:${PORTSDIR}/x11-themes/kde4-style-skulpture

NO_BUILD=	yes

do-install:
	${DO_NADA}

.include <bsd.port.mk>
