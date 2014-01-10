# New ports collection makefile for:	pcbsd-meta-development-qt
#
# $FreeBSD$
#

PORTNAME=	pcbsd-meta-development-qt
PORTVERSION=	CHGVERSION
CATEGORIES=	misc
MASTER_SITES=	# empty
DISTFILES=	# none

MAINTAINER=	kris@pcbsd.org
COMMENT=	The meta-port pcbsd-meta-development-qt

RUN_DEPENDS=	qt4-assistant>=0:${PORTSDIR}/devel/qt4-assistant \
		qt4-assistant-adp>=0:${PORTSDIR}/devel/qt4-assistant-adp \
		qt4-designer>=0:${PORTSDIR}/devel/qt4-designer \
		qt4-libQtAssistantClient>=0:${PORTSDIR}/devel/qt4-libqtassistantclient \
		qt4-linguist>=0:${PORTSDIR}/devel/qt4-linguist \
		qt4-uic>=0:${PORTSDIR}/devel/qt4-uic \
		qt4-moc>=0:${PORTSDIR}/devel/qt4-moc \
		qt4-rcc>=0:${PORTSDIR}/devel/qt4-rcc \
		qt4-doc>=0:${PORTSDIR}/misc/qt4-doc \
		qt4-qmake>=0:${PORTSDIR}/devel/qmake4 \
		qtcreator:${PORTSDIR}/devel/qtcreator \
		cmake>=0:${PORTSDIR}/devel/cmake \
		cmake-gui>=0:${PORTSDIR}/devel/cmake-gui \
		gmake>=0:${PORTSDIR}/devel/gmake \
		qt4-qtsolutions-singleapplication>=0:${PORTSDIR}/devel/qt4-qtsolutions-singleapplication

NO_BUILD=	yes

do-install:
	${DO_NADA}

.include <bsd.port.mk>
