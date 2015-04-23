#!/bin/sh
#-
# Copyright (c) 2015 iXsystems, Inc.  All rights reserved.
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
# Script to get life-preserver ssh / iscsi files from a USB stick
############################################################################

SYSKEYS="/tmp/lpreserver"
if [ ! -d "${SYSKEYS}" ] ; then
   mkdir -p ${SYSKEYS} >/dev/null 2>/dev/null
fi
MNTDIR="/root/usbmount"
if [ ! -d "${MNTDIR}" ] ; then
   mkdir -p ${MNTDIR} 2>/dev/null >/dev/null
fi
SAVEKEYDIR="${MNTDIR}/lpreserver/"

status=1

if [ -n "$1" -a "$1" = "ISCSI" ] ; then
  targetFiles=".lpiscsi"
else
  targetFiles="id_rsa"
fi

# Lets check the various da* devices, look for a FAT32 USB mount
for i in `ls /dev/da* 2>/dev/null`
do
   # Lets try to FAT mount
   mount_msdosfs -o longnames ${i} $MNTDIR >/dev/null 2>/dev/null
   if [ $? -ne 0 ] ; then continue ; fi
   if [ ! -d "${SAVEKEYDIR}" ] ; then
      umount ${MNTDIR} >/dev/null 2>/dev/null
      continue
   fi
   ls ${SAVEKEYDIR}/*${targetFiles} >/dev/null 2>/dev/null
   if [ $? -ne 0 ] ; then
      umount ${MNTDIR} >/dev/null 2>/dev/null
      continue
   fi

   # Looks like we have keys! Lets copy / umount / return
   cp ${SAVEKEYDIR}/*${targetFiles} ${SYSKEYS}/
   status=$?
   sync
   umount ${MNTDIR} >/dev/null 2>/dev/null
   break
done

for j in `ls ${SYSKEYS}/*${targetFiles} 2>/dev/null`
do
  chmod 600 ${j}
  echo "$j"
done

exit $status
