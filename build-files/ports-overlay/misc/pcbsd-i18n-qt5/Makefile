# New ports collection makefile for:	pcbsd-i18n-qt5
# Date created:				13 May 2014
# Whom:					Kris Moore <kmoore@FreeBSD.org>
#
# $FreeBSD$

PORTNAME=	pcbsd-i18n-qt5
PORTVERSION=	%TIMESTAMP%
CATEGORIES=	misc
MASTER_SITES=

MAINTAINER=	kmoore@FreeBSD.org
COMMENT=	PC-BSD Qt5 i18n Support

RUN_DEPENDS=	pcbsd-base>=0:${PORTSDIR}/misc/pcbsd-base

NO_BUILD=	yes
USES=		tar:xz

do-install:
	mkdir -p ${STAGEDIR}${PREFIX}/share/pcbsd/i18n
	tar xvf ${DISTDIR}/${PORTNAME}.tar.xz -C ${STAGEDIR}${PREFIX}/share/pcbsd/i18n

.include <bsd.port.mk>
