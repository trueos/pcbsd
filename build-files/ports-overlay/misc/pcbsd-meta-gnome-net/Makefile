# New ports collection makefile for:	pcbsd-meta-gnome-net
#
# $FreeBSD$
#

PORTNAME=	pcbsd-meta-gnome-net
PORTVERSION=	CHGVERSION
CATEGORIES=	misc
MASTER_SITES=	# empty
DISTFILES=	# none

MAINTAINER=	kris@pcbsd.org
COMMENT=	The meta-port pcbsd-meta-gnome-net

RUN_DEPENDS=	vino>=0:${PORTSDIR}/net/vino \
		evolution>=0:${PORTSDIR}/mail/evolution \
		evolution-webcal>=0:${PORTSDIR}/www/evolution-webcal \
		gnome-netstatus>=0:${PORTSDIR}/net/gnome-netstatus \
		evolution-exchange>=0:${PORTSDIR}/mail/evolution-exchange \
		p5-Mail-SpamAssassin>=0:${PORTSDIR}/mail/p5-Mail-SpamAssassin

NO_BUILD=	yes

do-install:
	${DO_NADA}

.include <bsd.port.mk>
