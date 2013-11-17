# Created by: Artyom Mirgorodskiy <artyom.mirgorodsky@gmail.com>
# $FreeBSD$

PORTNAME=	png
PORTVERSION=	1.2.46
RPMVERSION=	1.el6_1
USE_LINUX=	c6

CATEGORIES=	graphics linux
MAINTAINER=	emulation@FreeBSD.org
COMMENT=	RPM of the PNG lib (Linux CentOS ${LINUX_DIST_VER})

PKGNAMEPREFIX=	linux-${USE_LINUX}-
MASTER_SITES=	http://vault.centos.org/${LINUX_DIST_VER}/os/i386/Packages/
DISTNAME=	lib${PORTNAME}-${PORTVERSION}-${RPMVERSION}
DIST_SUBDIR=	rpm/${LINUX_RPM_ARCH}/centos/${LINUX_DIST_VER}

CONFLICTS=	linux-png-[0-9]* linux-f8-png-[0-9]* linux-f10-png-[0-9]*

LINUX_NAME=	libpng
ONLY_FOR_ARCHS=	i386 amd64
USE_LINUX_RPM=	yes
LINUX_RPM_ARCH=		i686
LINUX_DIST_VER=	6.2
USE_LDCONFIG=	yes

PLIST_FILES=	usr/lib/libpng.so.3 \
		usr/lib/libpng.so.3.46.0 \
		usr/lib/libpng12.so.0 \
		usr/lib/libpng12.so.0.46.0 \
		usr/share/man/man5/png.5.gz
DOCSDIR=	${PREFIX}/usr/share/doc/libpng-${PORTVERSION}
PORTDOCS=	CHANGES LICENSE README TODO example.c libpng-1.2.46.txt CMakeLists.txt
DESCR=		${.CURDIR}/../${PORTNAME}/pkg-descr

.include <bsd.port.mk>
