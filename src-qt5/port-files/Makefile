# Created by: Kris Moore <kmoore@FreeBSD.org>
# $FreeBSD$

PORTNAME=	pcbsd-utils-qt5
PORTVERSION=	CHGVERSION
CATEGORIES=	sysutils
MASTER_SITES=	http://www.pcbsd.org/~kris/software/ \
		ftp://ftp.pcbsd.org/pub/software/

MAINTAINER=	kmoore@FreeBSD.org
COMMENT=	PC-BSD Qt5 Utilities

RUN_DEPENDS=	lpreserver:${PORTSDIR}/sysutils/pcbsd-utils \
		${PYTHON_SITELIBDIR}/dbus/__init__.py:${PORTSDIR}/devel/py-dbus \
		cdda2wav:${PORTSDIR}/sysutils/cdrtools
LIB_DEPENDS=	libcurl.so:${PORTSDIR}/ftp/curl

MAKE_JOBS_UNSAFE=yes
ONLY_FOR_ARCHS=	amd64
USE_PYTHON=	yes
USE_LDCONFIG=	yes
USE_QT5=	corelib gui network svg linguist \
		moc_build rcc_build uic_build
USES=		qmake tar:xz

WRKSRC=		${WRKDIR}/src-qt5

QMAKE_ARGS=	CONFIG+="configure"

.include <bsd.port.options.mk>

.if ${OSVERSION} < 1000000
IGNORE=		requires FreeBSD 10 or later
.endif

.include <bsd.port.mk>
