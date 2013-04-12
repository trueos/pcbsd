# New ports collection makefile for:	pcbsd-meta-kde-network
#
# $FreeBSD$
#

PORTNAME=	pcbsd-meta-kde-network
PORTVERSION=	CHGVERSION
CATEGORIES=	misc
MASTER_SITES=	# empty
DISTFILES=	# none

MAINTAINER=	kris@pcbsd.org
COMMENT=	The meta-port pcbsd-meta-kde-network

RUN_DEPENDS=	kdenetwork>=0:${PORTSDIR}/net/kdenetwork4 \
		ktorrent>=0:${PORTSDIR}/net-p2p/ktorrent \
		konversation>=0:${PORTSDIR}/irc/konversation-kde4

NO_BUILD=	yes

do-install:
	${DO_NADA}

.include <bsd.port.mk>
