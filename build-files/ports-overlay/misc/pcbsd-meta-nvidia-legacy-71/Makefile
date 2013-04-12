# New ports collection makefile for:	pcbsd-meta-nvidia-legacy-71
#
# $FreeBSD$
#

PORTNAME=	pcbsd-meta-nvidia-legacy-71
PORTVERSION=	CHGVERSION
CATEGORIES=	misc
MASTER_SITES=	# empty
DISTFILES=	# none

MAINTAINER=	kris@pcbsd.org
COMMENT=	The meta-port pcbsd-meta-nvidia-legacy-71

RUN_DEPENDS=	nvidia-driver>=0:${PORTSDIR}/x11/nvidia-driver-71 \
		nvidia-settings>=0:${PORTSDIR}/x11/nvidia-settings \
		nvidia-xconfig>=0:${PORTSDIR}/x11/nvidia-xconfig

ONLY_FOR_ARCHS=	i386
NO_BUILD=	yes

do-install:
	${DO_NADA}

.include <bsd.port.mk>
