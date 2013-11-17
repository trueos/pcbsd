# Created by: Artyom Mirgorodskiy <artyom.mirgorodsky@gmail.com>
# $FreeBSD$

PORTNAME=	gtk2
PORTVERSION=	2.18.9
RPMVERSION=	6.el6.centos
USE_LINUX=	c6
USE_LINUX_APPS= png jpeg tiff xorglibs pango atk

CATEGORIES=	x11-toolkits linux
MAINTAINER=	emulation@FreeBSD.org
COMMENT=	GTK+ library, version 2.X  (Linux CentOS ${LINUX_DIST_VER})

DISTFILES=	${PORTNAME}-${PORTVERSION}-${RPMVERSION}.${LINUX_RPM_ARCH}.rpm \
		pixman-0.18.4-1.el6_0.1.${LINUX_RPM_ARCH}.rpm \
		jasper-libs-1.900.1-15.el6.${LINUX_RPM_ARCH}.rpm
PKGNAMEPREFIX=	linux-${USE_LINUX}-
MASTER_SITES=	http://vault.centos.org/${LINUX_DIST_VER}/os/i386/Packages/
DIST_SUBDIR=	rpm/${LINUX_RPM_ARCH}/centos/${LINUX_DIST_VER}

CONFLICTS=	linux-gtk2-[0-9]* linux-f8-gtk2-[0-9]* linux-f10-gtk2-[0-9]*

ONLY_FOR_ARCHS=	i386 amd64
USE_LINUX_RPM=	yes
LINUX_RPM_ARCH=		i686
LINUX_DIST_VER=	6.2
BRANDELF_DIRS=	usr/bin
USE_LDCONFIG=	yes

DESCR=		${.CURDIR}/../${PORTNAME}0/pkg-descr

.include <bsd.port.mk>
