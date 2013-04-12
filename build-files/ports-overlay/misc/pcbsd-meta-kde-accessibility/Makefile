# New ports collection makefile for:	pcbsd-meta-kde-accessibility
#
# $FreeBSD$
#

PORTNAME=	pcbsd-meta-kde-accessibility
PORTVERSION=	CHGVERSION
CATEGORIES=	misc
MASTER_SITES=	# empty
DISTFILES=	# none

MAINTAINER=	kris@pcbsd.org
COMMENT=	The meta-port pcbsd-meta-kde-accessibility

RUN_DEPENDS=	flite>=0:${PORTSDIR}/audio/flite \
		festival>=0:${PORTSDIR}/audio/festival \
		festvox-czech>=0:${PORTSDIR}/audio/festvox-czech \
		festvox-don>=0:${PORTSDIR}/audio/festvox-don \
		festvox-el11>=0:${PORTSDIR}/audio/festvox-el11 \
		festvox-kal16>=0:${PORTSDIR}/audio/festvox-kal16 \
		festvox-ked16>=0:${PORTSDIR}/audio/festvox-ked16 \
		festvox-rab16>=0:${PORTSDIR}/audio/festvox-rab16 \
		kdeaccessibility>=0:${PORTSDIR}/accessibility/kdeaccessibility4 \
		espeak>=0:${PORTSDIR}/audio/espeak \
		speech-dispatcher>=0:${PORTSDIR}/accessibility/speech-dispatcher

NO_BUILD=	yes

do-install:
	${DO_NADA}

.include <bsd.port.mk>
