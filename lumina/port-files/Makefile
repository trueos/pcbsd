# Created by: Ken Moore <ken@pcbsd.org>
# $FreeBSD$

PORTNAME=	lumina
PORTVERSION=	CHGVERSION
CATEGORIES=	x11
MASTER_SITES=	http://www.pcbsd.org/~kris/software/ \
		ftp://ftp.pcbsd.org/pub/software/

MAINTAINER=	kmoore@FreeBSD.org
COMMENT=	Lumina Desktop Environment

RUN_DEPENDS=	fluxbox:{PORTSDIR}/x11-wm/fluxbox \
		fluxbox-tenr-styles-pack>=0:{PORTSDIR}/x11-themes/fluxbox-tenr-styles-pack \
		kde4-icons-oxygen>=0:{PORTSDIR}/x11-themes/kde4-icons-oxygen \
		xscreensaver:{PORTSDIR}/x11/xscreensaver \
		xscreensaver-demo:{PORTSDIR}/x11/xscreensaver
LIB_DEPENDS=	libX11.so:${PORTSDIR}/x11/libX11 \
		libXrender.so:${PORTSDIR}/x11/libXrender

MAKE_JOBS_UNSAFE=yes
ONLY_FOR_ARCHS=	amd64
USE_CDRTOOLS=	yes
USE_BZIP2=	yes
USE_LDCONFIG=	yes
USE_QT4=	corelib gui network svg linguist \
		moc_build rcc_build uic_build
USES=		qmake

WRKSRC=		${WRKDIR}/lumina

QMAKE_ARGS=	CONFIG+="configure"

.include <bsd.port.pre.mk>

.if ${OSVERSION} < 1000000
IGNORE=		requires FreeBSD 10 or later
.endif

.include <bsd.port.post.mk>
