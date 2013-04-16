# New ports collection makefile for:	pcbsd-utils
<<<<<<< HEAD
# Date created:				01 Feb 2013
=======
# Date created:				04 Feb 2013
>>>>>>> pcbsd/master
# Whom:					Kris Moore <kmoore@FreeBSD.org>
#
# $FreeBSD$

PORTNAME=	pcbsd-utils
PORTVERSION=	CHGVERSION
CATEGORIES=	sysutils
MASTER_SITES=	http://www.pcbsd.org/~kris/software/ \
		ftp://ftp.pcbsd.org/pub/software/

MAINTAINER=	kmoore@FreeBSD.org
COMMENT=	PC-BSD Command-Line Utilities

RUN_DEPENDS=    pkg-static:${PORTSDIR}/ports-mgmt/pkg \
		svn:${PORTSDIR}/devel/subversion \
		mksquashfs:${PORTSDIR}/sysutils/squashfs-tools \
		debootstrap:${PORTSDIR}/sysutils/debootstrap \
		aria2c:${PORTSDIR}/www/aria2 \
		jailme:${PORTSDIR}/sysutils/jailme

WRKSRC=		${WRKDIR}/src-sh
CONFLICTS?=	pbi-manager-[0-9]*
NO_BUILD=	yes
USE_BZIP2=	yes

MAN1=		pbi_add.1 pbi_browser.1 pbi_create.1 \
		pbi_delete.1 \
		pbi_icon.1 pbi_indextool.1 pbi_info.1 \
		pbi_listrepo.1 pbi_update.1 \
		pbi_metatool.1 \
		pbi_patch.1 \
		pbi_update_hashdir.1
MAN5=		pbi.conf.5
MAN8=		pbi_addrepo.8 pbi_autobuild.8 pbi_deleterepo.8 \
		pbi_makepatch.8 pbi_makeport.8 pbi_makerepo.8 pbid.8 \
		pc-sysinstall.8

.include <bsd.port.pre.mk>

do-install:
	cd ${WRKSRC} && make install

post-install:
	PKG_PREFIX=${PREFIX} ${SH} ${PKGINSTALL} ${PKGNAME} POST-INSTALL

.include <bsd.port.post.mk>
