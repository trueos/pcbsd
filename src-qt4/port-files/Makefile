# Created by: Kris Moore <kmoore@FreeBSD.org>
# $FreeBSD: head/sysutils/pcbsd-utils-qt4/Makefile 335780 2013-12-06 17:43:18Z kmoore $

PORTNAME=	pcbsd-utils-qt4
PORTVERSION=	CHGVERSION
CATEGORIES=	sysutils
MASTER_SITES=	http://www.pcbsd.org/~kris/software/ \
		ftp://ftp.pcbsd.org/pub/software/

MAINTAINER=	kmoore@FreeBSD.org
COMMENT=	PC-BSD Qt4 Utilities

RUN_DEPENDS=	pc-metapkgmanager:${PORTSDIR}/sysutils/pcbsd-utils
LIB_DEPENDS=	libQtSolutions_SingleApplication-head.so:${PORTSDIR}/devel/qt4-qtsolutions-singleapplication \
		curl:${PORTSDIR}/ftp/curl

MAKE_JOBS_UNSAFE=yes
USE_CDRTOOLS=	yes
USE_BZIP2=	yes
USE_LDCONFIG=	yes
USE_QT4=	corelib gui network svg linguist \
		moc_build rcc_build uic_build
USES=		qmake

WRKSRC=		${WRKDIR}/src-qt4

QMAKE_ARGS=	CONFIG+="configure"

.include <bsd.port.pre.mk>

.if ${OSVERSION} < 900030
IGNORE=		requires FreeBSD 9.x
.endif

.if ${OSVERSION} > 1000000
IGNORE=		requires FreeBSD 9.x
.endif

.include <bsd.port.post.mk>
