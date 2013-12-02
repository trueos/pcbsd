# Created by: Artyom Mirgorodskiy <artyom.mirgorodsky@gmail.com>
# $FreeBSD$

PORTNAME=	dbus-libs
PORTVERSION=	1.2.24
RPMVERSION=	5.el6_1
USE_LINUX=	c6

CATEGORIES=	devel linux
MAINTAINER=	emulation@FreeBSD.org
COMMENT=	Libraries for accessing D-BUS (Linux CentOS 6)

PKGNAMEPREFIX=	linux-${USE_LINUX}-
DISTNAME=	${PORTNAME}-${PORTVERSION}-${RPMVERSION}

ONLY_FOR_ARCHS=	i386 amd64
USE_LINUX_RPM=	yes
LINUX_RPM_ARCH=		i686
SRC_DISTFILES=	dbus-${PORTVERSION}-${RPMVERSION}${SRC_SUFX}
USE_LDCONFIG=	yes
DESCR=		${.CURDIR}/../dbus/pkg-descr

PLIST_FILES=	lib/libdbus-1.so.3 lib/libdbus-1.so.3.4.0

CONFLICTS=	linux-dbus-libs-[0-9]*  linux-f10-dbus-libs-[0-9]*

.include <bsd.port.mk>
