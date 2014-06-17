#!/bin/sh
#-
# Copyright (c) 2013 iXsystems, Inc.  All rights reserved.
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
# Script to get pc-sysinstall config files from a USB stick
############################################################################

# Set the location of pc-sysinstall configs
SYSCFGS="/tmp/pc-sys/"
if [ ! -d "${SYSCFGS}" ] ; then
   mkdir -p ${SYSCFGS}
fi
MNTDIR="/root/usbmount"
if [ ! -d "${MNTDIR}" ] ; then
   mkdir -p ${MNTDIR}
fi
SAVECFGDIR="${MNTDIR}/pc-sys/"
KEYSDIR="${MNTDIR}/lpreserver/"

status=1

# Lets check the various da* devices, look for a FAT32 USB mount
for i in `ls /dev/da* 2>/dev/null`
do
   # Lets try to FAT mount
   mount_msdosfs ${i} $MNTDIR 2>/dev/null
   if [ $? -ne 0 ] ; then continue ; fi
   if [ ! -d "${SAVECFGDIR}" ] ; then
      umount ${MNTDIR}
      continue
   fi
   cp ${SAVECFGDIR}/*.cfg ${SYSCFGS}/

   # Do we have lpreserver key files on this USB stick?
   if [ -d "${KEYSDIR}" ] ; then
     rm -rf /root/lpreserver-keys
     cp -r ${KEYSDIR} /root/lpreserver-keys
   fi

   status=0
   echo "Loaded configs to: $SYSCFGS"
   sync
   sleep 1
   umount ${MNTDIR}
   break
done

exit $status
