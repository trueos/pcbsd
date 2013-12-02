# Created by: bsam
# $FreeBSD: multimedia/linux-c6-libv4l/Makefile 329569 2013-10-06 13:57:19Z rene $

PORTNAME=	libv4l
PORTVERSION=	0.6.3
RPMVERSION=	2.el6
USE_LINUX=	c6

CATEGORIES=	multimedia linux

PKGNAMEPREFIX=	linux-c6-
DISTNAME=	${PORTNAME}-${PORTVERSION}-${RPMVERSION}

MAINTAINER=	emulation@FreeBSD.org
COMMENT=	Collection of video4linux support libraries (Linux CentOS ${LINUX_DIST_VER})

CONFLICTS=	linux-f10-libv4l-[0-9]*

ONLY_FOR_ARCHS=	i386 amd64
USE_LINUX_RPM=	yes
LINUX_DIST_VER=	6.3
LINUX_RPM_ARCH=		i686
USE_LDCONFIG=	yes
PLIST_SUB+=	PORTVERSION=${PORTVERSION}
DESCR=		${.CURDIR}/../${PORTNAME}/pkg-descr

.include <bsd.port.mk>
