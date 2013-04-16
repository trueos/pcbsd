# New ports collection makefile for:	pcbsd-meta-development
#
# $FreeBSD$
#

PORTNAME=	pcbsd-meta-development
PORTVERSION=	CHGVERSION
CATEGORIES=	misc
MASTER_SITES=	# empty
DISTFILES=	# none

MAINTAINER=	kris@pcbsd.org
COMMENT=	The meta-port pcbsd-meta-development

RUN_DEPENDS=	findutils>=0:${PORTSDIR}/misc/findutils \
		autoconf>=0:${PORTSDIR}/devel/autoconf \
		automake>=0:${PORTSDIR}/devel/automake \
		doxygen>=0:${PORTSDIR}/devel/doxygen \
		cscope>=0:${PORTSDIR}/devel/cscope \
		global>=0:${PORTSDIR}/devel/global

NO_BUILD=	yes

do-install:
	${DO_NADA}

.include <bsd.port.mk>
