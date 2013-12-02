# Created by: Artyom Mirgorodskiy <artyom.mirgorodsky@gmail.com>
# $FreeBSD$

PORTNAME=	dri
PORTVERSION=	7.11
USE_LINUX=	c6
USE_LINUX_APPS=	xorglibs

CATEGORIES=	graphics linux
MAINTAINER=	emulation@FreeBSD.org
COMMENT=	>Mesa libGL runtime libraries (Linux CentOS 6)

PKGNAMEPREFIX=	linux-${USE_LINUX}-
DISTFILES=		mesa-libGL-7.11-3.el6.i686.rpm \
		mesa-libGLU-7.11-3.el6.i686.rpm \
		freeglut-2.6.0-1.el6.i686.rpm
SRC_DISTFILES=		mesa-7.11-3.el6.src.rpm \
		freeglut-2.6.0-1.el6.src.rpm

ALWAYS_KEEP_DISTFILES=  YES

CONFLICTS=	linux_dri-[0-9]* linux_dri-devel-[0-9]* linux-f10-dri-[0-9]* linux-dri-[0-9]*

ONLY_FOR_ARCHS=	i386 amd64
USE_LINUX_RPM=	yes
LINUX_RPM_ARCH=		i686
USE_LDCONFIG=	yes

.include <bsd.port.mk>
