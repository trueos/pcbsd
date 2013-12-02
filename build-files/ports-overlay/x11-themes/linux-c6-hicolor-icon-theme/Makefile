# Created by: bsam
# $FreeBSD: head/x11-themes/linux-f10-hicolor-icon-theme/Makefile 329597 2013-10-06 15:43:55Z rene $

PORTNAME=	hicolor-icon-theme
PORTVERSION=	0.5
CATEGORIES=	x11-themes linux
MASTER_SITES=	# empty
PKGNAMEPREFIX=	linux-c6-
DISTFILES=	# empty

MAINTAINER=	emulation@FreeBSD.org
COMMENT=	High-color icon theme shell from the FreeDesktop project

CONFLICTS=	linux-hicolor-icon-theme-[0-9]* linux-f8-hicolor-icon-theme-[0-9]* linux-f10-hicolor-icon-theme-[0-9]*

RUN_DEPENDS=	${LOCALBASE}/share/icons/hicolor/index.theme:${PORTSDIR}/misc/hicolor-icon-theme

NO_BUILD=	yes
USE_LINUX=	c6
USE_LINUX_PREFIX=	yes
PLIST_FILES=	usr/local/share/icons/hicolor

do-install:
	${INSTALL} -d ${STAGEDIR}${PREFIX}/usr/local/share/icons
	${LN} -sf ${LOCALBASE}/share/icons/hicolor ${STAGEDIR}${PREFIX}/usr/local/share/icons

.include <bsd.port.mk>
