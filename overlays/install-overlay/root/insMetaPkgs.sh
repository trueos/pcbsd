#!/bin/sh
# Author: Kris Moore
# Description: Helper script called by pc-sysinstall which runs pc-metapkgmanager 

PKGS="$1"
LOC="$2"
PKGSET="$3"

if [ -z "$PKGSET" ] ; then PKGSET="pcbsd" ; fi

# If installing pkgs from CD
if [ "$LOC" = "CD" ] ; then
	mkdir -p ${FSMNT}/.cdmnt
	mount_nullfs /dist/metapackages/${PKGSET} ${FSMNT}/.cdmnt
	LOC="/.cdmnt"
else
	# Need resolv.conf in the chroot environment
	if [ -e "/etc/resolv.conf" ] ; then
		cp /etc/resolv.conf ${FSMNT}/etc/resolv.conf
	fi
fi

nice -20 chroot $FSMNT /usr/local/bin/pc-metapkgmanager add $PKGS $LOC

# Unmount our CD
if [ "$LOC" = "/.cdmnt" ] ; then
	umount ${FSMNT}/.cdmnt
	rmdir ${FSMNT}/.cdmnt
fi
