#!/bin/sh

# Enable unloading the drivers
grep '^vmware_guest_vmblock_enable="YES"' /etc/rc.conf >/dev/null 2>/dev/null
if [ $? -eq 0 ] ; then
	sed -i .bak '/vmware_guest_vmblock_enable="YES"/d' /etc/rc.conf
fi
grep '^vmware_guest_vmhgfs_enable="YES"' /etc/rc.conf >/dev/null 2>/dev/null
if [ $? -eq 0 ] ; then
	sed -i .bak '/vmware_guest_vmhgfs_enable="YES"/d' /etc/rc.conf
fi
grep '^vmware_guest_vmmemctl_enable="YES"' /etc/rc.conf >/dev/null 2>/dev/null
if [ $? -eq 0 ] ; then
	sed -i .bak '/vmware_guest_vmmemctl_enable="YES"/d' /etc/rc.conf
fi
grep '^vmware_guest_vmxnet_enable="YES"' /etc/rc.conf >/dev/null 2>/dev/null
if [ $? -eq 0 ] ; then
	sed -i .bak '/vmware_guest_vmxnet_enable="YES"/d' /etc/rc.conf
fi
grep '^vmware_guestd_enable="YES"' /etc/rc.conf >/dev/null 2>/dev/null
if [ $? -eq 0 ] ; then
	sed -i .bak '/vmware_guestd_enable="YES"/d' /etc/rc.conf
fi
