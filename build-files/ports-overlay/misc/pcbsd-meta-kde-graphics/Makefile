# New ports collection makefile for:	pcbsd-meta-kde-graphics
#
# $FreeBSD$
#

PORTNAME=	pcbsd-meta-kde-graphics
PORTVERSION=	CHGVERSION
CATEGORIES=	misc
MASTER_SITES=	# empty
DISTFILES=	# none

MAINTAINER=	kris@pcbsd.org
COMMENT=	The meta-port pcbsd-meta-kde-graphics

RUN_DEPENDS=	kdegraphics>=0:${PORTSDIR}/graphics/kdegraphics4 \
		digikam>=0:${PORTSDIR}/graphics/digikam-kde4 \
		kphotoalbum>=0:${PORTSDIR}/graphics/kphotoalbum-kde4 \
		kipi-plugins>=0:${PORTSDIR}/graphics/kipi-plugins-kde4 \
		kcoloredit>=0:${PORTSDIR}/graphics/kcoloredit \
		kiconedit>=0:${PORTSDIR}/graphics/kiconedit \
		skanlite>=0:${PORTSDIR}/graphics/skanlite \
		hugin>=0:${PORTSDIR}/graphics/hugin

NO_BUILD=	yes

do-install:
	${DO_NADA}

.include <bsd.port.mk>
