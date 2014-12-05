#!/bin/sh
# Author: Kris Moore
# License: BSD
# Desc: Helper for sys-manager to find installation media on DVD or USB

# Create our mount-dir
mDir="/.install-mnt.$$"
mkdir -p ${mDir}

cd /dev
cDevs="`ls cd* 2>/dev/null` `ls acd* 2>/dev/null`"
usbDevs="`ls da* 2>/dev/null`"
cd /

# First try the CD devices
for cd in $cDevs
do
	mount_cd9660 /dev/${cd} ${mDir} 2>/dev/null
	if [ "$?" = "0" -a -d "${mDir}/packages" ] ; then
		echo ${mDir} >&1
		exit 0
	else
		umount $mDir >/dev/null 2>/dev/null
	fi
done

# Now try the USB devices
for usb in $usbDevs
do
	mount /dev/${usb} ${mDir} 2>/dev/null
	if [ "$?" = "0" -a -d "${mDir}/packages" ] ; then
		echo ${mDir} >&1
		exit 0
	else
		umount $mDir >/dev/null 2>/dev/null
	fi
done

# If we got here, no devices found :(
rmdir ${mDir}
echo "NOT FOUND" >&1
exit 1
