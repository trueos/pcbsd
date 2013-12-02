# Created by: Artyom Mirgorodskiy <artyom.mirgorodsky@gmail.com>
# $FreeBSD$

PORTNAME=	cyrus-sasl2
PORTVERSION=	2.1.23
RPMVERSION=	13.el6
USE_LINUX=	c6

CATEGORIES=	security linux
MAINTAINER=	emulation@FreeBSD.org
COMMENT=	RFC 2222 SASL (Simple Authentication and Security Layer) (Linux CentOS 6)

PKGNAMEPREFIX=	linux-${USE_LINUX}-
DISTNAME=	cyrus-sasl-lib-${PORTVERSION}-${RPMVERSION}

ONLY_FOR_ARCHS=	i386 amd64
USE_LINUX_RPM=	yes
LINUX_RPM_ARCH=		i686
USE_LDCONFIG=	yes
BRANDELF_FILES=	usr/sbin/sasldblistusers2 usr/sbin/saslpasswd2
SRC_DISTFILES=	cyrus-sasl-${PORTVERSION}-${RPMVERSION}.src.rpm
DESCR=	${.CURDIR}/../${PORTNAME}/pkg-descr

PLIST_DIRS=	etc/sasl2 usr/lib/sasl2
PLIST_FILES=	usr/lib/libsasl2.so.2 usr/lib/libsasl2.so.2.0.23 usr/lib/sasl2/libanonymous.so usr/lib/sasl2/libanonymous.so.2 usr/lib/sasl2/libanonymous.so.2.0.23 usr/lib/sasl2/libsasldb.so usr/lib/sasl2/libsasldb.so.2 usr/lib/sasl2/libsasldb.so.2.0.23 usr/sbin/sasldblistusers2 usr/sbin/saslpasswd2
PLIST_FILES+=	"@exec mkdir -p %D/etc/sasl2"
DOCSDIR=	usr/share/doc/cyrus-sasl-lib-${PORTVERSION}
PORTDOCS=	AUTHORS COPYING NEWS README advanced.html appconvert.html components.html gssapi.html index.html install.html macosx.html mechanisms.html options.html plugprog.html programming.html readme.html sysadmin.html upgrading.html windows.html

.include <bsd.port.mk>
