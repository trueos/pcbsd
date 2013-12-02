# Created by: Artyom Mirgorodskiy <artyom.mirgorodsky@gmail.com>
# $FreeBSD$

PORTNAME=	alsa-plugins-oss
PORTVERSION=	1.0.21
RPMVERSION=	3.el6
USE_LINUX=	c6
USE_LINUX_APPS=	alsalib

CATEGORIES=	audio linux
MAINTAINER=	emulation@FreeBSD.org

MAINTAINER=	hashiz@meridiani.jp
COMMENT=	OSS plugin for ALSA (Linux CentOS 6)

PKGNAMEPREFIX=	linux-${USE_LINUX}-
DISTNAME=	${PORTNAME}-${PORTVERSION}-${RPMVERSION}

CONFLICTS=	linux-f10-alsa-plugins-oss-[0-9]*

ONLY_FOR_ARCHS=	i386 amd64
USE_LINUX_RPM=	yes
LINUX_RPM_ARCH=		i686
SRC_DISTFILES=	${PORTNAME:S/-oss//}-${PORTVERSION}-${RPMVERSION}${SRC_SUFX}
NO_STAGE=	yes
USE_LDCONFIG=	yes

CONF_FILE=	etc/alsa/pcm/pcm-oss.conf

post-patch:
	@${RM} ${WRKSRC}/${CONF_FILE}.orig
	@${MV} ${WRKSRC}/${CONF_FILE} ${WRKSRC}/${CONF_FILE}-dist

post-install:
	@if [ ! -f ${PREFIX}/${CONF_FILE} ]; then \
		${CP} -p ${PREFIX}/${CONF_FILE}-dist ${PREFIX}/${CONF_FILE} ; \
	fi
	PKG_PREFIX=${PREFIX} ${SH} ${PKGINSTALL} ${PKGNAME} POST-INSTALL

.include <bsd.port.mk>
