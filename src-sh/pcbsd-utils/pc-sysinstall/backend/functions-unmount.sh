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
# $FreeBSD: head/usr.sbin/pc-sysinstall/backend/functions-unmount.sh 232899 2012-03-12 21:32:43Z jpaetzel $

# Functions which unmount all mounted disk filesystems

# Unmount all mounted partitions under specified dir
umount_all_dir()
{
  _udir="$1"
  _umntdirs=`mount | sort -r | grep "on $_udir" | cut -d ' ' -f 3`
  for _ud in $_umntdirs
  do
    echo_log "Unmounting: ${_ud}"
    sleep 2
    umount -f ${_ud} 
  done
}

# Script that adds our gmirror devices for syncing
start_gmirror_sync()
{

  cd ${MIRRORCFGDIR}
  for DISK in `ls ${MIRRORCFGDIR}`
  do
    MIRRORDISK="`cat ${DISK} | cut -d ':' -f 1`"
    MIRRORBAL="`cat ${DISK} | cut -d ':' -f 2`"
    MIRRORNAME="`cat ${DISK} | cut -d ':' -f 3`"
   
    # Start the mirroring service
    rc_nohalt "gmirror forget ${MIRRORNAME}"
    rc_halt "gmirror insert ${MIRRORNAME} ${MIRRORDISK}"

  done

};

# Unmounts all our mounted file-systems
unmount_all_filesystems()
{
  cd /

  # Check if we need to setup GRUB
  if [ -e "${TMPDIR}/.grub-install" ] ; then setup_grub; fi

  # Copy the logfile to disk before we unmount
  cp ${LOGOUT} ${FSMNT}/root/pc-sysinstall.log

  # Start by unmounting any ZFS partitions
  zfs_cleanup_unmount

  # Lets read our partition list, and unmount each
  ##################################################################
  for PART in `ls ${PARTDIR}`
  do
    PARTDEV=`echo $PART | sed 's|-|/|g'`    
    PARTFS="`cat ${PARTDIR}/${PART} | cut -d '#' -f 1`"
    PARTMNT="`cat ${PARTDIR}/${PART} | cut -d '#' -f 2`"
    PARTENC="`cat ${PARTDIR}/${PART} | cut -d '#' -f 3`"
    PARTLABEL="`cat ${PARTDIR}/${PART} | cut -d '#' -f 4`"

    if [ "${PARTENC}" = "ON" ]
    then
      EXT=".eli"
    else
      EXT=""
    fi

    if [ "${PARTFS}" = "SWAP" ]
    then
      rc_nohalt "swapoff ${PARTDEV}${EXT}"
    fi

    # Check if we've found "/", and unmount that last
    if [ "$PARTMNT" != "/" -a "${PARTMNT}" != "none" -a "${PARTFS}" != "ZFS" ]
    then
      echo_log "Unmounting: ${PARTDEV}${EXT}"
      rc_halt "umount -f ${PARTDEV}${EXT}"

      # Re-check if we are missing a label for this device and create it again if so
      if [ ! -e "/dev/label/${PARTLABEL}" ]
      then
        case ${PARTFS} in
          UFS) glabel label ${PARTLABEL} ${PARTDEV}${EXT} ;;
          UFS+S) glabel label ${PARTLABEL} ${PARTDEV}${EXT} ;;
          UFS+SUJ) glabel label ${PARTLABEL} ${PARTDEV}${EXT} ;;
          UFS+J) glabel label ${PARTLABEL} ${PARTDEV}${EXT}.journal ;;
          *) ;;
        esac 
      fi
    fi

    # Check if we've found "/" and make sure the label exists
    if [ "$PARTMNT" = "/" -a "${PARTFS}" != "ZFS" ]
    then
      if [ ! -e "/dev/label/${PARTLABEL}" ]
      then
        case ${PARTFS} in
          UFS) ROOTRELABEL="glabel label ${PARTLABEL} ${PARTDEV}${EXT}" ;;
          UFS+S) ROOTRELABEL="glabel label ${PARTLABEL} ${PARTDEV}${EXT}" ;;
          UFS+SUJ) ROOTRELABEL="glabel label ${PARTLABEL} ${PARTDEV}${EXT}" ;;
          UFS+J) ROOTRELABEL="glabel label ${PARTLABEL} ${PARTDEV}${EXT}.journal" ;;
          *) ;;
        esac 
      fi
    fi
  done

  # Last lets the /mnt partition
  #########################################################
  echo_log "Unmounting: ${FSMNT}"
  rc_nohalt "umount -f ${FSMNT}"

   # If are using a ZFS on "/" set it to legacy
  if [ ! -z "${FOUNDZFSROOT}" ]
  then
    rc_halt "zfs set mountpoint=legacy ${FOUNDZFSROOT}"
    rc_halt "zfs set mountpoint=/ ${FOUNDZFSROOT}/ROOT/default"
  fi

  # If we need to relabel "/" do it now
  if [ ! -z "${ROOTRELABEL}" ]
  then
    ${ROOTRELABEL}
  fi

  # Check if we need to unmount a media
  case $INSTALLMEDIUM in
     dvd|usb) echo_log "Unmounting DVD/USB media: ${CDMNT}"
              sleep 5
              rc_nohalt "umount -f ${CDMNT}" >/dev/null 2>/dev/null
              ;;
           *) ;;
  esac

  # Check if we need to run any gmirror syncing
  ls ${MIRRORCFGDIR}/* >/dev/null 2>/dev/null
  if [ $? -eq 0 ]
  then
    # Lets start syncing now
    start_gmirror_sync
  fi

};

# Unmounts any filesystems after a failure
unmount_all_filesystems_failure()
{
  cd /

  # Start by unmounting any ZFS partitions
  zfs_cleanup_unmount

  # Lets read our partition list, and unmount each
  ##################################################################
  for PART in `ls ${PARTDIR}`
  do
    PARTDEV=`echo $PART | sed 's|-|/|g'`    
    PARTFS="`cat ${PARTDIR}/${PART} | cut -d '#' -f 1`"
    PARTMNT="`cat ${PARTDIR}/${PART} | cut -d '#' -f 2`"
    PARTENC="`cat ${PARTDIR}/${PART} | cut -d '#' -f 3`"
    PARTLABEL="`cat ${PARTDIR}/${PART} | cut -d '#' -f 4`"

    if [ "${PARTENC}" = "ON" ]
    then
      EXT=".eli"
    else
      EXT=""
    fi

    if [ "${PARTFS}" = "SWAP" ]
    then
      rc_nohalt "swapoff ${PARTDEV}${EXT}"
    fi

    # Check if we've found "/", and unmount that last
    if [ "$PARTMNT" != "/" -a "${PARTMNT}" != "none" -a "${PARTFS}" != "ZFS" ]
    then
      echo_log "Unmounting: ${PARTDEV}${EXT}"
      rc_halt "umount -f ${PARTDEV}${EXT}"
    fi
  done

  # Last lets the /mnt partition
  #########################################################
  echo_log "Unmounting: ${FSMNT}"
  rc_nohalt "umount -f ${FSMNT}"

  # Check if we need to unmount a media
  case $INSTALLMEDIUM in
     dvd|usb) echo_log "Unmounting DVD/USB media: ${CDMNT}"
              sleep 5
              rc_nohalt "umount -f ${CDMNT}" >/dev/null 2>/dev/null
              ;;
           *) ;;
  esac

};

# Script which stamps grub on the specified disks
setup_grub() 
{
  # Mount devfs
  rc_halt "mount -t devfs devfs ${FSMNT}/dev"

  # Check for a custom beadm.install to copy before we run grub
  if [ -e "/root/beadm.install" ] ; then
     rc_halt "cp /root/beadm.install ${FSMNT}/root/beadm.install"
     rc_halt "chmod 755 ${FSMNT}/root/beadm.install"
  fi

  # Make sure to copy zpool.cache first
  if [ ! -d "${FSMNT}/boot/zfs/" ] ; then 
     rc_halt "mkdir ${FSMNT}/boot/zfs"
  fi
  
  # GhostBSD doesn't use ZFS.
  if [ -e "/boot/zfs/zpool.cache"] ; then
    rc_halt "cp /boot/zfs/zpool.cache ${FSMNT}/boot/zfs/"
  fi

  if [ ! -e "${FSMNT}/boot/kernel/zfs" ] ; then
    rc_halt "ln -s ../zfs ${FSMNT}/boot/kernel/zfs"
  fi

  # Copy the hostid so that our zfs cache works
  rc_nohalt "cp /etc/hostid ${FSMNT}/etc/hostid"

  # Are we using GELI?
  if [ -e "${TMPDIR}/.grub-install-geli" ] ; then
     echo "GRUB_ENABLE_CRYPTODISK=y" >> ${FSMNT}/usr/local/etc/default/grub
  fi

  # Check if we need to install in EFI mode
  EFIMODE="FALSE"
  FORMATEFI="FALSE"
  BOOTMODE=`kenv grub.platform`
  if [ "$BOOTMODE" = "efi" ]; then
    GRUBFLAGS="$GRUBFLAGS --efi-directory=/boot/efi --removable --target=x86_64-efi"
    EFIMODE="TRUE"
    if [ -e "${TMPDIR}/.grub-full-gpt" -o -e "${TMPDIR}/.grub-full-mbr" ] ; then
      FORMATEFI="TRUE"
    fi
  fi

  # Read through our list and stamp grub for each device
  while read line
  do
    # Make sure we have a /dev in front of the disk name
    echo $line | grep -q '/dev/'
    if [ $? -eq 0 ] ; then
      gDisk="$line"
    else
      gDisk="/dev/$line"
    fi

    # Do any EFI creation
    if [ "$EFIMODE" = "TRUE" ] ;then
       if [ "$FORMATEFI" = "TRUE" ] ; then
         # Format the EFI partition
         echo_log "Formatting EFI / FAT32 partition"
	 if [ -e "${TMPDIR}/.grub-full-gpt" ] ; then
           rc_halt "newfs_msdos -F 16 ${gDisk}p1"
           EFIPART="${gDisk}p1"
         else
           rc_halt "newfs_msdos -F 16 ${gDisk}s1"
           EFIPART="${gDisk}s1"
         fi
       else
         # Installing to disk with existing EFI setup
	 efip=`gpart show $gDisk | grep ' efi ' | awk '{print $3}'`
	 EFIPART="${gDisk}p${efip}"
       fi

       if [ -z "$DONEEFILABEL" ] ; then
         # Label this sucker
         rc_halt "glabel label efibsd ${EFIPART}"

         # Save to systems fstab file
         echo "/dev/label/efibsd	/boot/efi		msdosfs		rw	0	0" >> ${FSMNT}/etc/fstab
	 DONEEFILABEL="YES"
       fi

       # Mount the partition
       mkdir ${FSMNT}/boot/efi
       rc_halt "mount -t msdosfs ${EFIPART} ${FSMNT}/boot/efi"
    fi

    # Stamp GRUB now
    rc_halt "chroot ${FSMNT} grub-install $GRUBFLAGS --force $gDisk"

    # Cleanup after EFI
    if [ "$EFIMODE" = "TRUE" ] ;then
       rc_halt "umount ${FSMNT}/boot/efi"
    fi
  done < ${TMPDIR}/.grub-install

  # Make sure we re-create the default grub.cfg
  if [ "$EFIMODE" = "TRUE" ] ;then
    # For some reason this returns non-0 on EFI, but works perfectly fine with no
    # warnings / errors, need to investigate further
    rc_nohalt "chroot ${FSMNT} grub-mkconfig -o /boot/grub/grub.cfg"
  else
    # For some reason on GhostBSD this returns non-0 without EFI, but works perfectly fine with no
    rc_nohalt "chroot ${FSMNT} grub-mkconfig -o /boot/grub/grub.cfg"
  fi

  # Sleep and cleanup
  if [ -e "${FSMNT}/root/beadm.install" ] ; then
     rc_halt "rm ${FSMNT}/root/beadm.install"
  fi
  sleep 5
  rc_halt "umount ${FSMNT}/dev"
};
