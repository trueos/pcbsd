# New ports collection makefile for:	pcbsd-meta-development-science
#
# $FreeBSD$
#

PORTNAME=	pcbsd-meta-development-science
PORTVERSION=	CHGVERSION
CATEGORIES=	misc
MASTER_SITES=	# empty
DISTFILES=	# none

MAINTAINER=	kris@pcbsd.org
COMMENT=	The meta-port pcbsd-meta-development-science

RUN_DEPENDS=	R>=0:${PORTSDIR}/math/R \
		gnuplot>=0:${PORTSDIR}/math/gnuplot \
		valgrind>=0:${PORTSDIR}/devel/valgrind \
		boost-all>=0:${PORTSDIR}/devel/boost-all

NO_BUILD=	yes

do-install:
	${DO_NADA}

.include <bsd.port.mk>
