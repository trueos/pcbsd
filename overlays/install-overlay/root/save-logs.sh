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
# $FreeBSD$

# Script which creates a gzipped log and saves it to a USB stick / media
############################################################################

# Set the location of our compressed log
TMPLOG="/tmp/pc-sysinstall.log"

echo "# PC-SYSINSTALL LOG" >${TMPLOG}
cat /tmp/.pc-sysinstall/pc-sysinstall.log >> ${TMPLOG}

# Check if we have a GUI generated install cfg
if [ -e "/tmp/sys-install.cfg" ]
then
  echo "" >>${TMPLOG}
  echo "# PC-SYSINSTALL CFG " >>${TMPLOG}
  cat /tmp/sys-install.cfg | grep -vE 'rootPass|userPass|encpass' >> ${TMPLOG}
fi

# Save dmesg output
echo "" >>${TMPLOG}
echo "# DMESG OUTPUT " >>${TMPLOG}
dmesg >> ${TMPLOG}

# Get gpart info on all disks
for i in `pc-sysinstall disk-list | cut -d ':' -f 1`
do
  echo "" >>${TMPLOG}
  echo "# DISK INFO $i " >>${TMPLOG}
  ls /dev/${i}* >>${TMPLOG}
  gpart show ${i} >> ${TMPLOG}
done

# Show Mounted volumes
echo "" >>${TMPLOG}
echo "# MOUNT OUTPUT " >>${TMPLOG}
mount >> ${TMPLOG}


echo "Do you wish to save this logfile to a USB stick? (Y/N)"
read tmp
if [ "$tmp" = "Y" -o "$tmp" = "y" ]
then
  echo "Please insert a MSDOS formatted USB stick and press Enter when ready."
  read tmp
  suc=0
  for i in `ls /dev/da*`
  do
	# Try mounting msdos media
	mount_msdosfs ${i} /mnt >/dev/null 2>/dev/null
	if [ $? -eq 0 ] ; then
		cp ${TMPLOG} /mnt/pcbsd-install-debug.log
		umount /mnt
		suc=1
		break
	fi
  done
  if [ $suc -eq 1 ] ; then
	echo "Success! Saved to filename 'pcbsd-install-debug.log' on USB media."
  else
	echo "Failed copying log to USB media. You can manually copy from $TMPLOG"
  fi
else
  echo "Log file saved to ${TMPLOG}"
  echo "Warning: This file will be lost once the system is rebooted."
  echo "You may wish to use 'sftp' to save this logfile to a remote ssh server."
fi

echo "Press Enter to exit"
read tmp
