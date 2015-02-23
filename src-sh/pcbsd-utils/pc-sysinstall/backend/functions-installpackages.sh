#!/bin/sh
#-
# Copyright (c) 2010 iXsystems, Inc.  All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
# $FreeBSD: head/usr.sbin/pc-sysinstall/backend/functions-installpackages.sh 247734 2013-03-03 23:01:46Z jpaetzel $

# Functions which check and load any optional packages specified in the config

. ${BACKEND}/functions.sh
. ${BACKEND}/functions-parse.sh

# Check for any packages specified, and begin loading them
install_packages()
{
  echo "Checking for packages to install..."
  sleep 2

  # First, lets check and see if we even have any packages to install
  get_value_from_cfg installPackages

  # Nothing to do?
  if [ -z "${VAL}" ]; then return; fi

  echo "Installing packages..."
  sleep 3

  local PKGPTH

  HERE=`pwd`
  rc_halt "mkdir -p ${FSMNT}${PKGTMPDIR}"

  # Determine the directory we will install packages from
  get_package_location
  rc_halt "cd ${PKGDLDIR}"

  # We dont want to be bothered with scripts asking questions
  PACKAGE_BUILDING=yes
  export PACKAGE_BUILDING

  # Install PKGNG into the chroot
  bootstrap_pkgng

  # Update the repo database
  echo "Updating pkgng database"
  case "${INSTALLMEDIUM}" in
    usb|dvd|local) run_chroot_cmd "pkg -R /mnt/repo-installer update -f" ;;
                *) run_chroot_cmd "pkg update -f" ;;
  esac

  # Lets start by cleaning up the string and getting it ready to parse
  get_value_from_cfg_with_spaces installPackages
  PACKAGES="${VAL}"
  echo_log "Packages to install: `echo $PACKAGES | wc -w | awk '{print $1}'`"
  for i in $PACKAGES
  do
    PKGNAME="${i}"

    # When doing a pkg install, if on local media, use a pkg.conf from /dist/
    if [ "${INSTALLMEDIUM}" != "ftp" ] ; then
      # Get the package file-name
      PKGFILENAME=""
      PKGFILENAME=`chroot ${FSMNT} pkg -R /mnt/repo-installer rquery '%n-%v' ${PKGNAME}`
      if [ -z "$PKGFILENAME" ] ; then
         echo_log "Warning: No such package in repo: ${PKGNAME}"
	 sleep 2
         continue
      fi
      if [ ! -e "${FSMNT}/mnt/All/${PKGFILENAME}.txz" ] ; then
         echo_log "Warning: No such package file in repo: ${PKGFILENAME}"
	 sleep 2
         continue
      fi
      PKGADD="pkg add /mnt/All/${PKGFILENAME}.txz"
    else
      # Doing a network install, use the default pkg.conf
      PKGADD="pkg install -y ${PKGNAME}"
    fi

    PKGINFO="pkg info"

    # If the package is not already installed, install it!
    if ! run_chroot_cmd "${PKGINFO} -e ${PKGNAME}" >/dev/null 2>/dev/null
    then
      echo_log "Installing package: ${PKGNAME}"
      run_chroot_cmd "$PKGADD" 2>&1 | tee -a ${LOGOUT}
      if [ $? -ne 0 ] ; then
        exit_err "Failed installing: $PKGADD"
      fi
      run_chroot_cmd "rm -rf /usr/local/tmp/All"
      if ! run_chroot_cmd "${PKGINFO} -e ${PKGNAME}" >/dev/null 2>/dev/null
      then
        echo_log "WARNING: PKGNG reported 0, but pkg: ${PKGNAME} does not appear to be installed!"
      fi
    fi
  done

  echo_log "Package installation complete!"

  # Cleanup after ourselves
  echo_log "Cleaning up: ${FSMNT}${PKGTMPDIR}"
  sleep 1
  rc_halt "cd ${HERE}"
  if [ "${INSTALLMEDIUM}" != "ftp" ] ; then
    rc_halt "umount ${FSMNT}/mnt" >/dev/null 2>/dev/null
    rc_halt "rmdir ${FSMNT}${PKGTMPDIR}" >/dev/null 2>/dev/null
  fi
};
