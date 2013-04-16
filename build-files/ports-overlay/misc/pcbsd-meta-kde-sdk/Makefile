# New ports collection makefile for:	pcbsd-meta-kde-sdk
#
# $FreeBSD$
#

PORTNAME=	pcbsd-meta-kde-sdk
PORTVERSION=	CHGVERSION
CATEGORIES=	misc
MASTER_SITES=	# empty
DISTFILES=	# none

MAINTAINER=	kris@pcbsd.org
COMMENT=	The meta-port pcbsd-meta-kde-sdk

RUN_DEPENDS=	kdesdk>=0:${PORTSDIR}/devel/kdesdk4 \
		kdevelop>=0:${PORTSDIR}/devel/kdevelop-kde4 \
		kdesvn>=0:${PORTSDIR}/devel/kdesvn-kde4 \
		automoc4>=0:${PORTSDIR}/devel/automoc4

NO_BUILD=	yes

do-install:
	${DO_NADA}

.include <bsd.port.mk>
