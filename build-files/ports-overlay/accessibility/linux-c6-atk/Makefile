# Created by: Artyom Mirgorodskiy <artyom.mirgorodsky@gmail.com>
# $FreeBSD$

PORTNAME=	atk
PORTVERSION=	1.28.0
RPMVERSION=	2.el6
USE_LINUX=	c6

CATEGORIES=	accessibility linux
MAINTAINER=	emulation@FreeBSD.org
COMMENT=	Accessibility Toolkit, Linux/i386 binary (Linux CentOS 6)

PKGNAMEPREFIX=	linux-${USE_LINUX}-
DISTNAME=	${PORTNAME}-${PORTVERSION}-${RPMVERSION}

CONFLICTS=	linux-atk-[0-9]* linux-f10-atk-[0-9]*

ONLY_FOR_ARCHS=	i386 amd64
USE_LINUX_RPM=	yes
LINUX_RPM_ARCH=		i686
USE_LDCONFIG=	yes

.include <bsd.port.mk>
