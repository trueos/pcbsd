# New ports collection makefile for:	clutter-gtk
# Date created:		2008-12-25
# Whom:			Martin Wilke <miwi@FreeBSD.org>
#
# $FreeBSD: ports/graphics/clutter-gtk/Makefile,v 1.14 2012/11/17 05:58:02 svnexp Exp $
#

PORTNAME=	clutter-gtk
PORTVERSION=	0.10.8
PORTREVISION=	2
CATEGORIES=	graphics
MASTER_SITES=	GNOME \
		http://source.clutter-project.org/sources/${PORTNAME}/${PORTVERSION:R}/

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	GTK+ Integration library for Clutter

LIB_DEPENDS=	clutter-glx-1.0:${PORTSDIR}/graphics/clutter

USE_BZIP2=	yes
USE_GMAKE=	yes
USE_GETTEXT=	yes
USE_AUTOTOOLS=	libtool
USE_GNOME=	gnomehack gtk20 ltverhack gnomeprefix
USE_XORG=	xp x11 glproto
USE_LDCONFIG=	yes

CPPFLAGS+=	-I${LOCALBASE}/include -I${LOCALBASE}/include/clutter-1.0 ${PTHREAD_CFLAGS}
LDFLAGS+=	-L${LOCALBASE}/lib ${PTHREAD_LIBS}

.include <bsd.port.mk>
