# $FreeBSD$

PORTNAME=	pcbsd-i18n-qt5
PORTVERSION=	%TIMESTAMP%
CATEGORIES=	misc
MASTER_SITES=

MAINTAINER=	kmoore@FreeBSD.org
COMMENT=	PC-BSD Qt5 i18n Support

LICENSE=	BSD3CLAUSE

RUN_DEPENDS=	pcbsd-base>=0:${PORTSDIR}/misc/pcbsd-base

NO_BUILD=	yes
USES=		tar:xz

do-install:
	${MKDIR} ${STAGEDIR}${PREFIX}/share/pcbsd/i18n
	tar xvf ${DISTDIR}/${PORTNAME}-${PORTVERSION}.tar.xz -C ${STAGEDIR}${PREFIX}/share/pcbsd/i18n

.include <bsd.port.mk>
