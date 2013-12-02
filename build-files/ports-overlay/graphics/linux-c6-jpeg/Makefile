# Created by: Artyom Mirgorodskiy <artyom.mirgorodsky@gmail.com>
# $FreeBSD$

PORTNAME=	jpeg
PORTVERSION=	6b
RPMVERSION=	46.el6
USE_LINUX=	c6

CATEGORIES=	graphics linux
MAINTAINER=	emulation@FreeBSD.org
COMMENT=	RPM of the JPEG lib (Linux CentOS 6)

PKGNAMEPREFIX=	linux-${USE_LINUX}-
DISTNAME=	lib${PORTNAME}-${PORTVERSION}-${RPMVERSION}

CONFLICTS=	linux-jpeg-[0-9]* linux-f10-jpeg-[0-9]*

ONLY_FOR_ARCHS=	i386 amd64
USE_LINUX_RPM=	yes
LINUX_NAME=	lib${PORTNAME}
BRANDELF_DIRS=	usr/bin
LINUX_RPM_ARCH=		i686
USE_LDCONFIG=	yes

DESCR=		${.CURDIR}/../${PORTNAME}/pkg-descr

.include <bsd.port.mk>
