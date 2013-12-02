# Created by: Artyom Mirgorodskiy <artyom.mirgorodsky@gmail.com>
# $FreeBSD$

PORTNAME=	pango
PORTVERSION=	1.28.1
RPMVERSION=	3.el6_0.centos.5
USE_LINUX=	c6
USE_LINUX_APPS=	cairo expat fontconfig xorglibs

CATEGORIES=	x11-toolkits linux
MAINTAINER=	emulation@FreeBSD.org
COMMENT=	Pango library (Linux CentOS 6)

PKGNAMEPREFIX=	linux-${USE_LINUX}-
DISTNAME=	${PORTNAME}-${PORTVERSION}-${RPMVERSION}

CONFLICTS=	linux-pango-[0-9]* linux-f10-pango-[0-9]*

ONLY_FOR_ARCHS=	i386 amd64
USE_LINUX_RPM=	yes
LINUX_RPM_ARCH=		i686
BRANDELF_FILES=	usr/bin/pango-querymodules-32
USE_LDCONFIG=	yes

DESCR=		${.CURDIR}/../${PORTNAME}/pkg-descr

.include <bsd.port.mk>
