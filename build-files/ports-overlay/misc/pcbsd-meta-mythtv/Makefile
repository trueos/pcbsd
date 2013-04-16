# New ports collection makefile for:	pcbsd-meta-mythtv
#
# $FreeBSD$
#

PORTNAME=	pcbsd-meta-mythtv
PORTVERSION=	CHGVERSION
CATEGORIES=	misc
MASTER_SITES=	# empty
DISTFILES=	# none

MAINTAINER=	kris@pcbsd.org
COMMENT=	The meta-port pcbsd-meta-mythtv

RUN_DEPENDS=	mythtv>=0:${PORTSDIR}/multimedia/mythtv \
		mythplugin-mythmusic>=0:${PORTSDIR}/audio/mythplugin-mythmusic \
		mythplugin-mythweb>=0:${PORTSDIR}/www/mythplugin-mythweb \
		mysql-server>=0:${PORTSDIR}/databases/mysql55-server \
		lirc>=0:${PORTSDIR}/comms/lirc \
		cx88>=0:${PORTSDIR}/multimedia/cx88 \
		zenity>=0:${PORTSDIR}/x11/zenity

NO_BUILD=	yes

do-install:
	${DO_NADA}

.include <bsd.port.mk>
