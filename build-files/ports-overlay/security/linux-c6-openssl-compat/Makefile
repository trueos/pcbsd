# Created by: Artyom Mirgorodskiy <artyom.mirgorodsky@gmail.com>
# $FreeBSD$

PORTNAME=	openssl-compat
PORTVERSION=	0.9.8e
RPMVERSION=	17.el6.centos
USE_LINUX=	c6

CATEGORIES=	security linux
MAINTAINER=	emulation@FreeBSD.org
COMMENT=	OpenSSL toolkit (Linux CentOS 6)

PKGNAMEPREFIX=	linux-${USE_LINUX}-
DISTNAME=	openssl098e-${PORTVERSION}-${RPMVERSION}

CONFLICTS=	linux-openssl-[0-9]* linux-f8-openssl-[0-9]* linux-f10-openssl-[0-9]*

ONLY_FOR_ARCHS=	i386 amd64
USE_LINUX_RPM=	yes
LINUX_RPM_ARCH=		i686
USE_LDCONFIG=	yes

PLIST_SUB+=	PORTVERSION="${PORTVERSION}"
DESCR=		${.CURDIR}/../openssl/pkg-descr

post-install:
	${LN} -sf libssl.so.6 ${STAGEDIR}${PREFIX}/usr/lib/libssl.so.7

.include <bsd.port.mk>
