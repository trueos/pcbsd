# Created by: Artyom Mirgorodskiy <artyom.mirgorodsky@gmail.com>
# $FreeBSD$

PORTNAME=	fontconfig
PORTVERSION=	2.8.0
RPMVERSION=	3.el6
USE_LINUX=	c6
USE_LINUX_APPS=	expat

CATEGORIES=	x11-fonts linux
MAINTAINER=	emulation@FreeBSD.org
COMMENT=	XML-based font configuration API for X Windows (Linux CentOS 6)

PKGNAMEPREFIX=	linux-${USE_LINUX}-
DISTNAME=	${PORTNAME}-${PORTVERSION}-${RPMVERSION}

CONFLICTS=	linux-fontconfig-[0-9]* linux-f8-fontconfig-[0-9]* linux-f10-fontconfig-[0-9]*

ONLY_FOR_ARCHS=	i386 amd64
USE_LINUX_RPM=	yes
LINUX_RPM_ARCH=		i686
BRANDELF_DIRS=	usr/bin
USE_LDCONFIG=	yes

# We will use some other configuration file, so delete a linux one
# Don't let the linux fc-cache mess up the native cache file
post-extract:
		@${RM} -r ${WRKSRC}/etc ${WRKSRC}/usr/share/fonts ${WRKSRC}/var \
			${WRKSRC}/usr/bin/fc-cache \
			${WRKSRC}/usr/share/man/man1/fc-cache.1.gz

post-install:
		${MKDIR} ${STAGEDIR}${PREFIX}/etc
		@${LN} -s ${LOCALBASE}/etc/fonts ${STAGEDIR}${PREFIX}/etc/

DESCR=		${.CURDIR}/../${PORTNAME}/pkg-descr

.include <bsd.port.mk>
