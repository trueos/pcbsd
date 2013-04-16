# New ports collection makefile for:	pcbsd-meta-gnome
#
# $FreeBSD$
#

PORTNAME=	pcbsd-meta-gnome
PORTVERSION=	CHGVERSION
CATEGORIES=	misc
MASTER_SITES=	# empty
DISTFILES=	# none

MAINTAINER=	kris@pcbsd.org
COMMENT=	The meta-port pcbsd-meta-gnome

RUN_DEPENDS=	gnome2-lite>=0:${PORTSDIR}/x11/gnome2-lite \
		gstreamer-plugins-core>=0:${PORTSDIR}/multimedia/gstreamer-plugins-core \
		gstreamer-plugins-gconf>=0:${PORTSDIR}/devel/gstreamer-plugins-gconf \
		totem>=0:${PORTSDIR}/multimedia/totem \
		notification-daemon>=0:${PORTSDIR}/deskutils/notification-daemon \
		libgail-gnome>=0:${PORTSDIR}/x11-toolkits/libgail-gnome \
		gnome-backgrounds>=0:${PORTSDIR}/x11-themes/gnome-backgrounds \
		bug-buddy>=0:${PORTSDIR}/devel/bug-buddy \
		file-roller>=0:${PORTSDIR}/archivers/file-roller \
		gnome-system-tools>=0:${PORTSDIR}/sysutils/gnome-system-tools \
		evince>=0:${PORTSDIR}/graphics/evince \
		alacarte>=0:${PORTSDIR}/deskutils/alacarte \
		gnome-power-manager>=0:${PORTSDIR}/sysutils/gnome-power-manager \
		seahorse>=0:${PORTSDIR}/security/seahorse \
		seahorse-plugins>=0:${PORTSDIR}/security/seahorse-plugins \
		py27-gtk>=0:${PORTSDIR}/x11-toolkits/py-gtk2 \
		gtk2-qtcurve-theme>=0:${PORTSDIR}/x11-themes/qtcurve-gtk2 \
		nimbus>=0:${PORTSDIR}/x11-themes/nimbus \
		nautilus>=0:${PORTSDIR}/x11-fm/nautilus \
		nautilus-actions>=0:${PORTSDIR}/deskutils/nautilus-actions \
		nautilus-locked-folder>=0:${PORTSDIR}/deskutils/nautilus-locked-folder \
		nautilus-open-terminal>=0:${PORTSDIR}/deskutils/nautilus-open-terminal \
		nautilus-sendto>=0:${PORTSDIR}/deskutils/nautilus-sendto \
		nautilus-cd-burner>=0:${PORTSDIR}/sysutils/nautilus-cd-burner

NO_BUILD=	yes

do-install:
	${DO_NADA}

.include <bsd.port.mk>
