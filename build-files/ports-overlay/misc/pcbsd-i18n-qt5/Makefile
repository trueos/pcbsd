# New ports collection makefile for:	pcbsd-i18n-qt5
# Date created:				13 May 2014
# Whom:					Kris Moore <kmoore@FreeBSD.org>
#
# $FreeBSD$

PORTNAME=	pcbsd-i18n-qt5
PORTVERSION=	%TIMESTAMP%
CATEGORIES=	misc
MASTER_SITES=
DISTFILES=

MAINTAINER=	kmoore@FreeBSD.org
COMMENT=	PC-BSD Qt5 i18n Support

RUN_DEPENDS=	pcbsd-base>=0:${PORTSDIR}/misc/pcbsd-base

NO_BUILD=	yes

do-install:
	mkdir -p ${STAGEDIR}${PREFIX}/share/pcbsd/i18n
	fetch -o ${STAGEDIR}${PREFIX}/share/pcbsd/i18n/pcbsd-i18n.txz https://github.com/pcbsd/pcbsd-i18n/raw/master/dist/pcbsd-i18n.txz
	tar xvf ${STAGEDIR}${PREFIX}/share/pcbsd/i18n/pcbsd-i18n.txz -C ${STAGEDIR}${PREFIX}/share/pcbsd/i18n
	rm ${STAGEDIR}${PREFIX}/share/pcbsd/i18n/pcbsd-i18n.txz

.include <bsd.port.mk>
