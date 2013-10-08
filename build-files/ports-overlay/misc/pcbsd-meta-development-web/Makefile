# New ports collection makefile for:	pcbsd-meta-development-web
#
# $FreeBSD$
#

PORTNAME=	pcbsd-meta-development-web
PORTVERSION=	CHGVERSION
CATEGORIES=	misc
MASTER_SITES=	# empty
DISTFILES=	# none

MAINTAINER=	kris@pcbsd.org
COMMENT=	The meta-port pcbsd-meta-development-web

RUN_DEPENDS=	mysql-server>=0:${PORTSDIR}/databases/mysql55-server \
		php5>=0:${PORTSDIR}/lang/php5 \
		php5-extensions>=0:${PORTSDIR}/lang/php5-extensions \
		php5-mysql>=0:${PORTSDIR}/databases/php5-mysql \
		php5-pgsql>=0:${PORTSDIR}/databases/php5-pgsql \
		php5-zip>=0:${PORTSDIR}/archivers/php5-zip \
		php5-bz2>=0:${PORTSDIR}/archivers/php5-bz2 \
		php5-zlib>=0:${PORTSDIR}/archivers/php5-zlib \
		php5-mbstring>=0:${PORTSDIR}/converters/php5-mbstring \
		php5-gd>=0:${PORTSDIR}/graphics/php5-gd \
		php5-dav>=0:${PORTSDIR}/devel/php5-dav \
		php5-gettext>=0:${PORTSDIR}/devel/php5-gettext \
		php5-curl>=0:${PORTSDIR}/ftp/php5-curl \
		php5-ftp>=0:${PORTSDIR}/ftp/php5-ftp \
		php5-imap>=0:${PORTSDIR}/mail/php5-imap \
		php5-pdo_mysql>=0:${PORTSDIR}/databases/php5-pdo_mysql \
		php5-pdo_pgsql>=0:${PORTSDIR}/databases/php5-pdo_pgsql \
		pear>=0:${PORTSDIR}/devel/pear \
		py27-django>=0:${PORTSDIR}/www/py-django \
		apache22>=0:${PORTSDIR}/www/apache22 \
		py27-MySQLdb55>=0:${PORTSDIR}/databases/py-MySQLdb55 \
		p5-DBD-mysql55>=0:${PORTSDIR}/databases/p5-DBD-mysql55

NO_BUILD=	yes

do-install:
	${DO_NADA}

.include <bsd.port.mk>
