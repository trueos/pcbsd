# Created by: Palle Girgensohn <girgen@pingpong.net>
# $FreeBSD: databases/postgresql-libpqxx3/Makefile 327717 2013-09-20 16:13:47Z bapt $

PORTNAME=	libpqxx
PORTVERSION=	3.1.1
CATEGORIES=	databases
MASTER_SITES=	http://pqxx.org/download/software/${PORTNAME}/
PKGNAMEPREFIX=	postgresql-
DIST_SUBDIR=	postgresql

MAINTAINER=	20080111.freebsd.org@ab.ote.we.lv
COMMENT=	New C++ interface for PostgreSQL

CONFLICTS=	postgresql-libpqxx-[2-4].*
LATEST_LINK=	${PKGNAMEPREFIX}${PORTNAME}3
PORTSCOUT=	limit:^2\.

USE_PGSQL=	yes
USES=		gmake pkgconfig
GNU_CONFIGURE=	yes
USE_LDCONFIG=	yes
USE_GCC=	yes
PLIST_SUB=	PORTVERSION=${PORTVERSION}
USE_PYTHON_BUILD=	yes
USES=shebangfix
SHEBANG_FILES=	tools/splitconfig

CONFIGURE_ARGS=	--enable-shared

NO_STAGE=	yes
.include <bsd.port.mk>
