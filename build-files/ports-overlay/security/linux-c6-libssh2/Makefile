# Created by: Artyom Mirgorodskiy <artyom.mirgorodsky@gmail.com>
# $FreeBSD$

PORTNAME=	libssh2
PORTVERSION=	1.2.2
RPMVERSION=	7.el6_1.1
USE_LINUX=	c6
USE_LINUX_APPS=	openssl

CATEGORIES=	security linux
MAINTAINER=	emulation@FreeBSD.org
COMMENT=	Library implementing the SSH2 protocol (Linux CentOS ${LINUX_DIST_VER})

PKGNAMEPREFIX=	linux-${USE_LINUX}-
MASTER_SITES=	http://vault.centos.org/${LINUX_DIST_VER}/os/i386/Packages/
DISTNAME=	${PORTNAME}-${PORTVERSION}-${RPMVERSION}
DIST_SUBDIR=	rpm/${LINUX_RPM_ARCH}/centos/${LINUX_DIST_VER}

CONFLICTS=	linux-f10-libssh2-[0-9]*

ONLY_FOR_ARCHS=	i386 amd64
USE_LINUX_RPM=	yes
LINUX_RPM_ARCH=		i686
LINUX_DIST_VER=	6.2
USE_LDCONFIG=	yes

PLIST_FILES=	usr/lib/libssh2.so.1 usr/lib/libssh2.so.1.0.1
DOCSDIR=	${PREFIX}/usr/share/doc/${PORTNAME}-${PORTVERSION}
PORTDOCS=	AUTHORS COPYING ChangeLog NEWS README
DESCR=		${.CURDIR}/../${PORTNAME}/pkg-descr

.include <bsd.port.mk>
