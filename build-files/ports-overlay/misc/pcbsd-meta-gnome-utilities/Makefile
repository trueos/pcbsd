# New ports collection makefile for:	pcbsd-meta-gnome-utilities
#
# $FreeBSD$
#

PORTNAME=	pcbsd-meta-gnome-utilities
PORTVERSION=	CHGVERSION
CATEGORIES=	misc
MASTER_SITES=	# empty
DISTFILES=	# none

MAINTAINER=	kris@pcbsd.org
COMMENT=	The meta-port pcbsd-meta-gnome-utilities

RUN_DEPENDS=	gnome-blog>=0:${PORTSDIR}/deskutils/gnome-blog \
		gnome-utils>=0:${PORTSDIR}/deskutils/gnome-utils \
		gnochm>=0:${PORTSDIR}/deskutils/gnochm \
		conduit>=0:${PORTSDIR}/deskutils/conduit \
		pessulus>=0:${PORTSDIR}/sysutils/pessulus

NO_BUILD=	yes

do-install:
	${DO_NADA}

.include <bsd.port.mk>
