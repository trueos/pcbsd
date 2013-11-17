# Created by: Artyom Mirgorodskiy <artyom.mirgorodsky@gmail.com>
# $FreeBSD$

PORTNAME=	tiff
PORTVERSION=	3.9.4
RPMVERSION=	1.el6_0.3
USE_LINUX=	c6
USE_LINUX_APPS=	jpeg

CATEGORIES=	graphics linux
MAINTAINER=	emulation@FreeBSD.org
COMMENT=	TIFF library (Linux CentOS ${LINUX_DIST_VER})

PKGNAMEPREFIX=	linux-${USE_LINUX}-
MASTER_SITES=	http://vault.centos.org/${LINUX_DIST_VER}/os/i386/Packages/
DISTNAME=	lib${PORTNAME}-${PORTVERSION}-${RPMVERSION}
DIST_SUBDIR=	rpm/${LINUX_RPM_ARCH}/centos/${LINUX_DIST_VER}

CONFLICTS=	linux-tiff-[0-9]* linux-f10-tiff-[0-9]*

ONLY_FOR_ARCHS=	amd64 i386
USE_LINUX_RPM=	yes
LINUX_NAME=	lib${PORTNAME}
BRANDELF_DIRS=	usr/bin
LINUX_RPM_ARCH=		i686
LINUX_DIST_VER=	6.2
USE_LDCONFIG=	yes

DESCR=		${.CURDIR}/../${PORTNAME}/pkg-descr

.include <bsd.port.mk>
