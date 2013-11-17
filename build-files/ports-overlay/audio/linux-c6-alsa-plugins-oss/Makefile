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
COMMENT=	OSS plugin for ALSA (Linux CentOS ${LINUX_DIST_VER})

PKGNAMEPREFIX=	linux-${USE_LINUX}-
MASTER_SITES=	http://vault.centos.org/${LINUX_DIST_VER}/os/i386/Packages/
DISTNAME=	${PORTNAME}-${PORTVERSION}-${RPMVERSION}
DIST_SUBDIR=	rpm/${LINUX_RPM_ARCH}/centos/${LINUX_DIST_VER}

CONFLICTS=	linux-f10-alsa-plugins-oss-[0-9]*

ONLY_FOR_ARCHS=	i386 amd64
USE_LINUX_RPM=	yes
LINUX_RPM_ARCH=		i686
NO_STAGE=	yes
LINUX_DIST_VER=	6.2
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
