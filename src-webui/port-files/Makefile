# Created by: Kris Moore <kmoore@FreeBSD.org>
# $FreeBSD$

PORTNAME=	pcbsd-appweb
PORTVERSION=	CHGVERSION
CATEGORIES=	sysutils
MASTER_SITES=	http://www.pcbsd.org/~kris/software/ \
		ftp://ftp.pcbsd.org/pub/software/

MAINTAINER=	kmoore@FreeBSD.org
COMMENT=	PC-BSD Web Interface

RUN_DEPENDS=	pc-updatemanager:${PORTSDIR}/sysutils/pcbsd-utils \
		nginx:${PORTSDIR}/www/nginx

USE_PHP=	gd filter session
WRKSRC=		${WRKDIR}/src-webui
USES=		pkgconfig
NO_BUILD=	yes
USE_BZIP2=	yes

.include <bsd.port.pre.mk>

.if ${OSVERSION} < 1000000
IGNORE=		requires FreeBSD 10 or later
.endif

do-install:
	cd ${WRKSRC} && make install PREFIX=${STAGEDIR}${PREFIX}

post-install:
	PKG_PREFIX=${STAGEDIR}${PREFIX} ${SH} ${PKGINSTALL} ${PKGNAME} POST-INSTALL

.include <bsd.port.post.mk>
