# Created by: Artyom Mirgorodskiy <artyom.mirgorodsky@gmail.com>
# $FreeBSD$

PORTNAME=	sqlite3
PORTVERSION=	3.6.20
RPMVERSION=	1.el6
USE_LINUX=	c6

CATEGORIES=	databases linux
MAINTAINER=	emulation@FreeBSD.org
COMMENT=	Library that implements an embeddable SQL database engine (Linux CentOS ${LINUX_DIST_VER})

PKGNAMEPREFIX=	linux-${USE_LINUX}-
MASTER_SITES=	http://vault.centos.org/${LINUX_DIST_VER}/os/i386/Packages/
DISTNAME=	sqlite-${PORTVERSION}-${RPMVERSION}
DIST_SUBDIR=	rpm/${LINUX_RPM_ARCH}/centos/${LINUX_DIST_VER}

CONFLICTS=	linux-f8-sqlite3-[0-9]* linux-f10-sqlite3-[0-9]*

ONLY_FOR_ARCHS=	i386 amd64
USE_LINUX_RPM=	yes
LINUX_RPM_ARCH=		i686
LINUX_DIST_VER=	6.2
BRANDELF_FILES=	usr/bin/sqlite3
USE_LDCONFIG=	yes

PLIST_FILES=	usr/bin/sqlite3 usr/lib/libsqlite3.so.0 \
		usr/lib/libsqlite3.so.0.8.6 usr/share/man/man1/sqlite3.1.gz
DOCSDIR=	${PREFIX}/usr/share/doc/sqlite-${PORTVERSION}
PORTDOCS=	README
DESCR=		${.CURDIR}/../${PORTNAME}/pkg-descr

.include <bsd.port.mk>
