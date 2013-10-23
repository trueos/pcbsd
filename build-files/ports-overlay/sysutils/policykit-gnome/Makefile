# Created by: Joe Marcus Clarke <marcus@FreeBSD.org>
# $FreeBSD: sysutils/policykit-gnome/Makefile 327772 2013-09-20 23:05:58Z bapt $
#   $MCom: ports/sysutils/policykit-gnome/Makefile,v 1.6 2008/10/03 17:19:43 marcus Exp $

PORTNAME=	policykit-gnome
PORTVERSION=	0.9.2
PORTREVISION=	6
CATEGORIES=	sysutils gnome
MASTER_SITES=	http://hal.freedesktop.org/releases/
DISTNAME=	PolicyKit-gnome-${PORTVERSION}

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	GNOME frontend to the PolicyKit framework

LIB_DEPENDS=	polkit.2:${PORTSDIR}/sysutils/policykit

USE_BZIP2=	yes
USE_GMAKE=	yes
USE_GCC=	4.8+
USES=		gettext pkgconfig
USE_GNOME=	gnomehack intlhack gnomedocutils gconf2
GNU_CONFIGURE=	yes
USE_LDCONFIG=	yes
CONFIGURE_ARGS=	--disable-gtk-doc \
		--localstatedir=/var
CPPFLAGS+=	-I${LOCALBASE}/include
LDFLAGS+=	-L${LOCALBASE}/lib

GCONF_SCHEMAS=	polkit-gnome.schemas

NO_STAGE=	yes
.include <bsd.port.mk>
