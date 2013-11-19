# Created by: Artyom Mirgorodskiy <artyom.mirgorodsky@gmail.com>
# $FreeBSD$

PORTNAME=	cairo
PORTVERSION=	1.8.8
RPMVERSION=	3.1.el6
USE_LINUX=	c6
USE_LINUX_APPS=	fontconfig png xorglibs

CATEGORIES=	graphics linux
MAINTAINER=	emulation@FreeBSD.org
COMMENT=	Vector graphics library Cairo (Linux CentOS ${LINUX_DIST_VER})

PKGNAMEPREFIX=	linux-${USE_LINUX}-
MASTER_SITES=	http://vault.centos.org/${LINUX_DIST_VER}/os/i386/Packages/
DISTNAME=	${PORTNAME}-${PORTVERSION}-${RPMVERSION}
DIST_SUBDIR=	rpm/${LINUX_RPM_ARCH}/centos/${LINUX_DIST_VER}

CONFLICTS=	linux-cairo-[0-9]* linux-f10-cairo-[0-9]*

ONLY_FOR_ARCHS=	i386 amd64
USE_LINUX_RPM=	yes
LINUX_RPM_ARCH=		i686
LINUX_DIST_VER=	6.2
USE_LDCONFIG=	yes

PLIST_FILES=	usr/lib/libcairo.so.2 \
		usr/lib/libcairo.so.2.10800.8
DOCSDIR=	${PREFIX}/usr/share/doc/cairo-${PORTVERSION}
PORTDOCS=	AUTHORS BIBLIOGRAPHY BUGS COPYING COPYING-LGPL-2.1 \
		COPYING-MPL-1.1 NEWS README

.include <bsd.port.mk>
